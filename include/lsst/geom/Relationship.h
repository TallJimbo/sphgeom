/*
 * Copyright 2018 Jim Bosch
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

#ifndef LSST_GEOM_RELATIONSHIP_H_
#define LSST_GEOM_RELATIONSHIP_H_

#include "lsst/sphgeom/Relationship.h"

namespace lsst {
namespace geom {

// Just use sphgeom::Relationship types and constants for now.
// Will defer restructuring sphgeom until lsst::geom interfaces stabilize.
using sphgeom::Relationship;
using sphgeom::DISJOINT;
using sphgeom::INTERSECTS;
using sphgeom::CONTAINS;
using sphgeom::WITHIN;
using sphgeom::invert;

}} // namespace lsst::geom

#endif // LSST_GEOM_RELATIONSHIP_H_
