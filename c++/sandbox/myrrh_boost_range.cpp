#include <myrrh/python/boost_range.h>

unsigned
test( boost::python::object o )
{
    return boost::size( myrrh::python::make_boost_range< int >( o ) );
}

int
main( int argc, char * argv[] ) {
    return test( boost::python::list() );
}
