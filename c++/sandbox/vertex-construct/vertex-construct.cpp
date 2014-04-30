#include <seqan/index.h>
#include <iostream>

int main( int argc, char * argv[] ) {
    using namespace seqan;

    typedef StringSet< String< Dna5 > > string_set_t;
    typedef Index< string_set_t > index_t;
    typedef Iterator< index_t, TopDown<> >::Type iterator_t;
    typedef VertexDescriptor< index_t >::Type vertex_t;
    typedef Reference< iterator_t >::Type reference_t;

    string_set_t seqs;
    appendValue( seqs, "ACGTACGT" );
    index_t index( seqs );

    iterator_t it_acgt( index );
    goDown( it_acgt, "ACGT" );
    vertex_t v = value( it_acgt );

    //reference_t ref = value( it );

    // try to assign value to iterator
    std::cout << "Iterator at \"" << representative( it_acgt ) << "\"\n";
    value( it_acgt ) = value( iterator_t ( index ) );
    std::cout << "Iterator at \"" << representative( it_acgt ) << "\"\n";

    std::cout << sizeof( iterator_t ) << "\n";
    std::cout << sizeof( vertex_t ) << "\n";
    std::cout << sizeof( _getId( v ) ) << "\n";

    return 0;
}
