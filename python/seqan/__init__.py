#
# Copyright John Reid 2013, 2014
#

from __future__ import with_statement

import pkg_resources
from copy import copy

__doc__ = pkg_resources.resource_string(__name__, "README")
__license__ = pkg_resources.resource_string(__name__, "LICENSE")
__release__, __git_id__ = pkg_resources.resource_string(__name__, "VERSION").strip().split('-')
__major_version__, __minor_version__, __release_version__ = map(int, __release__.split('.'))
__version__ = '%d.%d' % (__major_version__, __minor_version__)


from ._build import *

logger = logging.getLogger(__name__)
try:
    logging.captureWarnings(True)
except AttributeError:
    warnings.warn('Could not ask logging system to capture warnings. Old logging module?')


def version_string():
    """Return the release and git id as a string."""
    return '%s %s' % (__release__, __git_id__)


def property_map_for(index, dtype=None):
    """Create a sparse matrix suitable for use as a property map for the index.
    The data type will be an unsigned int unless specified."""
    from scipy.sparse import dok_matrix
    from scipy import uint32
    if dtype is None:
        dtype = uint32
    return dok_matrix((2 * len(index), 4), dtype=dtype)


def depthpredicate(maxdepth):
    """Create a predicate that only descends the tree to a maximum depth.
    """
    def predicate(it):
        return it.repLength < maxdepth
    return predicate


def suffixpredicate(suffix):
    """Create a predicate that only descends the part of the tree
    that matches the suffix.
    """
    def predicate(it):
        minlen = min(it.repLength, len(suffix))
        return suffix[:minlen] == it.representative[:minlen]
    return predicate


class Descender(object):
    """Mix-in class to descend a suffix tree/array. Base class must implement
    the _visit_node(parent, child) method. A predicate can be supplied to
    filter parts of the tree from the descent. See depthpredicate() and
    suffixpredicate().
    """
    def __init__(self, predicate=None):
        self.predicate = predicate

    def _descend(self, parent, it):
        """Descend the index adding edges."""
        if self.predicate is not None and not self.predicate(it):
            return
        self._visit_node(parent, it)
        parent = copy(it)
        if it.goDown():
            while True:
                self._descend(parent, copy(it))
                if not it.goRight():
                    break

    def __call__(self, index, history=False):
        if history:
            self._descend(None, index.topdownhistory())
        else:
            self._descend(None, index.topdown())



class CallbackDescender(Descender):
    """Class that descends an index in a top-down manner,
    calling a callback at each vertex."""
    def __init__(self, callback, predicate=None):
        Descender.__init__(self, predicate)
        self._visit_node = callback


class ParallelDescender(object):
    """Descends two indexes (primary and secondary) in parallel. Each vertex in
    the primary is visited and the corresponding vertices (or closest vertices)
    in the secondary are simultaneously visited. The two iterators are called
    synchronised if and only if the representative of the primary iterator
    matches that start of the representative of the secondary iterator.
    """
    def descend(self, primit, secit, stillsynced=True):
        self._visit_node(primit, secit, stillsynced)
        if stillsynced != (
            primit.repLength <= secit.repLength
            and primit.representative == secit.representative[:primit.repLength]):
            1/0
        assert stillsynced == (
            primit.repLength <= secit.repLength
            and primit.representative == secit.representative[:primit.repLength])
        if primit.goDown():
            while True:
                # We have moved the primary iterator,
                # we should check if we are still synchronised
                if stillsynced:
                    parentstart = primit.repLength - primit.parentEdgeLength
                    end = min(primit.repLength, secit.repLength)
                    newstillsynced = (
                        primit.representative[parentstart:end]
                        == secit.representative[parentstart:end])
                else:
                    newstillsynced = False
                # Only move the secondary iterator if we are still
                # synchronised with primary iterator
                if newstillsynced:
                    # Move secondary iterator to same (or similar) position
                    # as primary iterator
                    newsecit = copy(secit)
                    while newsecit.repLength < primit.repLength:
                        # Try and descend
                        if not newsecit.goDown(
                                primit.representative[newsecit.repLength]):
                            #logger.debug('Could not goDown()')
                            newstillsynced = False
                            break
                        # Check we descended successfully
                        start = newsecit.repLength - newsecit.parentEdgeLength + 1
                        end = min(primit.repLength, newsecit.repLength)
                        if newsecit.representative[start:end] != primit.representative[start:end]:
                            # We did not manage to match primary's parent edge
                            #logger.debug('Parent edge mismatch')
                            newstillsynced = False
                            break
                else:
                    # Don't bother copying if we are not altering it
                    newsecit = secit
                # recurse
                self.descend(copy(primit), newsecit, newstillsynced)
                # Go to next vertex in primary index
                if not primit.goRight():
                    break



class CallbackParallelDescender(ParallelDescender):
    """Class that descends two indexes in a top-down manner,
    calling a callback at each vertex."""
    def __init__(self, callback):
        ParallelDescender.__init__(self)
        self._visit_node = callback


def findsuffixes(it, callback):
    """Counts all complete suffixes below the vertex represented by the iterator.
    Calls the callback(it, count) whenever the count > 0."""
    count = it.numOccurrences
    copyit = copy(it)
    if it.goDown():
        while True:
            count -= it.numOccurrences
            findsuffixes(copy(it), callback)
            if not it.goRight():
                break
    if count > 0:
        callback(copyit, count)
