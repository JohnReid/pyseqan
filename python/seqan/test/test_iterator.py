#
# Copyright John Reid 2013, 2014
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


def _test_representative_equality(index, Iterator):
    i = Iterator(index)
    for n, c in enumerate('ACG'):
        i.goDown(c)
        logging.info(i.representative[:n+1])
        assert i.representative[:n+1] == 'ACG'[:n+1]
        assert 'ACG'[:n+1] == i.representative[:n+1]


def test_representative_equality():
    logging.info(sys._getframe().f_code.co_name)
    index = _make_test_index()
    _test_representative_equality(index, index.TopDownIterator)
    _test_representative_equality(index, index.TopDownHistoryIterator)


def test_go_up():
    logging.info(sys._getframe().f_code.co_name)
    strings = seqan.StringDNA5Set()
    strings.appendValue(seqan.StringDNA5('ACGT'))
    strings.appendValue(seqan.StringDNA5('ACGA'))
    strings.appendValue(seqan.StringDNA5('ACAT'))
    strings.appendValue(seqan.StringDNA5('AAGT'))
    strings.appendValue(seqan.StringDNA5('T'))
    index = seqan.IndexStringDNA5SetESA(strings)
    i = index.TopDownHistoryIterator(index)
    i.goDown('ACGT')
    j = copy(i)
    assert 'ACGT' == j.representative
    j.goUp()
    assert 'ACG'  == j.representative
    j.goUp()
    assert 'AC'   == j.representative
    j.goUp()
    assert 'A'    == j.representative
    j.goUp()
    assert ''     == j.representative
    assert 'ACGT' == i.representative
    i.goUp()
    i.goDown('A')
    assert 'ACGA' == i.representative


def check_go_down_str(i, s, representative):
    i.goDown(s)
    assert representative == i.representative


def _test_go_down_str(index, Iterator):
    i = Iterator(index)
    assert copy(i).goDown(seqan.StringDNA5('C'))
    assert copy(i).goDown('C')
    assert not copy(i).goDown(seqan.StringDNA5('T'))
    assert not copy(i).goDown('T')
    assert not copy(i).goDown(seqan.StringDNA5('ACGT'))
    assert copy(i).goDown('AC')
    assert copy(i).goDown(seqan.StringDNA5('AC'))
    assert copy(i).goDown(seqan.StringDNA5('ACG'))
    assert copy(i).goDown('AA')
    assert copy(i).goDown('A')
    assert copy(i).goDown('NN')
    assert copy(i).goDown('N')
    check_go_down_str(copy(i), seqan.StringDNA5('ACG'), 'ACG')
    check_go_down_str(copy(i), seqan.StringDNA5('AC' ), 'ACG')


def test_go_down_str():
    logging.info(sys._getframe().f_code.co_name)
    index = _make_test_index()
    _test_go_down_str(index, index.TopDownIterator)
    _test_go_down_str(index, index.TopDownHistoryIterator)


def _build_index():
    _num_bases, sequences, _ids = seqan.readFastaDNA5(fasta_file('dm01r.fasta'))
    logging.info('Building index')
    return seqan.IndexStringDNA5SetESA(sequences)


def _test_iterator(index, Iterator):
    i = Iterator(index)
    assert i.numOccurrences >= i.numChildren
    assert not i.representative
    assert i.representative.empty()

    i.goDown('A')
    assert 'A' == i.representative, i.representative.Value
    assert i.numOccurrences >= i.numChildren
    i.goDown('C')
    assert 'AC' == i.representative
    i.goDown('T')
    assert 'ACT' == i.representative
    i.goDown('C')
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
    i = Iterator(index)
    j = Iterator(index)
    assert j != i
    print i.value.id, j.value.id
    assert j.value == i.value
    i.goDown('C')
    assert j.value != i.value
    j.goDown('C')
    assert j != i
    assert j.value == i.value


def test_iterator():
    logging.info(sys._getframe().f_code.co_name)
    index = _build_index()
    _test_iterator(index, index.TopDownIterator)
    _test_iterator(index, index.TopDownHistoryIterator)


def _test_get_occurrences(index, Iterator):
    """Test getting iterator occurrences"""
    i = Iterator(index)
    i.goDown('C')
    occs = i.occurrences
    print type(occs)
    for occ in occs:
        print occ


def test_get_occurrences():
    """Test getting iterator occurrences"""
    logging.info(sys._getframe().f_code.co_name)
    index = _build_index()
    _test_get_occurrences(index, index.TopDownIterator)
    _test_get_occurrences(index, index.TopDownHistoryIterator)


def test_by_value_init():
    """Test by-value initialisation"""
    logging.info(sys._getframe().f_code.co_name)
    index = _build_index()
    i = index.TopDownIterator(index)
    i.goDown('C')
    val = i.value
    j = index.TopDownIterator(index, val)
    assert 'C' == j.representative

    del index
    logging.info(j.representative)


if '__main__' == __name__:
    test_get_occurrences()
