#
# Copyright John Reid 2014
#

"""
Code to descend indexes with top-down and top-down-history iterators.
"""


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
    """Mix-in class to descend an index. Base class must implement
    the visit_node(parent, child) method. A predicate can be supplied to
    filter parts of the tree from the descent. See depthpredicate() and
    suffixpredicate().
    """
    def descend(self, it):
        """Descend the index."""
        if self.visitvertex(it):
            if it.goDown():
                while True:
                    self.descend(it)
                    if not it.goRight():
                        break
                it.goUp()


def traverse(it):
    """Top-down traversal of all vertexes beneath this iterator."""
    descender = Descender()
    descender.visitvertex = lambda it: True
    descender.descend(it)


class ParallelDescender(object):
    """Descends two indexes (primary and secondary) in parallel. Each vertex in
    the primary is visited and the corresponding vertices (or closest vertices)
    in the secondary are simultaneously visited. The two iterators are called
    synchronised if and only if the representative of the primary iterator
    matches that start of the representative of the secondary iterator.
    """
    def descend(self, primit, secit, stillsynced=True):
        self.visitvertex(primit, secit, stillsynced)
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
                # Count the number of vertexes we descend
                numgodowns = 0
                # Only move the secondary iterator if we are still
                # synchronised with primary iterator
                if newstillsynced:
                    # Move secondary iterator to same (or similar) position
                    # as primary iterator
                    while secit.repLength < primit.repLength:
                        # Try and descend
                        if not secit.goDown(
                                primit.representative[secit.repLength]):
                            #logger.debug('Could not goDown()')
                            newstillsynced = False
                            break
                        numgodowns += 1
                        # Check we descended successfully
                        start = secit.repLength - secit.parentEdgeLength + 1
                        end = min(primit.repLength, secit.repLength)
                        if secit.representative[start:end] != primit.representative[start:end]:
                            # We did not manage to match primary's parent edge
                            #logger.debug('Parent edge mismatch')
                            newstillsynced = False
                            break
                # recurse
                self.descend(primit, secit, newstillsynced)
                # Move secondary iterator back up to original position
                for i in xrange(numgodowns):
                    secit.goUp()
                # Go to next vertex in primary index
                if not primit.goRight():
                    break
            primit.goUp()



class CallbackParallelDescender(ParallelDescender):
    """Class that descends two indexes in a top-down manner,
    calling a callback at each vertex."""
    def __init__(self, callback):
        ParallelDescender.__init__(self)
        self.visitvertex = callback

