#
# Copyright John Reid 2011, 2012
#

from cookbook.script_basics import boost_python_dlopen_flags
import os, logging
logger = logging.getLogger(__name__)


#
# Do the importing with the correct dlopen flags.
#
with boost_python_dlopen_flags():
    from ._seqan import *



