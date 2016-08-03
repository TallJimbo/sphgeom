/*
 * LSST Data Management System
 * Copyright 2016 AURA/LSST.
 *
 * This product includes software developed by the
 * LSST Project (http://www.lsst.org/).
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the LSST License Statement and
 * the GNU General Public License along with this program.  If not,
 * see <https://www.lsstcorp.org/LegalNotices/>.
 */

#ifndef LSST_SPHGEOM_PIXELFINDER_H_
#define LSST_SPHGEOM_PIXELFINDER_H_

/// \file
/// \brief This file provides a base class for pixel finders.

#include "lsst/sphgeom/RangeSet.h"

#include "ConvexPolygonImpl.h"


namespace lsst {
namespace sphgeom {
namespace {

// `PixelFinder` is a CRTP base class that locates pixels intersecting a
// region. It assumes a hierarchical pixelization, and that pixels are
// convex spherical polygons with a fixed number of vertices.
//
// The algorithm used is top-down tree traversal, implemented via recursion
// for simplicity. Subclasses must provide a method named `subdivide` with
// the following signature:
//
//      void subdivide(UnitVector3d const * pixel,
//                     uint64_t index,
//                     int level);
//
// that subdivides a pixel into its children and then invokes visit() on
// each child. Children should be visited in ascending index order to keep
// RangeSet inserts efficient. The subclass is also responsible for
// implementing a top-level method that invokes visit() on each root pixel,
// or on some set of candidate pixels.
//
// The `RegionType` parameter avoids the need for virtual function calls to
// determine the spatial relationship between pixels and the input region. The
// boolean template parameter `InteriorOnly` is a flag that indicates whether
// to locate all pixels that intersect the input region, or only those that
// are entirely inside it. Finally, the `NumVertices` template parameter is
// the number of vertices in the polygonal representation of a pixel.
template <
    typename Derived,
    typename RegionType,
    bool InteriorOnly,
    size_t NumVertices
>
class PixelFinder {
public:
    PixelFinder(RangeSet & ranges,
                RegionType const & region,
                int level,
                size_t maxRanges):
        _ranges{&ranges},
        _region{&region},
        _level{level},
        _desiredLevel{level},
        _maxRanges{maxRanges == 0 ? maxRanges - 1 : maxRanges}
    {}

    void visit(UnitVector3d const * pixel,
               uint64_t index,
               int level)
    {
        if (level > _level) {
            // Nothing to do - the subdivision level has been reduced
            // or a pixel that completely contains the search region
            // has been found.
            return;
        }
        // Determine the relationship between the trixel and the search region.
        Relationship r = detail::relate(pixel, pixel + NumVertices, *_region);
        if ((r & DISJOINT) != 0) {
            // The pixel is disjoint from the search region.
            return;
        }
        if ((r & WITHIN) != 0) {
            // The tree traversal has reached a pixel that is entirely within
            // the search region.
            _insert(index, level);
            return;
        } else if (level == _level) {
            // The tree traversal has reached a leaf.
            if (!InteriorOnly) {
                _insert(index, level);
            }
            return;
        }
        static_cast<Derived *>(this)->subdivide(pixel, index, level);
    }

private:
    RangeSet * _ranges;
    RegionType const * _region;
    int _level;
    int const _desiredLevel;
    size_t const _maxRanges;

    void _insert(uint64_t index, int level) {
        int shift = 2 * (_desiredLevel - level);
        _ranges->insert(index << shift, (index + 1) << shift);
        while (_ranges->size() > _maxRanges) {
            // Reduce the subdivision level.
            --_level;
            shift += 2;
            // When looking for intersecting pixels, ranges are simplified
            // by expanding them outwards, causing nearly adjacent small ranges
            // to merge.
            //
            // When looking for interior pixels, ranges are simplified by
            // shrinking them inwards, causing small ranges to disappear.
            if (InteriorOnly) {
                _ranges->complement();
            }
            _ranges->simplify(shift);
            if (InteriorOnly) {
                _ranges->complement();
            }
        }
    }
};

}}} // namespace lsst::sphgeom::detail

#endif // LSST_SPHGEOM_PIXELFINDER_H_