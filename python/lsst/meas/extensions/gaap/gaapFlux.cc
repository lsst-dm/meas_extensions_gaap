/*
 * LSST Data Management System
 * Copyright 2008-2017  AURA/LSST.
 *
 * This product includes software developed by the
 * LSST Project (http://www.lsst.org/).
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

#include <memory>

#include "lsst/pex/config/python.h"
#include "lsst/meas/base/python.h"

#include "lsst/meas/base/GaussianFlux.h"
#include "lsst/meas/extensions/gaap/GaapFlux.h"

namespace py = pybind11;
using namespace pybind11::literals;

namespace lsst {
namespace meas {
namespace extensions {
namespace gaap {

namespace {


using PyFluxAlgorithm =
        py::class_<GaapFluxAlgorithm, std::shared_ptr<GaapFluxAlgorithm>, base::SimpleAlgorithm>;
using PyFluxControl = py::class_<GaapFluxControl>;

using PyFluxTransform =
        py::class_<GaapFluxTransform, std::shared_ptr<GaapFluxTransform>, base::BaseTransform>;

void declareGaapFluxControl(py::module &mod) {
    PyFluxControl cls(mod, "GaapFluxControl");

    cls.def(py::init<>());

    LSST_DECLARE_CONTROL_FIELD(cls, GaapFluxControl, background);
}

void declareGaapFluxAlgorithm(py::module &mod) {
    PyFluxAlgorithm cls(
            mod, "GaapFluxAlgorithm");
    cls.def_static("getFlagDefinitions", &GaapFluxAlgorithm::getFlagDefinitions,
                   py::return_value_policy::copy);
    cls.attr("FAILURE") = py::cast(GaapFluxAlgorithm::FAILURE);

    cls.def(py::init<GaapFluxAlgorithm::Control const &, std::string const &, afw::table::Schema &>(),
            "ctrl"_a, "name"_a, "schema"_a);

    // cls.attr("FAILURE") = py::cast(GaapFluxAlgorithm::FAILURE);

    cls.def("measure", &GaapFluxAlgorithm::measure, "measRecord"_a, "exposure"_a);
    cls.def("fail", &GaapFluxAlgorithm::fail, "measRecord"_a, "error"_a = NULL);
}

void declareGaapFluxTransform(py::module &mod) {
    PyFluxTransform cls(mod, "GaapFluxTransform");

    cls.def(py::init<GaapFluxTransform::Control const &, std::string const &,
                     afw::table::SchemaMapper &>(),
            "ctrl"_a, "name"_a, "mapper"_a);
}

}  // namespace

PYBIND11_MODULE(gaapFlux, mod) {
    py::module::import("lsst.afw.table");
    py::module::import("lsst.meas.base.algorithm");
    py::module::import("lsst.meas.base.flagHandler");
    py::module::import("lsst.meas.base.transform");

    declareGaapFluxControl(mod);
    declareGaapFluxAlgorithm(mod);
    declareGaapFluxTransform(mod);

    /*
    clsFluxAlgorithm.attr("Control") = clsFluxControl;
    clsFluxTransform.attr("Control") = clsFluxControl;

    python::declareAlgorithm<GaapFluxAlgorithm, GaapFluxControl, GaapFluxTransform>(
            clsFluxAlgorithm, clsFluxControl, clsFluxTransform);
    */
}

}  // namespace gaap
}  // namespace extensions
}  // namespace meas
}  // namespace lsst
