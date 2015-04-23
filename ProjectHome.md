# An simplified MPI binding for NCL #
NCL (NCAR Command Language) is a nice scripting language for data analysis and visualization. Sadly it lacks the possibility of parallelism.

This project will provide a simplified MPI binding in the form of a shared library witch can be loaded by NCL at runtime. This library won't contain the functionality of the complete MPI standard, but the most important features will be supported.

As NCL is available on many different Platforms, this library will only be distributed as source code.