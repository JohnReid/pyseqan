#
# Copyright John Reid 2013
#

"""
Create a graphtool graph of a suffix tree.
"""

import logging
logger = logging.getLogger(__name__)

import graph_tool.all as GT
import seqan
from copy import copy



class Builder(seqan.Descender):
    """Build a graphtool graph.
    """

    def __init__(self, index, predicate=None):
        "Construct the builder."
        super(Builder, self).__init__(predicate)
        self.index = index
        self.graph = GT.Graph(directed=True)
        self.occurrences = self.graph.new_vertex_property('int')
        self.labels = self.graph.new_edge_property('string')
        self.edge_lengths = self.graph.new_edge_property('int')
        self.vertices = dict()
        self.index_iterators = dict()
        self._descend(index.topdown())


    def get_vertex(self, index_it):
        result = self.vertices.get(index_it.value)
        if result is None:
            result = self.vertices[index_it.value] = self.graph.add_vertex()
            self.index_iterators[result] = copy(index_it)
            self.occurrences[result] = index_it.countOccurrences
        return result


    def _add_edge(self, parent, child):
        parent_vertex = self.get_vertex(parent)
        child_vertex  = self.get_vertex(child)
        edge = self.graph.add_edge(parent_vertex, child_vertex)
        self.labels[edge] = child.representative[-child.parentEdgeLength:]
        return edge


    def _visit_node(self, parent, child):
        """Descend the index adding edges."""
        self._add_edge(parent, child)


def edges_in_suffix(builder, suffix):
    """Iterate over the edges in the graph and yield (edge, insuffix).
    """
    predicate = seqan.suffixpredicate(suffix)
    for edge in builder.graph.edges():
        index_it = builder.index_iterators[edge.target()]
        yield edge, predicate(index_it)


def dash_non_suffix_edges(builder, suffix):
    edge_dash_styles = builder.graph.new_edge_property('object')
    for edge, insuffix in edges_in_suffix(builder, suffix=suffix):
        if insuffix:
            edge_dash_styles[edge] = []
        else:
            edge_dash_styles[edge] = [.1,.1,.2]
    return edge_dash_styles


def root_vertex_property(builder, proptype='float', rootvalue=2.):
    """Create a property of type proptype which is only set on the root
    node of the suffix tree.
    """
    prop = builder.graph.new_vertex_property(proptype)
    prop[builder.get_vertex(builder.index.topdown())] = rootvalue
    return prop


def edge_labels_for_output(builder, maxlabellen=3):
    output_labels = builder.graph.new_edge_property('string')
    for edge in builder.graph.edges():
        label = builder.labels[edge][:maxlabellen]
        if len(label) > maxlabellen:
            label = '{0}...'.format()
        output_labels[edge] = label
    return output_labels


def color_edges_by_first_symbol(builder, color_map=None, defaultcolor='black'):
    if color_map is None:
        color_map = {
            'A': [ 80/256., 80/256.,128/256., 1.], 
            'C': [204/256.,163/256.,  0/256., 1.], 
            'G': [128/256., 56/256., 56/256., 1.], 
            'T': [ 80/256.,128/256., 80/256., 1.],
        }
    edge_colors = builder.graph.new_edge_property('vector<float>')
    for edge in builder.graph.edges():
        edge_colors[edge] = color_map.get(builder.labels[edge][0], defaultcolor)
    return edge_colors
