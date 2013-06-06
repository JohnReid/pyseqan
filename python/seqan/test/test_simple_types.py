#
# Copyright John Reid 2013
#

"""
Test simple type functionality.
"""

from seqan import Dna, Dna5, StringDna5
import logging, sys

def assert_eq(x1, x2):
    assert x1 == x2, '(%s) %s != %s (%s)' % (type(x1).__name__, x1, x2, type(x2).__name__)
    

def assert_ne(x1, x2):
    assert x1 != x2, '(%s) %s == %s (%s)' % (type(x1).__name__, x1, x2, type(x2).__name__)
    

def check_n(x):
    logging.info('%s %s', x, type(x))
    assert_eq(x, 'N')
    assert_ne(x, 'G')
    assert_eq(x, Dna5('N'))
    assert_ne(x, Dna5('G'))
    assert_ne(x, Dna('G'))


def check_g(x):
    logging.info('%s %s', x, type(x))
    assert_ne(x, 'N')
    assert_eq(x, 'G')
    assert_ne(x, Dna5('N'))
    assert_eq(x, Dna5('G'))


def test_n():
    logging.info(sys._getframe().f_code.co_name)    
    check_n(Dna5('N'))
    check_n('N')
    for x in 'N':
        check_n(x)
    for x in StringDna5('N'):
        check_n(x)
        
def test_g():
    logging.info(sys._getframe().f_code.co_name)    
    check_g(Dna5('G'))
    check_g('G')
    for x in 'G':
        check_g(x)
    for x in StringDna5('G'):
        check_g(x)

def test_dna_against_dna5():
    assert_eq('G', Dna('G'))
    assert_eq('G', Dna5('G'))
    assert_ne(Dna('G'), Dna5('G')) # TODO: would be nice if these compared equal
    assert_eq(Dna('G').ordValue, Dna5('G').ordValue)
