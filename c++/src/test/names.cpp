#define BOOST_TEST_MODULE names
#include <boost/test/included/unit_test.hpp>

#include <seqan/python/names.h>
#include <seqan/index.h>

using namespace seqan;
using seqan::python::name;

typedef String< Dna5 > string_t;
typedef StringSet< string_t > string_set_t;
typedef Index< string_set_t > index_t;

BOOST_AUTO_TEST_CASE( names ) {
    // test ints
    BOOST_CHECK_EQUAL( "uint8", name< unsigned long >() );
    BOOST_CHECK_EQUAL(  "int8", name<          long >() );
    BOOST_CHECK_EQUAL( "uint4", name< unsigned  int >() );
    BOOST_CHECK_EQUAL(  "int4", name<           int >() );

    // test seqan simple types
    BOOST_CHECK( IsSimple< long >::Type::VALUE );
    BOOST_CHECK( IsSimple< Dna5 >::Type::VALUE );
    BOOST_CHECK_EQUAL( "Dna" , name< Dna  >() );
    BOOST_CHECK_EQUAL( "Dna5", name< Dna5 >() );
    BOOST_CHECK_EQUAL( "Rna5", name< Rna5 >() );
    BOOST_CHECK_EQUAL( "Rna" , name< Rna  >() );

    // test seqan pair
    BOOST_CHECK_EQUAL( "Pairint4int4", ( name< Pair< int, int > >() ) );
}
