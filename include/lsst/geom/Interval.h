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
#ifndef LSST_GEOM_INTERVAL_H_
#define LSST_GEOM_INTERVAL_H_

#include <algorithm>
#include <iosfwd>

#include "lsst/geom/scalars.h"
#include "lsst/geom/Relationship.h"

namespace lsst { namespace geom {

template <typename T>
class Interval {
    using Traits = detail::Traits<T>;
public:

    using Scalar = T;
    using Size = typename Traits::Size;
    using Pair = typename Traits::Pair;

    static Interval makeFull() {
        return Interval(Traits::makeFullInterval());
    }

    static Interval makeEmpty() {
        return Interval(Traits::makeEmptyInterval());
    }

    template <typename Iterator>
    static Interval makeHull(Iterator first, Iterator last) {
        auto p = std::minmax_element(first, last);
        return Interval(*p.first, *p.second);
    }

    template <typename Container>
    static Interval makeHull(Container const & container) {
        return makeHull(std::begin(container), std::end(container));
    }

    Interval() : _pair(Traits::makeEmptyInterval()) {}

    Interval(Scalar min, Scalar max) : _pair(min, max) { Traits::normalizeInterval(_pair); }

    explicit Interval(Pair const & pair) : _pair(pair) { Traits::normalizeInterval(_pair); }

    template <typename U>
    Interval(
        Interval<U> const & rhs,
        typename Traits::template ConvertFrom<U>::EnableImplicit=0
    )
        : _pair(typename Traits::template ConvertTo<U>()(rhs.asPair()))
    {}

    template <typename U>
    explicit Interval(
        Interval<U> const & rhs,
        typename Traits::template ConvertFrom<U>::EnableExplicit=0
    )
        : _pair(typename Traits::template ConvertTo<U>()(rhs.asPair()))
    {}

    Interval(Interval const &) = default;
    Interval(Interval &&) = default;

    Interval & operator=(Interval const &) = default;
    Interval & operator=(Interval &&) = default;

    ~Interval() = default;

    Pair const asPair() const { return _pair; }

    Scalar getMin() const { return _pair.first; }
    Scalar getMax() const { return _pair.second; }

    bool isEmpty() const { return Traits::isIntervalEmpty(_pair); }

    Size getSize() const { return Traits::getIntervalSize(_pair); }

    Real getCenter() const { return 0.5*(_pair.first + _pair.second); }

    bool operator==(Interval const & rhs) const { return _pair == rhs.asPair(); }
    bool operator!=(Interval const & rhs) const { return !(*this == rhs); }

    bool operator==(Scalar rhs) const { return getMin() == rhs && getMax() == rhs; }
    bool operator!=(Scalar rhs) const { return !(*this == rhs); }

    bool contains(Interval const & rhs) const;
    bool contains(Scalar rhs) const { return intersects(rhs) || (rhs != rhs); }

    bool isDisjointFrom(Interval const & rhs) const;
    bool isDisjointFrom(Scalar rhs) const { return !intersects(rhs); }

    bool intersects(Interval const & rhs) const { return !isDisjointFrom(rhs); }
    bool intersects(Scalar rhs) const { return getMin() <= rhs && rhs <= getMax();}

    bool isWithin(Interval const & rhs) const { return rhs.contains(*this); }
    bool isWithin(Scalar rhs) const { return (*this == rhs) || isEmpty(); }

    Relationship relate(Interval const & rhs) const;
    Relationship relate(Scalar rhs) const;

    Interval & clipTo(Interval const & rhs);
    Interval & clipTo(Scalar rhs);

    Interval clippedTo(Interval const & rhs) const { return Interval(*this).clipTo(rhs); }
    Interval clippedTo(Scalar rhs) const { return Interval(*this).clipTo(rhs); }

    Interval & expandTo(Interval const & rhs);
    Interval & expandTo(Scalar rhs);

    Interval expandedTo(Interval const & rhs) const { return Interval(*this).expandTo(rhs); }
    Interval expandedTo(Scalar rhs) const { return Interval(*this).expandTo(rhs); }

    Interval & dilateBy(Scalar rhs);
    Interval dilatedBy(Scalar rhs) const { return Interval(*this).dilateBy(rhs); }

    Interval & erodeBy(Scalar rhs) { return dilateBy(-rhs); }
    Interval erodedBy(Scalar rhs) const { return Interval(*this).erodeBy(rhs); }

private:
    Pair _pair;
};

template <typename T>
std::ostream & operator<<(std::ostream & os, Interval<T> const & interval);

using IntegerInterval = Interval<Integer>;
using RealInterval = Interval<Real>;


#define LSST_GEOM_INTERVAL_TEMPLATES(PREFIX)\
    PREFIX template class Interval<Integer>;\
    PREFIX template class Interval<Real>;\
    PREFIX template std::ostream & operator<<(std::ostream &, Interval<Integer> const &);\
    PREFIX template std::ostream & operator<<(std::ostream &, Interval<Real> const &)
#ifndef LSST_GEOM_INTERVAL_CC_
    LSST_GEOM_INTERVAL_TEMPLATES(extern);
    #undef LSST_GEOM_INTERVAL_TEMPLATES
#endif

}} // namespace lsst::geom

#endif // !LSST_GEOM_INTERVAL_H_
