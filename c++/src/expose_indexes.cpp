/** Copyright John Reid 2013, 2014
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
    //exposer< Index< StringSet< String< char > > > >::expose(); // doesn't compile - TODO: fix
    exposer< Index< StringSet< String< Dna  > > > >::expose();
    exposer< Index< StringSet< String< Dna5 > > > >::expose();
    //exposer< Index< StringSet< String< Dna5 > >, IndexSa<> > >::expose(); // doesn't compile _getId() problem on vertices
    exposer< Index< StringSet< String< Dna5 > >, IndexWotd<> > >::expose();
    //exposer< Index< StringSet< String< Dna5 > >, FMIndex<> > >::expose(); // doesn't compile _getId() problem on vertices
}
