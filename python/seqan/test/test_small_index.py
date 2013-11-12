#
# Copyright John Reid 2013
#

"""
Test some memory management problems
"""

import seqan

seqs = ('A',)
string_set = seqan.StringDnaSet()
for seq in seqs:
    #string_set.appendValue(seqan.StringDna(seq[::-1]))
    string_set.appendValue(seqan.StringDna(seq))
index = seqan.IndexEsaDna(string_set)
i = index.TopDownIterator(index)
i.goDownStr(seqan.StringDna('A'))
