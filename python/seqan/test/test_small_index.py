#
# Copyright John Reid 2013
#

"""
Test some memory management problems
"""

import seqan

def test_small_index():
    seqs = ('A',)
    string_set = seqan.StringDNASet()
    for seq in seqs:
        string_set.appendValue(seqan.StringDNA(seq))
    index = seqan.IndexStringDNASetESA(string_set)
    i = index.TopDownIterator(index)
    1/0  # The following core dumps due to seqan bug (http://trac.seqan.de/ticket/1122)
    i.goDown(seqan.StringDNA('A'))
