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
#ifndef LSST_GEOM_SCALARS_H_
#define LSST_GEOM_SCALARS_H_

#include <cstdint>
#include <cmath>
#include <limits>
#include <utility>
#include <iosfwd>

namespace lsst { namespace geom {

using Integer = std::int32_t;
using Real = double;

namespace detail {

template <typename T> struct Traits;

template <>
struct Traits<Integer> {

    using Size = std::uint32_t;
    using Pair = std::pair<Integer, Integer>;
    using Limits = std::numeric_limits<Integer>;

    template <typename U> struct ConvertTo;

    template <typename U>
    using ConvertFrom = typename Traits<U>::template ConvertTo<Integer>;

    static std::string getScalarName() { return "Integer"; }

    static void write(std::ostream & os, Integer i);

    static Pair makeFullInterval() {
        return std::make_pair(Limits::min(), Limits::max());
    }

    static Pair makeEmptyInterval() {
        return std::make_pair(Limits::max(), Limits::min());
    }

    static bool isIntervalEmpty(Pair const & p) {
        return p.second < p.first;
    }

    static Size getIntervalSize(Pair const & p) {
        return isIntervalEmpty(p) ? 0 : (1 + p.second - p.first);
    }

    static void normalizeInterval(Pair & p) {
        if (!(p.second >= p.first)) {
            p = makeEmptyInterval();
        }
    }


};

template <>
struct Traits<Real> {

    using Size = Real;
    using Pair = std::pair<Real, Real>;
    using Limits = std::numeric_limits<Real>;

    template <typename U> struct ConvertTo;

    template <typename U>
    using ConvertFrom = typename Traits<U>::template ConvertTo<Real>;

    static std::string getScalarName() { return "Real"; }

    static void write(std::ostream & os, Real r);

    static Pair makeFullInterval() {
        return Pair(-Limits::infinity(), Limits::infinity());
    }

    static Pair makeEmptyInterval() {
        return Pair(Limits::quiet_NaN(), Limits::quiet_NaN());
    }

    static bool isIntervalEmpty(Pair const & p) {
        return !(p.first <= p.second);
    }

    static Size getIntervalSize(Pair const & p) {
        return isIntervalEmpty(p) ? 0 : p.second - p.first;
    }

    static void normalizeInterval(Pair & p) {
        if (!(p.second >= p.first)) {
            p = makeEmptyInterval();
        }
    }

};

template <>
struct Traits<Integer>::ConvertTo<Real> {

    using EnableImplicit = int;  // SFINAE for converting ctors; see e.g. Vector.

    Real operator()(Integer i) const { return i; }

    std::pair<Real, Real> operator()(std::pair<Integer, Integer> const & i) const {
        if (isIntervalEmpty(i)) {
            return Traits<Real>::makeEmptyInterval();
        }
        return std::make_pair(i.first - 0.5, i.second + 0.5);
    }

};


template <>
struct Traits<Real>::ConvertTo<Integer> {

    using EnableExplicit = unsigned int;  // SFINAE for converting ctors; see e.g. Vector.

    Integer operator()(Real r) const { return std::lround(r); }

    std::pair<Integer, Integer> operator()(std::pair<Real, Real> const & r) const {
        if (isIntervalEmpty(r)) {
            return Traits<Integer>::makeEmptyInterval();
        }
        return std::make_pair((*this)(r.first), (*this)(r.second));
    }

};

}}} // namespace lsst::geom::detail

#endif // !LSST_GEOM_SCALARS_H_
