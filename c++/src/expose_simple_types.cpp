/** Copyright John Reid 2013
 *
 * \file Exposes seqan parts of STEME algorithm to python.
 */

#include "pyseqan_pch.h"
#include <seqan/python/defs.h>
#include <seqan/python/simple_type.h>


/**
 * Expose seqan functionality.
 */
void
expose_simple_types() {
	using namespace seqan;
    using namespace seqan::python;
    exposer< Dna >::expose();
    exposer< Dna5 >::expose();
    exposer< Rna >::expose();
    exposer< Rna5 >::expose();
    exposer< Iupac >::expose();
    exposer< AminoAcid >::expose();
}
