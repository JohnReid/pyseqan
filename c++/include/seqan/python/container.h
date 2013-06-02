/** Copyright John Reid 2013
 *
 * \file
 * \brief Code to expose strings and string sets.
 *
 */

#ifndef PYSEQAN_CONTAINER_JR_15FEB2013_DEFS_H_
#define PYSEQAN_CONTAINER_JR_15FEB2013_DEFS_H_

#include <seqan/python/defs.h>





namespace seqan {
namespace python {



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
        namespace py = boost::python;
        _class.def( "empty", _empty, "Is the container empty?" );
        _class.def( "__nonzero__", __nonzero__, "Is the container not empty?" );
        _class.def( "__len__", __len__, "The length of the container." );
        _class.def( "value", _value, "Value at the position.", py::return_internal_reference<>() );
        _class.def( "__getitem__", _value, "Value at the position.", py::return_internal_reference<>() );
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
        namespace py = boost::python;
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




} // namespace python
} // namespace seqan

#endif //PYSEQAN_CONTAINER_JR_15FEB2013_DEFS_H_
