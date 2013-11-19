#
# Copyright John Reid 2013
#

"""
Test building an index.
"""

import seqan, sys, logging, tempfile, os
from seqan.test import fasta_file, show_shallow_tree


IndexesDNA5 = (seqan.IndexStringDNA5SetESA, seqan.IndexStringDNA5SetWOTD)

def test_small_index():
    logging.info(sys._getframe().f_code.co_name)
    for IndexDNA5 in IndexesDNA5:
        sequences = seqan.StringDNA5Set()
        sequences.appendValue(seqan.StringDNA5('A'))
        index = IndexDNA5(sequences)
        del sequences
        _i = index.TopDownIterator(index)
        #assert not i.goDown('A')


def test_index_as_strings_custodian():
    logging.info(sys._getframe().f_code.co_name)
    for IndexDNA5 in IndexesDNA5:
        sequences = seqan.StringDNA5Set()
        sequences.appendValue(seqan.StringDNA5('ACG'))
        index = IndexDNA5(sequences)
        del sequences
        i = index.TopDownIterator(index)
        assert i.goDown('A')


def test_build_index():
    logging.info(sys._getframe().f_code.co_name)
    for IndexDNA5 in IndexesDNA5:
        _num_bases, sequences, _ids = seqan.readFastaDNA5(fasta_file('dm01r.fasta'))
        index = IndexDNA5(sequences)
        show_shallow_tree(index.TopDownIterator(index))


def _test_empty_index():
    logging.info(sys._getframe().f_code.co_name)
    for IndexDNA5 in IndexesDNA5:
        sequences = seqan.StringDNA5Set()
        sequences.appendValue(seqan.StringDNA5())
        index = IndexDNA5(sequences)
        logging.info('Creating top-down')
        i = index.topdown()
        1/0  # going down causes SEGV
        logging.info('Going down "A"')
        assert not i.goDown('A')


def test_long_edges():
    logging.info(sys._getframe().f_code.co_name)
    for IndexDNA5 in IndexesDNA5:
        sequences = seqan.StringDNA5Set()
        for seq in (
            'AA',
            'AC',
            'AG',
            'ATACCGGTT',
        ):
            sequences.appendValue(seqan.StringDNA5(seq))
        index = IndexDNA5(sequences)
        it = index.topdown()
        assert it.goDown('A')
        assert 'A' == it.representative
        assert it.goDown('T')
        logging.debug('%s: %s', IndexDNA5.__name__, it.representative)
        assert 'ATACCGGTT' == it.representative
        it = index.topdown()
        assert it.goDown('AT')
        logging.debug('%s: %s', IndexDNA5.__name__, it.representative)
        assert 'ATACCGGTT' == it.representative


def test_save_load_index():
    logging.info(sys._getframe().f_code.co_name)
    for IndexDNA5 in IndexesDNA5:
        _num_bases, sequences, _ids = seqan.readFastaDNA5(fasta_file('dm01r.fasta'))
        index = IndexDNA5(sequences)
        tmpdir = tempfile.mkdtemp(suffix='test-index-save')
        filename = os.path.join(tmpdir, 'index.saved')
        index.save(filename)
        index2 = IndexDNA5.load(filename)
        i = index2.topdown()
        assert i.goDown('A')
        text = index2.text
        print text[0][:4]


def test_index_text():
    logging.info(sys._getframe().f_code.co_name)
    for IndexDNA5 in IndexesDNA5:
        _num_bases, sequences, _ids = seqan.readFastaDNA5(fasta_file('dm01r.fasta'))
        index = IndexDNA5(sequences)
        assert len(index.text) == len(_ids)
        assert len(index.text[0]) == len(sequences[0])


def test_infix_comparison():
    logging.info(sys._getframe().f_code.co_name)
    for IndexDNA5 in IndexesDNA5:
        _num_bases, sequences, _ids = seqan.readFastaDNA5(fasta_file('dm01r.fasta'))
        index = IndexDNA5(sequences)
        i = index.TopDownIterator(index)
        assert i.goDown('A')
        assert i.goDown('T')
        assert i.goDown('C')
        assert 'ATC' == i.representative, i.representative


def test_go_down():
    logging.info(sys._getframe().f_code.co_name)
    for IndexDNA5 in IndexesDNA5:
        sequences = seqan.StringDNA5Set()
        for seq in (
            'ACGT',
            'AAAA',
            'GGGG',
            'AC',
        ):
            sequences.appendValue(seqan.StringDNA5(seq))
        index = IndexDNA5(sequences)
        for expected_occs, texts in (
            (2, (seqan.StringDNA5('AC'), 'AC')),
            (1, (seqan.DNA5('T'), 'T', seqan.StringDNA5('T'))),
            (6, (seqan.DNA5('A'), 'A', seqan.StringDNA5('A'))),
        ):
            for text in texts:
                logging.info('Trying %s: %s', type(text).__name__, text)
                it = index.TopDownIterator(index)
                assert it.goDown(text)
                for occ in it.occurrences:
                    logging.info('Occurrence for %s: %s', text, occ)
                assert expected_occs == len(it.occurrences)


if '__main__' == __name__:
    logging.basicConfig(level=logging.DEBUG)
    test_go_down()
    test_infix_comparison()
    test_index_text()
    test_save_load_index()
    test_long_edges()
    test_build_index()
    test_index_as_strings_custodian()
    test_small_index()
    test_empty_index()
