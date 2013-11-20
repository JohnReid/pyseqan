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
    suffix = 'ACGTATGC'
    predicate = seqan.suffixpredicate(suffix)
    #predicate = seqan.depthpredicate(4)
    builder = seqan.io.graphtool.Builder(index, predicate=predicate)
    #pos = GT.radial_tree_layout(builder.graph, builder.graph.vertex(0))
    pos = GT.sfdp_layout(builder.graph)
    GT.graph_draw(
        builder.graph,
        pos=pos,
        vertex_size=2,
        vertex_fill_color="lightgrey",
        vertex_text=builder.occurrences,
        vertex_pen_width=seqan.io.graphtool.root_vertex_property(builder),
        edge_text=seqan.io.graphtool.edge_labels_for_output(builder),
        edge_color=seqan.io.graphtool.color_edges_by_first_symbol(builder),
        edge_end_marker="none",
        edge_pen_width=2,
        edge_dash_style=seqan.io.graphtool.dash_non_suffix_edges(builder, suffix),
        #edge_pen_width=builder.edge_lengths,
        output="graphtool.png"
    )
    return builder


if '__main__' == __name__:
    graphbuilder = test_graphtool()
