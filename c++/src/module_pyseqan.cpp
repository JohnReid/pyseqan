/** Copyright John Reid 2013
 *
 * \file Exposes STEME algorithm to python.
 */

#include <boost/python.hpp>
#include <seqan/python/defs.h>


void expose_simple_types();
void expose_strings();
void expose_indexes();


BOOST_PYTHON_MODULE( _seqan )
{
    {
        //
        // A vector of std::strings.
        //
        using namespace seqan::python;
        py::class_<
            string_vector,
            boost::shared_ptr< string_vector >,
            boost::noncopyable
        > string_vec_class( "StringVec", "A sequence of strings", py::no_init );
        string_vec_class.def( py::indexing::container_suite< string_vector >() );
    }

    expose_simple_types();
    expose_strings();
    expose_indexes();
}

