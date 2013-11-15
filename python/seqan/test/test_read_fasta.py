#
# Copyright John Reid 2013
#

"""
Test reading a FASTA file.
"""

from seqan.test import fasta_file
import logging
import seqan

def test_read_fasta():
    filename = fasta_file('dm01r.fasta')
    logging.info('Reading %s', filename)
    num_bases, sequences, ids = seqan.readFastaDNA5(filename)
    logging.info('Read %d bases in %d sequences', num_bases, len(sequences))
    assert 4 == len(sequences), len(sequences) 
    for _id, seq in zip(ids, sequences):
        assert 1500 == len(seq)
        logging.info('%5d bases in %s', len(seq), _id)
    map(len, sequences)
