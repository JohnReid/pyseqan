#
# Copyright John Reid 2013
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

    def _descend(self, it):
        """Descend the index adding edges."""
        if self.predicate is not None and not self.predicate(it):
            return
        parent = copy(it)
        if it.goDown():
            while True:
                self._visit_node(parent, it)
                self._descend(copy(it))
                if not it.goRight():
                    break


