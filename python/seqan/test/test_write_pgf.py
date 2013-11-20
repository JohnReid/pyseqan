#
# Copyright John Reid 2013
#

"""Test writing a suffix tree as a PGF tree.
"""

import logging
logger = logging.getLogger('__name__')

from seqan.test import fasta_file
import seqan.io.pgf
import tempfile
import os


def test_write_pgf():
    """Test writing a PGF representing a suffix tree.
    """
    _num_bases, sequences, _ids = seqan.readFastaDNA(fasta_file('dm01r.fasta'))
    #for s in sequences:
    #    s.remove('T')
    index = seqan.IndexStringDNASetESA(sequences)
    tikz, filename = tempfile.mkstemp('.tex', prefix='seqan-test-pgf')
    logger.info('Writing PGF to: %s', filename)
    seqan.io.pgf.write_pgf(
        os.fdopen(tikz, 'w'),
        index,
        highlightsuffix='GCCGAA',
        maxdepth=None)

