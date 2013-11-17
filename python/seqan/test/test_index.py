#
# Copyright John Reid 2013
#

"""
Test building an index.
"""

import seqan, sys, logging
from seqan.test import fasta_file, show_shallow_tree


IndexesDNA5 = (seqan.IndexStringDNA5SetESA, seqan.IndexStringDNA5SetWOTD)

def test_empty_index():
    logging.info(sys._getframe().f_code.co_name)
    for IndexDNA5 in IndexesDNA5:
        sequences = seqan.StringDNA5Set()
        sequences.appendValue(seqan.StringDNA5('A'))
        index = IndexDNA5(sequences)
        del sequences
        _i = index.TopDownIterator(index)
        #assert not i.goDownChar('A')


def test_index_as_strings_custodian():
    logging.info(sys._getframe().f_code.co_name)
    for IndexDNA5 in IndexesDNA5:
        sequences = seqan.StringDNA5Set()
        sequences.appendValue(seqan.StringDNA5('ACG'))
        index = IndexDNA5(sequences)
        del sequences
        i = index.TopDownIterator(index)
        i.goDownChar('A')


def test_build_index():
    logging.info(sys._getframe().f_code.co_name)
    for IndexDNA5 in IndexesDNA5:
        _num_bases, sequences, _ids = seqan.readFastaDNA5(fasta_file('dm01r.fasta'))
        index = IndexDNA5(sequences)
        show_shallow_tree(index.TopDownIterator(index))


def test_save_load_index():
    logging.info(sys._getframe().f_code.co_name)
    for IndexDNA5 in IndexesDNA5:
        _num_bases, sequences, _ids = seqan.readFastaDNA5(fasta_file('dm01r.fasta'))
        index = IndexDNA5(sequences)
        index.save('index.saved')
        index2 = IndexDNA5.load('index.saved')
        i = index2.topdown()
        i.goDownChar('A')
        text = index2.text
        print text[0][:4]
        
        
def test_index_text():
    logging.info(sys._getframe().f_code.co_name)
    for IndexDNA5 in IndexesDNA5:
        _num_bases, sequences, _ids = seqan.readFastaDNA5(fasta_file('dm01r.fasta'))
        index = IndexDNA5(sequences)
        assert len(index.text) == len(_ids)
        assert len(index.text[0]) == len(sequences[0])


def test_infix_comparison():
    logging.info(sys._getframe().f_code.co_name)
    for IndexDNA5 in IndexesDNA5:
        _num_bases, sequences, _ids = seqan.readFastaDNA5(fasta_file('dm01r.fasta'))
        index = IndexDNA5(sequences)
        i = index.TopDownIterator(index)
        i.goDownChar('A')
        i.goDownChar('T')
        i.goDownChar('C')
        assert 'ATC' == i.representative, i.representative

