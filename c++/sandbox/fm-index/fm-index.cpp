#include <seqan/index.h>

using namespace seqan;

typedef String< Dna5 > string_t;
typedef StringSet< string_t > string_set_t;



template< typename Iterator >
void
descend( Iterator i ) {
    std::cout << representative( i ) << "\n";
    if( goDown( i ) ) {
        while( true ) {
            descend( i );
            if( ! goRight( i ) ) {
                break;
            }
        }
    }
}


template< typename Index >
void
test_index( Index index ) {
    typename Iterator< Index, TopDown<> >::Type iterator( index );
    descend( iterator );
    goDown( iterator, string_t( "AC" ) );
    goDown( iterator, 'A' );
    goDown( iterator, "AC" );
    goDown( iterator, std::string( "AC" ) );
}


int main( int argc, char * argv[] ) {
    string_set_t seqs;
    appendValue( seqs, "AC" );
    appendValue( seqs, "A" );
    appendValue( seqs, "ACGT" );
    test_index( Index< string_set_t, IndexEsa<> >( seqs ) );
    test_index( Index< string_set_t, IndexWotd<> >( seqs ) );
    test_index( Index< string_set_t, FMIndex<> >( seqs ) );
    return 0;
}
