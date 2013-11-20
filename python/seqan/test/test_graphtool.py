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
    #predicate = seqan.io.graphtool.depthfilter(3)
    suffix = 'ACGTATGC'
    predicate = seqan.io.graphtool.suffixpredicate(suffix)
    builder = seqan.io.graphtool.Builder(index, predicate=predicate)
    pos = GT.sfdp_layout(builder.graph)
    labels = seqan.io.graphtool.edge_labels_for_output(builder)
    edge_colors = seqan.io.graphtool.color_edges_by_first_symbol(builder)
    vertex_pen_widths = seqan.io.graphtool.root_vertex_property(builder)
    edge_dash_styles = builder.graph.new_edge_property('object')
    for edge, insuffix in seqan.io.graphtool.edges_in_suffix(builder, suffix=suffix):
        if insuffix:
            edge_dash_styles[edge] = []
        else:
            edge_dash_styles[edge] = [.1,.1,.2]
    GT.graph_draw(
        builder.graph,
        pos=pos,
        vertex_size=2,
        vertex_fill_color="lightgrey",
        vertex_text=builder.occurrences,
        vertex_pen_width=vertex_pen_widths,
        edge_text=labels,
        edge_color=edge_colors,
        edge_end_marker="none",
        edge_pen_width=2,
        edge_dash_style=edge_dash_styles,
        #edge_pen_width=builder.edge_lengths,
        output="graphtool.png"
    )
    return builder


if '__main__' == __name__:
    graphbuilder = test_graphtool()
