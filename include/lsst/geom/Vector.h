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
#ifndef LSST_GEOM_VECTOR_H_
#define LSST_GEOM_VECTOR_H_

#include <array>
#include <cassert>

#include "lsst/geom/scalars.h"
#include "lsst/geom/XYZ.h"

namespace lsst { namespace geom {

template <typename T, int N>
class Vector : public GetSetXYZ<Vector<T, N>, T, N>, public ArrayXYZ<T, N> {
    using Base = ArrayXYZ<T, N>;
    using Array = typename Base::Array;
    using Traits = detail::Traits<T>;
    static_assert(N > 0, "0-d Vectors are not supported");
public:

    using Scalar = T;
    using value_type = typename Array::value_type;
    using reference = typename Array::reference;
    using const_reference = typename Array::const_reference;
    using pointer = typename Array::pointer;
    using iterator = typename Array::iterator;
    using const_iterator = typename Array::const_iterator;
    using difference_type = typename Array::difference_type;
    using size_type = typename Array::size_type;

    using Base::Base; // specialized ctors for N=1, N=2, N=3.

    template <typename U>
    Vector(
        Vector<U, N> const & other,
        typename Traits::template ConvertFrom<U>::EnableImplicit=0
    ) {
        _convertFrom(other);
    }

    template <typename U>
    explicit Vector(
        Vector<U, N> const & other,
        typename Traits::template ConvertFrom<U>::EnableExplicit=0
    ) {
        _convertFrom(other);
    }

    constexpr Vector() : Base() { this->_array.fill(0); }

    Vector(Vector const &) = default;
    Vector(Vector &&) = default;

    Vector & operator=(Vector const &) = default;
    Vector & operator=(Vector &&) = default;

    ~Vector() = default;

    constexpr size_type size() const noexcept { return N; }
    constexpr size_type max_size() const noexcept { return N; }
    constexpr bool empty() const noexcept { return false; }

    iterator begin() noexcept { return this->_array.begin(); }
    iterator end() noexcept { return this->_array.end(); }
    const_iterator begin() const noexcept { return this->_array.begin(); }
    const_iterator end() const noexcept { return this->_array.end(); }
    const_iterator cbegin() const noexcept { return this->_array.cbegin(); }
    const_iterator cend() const noexcept { return this->_array.cend(); }

    Scalar * data() noexcept { return this->_array.data(); }
    Scalar const * data() const noexcept { return this->_array.data(); }

    reference operator[](size_type i) {
        return this->_array[i];
    }
    const_reference operator[](size_type i) const {
        return this->_array[i];
    }

    bool operator==(Vector const & rhs) const;
    bool operator!=(Vector const & rhs) const { return !(*this == rhs); }

    Vector & operator+=(Vector const & rhs);
    Vector & operator-=(Vector const & rhs);
    Vector & operator*=(Scalar rhs);

    Vector operator+(Vector const & rhs) const { return Vector(*this) += rhs; }
    Vector operator-(Vector const & rhs) const { return Vector(*this) -= rhs; }
    Vector operator*(Scalar rhs) const { return Vector(*this) *= rhs; }

    friend Vector operator*(Scalar a, Vector const & b) { return b*a; }

    Scalar computeSquaredNorm() const;
    Real computeNorm() const { return std::sqrt(computeSquaredNorm()); }

private:
    template <typename U>
    void _convertFrom(Vector<U, N> const & other);
};

template <int N>
using Point = Vector<Real, N>;

template <int N>
using Index = Vector<Integer, N>;

using Point1 = Point<1>;
using Point2 = Point<2>;
using Point3 = Point<3>;

using Index1 = Index<1>;
using Index2 = Index<2>;
using Index3 = Index<3>;


// Index*Real.  Promotes to Point.
template <int N> inline Point<N> operator*(Index<N> const & a, Real b) { return Point<N>(a) *= b; }
template <int N> inline Point<N> operator*(Real a, Index<N> const & b) { return b*a; }

// Point/Real (and Point/Integer, via Integer->Real implicit conversion).
template <int N> inline Point<N> & operator/=(Point<N> & a, Real b) { return a *= (1.0/b); }
template <int N> inline Point<N> operator/(Point<N> const & a, Real b) { return a*(1.0/b); }

// Index/Real (and Index/Integer, via Integer->Real implicit conversion).  Promotes to Point.
template <int N> Point<N> operator/(Index<N> const & a, Real b) { return Point<N>(a) /= b; }

// Inner products.
template <int N> Real dot(Point<N> const & a, Point<N> const & b);
template <int N> Real dot(Index<N> const & a, Point<N> const & b);
template <int N> Real dot(Point<N> const & a, Index<N> const & b);
template <int N> Integer dot(Index<N> const & a, Index<N> const & b);


#define LSST_GEOM_VECTOR_TEMPLATES_N(PREFIX, N)\
    PREFIX template class Vector<Integer, N>;\
    PREFIX template class Vector<Real, N>;\
    PREFIX template void Vector<Integer, N>::_convertFrom(Vector<Real, N> const &);\
    PREFIX template void Vector<Real, N>::_convertFrom(Vector<Integer, N> const &);\
    PREFIX template Real dot(Point<N> const &, Point<N> const &);\
    PREFIX template Real dot(Index<N> const &, Point<N> const &);\
    PREFIX template Real dot(Point<N> const &, Index<N> const &);\
    PREFIX template Integer dot(Index<N> const &, Index<N> const &)

#define LSST_GEOM_VECTOR_TEMPLATES(PREFIX)\
    LSST_GEOM_VECTOR_TEMPLATES_N(PREFIX, 1);\
    LSST_GEOM_VECTOR_TEMPLATES_N(PREFIX, 2);\
    LSST_GEOM_VECTOR_TEMPLATES_N(PREFIX, 3)

#ifndef LSST_GEOM_VECTOR_CC_
    LSST_GEOM_VECTOR_TEMPLATES(extern);
    #undef LSST_GEOM_VECTOR_TEMPLATES
    #undef LSST_GEOM_VECTOR_TEMPLATES_N
#endif

}} // namespace lsst::geom

#endif // !LSST_GEOM_VECTOR_H_
