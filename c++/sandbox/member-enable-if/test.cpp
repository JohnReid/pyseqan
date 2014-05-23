#include <string>
#include <iostream>

template< typename T >
struct other {
    typedef int type;
};

template<>
struct other< double > {
    typedef std::string type;
};

template< typename T >
struct A {
    typedef typename other< T >::type other_type;

    template< typename U=other_type >
    typename std::enable_if< ! std::is_convertible< U, int >::value >::type
    member_fn() {
        std::cout << "Is not convertible.\n";
    }

    template< typename U=other_type >
    typename std::enable_if< std::is_convertible< U, int >::value >::type
    member_fn() {
        std::cout << "Is convertible.\n";
    }
};

int main( int argc, char * argv[] ) {
    A< int > a_int;
    a_int.member_fn();

    A< double > a_double;
    a_double.member_fn();

    return 0;
}
