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
#ifndef LSST_PYUTILS_H_
#define LSST_PYUTILS_H_

#include "pybind11/pybind11.h"
#include <memory>
#include <sstream>

namespace lsst { namespace pyutils {


class Wrapper {
public:

    Wrapper() = default;

    Wrapper(Wrapper const &) = delete;
    Wrapper(Wrapper &&) = delete;

    Wrapper & operator=(Wrapper const &) = delete;
    Wrapper & operator=(Wrapper &&) = delete;

    virtual void define() = 0;

    virtual ~Wrapper() = default;
};

using WrapperPtr = std::unique_ptr<Wrapper>;


template <typename PyClass>
void addOutputOp(PyClass & cls, std::string const & method) {
    cls.def(
        method.c_str(),
        [](typename PyClass::type const & self) {
            std::ostringstream os;
            os << self;
            return os.str();
        }
    );
}


}} // namespace lsst::pyutils

#endif // !LSST_WRAPPER_H_
