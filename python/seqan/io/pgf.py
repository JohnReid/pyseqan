#
# Copyright John Reid 2013
#

"""
Write a suffix tree as PGF.

We're looking for something like::

    \tikz [
        font=\footnotesize,
        grow=right, level 1/.style={sibling distance=6em},
        level 2/.style={sibling distance=1em}, level distance=5cm
    ]
    \node {Computational Complexity} % root
    child { node {Computational Problems} }
    child { node {Problem Measures} }
    child { node {Problem Aspects} }
"""

import logging
logger = logging.getLogger(__name__)

from copy import copy


def _pgf_write_descend(tikz, i, highlightsuffix, maxdepth, only_highlight):
    """Recursion to write PGF::
        
        - tikz: output file
        - i: iterator for current node
        - highlightsuffix: suffix to highlight if not None
        - maxdepth: don't go deeper
    """
    if maxdepth and maxdepth < len(i.representative):
        return
    indent = ' ' * 2 * len(i.representative)
    is_root = i.isRoot
    if not is_root:
        edgeLabel = str(i.parentEdgeLabel)
        if len(edgeLabel) > 2:
            edgeLabel = edgeLabel[:2] + '...'
    highlightdepth = highlightsuffix and len(highlightsuffix) or 0
    shorter_length = highlightsuffix and min(i.repLength, highlightdepth) or i.repLength
    past_end = not is_root and highlightdepth + i.parentEdgeLength <= i.repLength
    on_path = highlightsuffix is not None and highlightsuffix[:shorter_length] == str(i.representative)[:shorter_length]
    last_node = highlightsuffix == str(i.representative)
    logging.info('%-10s : %5d : %5s %5s', str(i.representative)[:10], i.countOccurrences, on_path, past_end)

    if not is_root:
        print >>tikz, '%schild {' % (indent)
    print >>tikz, '%snode[%s] {}' % (
        indent,
        (on_path and not past_end) and (last_node and 'last node on path' or 'node on path') or 'node off path',
        #i.countOccurrences
    )

    # only go further down if we are on the right path
    if not only_highlight or on_path:
        if i.goDown():
            while True:
                _pgf_write_descend(tikz, copy(i), highlightsuffix, maxdepth, only_highlight)
                if not i.goRight():
                    break
    if not is_root:
        print >>tikz, '%sedge from parent[%s] node[edge label] {%s}' % (
            indent, (on_path and not past_end) and 'edge on path' or 'edge off path', edgeLabel
        )
        print >>tikz, '%s}' % indent


def write_pgf(tikz, index, highlightsuffix=None, maxdepth=None, only_highlight=False):
    """Test writing a PGF representing a suffix tree.
    """
    tikz.write('\\')
    _pgf_write_descend(tikz, index.topdown(), highlightsuffix, maxdepth, only_highlight)
    print >>tikz, ';'

