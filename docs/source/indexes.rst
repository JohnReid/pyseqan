
Indexes
=======

.. testsetup:: *

   import seqan

An index is the result of pre-processing a text to allow fast look-up
and search operations. The enhanced suffix array (ESA) is an index that allows
retrieval of substrings based on their content. We can build an ESA from a string
set easily.

..  doctest::

    >>> seqs = seqan.StringDNASet(('ACGT', 'AAAA', 'GGGG', 'AC'))
    >>> index = seqan.IndexStringDNASetESA(seqs)


Iterators
---------

Once we have the index we can find substrings in the string set using a top-down iterator.

..  doctest::

    >>> iterator = index.topdown()
    >>> if iterator.goDown('AC'):
    ...     print '"{0}" starts a suffix in our index'.format(iterator.representative)
    ...     for occ in iterator.occurrences:
    ...         print 'Found "{0}" in sequence {1} at position {2}'.format(iterator.representative, occ.i1, occ.i2)
    "AC" starts a suffix in our index
    Found "AC" in sequence 3 at position 0
    Found "AC" in sequence 0 at position 0

An iterator points to a particular location in an index. In the case of an ESA,
it identifies all the suffixes that start with a given representative substring.
Here we used the *occurrences* property of the top-down iterator to locate all the
occurrences of the substring AC in the original string set. We also used the
*representative* property to access the substring the iterator refers to. The *replength*
property is an efficient way to access the length of the representative substring.

..  doctest::

    >>> print '"{0}" has length {1}'.format(iterator.representative, iterator.repLength)
    "AC" has length 2

The *numOccurrences* property holds the number of occurrences of the substring in the
entire string set.

..  doctest::

    >>> print '"{0}" has {1} occurrences'.format(iterator.representative, iterator.numOccurrences)
    "AC" has 2 occurrences


The *numChildren* property holds the number of occurrences in the children of this iterator, that
is it shows how many of the representative substring for this node do not terminate here.

..  doctest::

    >>> print '"{0}" has {1} occurrence in child nodes'.format(iterator.representative, iterator.numChildren)
    "AC" has 1 occurrence in child nodes

Top-down iterators can go down and right to traverse the entire index from the root node.
SeqAn also provides a top-down-history iterator that can go up.

..  doctest::

    >>> historyiterator = index.topdownhistory()
    >>> if historyiterator.goDown('AC'):
    ...     print '"{0}" starts a suffix in our index'.format(historyiterator.representative)
    ...     if historyiterator.goUp():
    ...         print '"{0}" starts a suffix in our index'.format(historyiterator.representative)
    "AC" starts a suffix in our index
    "A" starts a suffix in our index


Traversal
---------

Top-down iterators can traverse the entire index in a top-down manner using recursion.

..  doctest::

    >>> def descend(it):
    ...     print 'Index has {0} occurrence(s) of representative "{1}"'.format(
    ...         it.numOccurrences, it.representative)
    ...     if it.goDown():
    ...         while True:
    ...             descend(it.copy())
    ...             if not it.goRight():
    ...                 break
    >>> descend(index.topdown())
    Index has 14 occurrence(s) of representative ""
    Index has 6 occurrence(s) of representative "A"
    Index has 3 occurrence(s) of representative "AA"
    Index has 2 occurrence(s) of representative "AAA"
    Index has 1 occurrence(s) of representative "AAAA"
    Index has 2 occurrence(s) of representative "AC"
    Index has 1 occurrence(s) of representative "ACGT"
    Index has 2 occurrence(s) of representative "C"
    Index has 1 occurrence(s) of representative "CGT"
    Index has 5 occurrence(s) of representative "G"
    Index has 3 occurrence(s) of representative "GG"
    Index has 2 occurrence(s) of representative "GGG"
    Index has 1 occurrence(s) of representative "GGGG"
    Index has 1 occurrence(s) of representative "GT"
    Index has 1 occurrence(s) of representative "T"

A similar descent with top-down-history iterators is best done using *goUp()* rather
than *copy()*. In fact this method seems to be slightly more efficient than using *copy()*
with top-down iterators.

..  doctest::

    >>> def descendhistory(it):
    ...     print 'Index has {0} occurrence(s) of representative "{1}"'.format(
    ...         it.numOccurrences, it.representative)
    ...     if it.goDown():
    ...         while True:
    ...             descendhistory(it)
    ...             if not it.goRight():
    ...                 break
    ...         it.goUp()
    >>> descend(index.topdown())
    Index has 14 occurrence(s) of representative ""
    Index has 6 occurrence(s) of representative "A"
    Index has 3 occurrence(s) of representative "AA"
    Index has 2 occurrence(s) of representative "AAA"
    Index has 1 occurrence(s) of representative "AAAA"
    Index has 2 occurrence(s) of representative "AC"
    Index has 1 occurrence(s) of representative "ACGT"
    Index has 2 occurrence(s) of representative "C"
    Index has 1 occurrence(s) of representative "CGT"
    Index has 5 occurrence(s) of representative "G"
    Index has 3 occurrence(s) of representative "GG"
    Index has 2 occurrence(s) of representative "GGG"
    Index has 1 occurrence(s) of representative "GGGG"
    Index has 1 occurrence(s) of representative "GT"
    Index has 1 occurrence(s) of representative "T"


Top-down-history iterators also provide *goBegin()* and *goNext()* methods that allow
the following depth-first method of traversal. This seems to be the most efficient
traversal method.

..  doctest::

    >>> it = index.topdownhistory()
    >>> it.goBegin()
    >>> while not it.atEnd:
    ...     print 'Index has {0} occurrence(s) of representative "{1}"'.format(
    ...         it.numOccurrences, it.representative)
    ...     it.goNext()
    Index has 1 occurrence(s) of representative "AAAA"
    Index has 2 occurrence(s) of representative "AAA"
    Index has 3 occurrence(s) of representative "AA"
    Index has 1 occurrence(s) of representative "ACGT"
    Index has 2 occurrence(s) of representative "AC"
    Index has 6 occurrence(s) of representative "A"
    Index has 1 occurrence(s) of representative "CGT"
    Index has 2 occurrence(s) of representative "C"
    Index has 1 occurrence(s) of representative "GGGG"
    Index has 2 occurrence(s) of representative "GGG"
    Index has 3 occurrence(s) of representative "GG"
    Index has 1 occurrence(s) of representative "GT"
    Index has 5 occurrence(s) of representative "G"
    Index has 1 occurrence(s) of representative "T"
    Index has 14 occurrence(s) of representative ""


Lazy initialisation
-------------------

Many of the seqan data structures are initialised lazily, that is they are not
constructed until they are first required. Sometimes you might want to ensure
the structures are initialised before saving or some other operation. This can
be achieved via the *traverse()* function.

..  doctest::

    >>> import seqan.traverse
    >>> seqan.traverse.depthfirsttraversal(index, lambda it: True)
    <...>



Vertexes
--------

At a low level in the SeqAn library each iterator refers to a vertex in the ESA. The values
of these vertexes can be accessed through the *value* property.

..  doctest::

    >>> print 'The ID of the vertex for "{0}" is {1}'.format(iterator.representative, iterator.value.id)
    The ID of the vertex for "AC" is 8

These values can be used to refer to vertexes in a stable way, providing the underlying ESA does
not change.

..  doctest::

    >>> it = index.topdown()
    >>> it.goDown('AC')
    True
    >>> print it.representative
    AC
    >>> vertex = it.value  # Save vertex for "AC"
    >>> it.goDown('GT')
    True
    >>> print it.representative
    ACGT
    >>> it.value = vertex  # Reset to vertex for "AC"
    >>> print it.representative
    AC

