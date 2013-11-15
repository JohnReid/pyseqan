/** Copyright John Reid 2013
 *
 * \file
 * \brief Code to expose modified strings.
 *
 */

#ifndef PYSEQAN_MODIFIED_JR_15NOV2013_
#define PYSEQAN_MODIFIED_JR_15NOV2013_

#include <seqan/python/defs.h>
#include <seqan/python/simple_type.h>
#include <seqan/python/names.h>
#include <seqan/python/segment.h>
#include <seqan/python/container.h>
#include <seqan/python/string_base.h>


namespace seqan {
namespace python {
namespace detail {


/// Specialisation for seqan modified string
template< typename THost, typename TSpec >
struct _name< ModifiedString< THost, TSpec >, void >
{
    std::string operator()() const {
        return MYRRH_MAKE_STRING( "" << name< THost >() << name< TSpec >() );
    }
};


/// Specialisation for reverse
template<>
struct _name< ModReverse >
{
    std::string operator()() const {
        return "Reverse";
    }
};


/// Specialisation for view
template< typename TSpec >
struct _name< ModView< TSpec > >
{
    std::string operator()() const {
        return name< TSpec >();
    }
};


/// Specialisation for complement
template< typename T >
struct _name< FunctorComplement< T > >
{
    std::string operator()() const {
        return "Complement";
    }
};


} // namespace detail


/**
 * Modified string exposer.
 */
template< typename THost, typename TSpec >
struct exposer< ModifiedString< THost, TSpec > >
: myrrh::python::ensure_exposer< exposer< ModifiedString< THost, TSpec > > >
{
    typedef ModifiedString< THost, TSpec > exposed_type;
    typedef typename Value< THost >::Type value_type;
    typedef typename Infix< exposed_type >::Type infix_t;
    typedef typename detail::is_char_convertible< value_type >::Type is_value_char_convertible;


    static
    void
    expose() {
        //std::cout << "Registering: " << name< exposed_type >() << " : " << typeid( exposed_type ).name() << "\n";
        py::class_<
            exposed_type
        > _class(
            name< exposed_type >().c_str(),
            "Wrapper for SeqAn modified string."
        );
        const_container_exposer< exposed_type >::expose( _class );
        detail::expose_string_conversions< exposed_type >( _class, is_value_char_convertible() );
        exposer< infix_t >().ensure_exposed_and_add_as_attr( _class, "Infix" );
    }
};


} //namespace seqan
} //namespace python


#endif //PYSEQAN_MODIFIED_JR_15NOV2013_


