/** Copyright John Reid 2013
 *
 * \file
 * \brief Code to expose strings and string sets.
 *
 */

#ifndef PYSEQAN_SIMPLE_TYPE_JR_15FEB2013_DEFS_H_
#define PYSEQAN_SIMPLE_TYPE_JR_15FEB2013_DEFS_H_

#include <seqan/python/defs.h>





namespace seqan {
namespace python {



/**
 * Meta-function to produce correct name for a simple type spec.
 */
template< typename TSpec >
struct _simple_type_name  {
};

/// Specialisation of meta-function to produce correct name for a simple type spec.
template<>
struct _simple_type_name< Dna_ > {
    static const char * result;
};

/// Specialisation of meta-function to produce correct name for a simple type spec.
template<>
struct _simple_type_name< Dna5_ > {
    static const char * result;
};

/// Specialisation of meta-function to produce correct name for a simple type spec.
template<>
struct _simple_type_name< Rna_ > {
    static const char * result;
};

/// Specialisation of meta-function to produce correct name for a simple type spec.
template<>
struct _simple_type_name< Rna5_ > {
    static const char * result;
};

/// Specialisation of meta-function to produce correct name for a simple type spec.
template<>
struct _simple_type_name< Iupac_ > {
    static const char * result;
};

/// Specialisation of meta-function to produce correct name for a simple type spec.
template<>
struct _simple_type_name< AminoAcid_ > {
    static const char * result;
};


/**
 * Returns the name of the simple type: Dna, Dna5, Rna, Rna5, AminoAcid, Iupac
 */
template< typename TValue >
const char *
simple_type_name() {
    return _simple_type_name< typename Spec< TValue >::Type >::result;
}

/**
 * Simple type exposer.
 */
template< typename Exposed >
struct simple_type_exposer {

	static
	Exposed
	_construct( char c ) {
		return Exposed( c );
	}

    static
    void
    expose() {
        namespace py = boost::python;

        py::class_< Exposed > _class(
            simple_type_name< Exposed >(),
            "Wrapper for SeqAn C++ alphabet.",
            py::init< char >()
        );
        _class.def( "__init__", py::make_constructor( make_from_string ) );
        _class.def( "__str__", __str__, "A string representation of this simple type." );

		_class.def( py::self == py::self );
		_class.def( py::self != py::self );
		_class.def( py::self <= py::self );
		_class.def( py::self >= py::self );
		_class.def( py::self <  py::self );
		_class.def( py::self >  py::self );

        _class.def( "__hash__", __hash__, "Hash value." );
        _class.add_static_property( "valueSize", _valueSize );

        py::implicitly_convertible< Exposed, char >();
        py::implicitly_convertible< char, Exposed >();
    }

    static
    Exposed *
    make_from_string( std::string const & s ) {
    	if( 1 != s.size() ) {
    		throw std::invalid_argument( "Can only make a simple type from strings of length one." );
    	}
        return new Exposed( s[ 0 ] );
    }

    static
    size_t
    __hash__( Exposed const & _self ) {
        return ordValue( _self );
    }

    static
    std::string
    __str__( Exposed const & _self ) {
        return std::string( 1, _self );
    }

    static
    size_t
    _valueSize() {
        return valueSize< Exposed >();
    }

};




} // namespace python
} // namespace seqan

#endif //PYSEQAN_SIMPLE_TYPE_JR_15FEB2013_DEFS_H_
