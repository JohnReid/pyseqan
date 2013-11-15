
Sequence types
==============

..  testsetup:: *

    import seqan

Simple types
------------

SeqAn simple types represent alphabets for DNA, RNA and amino acids.

..  doctest::

    >>> print seqan.DNA('A')
    A
    >>> print seqan.RNA('U')
    U
    >>> print seqan.IUPAC('V')
    V
    >>> print seqan.AminoAcid('K')
    K

The DNA and RNA types have versions that can represent ambiguous characters.

..  doctest::

    >>> print seqan.DNA5('N')
    N
    >>> print seqan.DNA.valueSize
    4
    >>> print seqan.DNA5.valueSize
    5
    >>> print seqan.RNA5('N')
    N


Strings
-------

Strings are sequence containers.

..  doctest::

    >>> seq = seqan.StringDNA5('ACGTNNNNACGT')
    >>> print seq[4:8]
    NNNN
    >>> seq = seqan.StringDNA('ACGTACGTACGT')
    >>> print seq[4:8]
    ACGT
    >>> print seqan.StringDNA('AAGG').reversecomplement()
    CCTT


String sets
-----------

A StringSet is a container of strings.

..  doctest::

    >>> seqs = seqan.StringDNASet(('ACGT', 'AAAA', 'GGGG'))
    >>> print type(seqs[0]).__name__
    StringDNA
    >>> print seqs[0]
    ACGT
    >>> print seqs[1]
    AAAA
    >>> print seqs[2]
    GGGG


Input/output
------------

Sequences can be read into string sets from FASTA files::

    >>> num_bases_read, seqs, seq_ids = seqan.readFastaDNA5('my-fasta-file.fa')
    >>> print 'Read %d bases in %d sequences' % (num_bases_read, len(seqs))
    Read 3412 bases in 10 sequences

