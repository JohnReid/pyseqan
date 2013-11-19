/** Copyright John Reid 2013
 *
 * \file
 * \brief Code to expose strings and string sets.
 *
 */

#ifndef PYSEQAN_SIMPLE_TYPE_JR_15FEB2013_
#define PYSEQAN_SIMPLE_TYPE_JR_15FEB2013_

#include <seqan/python/defs.h>
#include <seqan/python/names.h>





namespace seqan {
namespace python {
namespace detail {


template< typename T >
struct is_char_convertible {
    typedef False Type;
};


template< typename TValue, typename TSpec >
struct is_char_convertible< SimpleType< TValue, TSpec > > {
    typedef True Type;
};


template< typename T >
struct is_char_complementable {
    typedef False Type;
};


template<>
struct is_char_complementable< Dna > {
    typedef True Type;
};


template<>
struct is_char_complementable< Dna5 > {
    typedef True Type;
};


template<>
struct is_char_complementable< Rna > {
    typedef True Type;
};


template<>
struct is_char_complementable< Rna5 > {
    typedef True Type;
};


} //namespace detail


/**
 * Mutable pair exposer.
 */
template< typename T1, typename T2, typename TSpec >
struct exposer< Pair< T1, T2, TSpec > >
: myrrh::python::ensure_exposer< exposer< Pair< T1, T2, TSpec > > >
{
    typedef Pair< T1, T2, TSpec > exposed_type;


    static
    std::string
    __str__( exposed_type const & x ) {
        return MYRRH_MAKE_STRING( "Pair<" << x.i1 << "," << x.i2 << ">" );
    }

    static
    void
    expose() {
        py::class_< exposed_type > _class(
            name< exposed_type >().c_str(),
            "Wrapper for SeqAn Pair."
        );

        _class.def( "__str__", __str__, "A string representation." );
        _class.def_readwrite( "i1", &exposed_type::i1 );
        _class.def_readwrite( "i2", &exposed_type::i2 );
    }


};


/**
 * Const pair exposer.
 */
template< typename T1, typename T2, typename TSpec >
struct exposer< const Pair< T1, T2, TSpec > >
: myrrh::python::ensure_exposer< exposer< Pair< T1, T2, TSpec > > >
{
    typedef const Pair< T1, T2, TSpec > exposed_type;


    static
    std::string
    __str__( exposed_type const & x ) {
        return MYRRH_MAKE_STRING( "Pair<" << x.i1 << "," << x.i2 << ">" );
    }

    static
    void
    expose() {
        py::class_< exposed_type > _class(
            name< exposed_type >().c_str(),
            "Wrapper for SeqAn Pair."
        );

        _class.def( "__str__", __str__, "A string representation." );
        _class.def_readonly( "i1", &exposed_type::i1 );
        _class.def_readonly( "i2", &exposed_type::i2 );
    }


};


/**
 * Simple type exposer.
 */
template< typename TValue, typename TSpec >
struct exposer< SimpleType< TValue, TSpec > >
: myrrh::python::ensure_exposer< exposer< SimpleType< TValue, TSpec > > >
{
    typedef SimpleType< TValue, TSpec > exposed_type;

    static
    exposed_type
    fromOrdinal( unsigned i ) {
        return exposed_type( i );
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
        _class.def( py::init< char >() );
        _class.def( "fromOrdinal", fromOrdinal, "Construct from an ordinal value." );
        _class.staticmethod( "fromOrdinal" );
        _class.def( "__init__", py::make_constructor( make_from_string ) );
        _class.def( "__str__", __str__, "A string representation of this simple type." );
        _class.def( "__hash__", _ordValue, "Hash value." );
        _class.add_static_property( "valueSize", _valueSize );
        _class.add_property( "ordValue", _ordValue, "An unsigned value between 0 and valueSize." );

        // having implicitly convertible in both directions causes problems in
        // string comparison operators. Specifically infinite recursions
//        py::implicitly_convertible< exposed_type, char >();
        py::implicitly_convertible< char, exposed_type >();
    }


    static
    void
    expose() {
        py::class_< exposed_type > _class(
            name< exposed_type >().c_str(),
            "Wrapper for simple SeqAn type."
        );

        _class.def( "__eq__", __eq__, "Equality." );
        _class.def( "__ne__", __ne__, "Inequality." );

        expose_string_conversions( _class, typename detail::is_char_convertible< exposed_type >::Type() );
    }


    static
    exposed_type *
    make_from_string( std::string const & s ) {
        if( 1 != s.size() ) {
            throw std::invalid_argument( "Can only make a simple type from strings of length one." );
        }
        return new exposed_type( s[ 0 ] );
    }

    static
    size_t
    __ne__( exposed_type const & _self, py::object other ) {
        return ! __eq__( _self, other );
    }

    static
    bool
    __eq__str( exposed_type const & _self, py::object other, seqan::False && ) {
        return false;
    }

    static
    bool
    __eq__str( exposed_type const & _self, py::object other, seqan::True && ) {

        // otherwise check if we have char
        py::extract< char > char_extractor( other );
        if( char_extractor.check() ) {
            return char_extractor() == _self;
        }

        // if it is an object of length 1 then check whether we are equal
        // to this first element this allows us to compare simple types to
        // strings of length 1.
        try {
            return 1 == py::len( other ) && __eq__( _self, other[ 0 ] );
        } catch( ... ) {
            PyErr_Clear();
        }

        return false;
    }

    static
    bool
    __eq__( exposed_type const & _self, py::object other ) {
        // first check if we have an exposed type
        py::extract< exposed_type > exposed_extractor( other );
        if( exposed_extractor.check() ) {
            return exposed_extractor() == _self;
        }

        return __eq__str( _self, other, typename detail::is_char_convertible< exposed_type >::Type() );
    }

    static
    size_t
    _ordValue( exposed_type const & _self ) {
        return ordValue( _self );
    }

    static
    std::string
    __str__( exposed_type const & _self ) {
        return std::string( 1, _self );
    }

    static
    size_t
    _valueSize() {
        return valueSize< exposed_type >();
    }

};


} // namespace python
} // namespace seqan


#endif //PYSEQAN_SIMPLE_TYPE_JR_15FEB2013_
