#include <boost/python.hpp>

struct inner {
};

template< typename T >
struct outer {
	typedef inner inner_t;

	static const char * name();

	static
	void expose() {
		using namespace boost::python;

		class_< outer< T > > outer_class( name() );

		// check if inner type is in registry already
		const type_info inner_info = boost::python::type_id< inner_t >();
		const converter::registration * inner_registration
			= boost::python::converter::registry::query( inner_info );
		scope outer_scope( outer_class );
		if( inner_registration == 0 || inner_registration->m_to_python == 0 ) {
			// not already in registry
			std::cout << "Exposing inner class of " << name() << ".\n";
			class_< inner_t > inner_class( "Inner" );
		} else {
			// already in registry because exposed via different outer
			// what to put here? In python we need Outer.Inner to exist
			std::cout << "Setting inner class object as attribute of " << name() << ".\n";
			outer_scope.attr( "Inner" ) = handle<>( inner_registration->m_class_object );
		}
	}
};

template<>
const char *
outer< int >::name() { return "IntOuter"; }

template<>
const char *
outer< double >::name() { return "DoubleOuter"; }

BOOST_PYTHON_MODULE( inner_classes )
{
	outer< int >::expose();
	outer< double >::expose();
}
