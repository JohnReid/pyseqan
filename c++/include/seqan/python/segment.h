/** Copyright John Reid 2013
 *
 * \file
 * \brief Code to expose infixes.
 *
 */

#ifndef PYSEQAN_SEGMENT_JR_4JUN2013_
#define PYSEQAN_SEGMENT_JR_4JUN2013_

#include <seqan/python/defs.h>
#include <seqan/python/simple_type.h>
#include <seqan/python/names.h>
#include <seqan/python/container.h>


namespace seqan {
namespace python {


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



template< typename THost, typename TSpec >
struct exposer< Segment< THost, TSpec > >
: myrrh::python::ensure_exposer< exposer< Segment< THost, TSpec > > >
{
    typedef Segment< THost, TSpec >                 exposed_type;
    typedef typename Value< exposed_type >::Type    value_type;

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
        _class.def( "__str__", std_string_from_seqan< exposed_type >, "String representation." );
    }

    void
    expose() {
        py::class_< exposed_type > _class(
            name< exposed_type >().c_str(),
            "Infix of string."
        );
        const_container_exposer< exposed_type >::expose( _class );
        expose_string_conversions( _class, typename detail::is_char_convertible< value_type >::Type() );
        _class.def( "__getitem__", __getitem__< exposed_type >, "Get individual value or a slice. No support for irregular step sizes.", py::with_custodian_and_ward_postcall< 0, 1 >() );
        _class.def( "__eq__", string_equals< exposed_type > );
        _class.def( "__ne__", string_notequals< exposed_type > );

        exposer< value_type >().ensure_exposed_and_add_as_attr( _class, "Value" );
    }
};


} //namespace python
} //namespace seqan

#endif //PYSEQAN_SEGMENT_JR_4JUN2013_
