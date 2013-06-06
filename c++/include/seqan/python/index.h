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
#include <seqan/python/infix.h>

#include <seqan/index.h>




namespace seqan {
namespace python {


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


    struct top_down_exposer
    : myrrh::python::expose_or_set_attribute< top_down_exposer >
    {
    	typedef typename Iterator< exposed_t, TopDown<> >::Type exposed_type;

    	void
    	expose( const char * name ) {
    		topdown_iterator_exposer< exposed_type >::expose( name );
    	}
    };


    struct vertex_exposer
    : myrrh::python::expose_or_set_attribute< vertex_exposer >
    {
    	typedef vertex_t exposed_type;

    	void
    	expose( const char * name ) {
			py::class_< exposed_type > vertex_class(
				name,
				"A vertex in an index.",
				py::no_init
			);
			vertex_class.add_property( "id", id, "The id of this vertex. Can be used as an index into a property map." );
			vertex_class.def( "__eq__", _vertex_eq, "Tests equality." );
			vertex_class.def( "__hash__", id, "Hash function." );
    	}
    };


    static
    void
    expose() {

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

		py::scope scope( _class );
		top_down_exposer()( scope, "TopDownIterator" );
		infix_exposer< TText const >()( scope, "Infix" );
		vertex_exposer()( scope, "Vertex" );
    }
};


} // namespace python
} // namespace seqan

#endif //PYSEQAN_INDEX_JR_14FEB2013_DEFS_H_
