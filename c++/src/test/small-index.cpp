#define BOOST_TEST_MODULE smallindex
#include <boost/test/included/unit_test.hpp>

#include <seqan/index.h>

using namespace seqan;
typedef String< Dna5 > string_t;
typedef StringSet< string_t > string_set_t;
typedef Index< string_set_t > index_t;

BOOST_AUTO_TEST_CASE( empty_index ) {
    string_set_t seqs;
    index_t index( seqs );
    Iterator<
        index_t,
        TopDown<>
    >::Type iterator( index );
}

BOOST_AUTO_TEST_CASE( small_index ) {

	string_set_t seqs;
	string_t seq( "A" );
	appendValue( seqs, seq );
	index_t index( seqs );
	Iterator<
		index_t,
		TopDown<>
	>::Type iterator( index );
	goDown( iterator, seq );
}
