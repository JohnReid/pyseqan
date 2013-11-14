/** Copyright John Reid 2013
 *
 * \file Exposes seqan parts of STEME algorithm to python.
 */

#include "pyseqan_pch.h"
#include <seqan/python/defs.h>
#include <seqan/python/index.h>


/**
 * Expose indexes.
 */
void
expose_indexes() {
	using namespace seqan;
    using namespace seqan::python;
    exposer< Index< StringSet< String< Dna  > > > >::expose();
    exposer< Index< StringSet< String< Dna5 > > > >::expose();
}
