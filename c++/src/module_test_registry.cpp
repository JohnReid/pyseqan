/**
 * See boost trac ticket: https://svn.boost.org/trac/boost/ticket/7548
 */

#include <boost/python.hpp>
#include <vector>
#include <iostream>


std::vector<double> list_of_doubles() {
	std::vector<double> retval;
	retval.push_back(3);
	retval.push_back(2);
	return retval;
}


// Instantiate the core module.
BOOST_PYTHON_MODULE(test_registry)
{
	using namespace boost::python;

	//We check if a converter for std::vector has been registered
	type_info info = boost::python::type_id<std::vector<double> >();
	const converter::registration* reg = boost::python::converter::registry::query(info);
	//We output to screen the result (should be NULL)
	std::cout << reg << " ";

	//We also expose a function that needs such a conversion (to_python)
	def("list_of_doubles",list_of_doubles);
}
