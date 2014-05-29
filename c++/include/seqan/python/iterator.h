/** Copyright John Reid 2013, 2014
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


template< typename TIndex >
struct is_esa {
    typedef False Type;
};


template< typename TText >
struct is_esa< Index< TText, IndexEsa<> > > {
    typedef True Type;
};


template< typename TIterator >
struct is_history {
    typedef False Type;
};


template< typename TContainer, typename TSpec >
struct is_history< Iter< TContainer, VSTree< TopDown< ParentLinks< TSpec > > > > > {
    typedef True Type;
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
    go_up( exposed_type & it ) {
        return goUp( it );
    }

    /**
     * Descend the iterator following the text in the string. The iterator
     * will be moved as far down the tree as matches the string. Returns
     * true iff the whole string is matched.
     */
    static
    bool
    go_down_text( exposed_type & it, py::object text ) {
        try {
#define SEQAN_CHECK_AND_EXTRACT_TEXT( type ) \
            { \
                py::extract< type > extractor( text ); \
                if( extractor.check() ) { \
                    return goDown( it, extractor() ); \
                } \
            } \
            /**/

            SEQAN_CHECK_AND_EXTRACT_TEXT( string_t );
            SEQAN_CHECK_AND_EXTRACT_TEXT( infix_t );
            SEQAN_CHECK_AND_EXTRACT_TEXT( const char * );
            //SEQAN_CHECK_AND_EXTRACT_TEXT( std::string );
            // only try to extract value type if doesn't have a length or length is 1
            if( ! PyObject_HasAttrString( text.ptr(), "__len__" ) || 1 == py::len( text ) ) {
                SEQAN_CHECK_AND_EXTRACT_TEXT( typename Value< string_t >::Type );
            }
            throw std::logic_error(
                "Do not know how to go down index for this type." );
#undef SEQAN_CHECK_AND_EXTRACT_TEXT
        } catch( ... ) {
            // If an exception was thrown, translate it to Python
            boost::python::handle_exception();
        }
        return false;
    }

    static
    bool
    go_right( exposed_type & it ) {
        return goRight( it );
    }

    static
    bool
    at_end( exposed_type & it ) {
        return atEnd( it );
    }

    static
    bool
    at_begin( exposed_type & it ) {
        return atBegin( it );
    }

    static
    void
    go_previous( exposed_type & it ) {
        goPrevious( it );
    }

    static
    void
    go_next( exposed_type & it ) {
        goNext( it );
    }

    static
    void
    go_begin( exposed_type & it ) {
        goBegin( it );
    }

    static
    void
    go_end( exposed_type & it ) {
        goEnd( it );
    }

    static
    exposed_type &
    iter( exposed_type & it ) {
        return it;
    }

    static
    exposed_type &
    next( exposed_type & it ) {
        if( atEnd( it ) ) {
            PyErr_SetString( PyExc_StopIteration, "Finished iterating" );
            throw boost::python::error_already_set();
        }
        goNext( it );
        return it;
    }

    static
    exposed_type
    __copy__( exposed_type it ) {
        return it;
    }

    static
    void
    setvalue( exposed_type & it, vertex_t v ) {
        seqan::value( it ) = v;
    }

    static
    vertex_t
    getvalue( exposed_type it ) {
        return seqan::value( it );
    }

    static
    vertex_t
    node_up( exposed_type it ) {
        return seqan::nodeUp( it );
    }

    static
    bool
    is_root( exposed_type it ) {
        return isRoot( it );
    }

    // specialisation for non-TopDownHistory iterators
    template< typename Class >
    static
    void
    expose_history_methods( Class & _class, False && ) {
        // Only expose this constructor for non-history iterators
        _class.def(
            py::init< container_t &, vertex_t >(
                (
                    py::arg( "index" ),
                    py::arg( "vertex" )
                ),
                "Construct an iterator to the given vertex in the index."
            )[ py::with_custodian_and_ward< 1, 2 >() ] );
        // only make it easy for non-history iterators to make copies
        _class.def( "copy", __copy__, "Returns a copy of this iterator." );
    }

    // specialisation for TopDownHistory iterators
    template< typename Class >
    static
    void
    expose_history_methods( Class & _class, True && ) {
        _class.def( "copy", __copy__, "Returns a copy of this iterator." );
        _class.def( "goUp", go_up, "Iterates up one edge to the parent in a tree." );
        _class.def( "goPrevious", go_previous, "Iterates to previous position." );
        _class.def( "goNext", go_next, "Iterates to next position." );
        _class.def( "goBegin", go_begin, "Iterates to first position." );
        //_class.def( "goEnd", go_end, "Iterates to last position." );
        //_class.def( "__iter__", iter, "Iterator's iterable method.", py::return_internal_reference<>() );
        //_class.def( "__next__", next, "Iterator's next method.", py::return_internal_reference<>() );
        //_class.def( "next", next, "Iterator's next method.", py::return_internal_reference<>() );
        _class.add_property( "atEnd", at_end, "Whether the iterator is at the end position." );
        _class.add_property( "nodeUp", node_up, "The vertex descriptor of the parent node." );
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
        _class.add_property(
                "parentEdgeLabel",
                parent_edge_label,
                "The label of the edge from the parent node to this node." );
        expose_history_methods( _class, typename detail::is_history< exposed_type >::Type() );
        _class.add_property( "repLength", rep_length, "The length of the representative substring of this iterator." );
        _class.add_property( "representative", get_representative, "A representative substring of this iterator." );
        _class.add_property( "parentEdgeLength", parent_edge_length, "The length of the label of the edge from the parent node to this node." );
        _class.add_property( "numOccurrences", count_occurrences, "Number of occurrences of the prefix this iterator represents." );
        _class.add_property( "occurrences", get_occurrences, "All occurrences of the representative substring or a q-gram in the index text." );
        _class.add_property( "numChildren", count_children, "The number of children of this node." );
        _class.add_property( "value", getvalue, setvalue, "0 <= value < 2*len(index). Can be used to assign properties to nodes with a property map." );
        _class.add_property( "isRoot", is_root, "Does the iterator point at the root of the index?" );
        _class.def( "goDown", go_down, "Iterates down one edge or a path in a tree." );
        _class.def( "goDown", go_down_text, "Iterates down one edge or a path in a tree which starts with text." );
        _class.def( "goRight", go_right, "Iterates to the next sibling in a tree." );
        _class.def( "__copy__", __copy__, "Returns a copy of this iterator." );

        exposer< infix_t >().ensure_exposed_and_add_as_attr( _class, "Representative" );
        exposer< sa_infix_t >().ensure_exposed_and_add_as_attr( _class, "Occurrences" );
    }
};





} // namespace python
} // namespace seqan

#endif //PYSEQAN_ITERATOR_JR_14FEB2013_DEFS_H_
