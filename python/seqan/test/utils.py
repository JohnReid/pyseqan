#
# Copyright John Reid 2013
#

"""
Generic setup code shared between tests.
"""

import os, sys

def fasta_dir():
    return os.path.normpath(os.path.join(os.path.dirname(__file__), 'fasta'))

def fasta_file(filename):
    return os.path.join(fasta_dir(), filename)

def is_debug_python():
    "@return: True iff is a debug python."
    return hasattr(sys, "gettotalrefcount") # only in python debug build
