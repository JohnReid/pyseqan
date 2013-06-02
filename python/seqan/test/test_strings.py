#
# Copyright John Reid 2013
#

"""
Test string functionality.
"""

from setup_environment import init_test_env, update_path_for_seqan, logging, fasta_file, log_path
init_test_env(__file__, level=logging.INFO)
update_path_for_seqan()
log_path(logging.DEBUG)

import os, sys, seqan
from copy import copy

num_bases, sequences, ids = seqan.readFastaDna5(fasta_file('dm01r.fasta'))

logging.info('Size of Dna5 alphabet %d', seqan.Dna5.valueSize )
logging.info('Size of Dna alphabet %d', seqan.Dna.valueSize )
logging.info('Length of sequence %s %d', ids[3], len(sequences[3]))
logging.info('Tenth base of sequence %s %s', ids[2], sequences[2].value(9))
logging.info('Infix of sequence %s %s', ids[2], sequences[2].infix(9, 14))


s5 = seqan.StringDna5('ACGTACGTACGTACGT')
s4 = seqan.StringDna('ACGTACGTACGTACGT')
s = s4
infix = s.infix(3, 9)
slice = s[3:9]
logging.info('Infix %s', infix)
logging.info(type(infix))
logging.info('Slice %s', slice)
logging.info(type(slice))

# check object lifetimes are respected
del s
logging.info('Infix %s', infix)
logging.info('Slice %s', slice)

#
# Check iteration using __getitem__ works correctly
#
s = sequences[0]
chars = set()
for i, c in enumerate(s):
    if i >= len(s):
        assert False
    chars.add(c)
assert len(chars) <= 4
assert i == len(s) - 1

