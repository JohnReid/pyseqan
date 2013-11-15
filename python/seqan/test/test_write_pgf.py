#
# Copyright John Reid 2013
#

"""Test writing a suffix tree as a PGF tree.

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

from seqan.test import fasta_file
import logging
import seqan
from copy import copy



def test_write_pgf():
    """Test writing a PGF representing a suffix tree.
    """
    _num_bases, sequences, _ids = seqan.readFastaDNA(fasta_file('dm01r.fasta'))
    #for s in sequences:
    #    s.remove('T')
    index = seqan.IndexStringDNASetESA(sequences)
    suffix = 'GCCGAA'

    def descend(i):
        indent = ' ' * 2 * len(i.representative)
        is_root = i.isRoot
        if not is_root:
            edgeLabel = str(i.parentEdgeLabel)
            if len(edgeLabel) > 2:
                edgeLabel = edgeLabel[:2] + '...'
        shorter_length = min(i.repLength, len(suffix))
        past_end = not is_root and len(suffix) + i.parentEdgeLength <= i.repLength
        on_path = suffix[:shorter_length] == str(i.representative)[:shorter_length]
        last_node = suffix == str(i.representative)
        logging.info('%-10s : %5d : %5s %5s', str(i.representative)[:10], i.countOccurrences, on_path, past_end)

        if not is_root:
            print >>tikz, '%schild {' % (indent)
        print >>tikz, '%snode[%s] {}' % (
            indent,
            (on_path and not past_end) and (last_node and 'last node on path' or 'node on path') or 'node off path',
            #i.countOccurrences
        )

        # only go further down if we are on the right path
        if on_path:
            if i.goDown():
                while True:
                    descend(copy(i))
                    if not i.goRight():
                        break
        if not is_root:
            print >>tikz, '%sedge from parent[%s] node[edge label] {%s}' % (
                indent, (on_path and not past_end) and 'edge on path' or 'edge off path', edgeLabel
            )
            print >>tikz, '%s}' % indent

    with open('suffix-tree.tex', 'w') as tikz:
        tikz.write('\\')
        descend(index.TopDownIterator(index))
        print >>tikz, ';'

