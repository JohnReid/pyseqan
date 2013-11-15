#
# Copyright John Reid 2013
#

"""
Test assigning properties to nodes.
"""

from seqan.test import fasta_file
import logging
import seqan
from copy import copy

def test_property_map():
    _num_bases, sequences, _ids = seqan.readFastaDNA5(fasta_file('dm01r.fasta'))
    logging.info('Building index')
    index = seqan.IndexStringDNA5SetESA(sequences)
    logging.info('Creating property map of length %d', len(index))
    property_map = [None] * (2 * len(index))

    def set_properties(i):
        property_map[i.value.id] = i.representative, i.countOccurrences
        if i.goDown():
            while True:
                set_properties(copy(i)) 
                if not i.goRight():
                    break

    def show_properties(i):
        if len(i.representative) < 3:
            logging.info('%-2s : %5d', *property_map[i.value.id])
            if i.goDown():
                while True:
                    show_properties(copy(i)) 
                    if not i.goRight():
                        break

    logging.info('Setting properties')
    set_properties(index.TopDownIterator(index))
    logging.info('Showing properties')
    show_properties(index.TopDownIterator(index))
