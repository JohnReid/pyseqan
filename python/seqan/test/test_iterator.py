#
# Copyright John Reid 2013
#

"""
Test assigning iterator functionality.
"""

from seqan.test import fasta_file
import seqan
import logging
import sys
from copy import copy


def _make_test_index():
    strings = seqan.StringDna5Set()
    strings.appendValue(seqan.StringDna5('ACG'))
    strings.appendValue(seqan.StringDna5('AA'))
    strings.appendValue(seqan.StringDna5('NN'))
    return seqan.IndexEsaDna5(strings)


def test_representative_equality():
    logging.info(sys._getframe().f_code.co_name)
    index = _make_test_index()
    i = index.TopDownIterator(index)
    for n, c in enumerate('ACG'):
        i.goDownChar(c)
        logging.info(i.representative[:n+1])
        assert i.representative[:n+1] == 'ACG'[:n+1]
        assert 'ACG'[:n+1] == i.representative[:n+1]



def check_go_down_str(i, s, representative):
    i.goDownStr(s)
    assert representative == i.representative


def test_go_down_str():
    logging.info(sys._getframe().f_code.co_name)
    index = _make_test_index()
    i = index.TopDownIterator(index)
    assert copy(i).goDownStr(seqan.StringDna5('C'))
    assert copy(i).goDownStr('C')
    assert not copy(i).goDownStr(seqan.StringDna5('T'))
    assert not copy(i).goDownStr('T')
    assert not copy(i).goDownStr(seqan.StringDna5('ACGT'))
    assert copy(i).goDownStr('AC')
    assert copy(i).goDownStr(seqan.StringDna5('AC'))
    assert copy(i).goDownStr(seqan.StringDna5('ACG'))
    assert copy(i).goDownStr('AA')
    assert copy(i).goDownStr('A')
    assert copy(i).goDownStr('NN')
    assert copy(i).goDownStr('N')
    check_go_down_str(copy(i), seqan.StringDna5('ACG'), 'ACG')
    check_go_down_str(copy(i), seqan.StringDna5('AC' ), 'ACG')



def test_iterator():
    logging.info(sys._getframe().f_code.co_name)
    _num_bases, sequences, _ids = seqan.readFastaDna5(fasta_file('dm01r.fasta'))
    logging.info('Building index')
    index = seqan.IndexEsaDna5(sequences)

    i = index.TopDownIterator(index)
    assert not i.representative
    assert i.representative.empty()

    i.goDownChar('A')
    assert 'A' == i.representative, i.representative.Value
    i.goDownChar('C')
    assert 'AC' == i.representative
    i.goDownChar('T')
    assert 'ACT' == i.representative
    i.goDownChar('C')
    assert 'ACTC' == i.representative
    logging.info(i.representative)

    # check memory handling
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


def test_iterator():
    """Test by-value initialisation"""
    logging.info(sys._getframe().f_code.co_name)
    _num_bases, sequences, _ids = seqan.readFastaDna5(fasta_file('dm01r.fasta'))
    logging.info('Building index')
    index = seqan.IndexEsaDna5(sequences)
    i = index.TopDownIterator(index)
    i.goDownChar('C')
    val = i.value
    j = index.TopDownIterator(index, val)
    assert 'C' == j.representative

    del index
    logging.info(j.representative)

