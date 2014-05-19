
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

Once we have the index we can find substrings in the string set using a top down iterator.

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
Here we used the *occurrences* property of the top down iterator to locate all the
occurrences of the substring AC in the original string set. We also used the
*representative* property to access the substring the iterator refers to. The *replength*
property is an efficient way to access the length of the representative substring.

..  doctest::

    >>> print '"{0}" has length {1}'.format(iterator.representative, iterator.repLength)
    "AC" has length 2

The *numOccurrences* property returns the number of occurrences of the substring in the
entire string set.

..  doctest::

    >>> print '"{0}" has {1} occurrences'.format(iterator.representative, iterator.numOccurrences)
    "AC" has 2 occurrences


Vertexes
--------

At a lower level in the SeqAn library each iterator refers to a vertex in the ESA. The values
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

