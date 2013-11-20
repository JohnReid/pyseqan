#
# Copyright John Reid 2013
#

"""
Create a graphtool graph of a suffix tree.
"""

import logging
logger = logging.getLogger(__name__)

import graph_tool.all as GT
from copy import copy


class Builder(object):
    """Build a graphtool graph.
    """

    def __init__(self, index):
        "Construct the builder."
        self.graph = GT.Graph(directed=True)
        self.labels = self.graph.new_edge_property('string')
        self.vertices = dict()


    def get_vertex(self, index_vertex):
        result = self.vertices.get(index_vertex)
        if result is None:
            result = self.vertices[index_vertex] = self.graph.add_vertex()
        return result


    def _add_edge(self, parent, child):
        parent_vertex = self.get_vertex(parent.value)
        child_vertex  = self.get_vertex(child.value)
        edge = self.graph.add_edge(parent_vertex, child_vertex)
        self.labels[edge] = child.representative[:-child.parentEdgeLen]
        return edge


    def _descend(self, it):
        """Descend the index adding edges."""
        parent = copy(it)
        if it.goDown():
            while True:
                self._add_edge(parent, it)
                self._descend(copy(it))
                if not it.goRight():
                    break
