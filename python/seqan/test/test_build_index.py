#
# Copyright John Reid 2013, 2014
#

"""
Test building an index.
"""

from seqan.test import fasta_file
import seqan.traverse
import logging


def visitvertex(it):
    if len(it.representative) >= 3:
        return False
    logging.info('%-2s : %5d', it.representative, it.numOccurrences)
    return True


def test_build_index():
    _num_bases, sequences, _ids = seqan.readFastaDNA5(fasta_file('dm01r.fasta'))
    index = seqan.IndexStringDNA5SetESA(sequences)
    seqan.traverse.topdownhistorytraversal(index.topdownhistory(), visitvertex)
    seqan.traverse.topdowncopytraversal(index.topdown(), visitvertex)
