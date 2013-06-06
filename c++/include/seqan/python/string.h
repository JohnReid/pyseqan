/** Copyright John Reid 2013
 *
 * \file
 * \brief Code to expose strings and string sets.
 *
 */

#ifndef PYSEQAN_STRING_JR_14FEB2013_DEFS_H_
#define PYSEQAN_STRING_JR_14FEB2013_DEFS_H_

#include <seqan/python/defs.h>
#include <seqan/python/container.h>
#include <seqan/python/simple_type.h>
#include <seqan/python/infix.h>

#include <seqan/sequence.h>





namespace seqan {
namespace python {



/**
 * String exposer.
 */
template< typename TValue, typename TSpec = Alloc<> >
struct string_exposer
{

    typedef String< TValue, TSpec > exposed_t;
    typedef typename Position< exposed_t >::Type position_t;
    typedef typename Infix< exposed_t >::Type infix_t;

    static
    infix_t
    _infix( exposed_t & _self, position_t begin, position_t end ) {
        if( begin < 0 ) {
            throw std::invalid_argument( "Begin before start of string." );
        }
        if( end > length( _self ) ) {
            throw std::invalid_argument( "End too large." );
        }
        return infix( _self, begin, end );
    }

    static
    void
    expose() {
		py::class_<
			exposed_t,
			boost::shared_ptr< exposed_t >,
			boost::noncopyable
		> _class(
			MYRRH_MAKE_STRING( "String" << simple_type_name< TValue >() ).c_str(),
			"Wrapper for SeqAn C++ string.",
			py::init< std::string const & >( py::args( "x" ), "Construct a SeqAn string from the python string, x." )
		);
        container_exposer< exposed_t >::expose( _class );
        _class.def( "__str__", std_string_from_seqan< exposed_t >, "String representation." );
        _class.def( "__getitem__", __getitem__< exposed_t >, "Get individual value or a slice. No support for irregular step sizes.", py::with_custodian_and_ward_postcall< 0, 1 >() );
        _class.def( "infix", _infix, "Infix of the string.", py::with_custodian_and_ward_postcall< 0, 1 >() );
        _class.def( "__eq__", string_equals< exposed_t > );
//        _class.def( "__eq__", equals_value< exposed_t > );
//        _class.def( "__ne__", notequals_value< exposed_t > );
//        _class.def( py::self == py::self );
//        _class.def( py::self != py::self );
//        _class.def( py::self == std::string() );
//        _class.def( py::self != std::string() );
//        _class.def( py::self == infix_t() );
//        _class.def( py::self != infix_t() );

        py::implicitly_convertible< std::string, exposed_t >();

        py::scope scope( _class );
        infix_exposer< exposed_t >()( scope, "Infix" );
        simple_type_exposer< TValue >()( scope, "Value" );
    }
};




/**
 * StringSet exposer.
 */
template< typename TString, typename TSpec = Owner< Generous > >
struct string_set_exposer
{
    typedef StringSet< TString > exposed_t;
    typedef typename Value< TString >::Type string_value_t;

    static
    void
    expose() {
        py::class_<
            exposed_t,
            boost::shared_ptr< exposed_t >,
            boost::noncopyable
        > _class(
            MYRRH_MAKE_STRING( "String" << simple_type_name< string_value_t >() << "Set" ).c_str(),
            "Wrapper for SeqAn C++ string set."
        );
        container_exposer< exposed_t >::expose( _class );
    }

};




/**
 * Read a FASTA file into a string set.
 */
template< bool Reverse, typename TString, typename TSpec >
size_t
read_fasta_(
    const char * fasta_filename,
    StringSet< TString, TSpec > & sequences,
    string_vector & ids
)
{
    ::std::ifstream f( fasta_filename, ::std::ios_base::in | ::std::ios_base::binary );
    size_t num_bases = 0;
    if( ! f ) {
        throw std::logic_error( MYRRH_MAKE_STRING( "Could not open: \""<<fasta_filename<<"\"" ) );
    } else {
        //scope_progress_timer timer( std::cout, MYRRH_MAKE_STRING( "Reading fasta file \"" << fasta_filename << "\"" ) );
        String< char > meta;
        TString str;
        while( ! _streamEOF( f ) ) {
            readMeta( f, meta, Fasta() );
            read( f, str, Fasta() );
            if( length( str ) == 0 && length( meta ) == 0 ) continue;
            num_bases += length( str );
            std::string tmp_meta;
            assign( tmp_meta, meta );
            ids.push_back( tmp_meta );
            if( Reverse ) {
            	appendValue( sequences, seqan::ModifiedString< TString, seqan::ModReverse >( str ) );
            } else {
            	appendValue( sequences, str );
            }
        }
        //std::cout << "Read " << length( sequences ) << " sequences with a total of " << num_bases << " bases\n";
        f.close();
    }
    return num_bases;
}



/**
 * Read a FASTA file into a SeqAn string set.
 */
template< typename TStringSet >
boost::python::tuple
read_fasta( const char * fasta_filename, bool reverse ) {
    boost::shared_ptr< string_vector > ids( new string_vector );
    boost::shared_ptr< TStringSet > sequences( new TStringSet );
    const size_t num_bases =
		reverse
			? read_fasta_< true  >( fasta_filename, *sequences, *ids )
			: read_fasta_< false >( fasta_filename, *sequences, *ids )
		    ;
    return boost::python::make_tuple( num_bases, sequences, ids );
}






/**
 * Exposes all string related methods.
 */
template<
    typename TValue,
    typename TStringSpec = Alloc<>,
    typename TSetSpec = Owner< Generous >
>
void
expose_string_functionality()
{
    typedef String< TValue, TStringSpec > string_t;
    typedef StringSet< string_t > string_set_t;

    //register_seqan_string_to_python< typename Infix< string_t >::Type >();
    string_exposer< TValue, TStringSpec >::expose();
    string_set_exposer< string_t, TSetSpec >::expose();
    boost::python::def(
        MYRRH_MAKE_STRING( "readFasta" << simple_type_name< TValue >() ).c_str(),
        read_fasta< string_set_t >,
        ( py::arg( "fasta_filename" ), py::arg( "reverse" )=false ),
        "Read a FASTA file into a seqan string set."
    );
}

} // namespace python
} // namespace seqan

#endif //PYSEQAN_STRING_JR_14FEB2013_DEFS_H_
