#
# Copyright John Reid 2013
#

"""
Test reading a FASTA file.
"""

from . import fasta_file
import logging, seqan

def test_read_fasta():
    filename = fasta_file('dm01r.fasta')
    logging.info('Reading %s', filename)
    num_bases, sequences, ids = seqan.readFastaDna5(filename)
    logging.info('Read %d bases in %d sequences', num_bases, len(sequences))
    logging.info('IDs: %s', ', '.join(ids))