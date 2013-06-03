#
# Copyright John Reid 2013
#

"""
Test building an index.
"""

import seqan
from . import fasta_file, show_shallow_tree

def test_build_index():
    _num_bases, sequences, _ids = seqan.readFastaDna5(fasta_file('dm01r.fasta'))
    index = seqan.IndexEsaDna5(sequences)
    show_shallow_tree(index.TopDownIterator(index))

