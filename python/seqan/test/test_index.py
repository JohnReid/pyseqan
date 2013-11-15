#
# Copyright John Reid 2013
#

"""
Test building an index.
"""

import seqan, sys, logging
from seqan.test import fasta_file, show_shallow_tree


def test_empty_index():
    logging.info(sys._getframe().f_code.co_name)
    sequences = seqan.StringDNA5Set()
    sequences.appendValue(seqan.StringDNA5('A'))
    index = seqan.IndexStringDNA5SetESA(sequences)
    del sequences
    _i = index.TopDownIterator(index)
    #assert not i.goDownChar('A')


def test_index_as_strings_custodian():
    logging.info(sys._getframe().f_code.co_name)
    sequences = seqan.StringDNA5Set()
    sequences.appendValue(seqan.StringDNA5('ACG'))
    index = seqan.IndexStringDNA5SetESA(sequences)
    del sequences
    i = index.TopDownIterator(index)
    i.goDownChar('A')


def test_build_index():
    logging.info(sys._getframe().f_code.co_name)
    _num_bases, sequences, _ids = seqan.readFastaDNA5(fasta_file('dm01r.fasta'))
    index = seqan.IndexStringDNA5SetESA(sequences)
    show_shallow_tree(index.TopDownIterator(index))


def test_infix_comparison():
    logging.info(sys._getframe().f_code.co_name)
    _num_bases, sequences, _ids = seqan.readFastaDNA5(fasta_file('dm01r.fasta'))
    index = seqan.IndexStringDNA5SetESA(sequences)
    i = index.TopDownIterator(index)
    i.goDownChar('A')
    i.goDownChar('T')
    i.goDownChar('C')
    assert 'ATC' == i.representative, i.representative

