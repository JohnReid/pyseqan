/** Copyright John Reid 2013
 *
 * \file
 * \brief Code to expose iterators.
 *
 */

#ifndef PYSEQAN_ITERATOR_JR_14FEB2013_DEFS_H_
#define PYSEQAN_ITERATOR_JR_14FEB2013_DEFS_H_

#include <seqan/python/defs.h>
#include <seqan/python/names.h>
#include <seqan/python/string.h>
#include <myrrh/python/boost_range.h>
#include <seqan/index.h>




namespace seqan {
namespace python {
namespace detail {


/// Specialisation for iterator
template< typename TContainer, typename TSpec >
struct _name< Iter< TContainer, TSpec > >
{
    std::string operator()() const {
        return MYRRH_MAKE_STRING( name< TContainer >() << "Iterator" );
    }
};


/// Specialisation for top down spec
template< typename TSpec >
struct _name< TopDown< TSpec > >
{
    std::string operator()() const {
        return MYRRH_MAKE_STRING( "TopDown" << name< TSpec >() );
    }
};


/// Specialisation for top down history spec
template< typename TSpec >
struct _name< ParentLinks< TSpec > >
{
    std::string operator()() const {
        return "History";
    }
};


} //namespace detail

/**
 * SeqAn-ise a boost::python::object that should be some sort of string.
 * That is leave it alone if it is already a SeqAn type (or some other random type).
 * If it is a python string then convert it to a SeqAn string over the given
 * alphabet.
 *
 * The motivation is that plenty of the methods in this package can handle
 * arguments of several types of SeqAn string but not python strings. We
 * use this function to check the arguments.
 */
template< typename TString >
py::object
seqanise_string( py::object str ) {
    if( PyString_Check( str.ptr() ) ) {
        //std::cout << "Seqanising\n";
        boost::shared_ptr< TString > seqanised( new TString( py::extract< TString >( str )() ) );
        return py::object( seqanised );
    } else {
        return str; // do not modify
    }
}


/**
 * Iterator exposer.
 */
template<
    typename TIt
>
struct iterator_exposer
: myrrh::python::ensure_exposer< iterator_exposer< TIt > >
{

    typedef TIt exposed_type;
    typedef typename Container< TIt >::Type                  container_t;
    typedef typename Size< container_t >::Type               size_t;
    typedef typename VertexDescriptor< container_t >::Type   vertex_t;
    typedef typename Fibre< container_t, EsaText >::Type     text_t;
    typedef typename Value< text_t >::Type                   string_t;
    typedef typename Value< string_t >::Type                 alphabet_t;
    typedef typename Infix< text_t const >::Type             infix_t;
    typedef typename Fibre< container_t, EsaSA >::Type       sa_fibre_t;
    typedef typename Infix< sa_fibre_t const >::Type         sa_infix_t;

    static
    size_t
    rep_length( exposed_type it ) {
        return repLength( it );
    }

    static
    infix_t
    get_representative( exposed_type it ) {
        return representative( it );
    }

    static
    size_t
    parent_edge_length( exposed_type it ) {
        if( isRoot( it ) ) {
            throw std::invalid_argument( "The root of the index has no parent edge." );
        }
        return parentEdgeLength( it );
    }

    static
    typename Infix< text_t const >::Type
    parent_edge_label( exposed_type it ) {
        if( isRoot( it ) ) {
            throw std::invalid_argument( "The root of the index has no parent edge." );
        }
        return parentEdgeLabel( it );
    }

    static
    size_t
    count_occurrences( exposed_type it ) {
        return countOccurrences( it );
    }

    static
    sa_infix_t
    get_occurrences( exposed_type it ) {
        return getOccurrences( it );
    }

    static
    size_t
    count_children( exposed_type it ) {
        return countChildren( it );
    }

    static
    bool
    go_down( exposed_type & it ) {
        return goDown( it );
    }

    static
    bool
    go_down_char( exposed_type & it, alphabet_t c ) {
        return goDown( it, c );
    }

    /**
     * Descend the iterator following the text in the string. The iterator
     * will be moved as far down the tree as matches the string. Returns
     * true iff the whole string is matched.
     */
    static
    bool
    go_down_str( exposed_type & it, py::object str ) {
        using boost::adaptors::transformed;
        using boost::adaptors::sliced;
        str = seqanise_string< string_t >( str );
        try {
            myrrh::python::extract_fn< alphabet_t > extract;
            const size_t N = py::len( str );
            size_t n = 0;
            while( n < N ) {
                // if we can't match the first character of the rest of our string return false
                const alphabet_t first = extract( str[ n ] );
                if( ! goDown( it, first ) ) {
                    return false;
                }
                const size_t edge_len = std::min( parentEdgeLength( it ), N - n );
                // check we can match the rest of our parent edge
                if( edge_len > 1 ) {
                    for( size_t i = 1; edge_len != i; ++i ) {
                        if( parentEdgeLabel( it )[ i ] != extract( str[ n + i ] ) ) {
                            return false;
                        }
                    }
                }
                // increment our index into the string
                n += edge_len;
            }
        } catch( ... ) {
            // If an exception was thrown, translate it to Python
            boost::python::handle_exception();
            return false;
        }
        return true;
    }

    static
    bool
    go_right( exposed_type & it ) {
        return goRight( it );
    }

    static
    exposed_type
    __copy__( exposed_type it ) {
        return it;
    }

    static
    vertex_t
    value( exposed_type it ) {
        return seqan::value( it );
    }

    static
    bool
    is_root( exposed_type it ) {
        return isRoot( it );
    }

    static
    void
    expose() {

        py::class_<
        exposed_type
        > _class(
            name< exposed_type >().c_str(),
            "Wrapper for C++ SeqAn top down iterator.",
            py::init< container_t & >(
                py::arg( "index" ),
                "Construct the iterator from the index."
            )[ py::with_custodian_and_ward< 1, 2 >() ]
        );
        _class.def(
            py::init< container_t &, vertex_t >(
                (
                    py::arg( "index" ),
                    py::arg( "vertex" )
                ),
                "Construct an iterator to the given vertex in the index."
            )[ py::with_custodian_and_ward< 1, 2 >() ] );
        _class.add_property( "repLength", rep_length, "The length of the representative substring of this iterator." );
        _class.add_property( "representative", get_representative, "A representative substring of this iterator." );
        _class.add_property( "parentEdgeLength", parent_edge_length, "The length of the label of the edge from the parent node to this node." );
        _class.add_property( "parentEdgeLabel", parent_edge_label, "The label of the edge from the parent node to this node." );
        _class.add_property( "countOccurrences", count_occurrences, "Number of occurrences of the prefix this iterator represents." );
        _class.add_property( "occurrences", get_occurrences, "All occurrences of the representative substring or a q-gram in the index text." );
        _class.add_property( "countChildren", count_children, "The number of children of this node." );
        _class.add_property( "value", value, "0 <= value < 2*len(index). Can be used to assign properties to nodes with a property map." );
        _class.add_property( "isRoot", is_root, "Does the iterator point at the root of the index?" );
        _class.def( "goDown", go_down, "Iterates down one edge or a path in a tree." );
        _class.def( "goDownChar", go_down_char, "Iterates down one edge or a path in a tree which starts with given character." );
        _class.def( "goDownStr", go_down_str, "Iterates down the iterator following the string." );
        _class.def( "goRight", go_right, "Iterates to the next sibling in a tree." );
        _class.def( "__copy__", __copy__, "Returns a copy of this iterator." );

        string_exposer< sa_infix_t >().ensure_exposed_and_add_as_attr( _class, "Occurrences" );
    }
};





} // namespace python
} // namespace seqan

#endif //PYSEQAN_ITERATOR_JR_14FEB2013_DEFS_H_
