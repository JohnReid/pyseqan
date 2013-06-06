#
# Copyright John Reid 2013
#

"""
Test building an index.
"""

import seqan, sys, logging
from . import fasta_file, show_shallow_tree

def test_build_index():
    logging.info(sys._getframe().f_code.co_name)
    _num_bases, sequences, _ids = seqan.readFastaDna5(fasta_file('dm01r.fasta'))
    index = seqan.IndexEsaDna5(sequences)
    show_shallow_tree(index.TopDownIterator(index))



def test_infix_comparison():
    logging.info(sys._getframe().f_code.co_name)
    _num_bases, sequences, _ids = seqan.readFastaDna5(fasta_file('dm01r.fasta'))
    index = seqan.IndexEsaDna5(sequences)
    i = index.TopDownIterator(index)
    i.goDownChar('A')
    i.goDownChar('T')
    i.goDownChar('C')
    assert 'ATC' == i.representative, i.representative

