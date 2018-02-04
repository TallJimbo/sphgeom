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

#include <iostream>
#include <iomanip>

#include "lsst/geom/scalars.h"

namespace lsst { namespace geom { namespace detail {

void Traits<Integer>::write(std::ostream & os, Integer i) {
    os << i;
}

void Traits<Real>::write(std::ostream & os, Real r) {
    auto oldFlags = os.flags();
    auto oldPrecision = os.precision();
    os.precision(std::numeric_limits<Real>::max_digits10 - 1);
    os << std::scientific << r;
    os.precision(oldPrecision);
    os.flags(oldFlags);
}

}}} // namespace lsst::geom::detail
