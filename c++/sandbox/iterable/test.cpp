#include <boost/timer/timer.hpp>

#include <seqan/index.h>
#include <seqan/seq_io.h>
#include <seqan/sequence.h>

#include <fstream>
#include <iostream>

using namespace seqan;


typedef StringSet< DnaString > string_set_t;
typedef Index< string_set_t > index_t;
typedef Iterator< index_t, TopDown< ParentLinks<> > >::Type top_down_history_t;


StringSet< CharString > ids;
string_set_t seqs;


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

    for( top_down_history_t i = top_down_history_t( index );
        ! atEnd( i );
        goNext( i ) ) {
        std::cout << representative( i ) << "\n";
    }

    return 0;
}
