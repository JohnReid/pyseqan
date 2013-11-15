/** Copyright John Reid 2013
 *
 * \file
 * \brief Code to expose strings and string sets.
 *
 */

#ifndef PYSEQAN_STRING_JR_14FEB2013_
#define PYSEQAN_STRING_JR_14FEB2013_

#include <seqan/python/defs.h>
#include <seqan/python/container.h>
#include <seqan/python/simple_type.h>
#include <seqan/python/names.h>
#include <seqan/python/segment.h>
#include <seqan/sequence.h>
#include <myrrh/python/boost_range.h>


namespace seqan {
namespace python {
namespace detail {


/// Specialisation for seqan string
template< typename TValue, typename TSpec >
struct _name< String< TValue, TSpec >, void >
{
    std::string operator()() const {
        return MYRRH_MAKE_STRING( "String" << name< TValue >() );
    }
};


/// Specialisation for seqan segment
template< typename THost, typename TSpec >
struct _name< Segment< THost, TSpec >, void >
{
    std::string operator()() const {
        return MYRRH_MAKE_STRING( "" << name< THost >() << name< TSpec >() );
    }
};


/// Specialisation for infix
template<>
struct _name< InfixSegment, void >
{
    std::string operator()() const {
        return "Infix";
    }
};


/// Specialisation for suffix
template<>
struct _name< SuffixSegment, void >
{
    std::string operator()() const {
        return "Suffix";
    }
};


/// Specialisation for postfix
template<>
struct _name< PrefixSegment, void >
{
    std::string operator()() const {
        return "Prefix";
    }
};


/// Specialisation for seqan string set
template< typename TString, typename TSpec >
struct _name< StringSet< TString, TSpec >, void >
{
    std::string operator()() const {
        return MYRRH_MAKE_STRING( name< TString >() << "Set" );
    }
};


} // namespace detail


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
 * String exposer.
 */
template< typename TValue, typename TSpec >
struct exposer< String< TValue, TSpec > >
: myrrh::python::ensure_exposer< exposer< String< TValue, TSpec > > >
{

    typedef String< TValue, TSpec > exposed_type;
    typedef typename Position< exposed_type >::Type position_t;
    typedef typename Infix< exposed_type >::Type infix_t;

    static
    infix_t
    _infix( exposed_type & _self, position_t begin, position_t end ) {
        if( begin < 0 ) {
            throw std::invalid_argument( "Begin before start of string." );
        }
        if( end > length( _self ) ) {
            throw std::invalid_argument( "End too large." );
        }
        return infix( _self, begin, end );
    }

    /** Expose string conversions if value is suitable. */
    template< typename Class >
    static
    void
    expose_string_conversions( Class & _class, seqan::False && ) {
    }

    /** Expose string conversions if value is suitable. */
    template< typename Class >
    static
    void
    expose_string_conversions( Class & _class, seqan::True && ) {
        _class.def(
            py::init< std::string const & >(
                py::arg( "x" ),
                "Construct a SeqAn string from the python string, x." ));
        _class.def( "__str__", std_string_from_seqan< exposed_type >, "String representation." );
        py::implicitly_convertible< std::string, exposed_type >();
    }

    static
    void
    expose() {
        //std::cout << "Registering: " << name< exposed_type >() << " : " << typeid( exposed_type ).name() << "\n";
        py::class_<
            exposed_type,
            boost::shared_ptr< exposed_type >,
            boost::noncopyable
        > _class(
            name< exposed_type >().c_str(),
            "Wrapper for SeqAn C++ string."
        );
        container_exposer< exposed_type >::expose( _class );
        _class.def( "__getitem__", __getitem__< exposed_type >, "Get individual value or a slice. No support for irregular step sizes.", py::with_custodian_and_ward_postcall< 0, 1 >() );
        _class.def( "infix", _infix, "Infix of the string.", py::with_custodian_and_ward_postcall< 0, 1 >() );
        _class.def( "__eq__", string_equals< exposed_type > );
        _class.def( "__ne__", string_notequals< exposed_type > );

        expose_string_conversions( _class, typename detail::is_char_convertible< TValue >::Type() );

        exposer< infix_t >().ensure_exposed_and_add_as_attr( _class, "Infix" );
        exposer< TValue >().ensure_exposed_and_add_as_attr( _class, "Value" );
    }
};




/**
 * StringSet exposer.
 */
template< typename TString, typename TSpec >
struct exposer< StringSet< TString, TSpec > >
{
    typedef StringSet< TString, TSpec > exposed_type;
    typedef typename Value< TString >::Type string_value_t;


    static
    boost::shared_ptr< exposed_type >
    init_from_sequence( py::object pyseq ) {
        boost::shared_ptr< exposed_type > result( new exposed_type );
        BOOST_FOREACH(
            TString const & x,
            myrrh::python::make_boost_range< TString const >( pyseq )
        ) {
            appendValue( *result, x );
        }
        return result;
    }


    static
    void
    expose() {
        py::def(
            MYRRH_MAKE_STRING( "readFasta" << name< string_value_t >() ).c_str(),
            read_fasta< exposed_type >,
            ( py::arg( "fasta_filename" ), py::arg( "reverse" )=false ),
            "Read a FASTA file into a seqan string set. Returns tuple "
            "(num_bases, sequences, sequence_ids)."
        );

        py::class_<
            exposed_type,
            boost::shared_ptr< exposed_type >,
            boost::noncopyable
        > _class(
            name< exposed_type >().c_str(),
            "Wrapper for SeqAn C++ string set."
        );
        _class.def( "__init__", py::make_constructor( init_from_sequence ), "Construct a string set from a sequence of strings." );
        container_exposer< exposed_type >::expose( _class );
    }
};


/**
 * Exposes all string related methods.
 */
template<
    typename TValue,
    typename TStringSpec = Alloc<>
>
void
expose_string_functionality()
{
    typedef String< TValue, TStringSpec > string_t;
    exposer< string_t >::expose();
    exposer< StringSet< string_t > >::expose();
}


} // namespace python
} // namespace seqan


#endif //PYSEQAN_STRING_JR_14FEB2013_
