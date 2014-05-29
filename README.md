The pyseqan python package
==========================

pyseqan is a python interface to the SeqAn (http://www.seqan.de/) C++
sequence analysis library. pyseqan exposes functionality such as enhanced
suffix arrays via an easy-to-use python interface.


Efficiency and ease-of-use
--------------------------

pyseqan is efficient because it uses SeqAn's C++ data structures
and implementation. However, its python interface is much
easier to use for non-experts than SeqAn's C++ interface.


Functionality
-------------

pyseqan currently exposes:

- :ref:`Simple types <simple-types>` representing biological alphabets such as DNA and RNA
- :ref:`Sequences <strings>` over these alphabets
- :ref:`Methods <seq-io>` to load sequences from disk
- :ref:`Indexes <indexes>` such as enhanced suffix arrays for efficient look-up and search operations
- :ref:`Iterators <iterators>` over indexes and sequences
- :ref:`Algorithms <traversal>` to traverse over indexes
- :ref:`Methods <disk-io>` to save and load indexes to and from disk
- :ref:`Methods <graphical>` to visualise indexes graphically

There is much more functionality available through the SeqAn C++ API. pyseqan
is extensible and this functionality will be added as the need arises. Please
make requests for additions and any other comments on the Github issues page
(https://github.com/JohnReid/pyseqan/issues).
