/** Copyright John Reid 2013
 *
 * \file
 * \brief Code to expose strings and string sets.
 *
 */

#ifndef PYSEQAN_STRING_BASE_JR_15NOV2013_
#define PYSEQAN_STRING_BASE_JR_15NOV2013_

#include <seqan/python/defs.h>
#include <myrrh/python/boost_range.h>


namespace seqan {
namespace python {
namespace detail {


template< typename TString >
bool
string_equals( TString const & str, py::object obj ) {
    typedef typename Value< TString >::Type value_type;

    // if our string is length 1 we check whether the python object is in
    // fact the same as the value type of our string that matches it
    typedef typename Size< TString >::Type size_type;
    const size_type len_str = length( str );
    if( 1 == len_str ) {
        py::extract< value_type > extractor( obj );
        if( extractor.check() ) {
            return str[ 0 ] == extractor();
        }
    }

    // string and object must be same length
    try {
        if( int( len_str ) != py::len( obj ) ) {
            return false;
        }
    } catch( ... ) {
        PyErr_Clear();
        return false;
    }

    // string and object must have same values at all positions
    try {
        for( size_t i = 0; length( str ) != i; ++i ) {
            if( str[ i ] != py::extract< value_type >( obj[ i ] )() ) {
                return false;
            }
        }
        return true;
    } catch( ... ) {
        PyErr_Clear();
        return false;
    }
}


template< typename TString >
bool
string_notequals( TString const & str, py::object obj ) {
    return ! string_equals( str, obj );
}



/** Expose string conversions if value is suitable. */
template< typename Exposed, typename Class >
void
expose_string_conversions( Class & _class, seqan::False && ) {
}


/** Expose string conversions if value is suitable. */
template< typename Exposed, typename Class >
void
expose_string_conversions( Class & _class, seqan::True && ) {
    _class.def( "__str__", std_string_from_seqan< Exposed >, "String representation." );
}


/** Expose string conversions if value is suitable. */
template< typename Exposed, typename Class >
void
expose_string_reverse_complement( Class & _class, seqan::False && ) {
}


template< typename Exposed, typename Modified >
Modified
make_modified( Exposed & x ) {
    return Modified( x );
}


/** Expose string conversions if value is suitable. */
template< typename Exposed, typename Class >
void
expose_string_reverse_complement( Class & _class, seqan::True && ) {
    typedef typename Value< Exposed >::Type value_type;
    typedef ModifiedString< ModifiedString< Exposed, ModView< FunctorComplement< value_type > > >, ModReverse > revcomp_t;
    exposer< revcomp_t >().ensure_exposed_and_add_as_attr( _class, "ReverseComplement" );
    _class.def( "reversecomplement", make_modified< Exposed, revcomp_t >, "Reverse complement of string.", py::with_custodian_and_ward_postcall< 0, 1 >() );
}


/** Expose string conversions if value is suitable. */
template< typename Exposed, typename Class >
void
expose_string_constructors( Class & _class, seqan::False && ) {
}


/** Expose string conversions if value is suitable. */
template< typename Exposed, typename Class >
void
expose_string_constructors( Class & _class, seqan::True && ) {
    _class.def(
        py::init< std::string const & >(
            py::arg( "x" ),
            "Construct a SeqAn string from the python string, x." ));
    py::implicitly_convertible< std::string, Exposed >();
}


} // namespace detail
} // namespace python
} // namespace seqan


#endif //PYSEQAN_STRING_BASE_JR_15NOV2013_
