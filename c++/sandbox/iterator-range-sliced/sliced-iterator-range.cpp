#include <boost/range/adaptor/transformed.hpp>
#include <boost/range/adaptor/sliced.hpp>
#include <boost/range/algorithm/copy.hpp>
#include <boost/assign.hpp>
#include <iterator>
#include <iostream>
#include <vector>
#include <functional>

struct identity {
	typedef int result_type;
	result_type operator()( int i ) const { return i; }
};

int main(int argc, const char* argv[])
{
    using namespace boost::adaptors;
    using namespace boost::assign;

    std::vector< int > input;
    input += 1,2,3,4,5,6,7,8,9;

	// slicing then transforming my iterator range
    std::cout << "Sliced then transformed: ";
	boost::copy(
		input | sliced( 2, 8 ) | transformed( identity() ),
		std::ostream_iterator< int >( std::cout, ",") );
	std::cout << "\n";

	// transforming then slicing my iterator range - takes a very long time....
    std::cout << "Transformed then sliced: ";
	boost::copy(
		input | transformed( identity() ) | sliced( 2, 8 ),
		std::ostream_iterator< int >(std::cout, ","));
	std::cout << "\n";

    return 0;
}

