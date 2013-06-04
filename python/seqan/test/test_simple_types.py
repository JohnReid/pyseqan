#
# Copyright John Reid 2013
#

"""
Test string functionality.
"""

from seqan import Dna, Dna5, StringDna5

def check_n(n):
    print n, type(n)
    assert n == 'N'
    assert n != 'G'
    assert n == Dna5('N')
    assert n != Dna5('G')
    assert n != Dna('G')

def test_n():
    check_n(Dna5('N'))
    check_n('N')
    for n in 'N':
        check_n(n)
    for n in StringDna5('N'):
        check_n(n)
    