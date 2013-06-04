/** Copyright John Reid 2013
 *
 * \file
 * \brief Code to expose infixes.
 *
 */

#ifndef PYSEQAN_INFIX_JR_4JUN2013_
#define PYSEQAN_INFIX_JR_4JUN2013_

#include <seqan/python/defs.h>




namespace seqan {
namespace python {


template< typename TString >
struct infix_exposer
: myrrh::python::expose_or_set_attribute< infix_exposer< TString > >
{

	typedef typename seqan::Infix< TString >::Type exposed_type;
	typedef typename Value< exposed_type >::Type value_type;

	static
	bool
	equals_value( exposed_type const & s, value_type v ) {
		return 1 == seqan::length( s ) && s[ 0 ] == v;
	}

	void
	expose( const char * name ) {
		namespace py = boost::python;
		py::class_< exposed_type > _class(
			name,
			"Infix of string."
		);
		const_container_exposer< exposed_type >::expose( _class );
		_class.def( "__str__", std_string_from_seqan< exposed_type >, "String representation." );
		_class.def( "__eq__", equals_value );
		_class.def( py::self == exposed_type() );
		_class.def( py::self == TString() );
		_class.def( py::self == std::string() );

		py::scope scope( _class );
		simple_type_exposer< value_type >()( scope, "Value" );
	}
};

} //namespace python
} //namespace seqan

#endif //PYSEQAN_INFIX_JR_4JUN2013_
