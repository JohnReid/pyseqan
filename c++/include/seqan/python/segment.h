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
#include <seqan/python/string_base.h>


namespace seqan {
namespace python {
namespace detail {

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


} // namespace detail


template< typename THost, typename TSpec >
struct exposer< Segment< THost, TSpec > >
: myrrh::python::ensure_exposer< exposer< Segment< THost, TSpec > > >
{
    typedef Segment< THost, TSpec >                 exposed_type;
    typedef typename Value< exposed_type >::Type    value_type;
    typedef typename detail::is_char_convertible< value_type >::Type is_value_char_convertible;

    void
    expose() {
        py::class_< exposed_type > _class(
            name< exposed_type >().c_str(),
            "Infix of string."
        );
        const_container_exposer< exposed_type >::expose( _class );
        detail::expose_string_conversions< exposed_type >( _class, is_value_char_convertible() );
        _class.def( "__getitem__", __getitem__< exposed_type >, "Get individual value or a slice. No support for irregular step sizes.", py::with_custodian_and_ward_postcall< 0, 1 >() );
        _class.def( "__eq__", detail::string_equals< exposed_type > );
        _class.def( "__ne__", detail::string_notequals< exposed_type > );

        exposer< value_type >().ensure_exposed_and_add_as_attr( _class, "Value" );
    }
};


} //namespace python
} //namespace seqan

#endif //PYSEQAN_SEGMENT_JR_4JUN2013_
