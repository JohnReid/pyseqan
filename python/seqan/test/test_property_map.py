#
# Copyright John Reid 2013
#

"""
Test assigning properties to nodes.
"""

from setup_environment import init_test_env, update_path_for_seqan, logging, fasta_file, log_path
init_test_env(__file__, level=logging.INFO)
update_path_for_seqan()
log_path(logging.DEBUG)

import os, sys, seqan
from copy import copy

num_bases, sequences, ids = seqan.readFastaDna5(fasta_file('dm01r.fasta'))
logging.info('Building index')
index = seqan.IndexEsaDna5(sequences)
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
