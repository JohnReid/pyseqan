#!/usr/bin/env python
# -*- coding: latin-1 -*-
#
# Copyright John Reid 2010, 2011, 2012, 2013
#

"""
aksetup setup script for pyseqan. Adapted from http://git.tiker.net/pyublas.git/tree.
"""

import os

def read(*fnames):
    """
    Utility function to read the README file.
    Used for the long_description.  It's nice, because now 1) we have a top level
    README file and 2) it's easier to type in the README file than to put a raw
    string in below ...
    """
    return open(os.path.join(os.path.dirname(__file__), *fnames)).read()



def get_config_schema():
    from aksetup_helper import ConfigSchema, BoostLibraries, \
            StringListOption, make_boost_base_options
    import sys

    if 'darwin' in sys.platform:
        default_libs = []
        default_cxxflags = ['-arch', 'i386', '-arch', 'x86_64',
                '-isysroot', '/Developer/SDKs/MacOSX10.6.sdk']
        default_ldflags = default_cxxflags[:]
    else:
        default_cxxflags = []
        default_ldflags = []

    return ConfigSchema(
        make_boost_base_options() + [
            BoostLibraries("python"),

            StringListOption("CXXFLAGS", ["-Wno-sign-compare"],
                help="Any extra C++ compiler options to include"),
            StringListOption("LDFLAGS", [],
                help="Any extra linker options to include"),
        ]
    )




def main():
    from aksetup_helper import hack_distutils, get_config, setup, NumpyExtension
    from setuptools import find_packages

    hack_distutils()
    conf = get_config(get_config_schema())

    INCLUDE_DIRS = ['C++/myrrh'] + conf['BOOST_INC_DIR']
    LIBRARY_DIRS = conf['BOOST_LIB_DIR']
    LIBRARIES = conf['BOOST_PYTHON_LIBNAME']
    EXTRA_DEFINES = { }

    try:
        from distutils.command.build_py import build_py_2to3 as build_py
    except ImportError:
        # 2.x
        from distutils.command.build_py import build_py

    #
    # C++ extension
    #
    seqanext = NumpyExtension(
        'seqan._seqan',
        [
            'c++/src/expose_indexes.cpp',
            'c++/src/expose_simple_types.cpp',
            'c++/src/expose_strings.cpp',
            'c++/src/module_pyseqan.cpp',
        ],
        include_dirs         = INCLUDE_DIRS,
        library_dirs         = LIBRARY_DIRS,
        libraries            = LIBRARIES,
        define_macros        = list(EXTRA_DEFINES.items()),
        extra_compile_args   = conf['CXXFLAGS'],
        extra_link_args      = conf['LDFLAGS'],
    )

    #
    # Main setup
    #
    setup(
        name                 = 'pyseqan',
        version              = read('python', 'seqan', 'VERSION').strip().split('-')[0],
        description          = 'pyseqan: A package to expose SeqAn suffix array functionality.',
        long_description     = read('python', 'seqan', 'README'),
        author               ='John Reid',
        author_email         ='johnbaronreid@netscape.net',
        license              = 'BSD',
        url                  ='http://sysbio.mrc-bsu.cam.ac.uk/johns/pyseqan/docs/build/html/index.html',
        classifiers          = [
            'Development Status :: 5 - Production/Stable',
            'Environment :: Console',
            'Intended Audience :: Developers',
            'Intended Audience :: Science/Research',
            'License :: OSI Approved :: BSD License',
            'Operating System :: MacOS :: MacOS X',
            'Operating System :: POSIX',
            'Operating System :: Microsoft :: Windows',
            'Programming Language :: Python',
            'Programming Language :: C++',
            'Topic :: Scientific/Engineering',
            'Topic :: Scientific/Engineering :: Mathematics',
            'Topic :: Scientific/Engineering :: Bio-Informatics',
            'Topic :: Utilities',
        ],

        packages             = find_packages(where='python'),
        package_dir          = { '' : 'python' },
        #py_modules           = ['pybool.examples.tutorial']
        package_data         = { 'pyseqan': ['README', 'LICENSE', 'VERSION'] },
        install_requires     = [
                                'cookbook',
                               ],
        #scripts              = ['python/scripts/pybool-find-consistent'],
        ext_modules          = [seqanext],

        # 2to3 invocation
        cmdclass             = {'build_py': build_py},

        include_package_data = False,
    )





if __name__ == '__main__':
    main()

