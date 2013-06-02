/** Copyright John Reid 2013
 *
 * \file Exposes STEME algorithm to python.
 */

#include <boost/python.hpp>

void expose_seqan();

BOOST_PYTHON_MODULE( _seqan )
{
	expose_seqan();
}

