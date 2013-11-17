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
    strings = seqan.StringDNA5Set()
    strings.appendValue(seqan.StringDNA5('ACG'))
    strings.appendValue(seqan.StringDNA5('AA'))
    strings.appendValue(seqan.StringDNA5('NN'))
    return seqan.IndexStringDNA5SetESA(strings)


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
    assert copy(i).goDownStr(seqan.StringDNA5('C'))
    assert copy(i).goDownStr('C')
    assert not copy(i).goDownStr(seqan.StringDNA5('T'))
    assert not copy(i).goDownStr('T')
    assert not copy(i).goDownStr(seqan.StringDNA5('ACGT'))
    assert copy(i).goDownStr('AC')
    assert copy(i).goDownStr(seqan.StringDNA5('AC'))
    assert copy(i).goDownStr(seqan.StringDNA5('ACG'))
    assert copy(i).goDownStr('AA')
    assert copy(i).goDownStr('A')
    assert copy(i).goDownStr('NN')
    assert copy(i).goDownStr('N')
    check_go_down_str(copy(i), seqan.StringDNA5('ACG'), 'ACG')
    check_go_down_str(copy(i), seqan.StringDNA5('AC' ), 'ACG')


def _build_index():
    _num_bases, sequences, _ids = seqan.readFastaDNA5(fasta_file('dm01r.fasta'))
    logging.info('Building index')
    return seqan.IndexStringDNA5SetESA(sequences)


def test_iterator():
    logging.info(sys._getframe().f_code.co_name)
    index = _build_index()

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
    assert i.parentEdgeLabel == 'ACTC'[-i.parentEdgeLength:]

    # check memory handling
    r = i.representative
    print r
    del i
    del index
    print r

    index = _build_index()
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


def test_get_occurrences():
    """Test getting iterator occurrences"""
    logging.info(sys._getframe().f_code.co_name)
    index = _build_index()
    i = index.TopDownIterator(index)
    i.goDownChar('C')
    occs = i.occurrences
    print type(occs)
    for occ in occs:
        print occ


def test_by_value_init():
    """Test by-value initialisation"""
    logging.info(sys._getframe().f_code.co_name)
    index = _build_index()
    i = index.TopDownIterator(index)
    i.goDownChar('C')
    val = i.value
    j = index.TopDownIterator(index, val)
    assert 'C' == j.representative

    del index
    logging.info(j.representative)


if '__main__' == __name__:
    test_get_occurrences()