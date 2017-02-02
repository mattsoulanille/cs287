from distutils.core import setup, Extension


setup(
    ext_modules=[Extension("_darts", ["_darts.c", "dartsmodule.c"])]
)
    
