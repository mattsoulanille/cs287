from distutils.core import setup, Extension
import os
os.environ["CC"] = "gcc-6"

setup(
    ext_modules=[Extension("_matrix",
                           ["_matrix.c", "matrixModule.c"],
                           extra_compile_args=["-fopenmp"],
                           extra_link_args=["-fopenmp"])]

    #ext_modules=[Extension("_darts", ["_darts.c"])]
)
    
