#
# Copyright John Reid 2013
#

"""
Code to choose whether to import debug or release build of module.
"""

import os, sys, logging
logger = logging.getLogger(__name__)

#
# decide whether to import debug or release stempy C++ extension
#
_python_debug_build = hasattr(sys, "gettotalrefcount") # only available in python debug build
if os.environ.get('PYSEQANDEBUG') or _python_debug_build:

    from ._debug_build import *
    from . import _debug_build as S

else:

    from ._release_build import *
    from . import _release_build as S

logger.info('Loaded SeqAn C++-python interface from %s', S.__name__)



