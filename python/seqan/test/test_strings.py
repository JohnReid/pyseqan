#
# Copyright John Reid 2013
#

"""
Test string functionality.
"""

from . import fasta_file
import logging, seqan, sys
from itertools import combinations

def test_strings():
    logging.info(sys._getframe().f_code.co_name)
    _num_bases, sequences, ids = seqan.readFastaDna5(fasta_file('dm01r.fasta'))
    
    logging.info('Size of Dna5 alphabet %d', seqan.Dna5.valueSize )
    logging.info('Size of Dna alphabet %d', seqan.Dna.valueSize )
    logging.info('Length of sequence %s %d', ids[3], len(sequences[3]))
    logging.info('Tenth base of sequence %s %s', ids[2], sequences[2].value(9))
    logging.info('Infix of sequence %s %s', ids[2], sequences[2].infix(9, 14))
    
    
    _s5 = seqan.StringDna5('ACGTACGTACGTACGT')
    s4 = seqan.StringDna('ACGTACGTACGTACGT')
    s = s4
    infix = s.infix(3, 9)
    slice_ = s[3:9]
    logging.info('Infix %s', infix)
    logging.info(type(infix))
    logging.info('Slice %s', slice_)
    logging.info(type(slice_))
    
    # check object lifetimes are respected
    del s
    logging.info('Infix %s', infix)
    logging.info('Slice %s', slice_)
    
    #
    # Check iteration using __getitem__ works correctly
    #
    s = sequences[0]
    chars = set()
    for i, c in enumerate(s):
        if i >= len(s):
            assert False
        chars.add(c)
    assert len(chars) <= 4
    assert i == len(s) - 1
    
    
    s5 = seqan.StringDna5('NACGTNNACGTNACGTNACGT')
    print s5[0] == 'N'
    assert s5[0] == 'N'
    print s5[0] != 'G'
    assert s5[0] != 'G'
    print s5[1] != 'N'
    assert s5[1] != 'N'
    print s5[1] == 'A'
    assert s5[1] == 'A'
    print 'N' == s5[0]
    assert 'N' == s5[0]
    print 'G' != s5[0]
    assert 'G' != s5[0]
    print 'N' != s5[1]
    assert 'N' != s5[1]
    print 'A' == s5[1]
    assert 'A' == s5[1]


def test_string_set_iteration():
    logging.info(sys._getframe().f_code.co_name)
    stringset = seqan.StringDna5Set()
    stringset.appendValue(seqan.StringDna5('ACGTACGTACGTNNN'))
    i = iter(stringset)
    try:
        while True:
            logging.info(len(i.next()))
    except StopIteration:
        pass
    map(len, stringset)


def infix_maker(str_type):
    def make_infix(s):
        string = str_type(s)
        infix = string[:]
        return infix
    return make_infix


def check_comparisons(str_type_1, str_type_2):
    a1, a2 = str_type_1('A'), str_type_2('A')
    a3 = a1[0]
    assert isinstance(a1, str) or type(a3) == a1.Value
    g1, g2 = str_type_1('G'), str_type_2('G')
    g3 = g1[0]
    logging.info('Checking %s(%s) against %s(%s)', type(a1), a1, type(a2), a2)
    assert a1 == a1
    assert a1 == a2
    assert a1 == a3, '%s != %s' % (a1, a3)
    assert a3 == a1, '%s != %s' % (a3, a1)
    assert a1 != g1
    assert a1 != g2
    assert a1 != g3
    assert a1 != g3, '%s == %s' % (a1, g3)
    assert g3 != a1, '%s == %s' % (g3, a1)


def check_comparisons_both_ways(str_type_1, str_type_2):
    check_comparisons(str_type_1, str_type_2)
    check_comparisons(str_type_2, str_type_1)


def test_string_comparison():
    logging.info(sys._getframe().f_code.co_name)
    for str_type_1, str_type_2 in combinations((str, seqan.StringDna, infix_maker(seqan.StringDna)), 2):
        check_comparisons_both_ways(str_type_1, str_type_2)
