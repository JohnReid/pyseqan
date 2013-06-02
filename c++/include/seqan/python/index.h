/** Copyright John Reid 2013
 *
 * \file
 * \brief Code to expose indexes.
 *
 */

#ifndef PYSEQAN_INDEX_JR_14FEB2013_DEFS_H_
#define PYSEQAN_INDEX_JR_14FEB2013_DEFS_H_

#include <seqan/python/defs.h>
#include <seqan/python/iterator.h>
#include <seqan/index.h>




namespace seqan {
namespace python {


/**
 * Queries if the the type specified is in the registry.
 */
template< typename QueryT >
bool
in_registry() {
	namespace bp = boost::python;
	const bp::type_info info = boost::python::type_id< QueryT >();
	const bp::converter::registration * registration = boost::python::converter::registry::query( info );
	return registration != 0;
}

/**
 * Meta-function to produce correct name for an index specialisation.
 */
template< typename TSpec >
struct _index_spec_name  {
};

/// Specialisation of meta-function to produce correct name for an index specialisation.
//template<>
//struct _index_spec_name< IndexSa<> > {
//    const char * operator()() { return "Sa"; }
//};

/// Specialisation of meta-function to produce correct name for an index specialisation.
template<>
struct _index_spec_name< IndexEsa<> > {
    const char * operator()() { return "Esa"; }
};

/// Specialisation of meta-function to produce correct name for an index specialisation.
template<>
struct _index_spec_name< IndexWotd<> > {
    const char * operator()() { return "Wotd"; }
};

template< typename Index >
const char *
index_spec_name() {
    return _index_spec_name< typename Spec< Index >::Type >()();
}





/**
 * Index exposer.
 */
template<
    typename TText,
    typename TSpec = typename DefaultIndexSpec< TText >::Type
>
struct index_exposer {

    typedef Index< TText, TSpec > exposed_t;
    typedef typename Value< TText >::Type string_t;
    typedef typename Value< string_t >::Type alphabet_t;
    typedef typename Infix< TText const>::Type infix_t;
    typedef typename VertexDescriptor< exposed_t >::Type vertex_t;


    /**
     * Visit a node in the index.
     */
    template< typename TTopDownIt >
    static
    void
    visit_node( TTopDownIt it ) {
        if( goDown( it ) ) {
            visit_node( it );
            while( goRight( it ) ) {
                visit_node( it );
            }
        }
    }


    /**
     * Visit all the nodes in the index. This can be useful to build a suffix tree ahead of doing any timings.
     */
    static
    void
    visit_tree( exposed_t & index ) {
        typename Iterator< exposed_t, TopDown<> >::Type root( index );
        visit_node( root );
    }


    /**
     * The length of the index.
     */
    static
    typename Size< exposed_t >::Type
    __len__( exposed_t const & index ) {
        return length( index );
    }


    static
    typename Size< exposed_t >::Type
    id( vertex_t const & v ) {
        return _getId( v );
    }


    static
    bool
    _vertex_eq( vertex_t const & v, vertex_t const & w ) {
        return _getId( v ) == _getId( w );
    }


    static
    void
    expose() {
        namespace py = boost::python;

        py::class_<
            exposed_t,
            boost::shared_ptr< exposed_t >,
            boost::noncopyable
        > _class(
            MYRRH_MAKE_STRING( "Index" << index_spec_name< exposed_t >() << simple_type_name< alphabet_t >() ).c_str(),
            "Wrapper for C++ seqan index.",
            py::init< TText const & >( py::args( "index" ), "Construct an index from the text." )
        );
        _class.def( "visit", visit_tree, "Visit all the nodes in a tree. This can be useful to build the suffix tree ahead of doing any timings." );
        _class.def( "__len__", __len__, "The length of the index." );

        {
            py::scope scope( _class );

            topdown_iterator_exposer< typename Iterator< exposed_t, TopDown<> >::Type >::expose();

            py::class_<
                infix_t
            > infix_class(
                "Infix",
                "Infix of string."
            );
            const_container_exposer< infix_t >::expose( infix_class );
            infix_class.def( "__str__", std_string_from_seqan< infix_t >, "String representation." );

            // The class to represent a vertex
            if( ! in_registry< vertex_t >() ) {
            	std::cout << "Registering\n";
				py::class_< vertex_t > vertex_class(
					"Vertex",
					"A vertex in an index.",
					py::no_init
				);
				vertex_class.add_property( "id", id, "The id of this vertex. Can be used as an index into a property map." );
				vertex_class.def( "__eq__", _vertex_eq, "Tests equality." );
				vertex_class.def( "__hash__", id, "Hash function." );
            }
        }
    }
};


//    //
//    // Get the limits on number, length and total length of sequences.
//    //
//    static const max_representing_t index_max_seqs
//        = ( max_representing_t )( std::numeric_limits< types<>::seq_index_t >::max() ) + 1;
//    static const max_representing_t index_max_seq_length
//        = ( max_representing_t )( std::numeric_limits< types<>::seq_offset_t >::max() ) + 1;
//    static const max_representing_t index_max_text_length
//        = ( max_representing_t )( std::numeric_limits< types<>::text_offset_t >::max() ) + 1;
//
//    //
//    // Limits on the lengths of texts and sequences
//    //
//    py::scope().attr( "index_max_seqs" ) = index_max_seqs;
//    py::scope().attr( "index_max_seq_length" ) = index_max_seq_length;
//    py::scope().attr( "index_max_text_length" ) = index_max_text_length;
//
//
//
//    /**
//     * Build an index from the text.
//     */
//    boost::shared_ptr< exposed_t >
//    build_index( TText const & sequences )
//    {
//        if( index_max_seqs < length( sequences ) ) {
//            throw std::length_error( "Too many sequences for this index type." );
//        }
//        max_representing_t total = 0;
//        BOOST_FOREACH( const pyseqan_types::string_t & seq, sequences ) {
//            if( index_max_seq_length < length( seq ) ) {
//                throw std::length_error( "Sequence too long for this index type." );
//            }
//            total += length( seq );
//        }
//        if( total > index_max_text_length ) {
//            throw std::length_error( "Total sequence length too big for this index type." );
//        }
//        return pyseqan_types::index_ptr_t( new pyseqan_types::index_t( sequences ) );
//    }
//    //
//    // Building an index.
//    //
//    py::def( "build_index", build_index, "Build a seqan index from a string set." );


} // namespace python
} // namespace seqan

#endif //PYSEQAN_INDEX_JR_14FEB2013_DEFS_H_
