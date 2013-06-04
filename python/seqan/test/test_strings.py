#
# Copyright John Reid 2013
#

"""
Test string functionality.
"""

from . import fasta_file
import logging, seqan

def test_strings():
    _num_bases, sequences, ids = seqan.readFastaDna5(fasta_file('dm01r.fasta'))
    
    logging.info('Size of Dna5 alphabet %d', seqan.Dna5.valueSize )
    logging.info('Size of Dna alphabet %d', seqan.Dna.valueSize )
    logging.info('Length of sequence %s %d', ids[3], len(sequences[3]))
    logging.info('Tenth base of sequence %s %s', ids[2], sequences[2].value(9))
    logging.info('Infix of sequence %s %s', ids[2], sequences[2].infix(9, 14))
    
    
    _s5 = seqan.StringDna5('ACGTACGTACGTACGT')
    s4 = seqan.StringDna('ACGTACGTACGTACGT')
    s = s4
    infix = s.infix(3, 9)
    slice_ = s[3:9]
    logging.info('Infix %s', infix)
    logging.info(type(infix))
    logging.info('Slice %s', slice_)
    logging.info(type(slice_))
    
    # check object lifetimes are respected
    del s
    logging.info('Infix %s', infix)
    logging.info('Slice %s', slice_)
    
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
    
    
    s5 = seqan.StringDna5('NACGTNNACGTNACGTNACGT')
    print s5[0] == 'N'
    assert s5[0] == 'N'
    print s5[0] != 'G'
    assert s5[0] != 'G'
    print s5[1] != 'N'
    assert s5[1] != 'N'
    print s5[1] == 'A'
    assert s5[1] == 'A'
    print 'N' == s5[0]
    assert 'N' == s5[0]
    print 'G' != s5[0]
    assert 'G' != s5[0]
    print 'N' != s5[1]
    assert 'N' != s5[1]
    print 'A' == s5[1]
    assert 'A' == s5[1]


def test_string_set():
    stringset = seqan.StringDna5Set()
    stringset.appendValue(seqan.StringDna5('ACGTACGTACGTNNN'))
    i = iter(stringset)
    try:
        while True:
            logging.info(i.next())
            logging.info(len(i.next()))
    except StopIteration:
        pass
    map(len, stringset)
    