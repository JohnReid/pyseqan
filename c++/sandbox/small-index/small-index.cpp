#include <seqan/index.h>

int main( int argc, char * argv[] ) {
    using namespace seqan;

    typedef String< Dna5 > string_t;
    typedef StringSet< string_t > string_set_t;
    typedef Index< string_set_t > index_t;

    string_set_t seqs;
    string_t seq( "A" );
    appendValue( seqs, seq );
    index_t index( seqs );
    Iterator<
        index_t,
        TopDown<>
    >::Type iterator( index );
    goDown( iterator, seq );

    return 0;
}
