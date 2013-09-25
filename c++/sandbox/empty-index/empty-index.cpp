#include <seqan/index.h>

int main( int argc, char * argv[] ) {
	using namespace seqan;

	typedef StringSet< String< Dna5 > > string_set_t;
	typedef Index< string_set_t > index_t;

	string_set_t seqs;
	index_t index( seqs );
	Iterator<
		index_t,
		TopDown<>
	>::Type iterator( index );

	return 0;
}
