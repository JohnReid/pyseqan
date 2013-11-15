#
# Copyright John Reid 2013
#

"""
Test building an index.
"""

from seqan.test import fasta_file
import seqan
import logging
from copy import copy

def descend(i):
    if len(i.representative) < 3:
        logging.info('%-2s : %5d', i.representative, i.countOccurrences)
        if i.goDown():
            while True:
                descend(copy(i))
                if not i.goRight():
                    break

def test_build_index():
    _num_bases, sequences, _ids = seqan.readFastaDNA5(fasta_file('dm01r.fasta'))
    index = seqan.IndexStringDNA5SetESA(sequences)
    descend(index.TopDownIterator(index))
