#
# Copyright John Reid 2013
#

"""Test creating a tree from a suffix tree.
"""

import logging
logger = logging.getLogger('__name__')

from seqan.test import fasta_file
import seqan.io.graphtool
import graph_tool.all as GT


def test_graphtool():
    """Test creating a tree from a suffix tree.
    """
    _num_bases, sequences, _ids = seqan.readFastaDNA(fasta_file('dm01r.fasta'))
    #for s in sequences:
    #    s.remove('T')
    index = seqan.IndexStringDNASetESA(sequences)
    builder = seqan.io.graphtool.Builder(index)
    pos = GT.sfdp_layout(builder.graph)
    GT.graph_draw(
        builder.graph,
        pos=pos)


if '__main__' == __name__:
    test_graphtool()
