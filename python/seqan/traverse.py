#
# Copyright John Reid 2014
#

"""
Code to traverse indexes.
"""


from copy import copy


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


def topdowncopytraversal(it, visitvertex):
    """Visit the vertexes beneath the iterator in a top-down manner.
    visitvertex(it) is called for each vertex. If this method
    returns False, the top-down descent is truncated. Note
    this method copies the iterator once per vertex, so it is
    suitable for top-down iterators but not top-down-history
    iterators."""
    if visitvertex(it):
        if it.goDown():
            while True:
                topdowncopytraversal(copy(it), visitvertex)
                if not it.goRight():
                    break


def topdownhistorytraversal(it, visitvertex):
    """Visit the vertexes beneath the iterator in a top-down manner.
    visitvertex(it) is called for each vertex. If this method
    returns False, the top-down descent is truncated. Note
    this method uses *it.goUp()*, so it is
    suitable for top-down-history iterators but not top-down
    iterators."""
    if visitvertex(it):
        if it.goDown():
            while True:
                topdownhistorytraversal(it, visitvertex)
                if not it.goRight():
                    break
            it.goUp()


def depthfirsttraversal(index, visitvertex):
    """Visit the vertexes of index in a depth-first traversal.
    The traversal is abandoned if visitvertex(it) returns False
    for any vertex."""
    it = index.topdownhistory()
    it.goBegin()
    while not it.atEnd and visitvertex(it):
        it.goNext()
    return it


def topdownparalleltraversal(primit, secit, visitvertex, stillsynced=True):
    """Descends two top-down-history iterators (primary and secondary) in parallel.

    Each vertex in the primary index is visited and the corresponding vertices
    (or closest vertices) in the secondary are simultaneously visited.
    The two iterators are called synchronised if and only if the
    representative of the primary iterator matches the start of the
    representative of the secondary iterator.
    """
    if not visitvertex(primit, secit, stillsynced):
        return
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
                    numgodowns += 1  # We went down secit one more time
                    # Check we descended successfully
                    start = secit.repLength - secit.parentEdgeLength + 1
                    end = min(primit.repLength, secit.repLength)
                    if secit.representative[start:end] != primit.representative[start:end]:
                        # We did not manage to match primary's parent edge
                        #logger.debug('Parent edge mismatch')
                        newstillsynced = False
                        break
            # recurse
            topdownparalleltraversal(primit, secit, visitvertex, newstillsynced)
            # Move secondary iterator back up to original position
            for i in xrange(numgodowns):
                secit.goUp()
            # Go to next vertex in primary index
            if not primit.goRight():
                break
        primit.goUp()

