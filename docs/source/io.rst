
Input/Output
============

.. testsetup:: *

   import seqan.traverse
   import seqan.io.graphtool
   import graph_tool.draw



.. _disk-io:

Saving and loading
------------------

Indexes can be saved to disk and loaded back from disk. The *save()* method serialises
the index to disk.

..  doctest::

    >>> seqs = seqan.StringDNASet(('ACGT', 'AAAA', 'GGGG', 'AC'))
    >>> index = seqan.IndexStringDNASetESA(seqs)
    >>> seqan.traverse.depthfirsttraversal(index, lambda it: True)
    <...>
    >>> index.save('my-index')

This will create several (typically many) files with names such as `my-index.bwt`,
`my-index.child`, etc... One thing to note is that seqan data structures
can be :ref:`lazily initialised <lazy-initialisation>` so it can be worth traversing the
entire index before saving, otherwise the index may be saved in an
uninitialised state. Indexes can be restored from disk using the *load()* method.

..  doctest::

    >>> index2 = seqan.IndexStringDNASetESA.load('my-index')
    >>> it = index2.topdown()
    >>> it.goDown('AC')
    True
    >>> print 'Index has {0} occurrence(s) of representative "{1}"'.format(
    ...     it.numOccurrences, it.representative)
    Index has 2 occurrence(s) of representative "AC"



.. _graphical:

Graphical output
----------------

If you have the graph-tool_ package installed, you can use it to create graphs
that represent suffix trees or arrays. The graphs can be saved to various
output formats or examined interactively. For example, suppose we have an index

.. _graph-tool: http://graph-tool.skewed.de/

We can build a graphtool graph from the index

..  doctest::

    >>> builder = seqan.io.graphtool.Builder(index)

and save it as a figure using a scale force directed placement (SFDP) layout algorithm

..  doctest::

    >>> pos = graph_tool.draw.graph_draw(
    ...     builder.graph,
    ...     pos=graph_tool.draw.sfdp_layout(builder.graph),
    ...     vertex_size=30,
    ...     vertex_fill_color="lightgrey",
    ...     vertex_text=builder.occurrences,
    ...     vertex_pen_width=seqan.io.graphtool.root_vertex_property(builder),
    ...     edge_text=seqan.io.graphtool.edge_labels_for_output(builder),
    ...     edge_color=seqan.io.graphtool.color_edges_by_first_symbol(builder),
    ...     edge_end_marker="none",
    ...     edge_pen_width=2,
    ...     output="index.png"
    ... )

Here we have set various edge and vertex properties such that:

- The root vertex has a thicker border
- The edges are colored by their first base
- The number of occurrences for each vertex are shown

..  image:: _static/index.png

We could have used predicates to control which parts of the suffix we built a graph for.
A depthpredicate only shows those vertices within a certain distance of the root vertex

..  doctest::

    >>> builder = seqan.io.graphtool.Builder(index, predicate=seqan.traverse.depthpredicate(2))
    >>> pos = graph_tool.draw.graph_draw(
    ...     builder.graph,
    ...     pos=graph_tool.draw.sfdp_layout(builder.graph),
    ...     vertex_size=30,
    ...     vertex_fill_color="lightgrey",
    ...     vertex_text=builder.occurrences,
    ...     vertex_pen_width=seqan.io.graphtool.root_vertex_property(builder),
    ...     edge_text=seqan.io.graphtool.edge_labels_for_output(builder),
    ...     edge_color=seqan.io.graphtool.color_edges_by_first_symbol(builder),
    ...     edge_end_marker="none",
    ...     edge_pen_width=2,
    ...     output="maxdepth-2.png"
    ... )

..  image:: _static/maxdepth-2.png

or a suffix predicate only shows those vertices and edges near a given suffix

..  doctest::

    >>> suffix = 'ACG'
    >>> builder = seqan.io.graphtool.Builder(index, predicate=seqan.traverse.suffixpredicate(suffix))
    >>> pos = graph_tool.draw.graph_draw(
    ...     builder.graph,
    ...     pos=graph_tool.draw.sfdp_layout(builder.graph),
    ...     vertex_size=30,
    ...     vertex_fill_color="lightgrey",
    ...     vertex_text=builder.occurrences,
    ...     vertex_pen_width=seqan.io.graphtool.root_vertex_property(builder),
    ...     edge_text=seqan.io.graphtool.edge_labels_for_output(builder),
    ...     edge_color=seqan.io.graphtool.color_edges_by_first_symbol(builder),
    ...     edge_end_marker="none",
    ...     edge_pen_width=2,
    ...     edge_dash_style=seqan.io.graphtool.dash_non_suffix_edges(builder, suffix),
    ...     output="suffix.png"
    ... )

..  image:: _static/suffix.png

