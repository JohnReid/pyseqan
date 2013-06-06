/** Copyright John Reid 2013
 *
 * \file
 * \brief Code to expose strings and string sets.
 *
 */

#ifndef PYSEQAN_CONTAINER_JR_15FEB2013_DEFS_H_
#define PYSEQAN_CONTAINER_JR_15FEB2013_DEFS_H_

#include <seqan/python/defs.h>

#include <seqan/basic.h>




namespace seqan {
namespace python {


inline
void index_error() {
	PyErr_SetString( PyExc_IndexError, "Index out of range" );
}


template< typename TContainer >
bool
equals_value( TContainer const & s, typename GetValue< TContainer >::Type v ) {
	return 1 == seqan::length( s ) && s[ 0 ] == v;
}


template< typename TContainer >
bool
notequals_value( TContainer const & s, typename GetValue< TContainer >::Type v ) {
	return 1 != seqan::length( s ) || s[ 0 ] != v;
}




/**
 * Const container exposer.
 */
template< typename Exposed >
struct const_container_exposer {

    typedef Exposed exposed_t;
    typedef typename GetValue< exposed_t >::Type getvalue_t;
    typedef typename Iterator< exposed_t >::Type iterator_t;
    typedef typename Position< exposed_t >::Type position_t;
    typedef typename Reference< exposed_t >::Type reference_t;
    typedef typename Size< exposed_t >::Type size_t;
    typedef typename Value< exposed_t >::Type value_t;

    template< typename Class >
    static
    void
    expose( Class & _class ) {
        _class.def( "empty", _empty, "Is the container empty?" );
        _class.def( "__nonzero__", __nonzero__, "Is the container not empty?" );
        _class.def( "__len__", __len__, "The length of the container." );
        _class.def( "value", _value, "Value at the position.", py::return_internal_reference<>() );
        _class.def( "__getitem__", __getitem__, "Value at the position.", py::return_internal_reference<>() );
    }

    static
    bool
    _empty( exposed_t & _self ) {
        return empty( _self );
    }

    static
    bool
    __nonzero__( exposed_t & _self ) {
        return ! empty( _self );
    }

    static
    typename Size< exposed_t >::Type
    __len__( exposed_t const & _self ) {
        return length( _self );
    }

    static
    reference_t
    _value( exposed_t & _self, position_t pos ) {
        return value( _self, pos );
    }

    static
    reference_t
    __getitem__( exposed_t & _self, position_t pos ) {
    	if( pos >= length( _self ) ) {
    		index_error();
    		throw boost::python::error_already_set();
    	} else {
    		return value( _self, pos );
    	}
    }

};



/**
 * Container exposer.
 */
template< typename Exposed >
struct container_exposer {

    typedef Exposed exposed_t;
    typedef typename GetValue< exposed_t >::Type getvalue_t;
    typedef typename Iterator< exposed_t >::Type iterator_t;
    typedef typename Position< exposed_t >::Type position_t;
    typedef typename Reference< exposed_t >::Type reference_t;
    typedef typename Size< exposed_t >::Type size_t;
    typedef typename Value< exposed_t >::Type value_t;

    template< typename Class >
    static
    void
    expose( Class & _class ) {
        const_container_exposer< exposed_t >::expose( _class );
        _class.def( "appendValue", _appendValue, "Append a value to this container." );
        _class.def( "resize", _resize, "Resize the container." );
    }

    static
    void
    _appendValue( exposed_t & _self, reference_t x ) {
        return appendValue( _self, x );
    }

    static
    size_t
    _resize( exposed_t & _self, size_t new_size ) {
        return resize( _self, new_size );
    }

};


template< typename TContainer >
boost::python::object
__getitem__( TContainer & _self, boost::python::object i ) {
    typedef typename Position< TContainer >::Type position_t;

    if( PySlice_Check( i.ptr() ) ) {
        Py_ssize_t start, stop, step, slicelength;
        if( 0 != PySlice_GetIndicesEx(
                reinterpret_cast< PySliceObject * >( i.ptr() ),
                length( _self ),
                &start,
                &stop,
                &step,
                &slicelength
            )
        ) {
            throw std::logic_error( "Could not get indices from slice object." );
        }
        if( 1 != step ) {
            throw std::logic_error( "Step size not supported. Only step sizes of 1 are supported." );
        }
        return py::object( infix( _self, start, stop ) );
    } else {
        const position_t pos = py::extract< position_t >( i )();
        if( pos < 0 || pos >= length( _self ) ) {
            set_index_error();
            return py::object();
        } else {
            return py::object( const_container_exposer< TContainer >::_value( _self, pos ) );
        }
    }
}



} // namespace python
} // namespace seqan

#endif //PYSEQAN_CONTAINER_JR_15FEB2013_DEFS_H_
