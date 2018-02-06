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
#ifndef LSST_GEOM_XYZ_H_
#define LSST_GEOM_XYZ_H_

#include <array>

namespace lsst { namespace geom {


class AccessXYZ {
public:

    template <typename T,typename Derived>
    static T get(Derived const & self, int i) { return self[i]; }

    template <typename T,typename Derived>
    static void set(Derived & self, int i, T value) { return self[i] = value; }

};


template <typename Derived, typename T, int N>
class GetXYZ {};

template <typename Derived, typename T>
class GetXYZ<Derived, T, 1> {
public:
    T getX() const { return AccessXYZ::get(static_cast<Derived const &>(*this), 0); }
};

template <typename Derived, typename T>
class GetXYZ<Derived, T, 2> {
public:
    T getX() const { return AccessXYZ::get(static_cast<Derived const &>(*this), 0); }
    T getY() const { return AccessXYZ::get(static_cast<Derived const &>(*this), 1); }
};

template <typename Derived, typename T>
class GetXYZ<Derived, T, 3> {
public:
    T getX() const { return AccessXYZ::get(static_cast<Derived const &>(*this), 0); }
    T getY() const { return AccessXYZ::get(static_cast<Derived const &>(*this), 1); }
    T getZ() const { return AccessXYZ::get(static_cast<Derived const &>(*this), 2); }
};


template <typename Derived, typename T, int N>
class GetSetXYZ {};

template <typename Derived, typename T>
class GetSetXYZ<Derived, T, 1> {
public:
    void setX(T value) { AccessXYZ::set(static_cast<Derived &>(*this), 0, value); }
};

template <typename Derived, typename T>
class GetSetXYZ<Derived, T, 2> {
public:
    void setX(T value) { AccessXYZ::set(static_cast<Derived &>(*this), 0, value); }
    void setY(T value) { AccessXYZ::set(static_cast<Derived &>(*this), 1, value); }
};

template <typename Derived, typename T>
class GetSetXYZ<Derived, T, 3> {
public:
    void setX(T value) { AccessXYZ::set(static_cast<Derived &>(*this), 0, value); }
    void setY(T value) { AccessXYZ::set(static_cast<Derived &>(*this), 1, value); }
    void setZ(T value) { AccessXYZ::set(static_cast<Derived &>(*this), 2, value); }
};


template <typename T, int N>
class ArrayXYZ {
protected:

    using Array = std::array<T, N>;

    constexpr ArrayXYZ() : _array() {}

    Array _array;
};

template <typename T>
class ArrayXYZ<T, 1> {
public:

    explicit constexpr ArrayXYZ(T x) : _array({x}) {}

protected:

    using Array = std::array<T, 1>;

    constexpr ArrayXYZ() : _array() {}

    Array _array;
};


template <typename T>
class ArrayXYZ<T, 2> {
public:

    constexpr ArrayXYZ(T x, T y) : _array({x, y}) {}

protected:

    using Array = std::array<T, 2>;

    constexpr ArrayXYZ() : _array() {}

    Array _array;
};



template <typename T>
class ArrayXYZ<T, 3> {
public:

    constexpr ArrayXYZ(T x, T y, T z) : _array({x, y, z}) {}

protected:

    using Array = std::array<T, 3>;

    constexpr ArrayXYZ() : _array() {}

    Array _array;
};


}} // namespace lsst::geom

#endif // !LSST_GEOM_XYZ_H_
