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
#include <seqan/python/string.h>
#include <seqan/python/infix.h>
#include <seqan/index.h>




namespace seqan {
namespace python {
namespace detail {


/// Specialisation for vertex
template< typename T >
struct _name< VertexEsa< T > >
{
    std::string operator()() const {
        return MYRRH_MAKE_STRING( "VertexEsa" );
    }
};


/// Specialisation for index
template< typename TText, typename TSpec >
struct _name< Index< TText, TSpec > >
{
    std::string operator()() const {
        return MYRRH_MAKE_STRING( "Index" << name< TText >() << name< TSpec >() );
    }
};


/// Specialisation for ESA
template<>
struct _name< IndexEsa<> >
{
    std::string operator()() const {
        return "ESA";
    }
};


/// Specialisation for Wotd
template<>
struct _name< IndexWotd<> >
{
    std::string operator()() const {
        return "WOTD";
    }
};


} //namespace detail


template< typename TSize >
struct exposer< VertexEsa< TSize > >
: myrrh::python::ensure_exposer< exposer< VertexEsa< TSize > > >
{
    typedef VertexEsa< TSize >              exposed_type;


    static
    bool
    __eq__( exposed_type const & v, exposed_type const & w ) {
        return _getId( v ) == _getId( w );
    }


    static
    TSize
    id( exposed_type const & v ) {
        return _getId( v );
    }


    void
    expose() {
        py::class_< exposed_type > class_(
            name< exposed_type >().c_str(),
            "A vertex in an index.",
            py::no_init
        );
        class_.add_property( "id", id, "The id of this vertex. Can be used as an index into a property map." );
        class_.def( "__eq__", __eq__, "Tests equality." );
        class_.def( "__hash__", id, "Hash function." );
    }
};


/**
 * Index exposer.
 */
template< typename TText, typename TSpec >
struct exposer< Index< TText, TSpec > > {

    typedef Index< TText, TSpec >                               exposed_type;
    typedef typename Value< TText >::Type                       string_t;
    typedef typename Value< string_t >::Type                    alphabet_t;
    typedef typename VertexDescriptor< exposed_type >::Type     vertex_t;
    typedef typename Fibre< exposed_type, EsaSA >::Type         sa_fibre_t;
    typedef typename Infix< sa_fibre_t const >::Type            sa_infix_t;
    typedef typename Value< sa_infix_t >::Type                  sa_value_t;
    typedef typename Iterator< exposed_type, TopDown<> >::Type  top_down_it;


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
     * Visit all the nodes in the index. This can be useful to build a suffix
     * tree ahead of doing any timings.
     */
    static
    void
    visit_tree( exposed_type & index ) {
        top_down_it root( index );
        visit_node( root );
    }


    /**
     * The length of the index.
     */
    static
    typename Size< exposed_type >::Type
    __len__( exposed_type const & index ) {
        return length( index );
    }


    static
    void
    expose() {

        py::class_<
            exposed_type,
            boost::shared_ptr< exposed_type >,
            boost::noncopyable
        > _class(
            name< exposed_type >().c_str(),
            "Wrapper for C++ seqan index.",
            py::init< TText const & >(
                py::arg( "index" ),
                "Construct an index from the text."
            )[ py::with_custodian_and_ward< 1, 2 >() ]
        );
        _class.def( "visit", visit_tree, "Visit all the nodes in a tree. This can be useful to build the suffix tree ahead of doing any timings." );
        _class.def( "__len__", __len__, "The length of the index." );

        iterator_exposer< top_down_it >().ensure_exposed_and_add_as_attr( _class, "TopDownIterator" );
        exposer< sa_infix_t >().ensure_exposed_and_add_as_attr( _class, "Infix" );
        exposer< vertex_t >().ensure_exposed_and_add_as_attr( _class, "Vertex" );
    }
};


} // namespace python
} // namespace seqan

#endif //PYSEQAN_INDEX_JR_14FEB2013_DEFS_H_
