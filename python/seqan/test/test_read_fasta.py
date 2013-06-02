#
# Copyright John Reid 2013
#

"""
Test reading a FASTA file.
"""

from setup_environment import init_test_env, update_path_for_seqan, logging, fasta_file, log_path
init_test_env(__file__, level=logging.INFO)
update_path_for_seqan()
log_path(logging.DEBUG)

import os, sys, seqan

filename = fasta_file('dm01r.fasta')
logging.info('Reading %s', filename)
num_bases, sequences, ids = seqan.readFastaDna5(filename)
logging.info('Read %d bases in %d sequences', num_bases, len(sequences))
logging.info('IDs: %s', ', '.join(ids))