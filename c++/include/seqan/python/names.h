/** Copyright John Reid 2013
 *
 * \file
 * \brief Code to expose strings and string sets.
 *
 */

#ifndef PYSEQAN_TYPE_JR_13NOV2013_
#define PYSEQAN_TYPE_JR_13NOV2013_

#include <myrrh/defs.h>
#include <seqan/basic.h>
#include <boost/type_traits.hpp>
#include <boost/mpl/logical.hpp>


namespace seqan {
namespace python {


// forward decls
template< typename T > std::string name();
template< typename TValue > const char * simple_type_name();


namespace detail {

using boost::enable_if;
using boost::is_integral;
using boost::is_unsigned;
using boost::true_type;
using boost::false_type;
using boost::mpl::not_;
using boost::mpl::and_;

/**
 * Meta-function to produce correct name for a simple type spec.
 */
template< typename TSpec >
struct _simple_type_name  {
};

/// Specialisation of meta-function to produce correct name for a simple type spec.
template<>
struct _simple_type_name< Dna_ > {
    static constexpr const char * result = "DNA";
};

/// Specialisation of meta-function to produce correct name for a simple type spec.
template<>
struct _simple_type_name< Dna5_ > {
    static constexpr const char * result = "DNA5";
};

/// Specialisation of meta-function to produce correct name for a simple type spec.
template<>
struct _simple_type_name< Rna_ > {
    static constexpr const char * result = "RNA";
};

/// Specialisation of meta-function to produce correct name for a simple type spec.
template<>
struct _simple_type_name< Rna5_ > {
    static constexpr const char * result = "RNA5";
};

/// Specialisation of meta-function to produce correct name for a simple type spec.
template<>
struct _simple_type_name< Iupac_ > {
    static constexpr const char * result = "IUPAC";
};

/// Specialisation of meta-function to produce correct name for a simple type spec.
template<>
struct _simple_type_name< AminoAcid_ > {
    static constexpr const char * result = "AminoAcid";
};

/// Specialisation of meta-function to produce correct name for a simple type spec.
template<>
struct _simple_type_name< long unsigned int > {
    static const char * result;
};


/**
 * Meta-function to produce correct name for a type.
 */
template< typename T, class Enable=void >
struct _name  {
    std::string
    operator()() const {
        throw std::logic_error(
            MYRRH_MAKE_STRING(
                "_name<>() is not specialized for "
                << typeid( T ).name() ) );
    }
};


/// Specialisation for integral types.
template< typename T >
struct _name< T, typename enable_if< is_integral< T > >::type > {
    std::string operator()() const {
        return MYRRH_MAKE_STRING(
            ( is_unsigned< T >::value ? "u" : "" )
            << "int"
            << sizeof( T )
        );
    }
};


/** Conversion from seqan true/false to boost. */
template< typename TrueFalse >
struct seqan_true_false_to_boost {
};

template<>
struct seqan_true_false_to_boost< seqan::True > {
    typedef true_type type;
};

template<>
struct seqan_true_false_to_boost< seqan::False > {
    typedef false_type type;
};


/// Specialisation for seqan simple types
template< typename T >
struct _name<
    T,
    typename enable_if<
        typename and_<
            typename seqan_true_false_to_boost<
                typename IsSimple< T >::Type
            >::type,
            typename not_< typename is_integral< T >::type >::type
        >::type
    >::type
> {
    std::string operator()() const {
        return simple_type_name< T >();
    }
};


/// Specialisation for seqan pair
template< typename T1, typename T2, typename TSpec >
struct _name< Pair< T1, T2, TSpec >, void >
{
    std::string operator()() const {
        return MYRRH_MAKE_STRING( "Pair" << name< T1 >() << name< T2 >() );
    }
};


} // namespace detail


/**
 * Returns the name of the simple type: Dna, Dna5, Rna, Rna5, AminoAcid, Iupac
 */
template< typename TValue >
const char *
simple_type_name() {
    return detail::_simple_type_name< typename Spec< TValue >::Type >::result;
}


/**
 * Get name for type.
 */
template< typename T >
std::string
name() {
    return detail::_name< typename boost::remove_const< T >::type >()();
}


} // namespace python
} // namespace seqan

#endif //PYSEQAN_TYPE_JR_13NOV2013_
