#
# Copyright John Reid 2013
#

"""
Test assigning iterator functionality.
"""

from . import fasta_file
import seqan, logging

def test_iterator():
    _num_bases, sequences, _ids = seqan.readFastaDna5(fasta_file('dm01r.fasta'))
    logging.info('Building index')
    index = seqan.IndexEsaDna5(sequences)
    
    i = index.TopDownIterator(index)
    assert not i.representative
    assert i.representative.empty()
    
    for c in 'ACTC':
        i.goDownChar(c)
        logging.info(i.representative)
    
    r = i.representative
    print r
    del i
    del index
    print r
    
    index = seqan.IndexEsaDna5(sequences)
    i = index.TopDownIterator(index)
    j = index.TopDownIterator(index)
    assert j != i
    print i.value.id, j.value.id
    assert j.value == i.value
    i.goDownChar('C')
    assert j.value != i.value
    j.goDownChar('C')
    assert j != i
    assert j.value == i.value
