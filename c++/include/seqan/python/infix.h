/** Copyright John Reid 2013
 *
 * \file
 * \brief Code to expose infixes.
 *
 */

#ifndef PYSEQAN_INFIX_JR_4JUN2013_
#define PYSEQAN_INFIX_JR_4JUN2013_

#include <seqan/python/defs.h>





namespace seqan {
namespace python {


/**
 * SeqAnise a boost::python::object that should be some sort of string.
 * That is leave it alone if it is already a SeqAn type (or some other random type).
 * If it is a python string then convert it to a SeqAn string over the given
 * alphabet.
 *
 * The motivation is that plenty of the methods in this package can handle
 * arguments of several types of SeqAn string but not python strings. We
 * use this function to check the arguments.
 */
template< typename TString >
py::object
seqanise_string( py::object str ) {
    if( PyString_Check( str.ptr() ) ) {
        //std::cout << "Seqanising\n";
        boost::shared_ptr< TString > seqanised( new TString( py::extract< TString >( str )() ) );
        return py::object( seqanised );
    } else {
        return str; // do not modify
    }
}


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



template< typename TString >
struct infix_exposer
: myrrh::python::expose_or_set_attribute< infix_exposer< TString > >
{

    typedef typename seqan::Infix< TString >::Type exposed_type;
    typedef typename Value< exposed_type >::Type value_type;

    void
    expose( const char * name ) {
        py::class_< exposed_type > _class(
            name,
            "Infix of string."
        );
        const_container_exposer< exposed_type >::expose( _class );
        _class.def( "__str__", std_string_from_seqan< exposed_type >, "String representation." );
        _class.def( "__getitem__", __getitem__< exposed_type >, "Get individual value or a slice. No support for irregular step sizes.", py::with_custodian_and_ward_postcall< 0, 1 >() );
        _class.def( "__eq__", string_equals< exposed_type > );
        _class.def( "__ne__", string_notequals< exposed_type > );

        py::scope scope( _class );
        simple_type_exposer< value_type >()( scope, "Value" );
    }
};

} //namespace python
} //namespace seqan

#endif //PYSEQAN_INFIX_JR_4JUN2013_
