#!/usr/bin/env python

import os
import argparse

BASE_VARIABLES = ("PATH", "PYTHONPATH", "LD_LIBRARY_PATH")


def main(filename, variables):
    variables = list(variables)
    variables.extend(var for var in os.environ
                     if (var.endswith("DIR") and f"SETUP_{var[:-4]}" in os.environ))
    with open(filename, "w") as f:
        for var in variables:
            if var != "LD_LIBRARY_PATH":
                f.write(f"{var}={os.environ[var]}\n")
            else:
                f.write("LD_LIBRARY_PATH=/home/kannawad/repo/meas_extensions_gaap/lib:" +
                        f"{os.environ['LD_LIBRARY_PATH']}\n")


if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description=("Write selected variables from the current environment "
                     "into a Visual Studio Code environemnt files.")
    )
    parser.add_argument("-f", "--filename", default=".env",
                        help="Filename to write")
    parser.add_argument("-v", "--variable", default=list(BASE_VARIABLES),
                        action="append", dest="variables",
                        help=("An additional variables to export; may be "
                              "provided multiple times."))
    args = parser.parse_args()
    main(args.filename, args.variables)
