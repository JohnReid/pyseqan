#
# Copyright John Reid 2013
#

"""
Test building an index.
"""

from utils import fasta_file
import seqan
import logging
from copy import copy

num_bases, sequences, ids = seqan.readFastaDna5(fasta_file('dm01r.fasta'))
index = seqan.IndexEsaDna5(sequences)

def descend(i):
    if len(i.representative) < 3:
        logging.info('%-2s : %5d', i.representative, i.countOccurrences)
        #1/0
        if i.goDown():
            while True:
                descend(copy(i))
                if not i.goRight():
                    break

descend(index.TopDownIterator(index))
