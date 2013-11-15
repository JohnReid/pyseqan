The pyseqan python package
==========================

pyseqan is a python interface to the SeqAn (http://www.seqan.de/) C++
sequence analysis library. pyseqan exposes functionality such as enhanced
suffix arrays via an easy-to-use python interface.


Efficiency
----------

pyseqan is efficient because it uses SeqAn's C++ data structures
and implementation. However, its python interface is much
easier to use for non-experts than SeqAn's C++ interface.


Functionality
-------------

pyseqan currently exposes:

- Simple types representing biological alphabets such as DNA and RNA
- Sequences over these alphabets
- Indexes such as enhanced suffix arrays for efficient look-up and search operations
- Iterators over indexes and sequences

There is much more functionality available through the SeqAn C++ API. pyseqan
is extensible and will add this functionality as needed. Please make requests
for additions and any other comments on the
Github issues page (https://github.com/JohnReid/pyseqan/issues).
