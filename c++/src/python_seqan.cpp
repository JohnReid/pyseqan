/** Copyright John Reid 2013
 *
 * \file Exposes seqan parts of STEME algorithm to python.
 */

#include "pyseqan_pch.h"
#include <seqan/python/defs.h>
#include <seqan/python/simple_type.h>
#include <seqan/python/string.h>
#include <seqan/python/index.h>
#include <boost/static_assert.hpp>
#include <boost/type_traits.hpp>



//
// Use some namespaces.
//
using namespace seqan::python;



//
// Instantiate the names of our simple types.
//
namespace seqan {
namespace python {

const char * _simple_type_name< Dna_ >::result       = "Dna";
const char * _simple_type_name< Dna5_ >::result      = "Dna5";
const char * _simple_type_name< Rna_ >::result       = "Rna";
const char * _simple_type_name< Rna5_ >::result      = "Rna5";
const char * _simple_type_name< Iupac_ >::result     = "Iupac";
const char * _simple_type_name< AminoAcid_ >::result = "AminoAcid";

void
check_string_conversion( String< Dna5 > const & s ) {
}

} // namespace python
} // namespace seqan




/**
 * Expose seqan functionality.
 */
void
expose_seqan() {

    //
    // A vector of std::strings.
    //
	py::class_<
		string_vector,
		boost::shared_ptr< string_vector >,
		boost::noncopyable
	> string_vec_class( "StringVec", "A sequence of strings", py::no_init );
	string_vec_class.def( py::indexing::container_suite< string_vector >() );


	using namespace seqan;

	py::def( "check_string_conversion", check_string_conversion );


    simple_type_exposer< Dna >::expose();
    simple_type_exposer< Dna5 >::expose();
    simple_type_exposer< Rna >::expose();
    simple_type_exposer< Rna5 >::expose();
    simple_type_exposer< Iupac >::expose();
    simple_type_exposer< AminoAcid >::expose();
    expose_string_functionality< Dna >();
    expose_string_functionality< Dna5 >();
    index_exposer< StringSet< String< Dna > > >::expose();
    index_exposer< StringSet< String< Dna5 > > >::expose();
}
