#
# Copyright John Reid 2013
#

"""
Generic setup code shared between tests.
"""

import logging, os, sys
from copy import copy

def init_env(f, level=logging.DEBUG):
    "Initialise test environment."
    logging.basicConfig(level=level, format="%(asctime)s - %(levelname)s - %(message)s")
    # logging.getLogger('').addHandler(logging.FileHandler('%s.log' % os.path.splitext(os.path.basename(file))[0]))
    logging.info('Command line: %s', ' '.join(sys.argv))


def append_to_path(d):
    logging.info('Appending to sys.path: %s', d)
    sys.path.append(d) # stempy


def prepend_to_path(d):
    logging.info('Prepending to sys.path: %s', d)
    sys.path.insert(0, d) # stempy


def log_path(level=logging.DEBUG):
    for d in sys.path:
        logging.log(level, 'PYTHONPATH: %s', d)


def update_path_for_seqan():
    d = os.path.dirname(__file__)
#    prepend_to_path(os.path.normpath(os.path.join(d, '..', '..', '..', '..', 'Infpy', 'python'))) # Infpy
#    prepend_to_path(os.path.normpath(os.path.join(d, '..', '..', '..', '..', 'PyICL', 'Python'))) # PyICL
    prepend_to_path(os.path.normpath(os.path.join(d, '..', '..', '..', '..', '..', 'Python', 'Cookbook', 'python'))) # cookbook
    prepend_to_path(os.path.normpath(os.path.join(d, '..', '..'))) # seqan
    #print sys.path


def fasta_dir():
    return os.path.normpath(os.path.join(os.path.dirname(__file__), 'fasta'))


def fasta_file(filename):
    return os.path.join(fasta_dir(), filename)


def is_debug_python():
    "@return: True iff is a debug python."
    return hasattr(sys, "gettotalrefcount") # only available in python debug build


def show_shallow_tree(i):
    if len(i.representative) < 3:
        logging.info('%-2s : %5d', i.representative, i.countOccurrences)
        if i.goDown():
            while True:
                show_shallow_tree(copy(i)) 
                if not i.goRight():
                    break


