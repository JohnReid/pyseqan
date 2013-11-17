/** Copyright John Reid 2013
 *
 * \file
 * \brief Types for pyseqan.
 *
 */

#ifndef PYSEQAN_DEFS_JR_13FEB2013_DEFS_H_
#define PYSEQAN_DEFS_JR_13FEB2013_DEFS_H_

#include <seqan/python/pch.h>

namespace seqan {
namespace python {

namespace py = boost::python;


typedef std::vector< std::string > string_vector; ///< A vector of strings.
typedef unsigned long long max_representing_t; ///< Largest integer type to store sizes.


/**
 * Return a std::string version of a seqan string.
 */
template< typename TSeqanStr >
std::string
std_string_from_seqan( TSeqanStr seqan_str ) {
    std::string result;
    seqan::assign( result, seqan_str );
    return result;
}

inline
void
set_index_error() {
    PyErr_SetString(PyExc_IndexError, "Index out of range");
    boost::python::throw_error_already_set();
}


/** Expose a seqan type. */
template< typename Exposed, typename Enable=void >
struct exposer {
    static
    void
    expose() {
        throw std::logic_error(
            MYRRH_MAKE_STRING(
                "exposer<> is not specialized for "
                << typeid( Exposed ).name() ) );
    }
};


// Don't need to expose arithmetic types
template< typename Exposed >
struct exposer<
    Exposed,
    typename boost::enable_if<
        typename boost::is_arithmetic< Exposed >::type
    >::type
> {
    template< typename Class >
    const boost::python::converter::registration *
    ensure_exposed_and_add_as_attr( Class &, const char * ) {
        return 0;
    }
};


} // namespace python
} // namespace seqan

#endif //PYSEQAN_DEFS_JR_13FEB2013_DEFS_H_
