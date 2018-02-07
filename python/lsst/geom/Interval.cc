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
#include "pybind11/pybind11.h"
#include "pybind11/stl.h"

#include <memory>

#include "lsst/pyutils.h"
#include "lsst/geom/Interval.h"

namespace py = pybind11;

using namespace pybind11::literals;

namespace lsst { namespace geom {

namespace {

template <typename T>
using PyInterval = py::class_<Interval<T>>;

class IntervalWrapper : public pyutils::Wrapper {
public:

    IntervalWrapper(PyInterval<Real> && clsReal, PyInterval<Integer> && clsInteger) :
        _clsReal(std::move(clsReal)),
        _clsInteger(std::move(clsInteger))
    {}

    template <typename T>
    void defineT(PyInterval<T> & cls) {
        cls.def_static("makeFull", &Interval<T>::makeFull);
        cls.def_static("makeEmpty", &Interval<T>::makeEmpty);
        cls.def_static(
            "makeHull",
            static_cast<Interval<T> (*)(std::vector<T> const &)>(&Interval<T>::makeHull)
        );
        cls.def(py::init<>());
        cls.def(py::init<T, T>(), "min"_a, "max"_a);
        cls.def_property_readonly("min", &Interval<T>::getMin);
        cls.def_property_readonly("max", &Interval<T>::getMax);
        cls.def("isEmpty", &Interval<T>::isEmpty);
        cls.def_property_readonly("size", &Interval<T>::getSize);
        cls.def_property_readonly("center", &Interval<T>::getCenter);

        using IntervalPredicate = bool (Interval<T>::*)(Interval<T> const & other) const;
        using ScalarPredicate = bool (Interval<T>::*)(T other) const;
        cls.def("__eq__", static_cast<IntervalPredicate>(&Interval<T>::operator==), py::is_operator());
        cls.def("__ne__", static_cast<IntervalPredicate>(&Interval<T>::operator!=), py::is_operator());
        cls.def("__eq__", static_cast<ScalarPredicate>(&Interval<T>::operator==), py::is_operator());
        cls.def("__ne__", static_cast<ScalarPredicate>(&Interval<T>::operator!=), py::is_operator());

        cls.def("contains", static_cast<IntervalPredicate>(&Interval<T>::contains));
        cls.def("contains", static_cast<ScalarPredicate>(&Interval<T>::contains));
        cls.attr("__contains__") = cls.attr("contains");
        cls.def("isDisjointFrom", static_cast<IntervalPredicate>(&Interval<T>::isDisjointFrom));
        cls.def("isDisjointFrom", static_cast<ScalarPredicate>(&Interval<T>::isDisjointFrom));
        cls.def("intersects", static_cast<IntervalPredicate>(&Interval<T>::intersects));
        cls.def("intersects", static_cast<ScalarPredicate>(&Interval<T>::intersects));
        cls.def("isWithin", static_cast<IntervalPredicate>(&Interval<T>::isWithin));
        cls.def("isWithin", static_cast<ScalarPredicate>(&Interval<T>::isWithin));
        cls.def(
            "relate",
            static_cast<Relationship (Interval<T>::*)(Interval<T> const &) const>(&Interval<T>::relate)
        );
        cls.def(
            "relate",
            static_cast<Relationship (Interval<T>::*)(T) const>(&Interval<T>::relate)
        );

        using IntervalOp = Interval<T> (Interval<T>::*)(Interval<T> const &) const;
        using ScalarOp = Interval<T> (Interval<T>::*)(T) const;
        cls.def("clippedTo", static_cast<IntervalOp>(&Interval<T>::clippedTo));
        cls.def("clippedTo", static_cast<ScalarOp>(&Interval<T>::clippedTo));
        cls.def("expandedTo", static_cast<IntervalOp>(&Interval<T>::expandedTo));
        cls.def("expandedTo", static_cast<ScalarOp>(&Interval<T>::expandedTo));
        cls.def("dilatedBy", static_cast<ScalarOp>(&Interval<T>::dilatedBy));
        cls.def("erodedBy", static_cast<ScalarOp>(&Interval<T>::erodedBy));

        pyutils::addOutputOp(cls, "__repr__");
    }

    void define() override {
        _clsReal.attr("Scalar") = py::reinterpret_borrow<py::object>(
            reinterpret_cast<PyObject*>(&PyFloat_Type)
        );
        _clsInteger.attr("Scalar") = py::reinterpret_borrow<py::object>(
            reinterpret_cast<PyObject*>(&PyLong_Type)
        );
        defineT(_clsReal);
        defineT(_clsInteger);
        _clsReal.def(py::init<IntegerInterval const &>());
        _clsInteger.def(py::init<RealInterval const &>());
        py::implicitly_convertible<IntegerInterval, RealInterval>();
        _clsReal.def(
            "__str__",
            [](RealInterval & self) {
                return py::str("[{}, {}]").format(self.getMin(), self.getMax());
            }
        );
        _clsInteger.def(
            "__str__",
            [](IntegerInterval & self) {
                return py::str("{}..{}").format(self.getMin(), self.getMax());
            }
        );
    }

private:
    PyInterval<Real> _clsReal;
    PyInterval<Integer> _clsInteger;
};

} // anonymous

pyutils::WrapperPtr declareInterval(py::handle scope) {
    return std::make_unique<IntervalWrapper>(
        PyInterval<Real>(scope, "RealInterval"),
        PyInterval<Integer>(scope, "IntegerInterval")
    );
}

}}  // lsst::geom
