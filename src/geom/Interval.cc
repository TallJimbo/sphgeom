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
#define LSST_GEOM_INTERVAL_CC_

#include <iostream>

#include "lsst/geom/Interval.h"

namespace lsst { namespace geom {

template <typename T>
bool Interval<T>::contains(Interval const & rhs) const {
    if (rhs.isEmpty()) {
        return true;
    }
    if (isEmpty()) {
        return false;
    }
    return getMin() <= rhs.getMin() && rhs.getMax() <= getMax();
}

template <typename T>
bool Interval<T>::isDisjointFrom(Interval const & rhs) const {
    if (isEmpty() || rhs.isEmpty()) {
        return true;
    }
    return getMax() < rhs.getMin() || getMin() > rhs.getMax();
}

template <typename T>
Relationship Interval<T>::relate(Interval const & rhs) const {
    if (isEmpty()) {
        if (rhs.isEmpty()) {
            return CONTAINS | DISJOINT | WITHIN;
        }
        return DISJOINT | WITHIN;
    }
    if (rhs.isEmpty()) {
        return CONTAINS | DISJOINT;
    }
    if (*this == rhs) {
        return CONTAINS | WITHIN;
    }
    if (getMin() > rhs.getMax() || getMax() < rhs.getMin()) {
        return DISJOINT;
    }
    if (getMin() <= rhs.getMin() && getMax() >= getMax()) {
        return CONTAINS;
    }
    if (rhs.getMin() <= getMin() && rhs.getMax() >= getMax()) {
        return WITHIN;
    }
    return INTERSECTS;
}

template <typename T>
Relationship Interval<T>::relate(Scalar rhs) const {
    if (isEmpty()) {
        if (rhs != rhs) {
            return CONTAINS | DISJOINT | WITHIN;
        }
        return DISJOINT | WITHIN;
    }
    if (rhs != rhs) {
        return CONTAINS | DISJOINT;
    }
    if (*this == rhs) {
        return CONTAINS | WITHIN;
    }
    if (intersects(rhs)) {
        return CONTAINS;
    }
    return DISJOINT;
}

template <typename T>
Interval<T> & Interval<T>::clipTo(Interval const & rhs) {
    if (!isEmpty()) {
        if (rhs.isEmpty()) {
            *this = makeEmpty();
        } else {
            *this = Interval(std::max(getMin(), rhs.getMin()),
                             std::min(getMax(), rhs.getMax()));
        }
    }
    return *this;
}

template <typename T>
Interval<T> & Interval<T>::clipTo(Scalar rhs) {
    if (!isEmpty()) {
        if (rhs != rhs) {
            *this = makeEmpty();
        } else {
            *this = Interval(std::max(getMin(), rhs),
                             std::min(getMax(), rhs));
        }
    }
    return *this;
}

template <typename T>
Interval<T> & Interval<T>::expandTo(Interval const & rhs) {
    if (!rhs.isEmpty()) {
        if (isEmpty()) {
            *this = rhs;
        } else {
            *this = Interval(std::min(getMin(), rhs.getMin()),
                             std::max(getMax(), rhs.getMax()));
        }
    }
    return *this;
}

template <typename T>
Interval<T> & Interval<T>::expandTo(Scalar rhs) {
    if (rhs == rhs) {
        if (isEmpty()) {
            *this = Interval(rhs, rhs);
        } else {
            *this = Interval(std::min(getMin(), rhs),
                             std::max(getMax(), rhs));
        }
    }
    return *this;
}

template <typename T>
Interval<T> & Interval<T>::dilateBy(Scalar rhs) {
    if (!isEmpty() && rhs == rhs) {
        // TODO: check for integer overflow?
        *this = Interval(getMin() - rhs, getMax() + rhs);
    }
    return *this;
}


template <typename T>
std::ostream & operator<<(std::ostream & os, Interval<T> const & interval) {
    using Traits = detail::Traits<T>;
    os << Traits::getScalarName() << "Interval(min=";
    Traits::write(os, interval.getMin());
    os << ", max=";
    Traits::write(os, interval.getMax());
    return os << ")";
}


LSST_GEOM_INTERVAL_TEMPLATES();

}} // namespace lsst::geom
