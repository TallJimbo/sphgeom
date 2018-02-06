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
#define LSST_GEOM_VECTOR_CC_

#include <algorithm>
#include "lsst/geom/Vector.h"

namespace lsst { namespace geom {

template <typename T, int N>
bool Vector<T, N>::operator==(Vector const & rhs) const {
    return std::equal(begin(), end(), rhs.begin());
}

namespace {

template <typename Iter1, typename Iter2, typename BinaryFunction>
void for_each_zip(Iter1 i1, Iter1 const e1, Iter2 i2, BinaryFunction f) {
    for (; i1 != e1; ++i1, ++i2) {
        f(*i1, *i2);
    }
}

} // anonymous


template <typename T, int N>
Vector<T, N> & Vector<T, N>::operator+=(Vector const & rhs) {
    for_each_zip(begin(), end(), rhs.begin(), [](T & a, T b) { a += b; });
    return *this;
}

template <typename T, int N>
Vector<T, N> & Vector<T, N>::operator-=(Vector const & rhs) {
    for_each_zip(begin(), end(), rhs.begin(), [](T & a, T b) { a -= b; });
    return *this;
}

template <typename T, int N>
Vector<T, N> & Vector<T, N>::operator*=(Scalar rhs) {
    std::for_each(begin(), end(), [rhs](T & a) { a *= rhs; });
    return *this;
}

template <typename T, int N>
T Vector<T, N>::computeSquaredNorm() const {
    return std::inner_product(begin(), end(), begin(), T(0));
}

template <typename T, int N>
template <typename U>
void Vector<T, N>::_convertFrom(Vector<U, N> const & other) {
    std::transform(other.begin(), other.end(), begin(),
                   typename Traits::template ConvertFrom<U>());
}

template <int N>
Real dot(Point<N> const & a, Point<N> const & b) {
    return std::inner_product(a.begin(), a.end(), b.begin(), Real(0));
}

template <int N>
Real dot(Index<N> const & a, Point<N> const & b) {
    return std::inner_product(a.begin(), a.end(), b.begin(), Real(0));
}

template <int N>
Real dot(Point<N> const & a, Index<N> const & b) {
    return std::inner_product(a.begin(), a.end(), b.begin(), Real(0));
}

template <int N>
Integer dot(Index<N> const & a, Index<N> const & b) {
    return std::inner_product(a.begin(), a.end(), b.begin(), Integer(0));
}


LSST_GEOM_VECTOR_TEMPLATES();

}} // namespace lsst::geom
