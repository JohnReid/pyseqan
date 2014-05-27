#include <boost/timer/timer.hpp>

#include <seqan/index.h>
#include <seqan/seq_io.h>
#include <seqan/sequence.h>

#include <fstream>
#include <iostream>

using namespace seqan;


typedef StringSet< DnaString > string_set_t;
typedef Index< string_set_t > index_t;
typedef Iterator< index_t, TopDown<> >::Type top_down_t;
typedef Iterator< index_t, TopDown< ParentLinks<> > >::Type top_down_history_t;


StringSet< CharString > ids;
string_set_t seqs;
const unsigned numdescents = 5000;


int
read_seqs( const char * filename ) {
    // Open file and create RecordReader.
    std::fstream in( filename, std::ios::binary | std::ios::in );
    RecordReader<std::fstream, SinglePass<> > reader( in );

    // Read file in one pass.
    if( read2( ids, seqs, reader, Fasta() ) != 0 ) {
        return 1;  // Could not read file.
    }

    return 0;
}


template< typename It >
unsigned
descendcopy( It it ) {
    unsigned n = 1;
    if( goDown( it ) ) {
        while( true ) {
            n += descendcopy( it );
            if( ! goRight( it ) ) {
                break;
            }
        }
    }
    return n;
}


template< typename It >
unsigned
descendhistory( It & it ) {
    unsigned n = 1;
    if( goDown( it ) ) {
        while( true ) {
            n += descendhistory( it );
            if( ! goRight( it ) ) {
                break;
            }
        }
        goUp( it );
    }
    return n;
}


void
timedescendcopy( index_t & index ) {
    unsigned n;
    std::cout << "Descend copy   ... ";
    boost::timer::auto_cpu_timer t;
    for( unsigned i = 0; numdescents != i; ++i ) {
        n = descendcopy( top_down_t( index ) );
    }
    std::cout << n << " ";
}


void
timedescendhistory( index_t & index ) {
    unsigned n;
    std::cout << "Descend history... ";
    boost::timer::auto_cpu_timer t;
    for( unsigned i = 0; numdescents != i; ++i ) {
        top_down_history_t it( index );
        n = descendhistory( it );
    }
    std::cout << n << " ";
}


int main( int argc, char * argv[] ) {
    if (argc != 2) {
        std::cerr << "Wrong number of arguments.\n";
        return 1;  // Invalid number of arguments.
    }

    if( read_seqs( argv[ 1 ] ) ) {
        std::cerr << "Cannot read sequences\n";
        return 1;
    }

    index_t index( seqs );
    timedescendcopy( index );
    timedescendhistory( index );
    timedescendcopy( index );

    return 0;
}
