/** Copyright John Reid 2013
 *
 * \file Exposes seqan parts of STEME algorithm to python.
 */

#include "pyseqan_pch.h"
#include <seqan/python/defs.h>
#include <seqan/python/string.h>


/**
 * Expose seqan functionality.
 */
void
expose_strings() {

	using namespace seqan;
    using namespace seqan::python;
    expose_string_functionality< Dna  >();
    expose_string_functionality< Dna5 >();
}
