/** Copyright John Reid 2013
 *
 * \file
 * \brief Code to expose iterators.
 *
 */

#ifndef PYSEQAN_ITERATOR_JR_14FEB2013_DEFS_H_
#define PYSEQAN_ITERATOR_JR_14FEB2013_DEFS_H_

#include <seqan/python/defs.h>




namespace seqan {
namespace python {



/**
 * Iterator exposer.
 */
template<
    typename TIt
>
struct topdown_iterator_exposer {

    typedef TIt exposed_t;
    typedef typename Container< TIt >::Type container_t;
    typedef typename VertexDescriptor< container_t >::Type vertex_t;
    typedef typename Fibre< container_t, EsaText >::Type text_t;
    typedef typename Value< text_t >::Type string_t;
    typedef typename Value< string_t >::Type alphabet_t;
    typedef typename Infix< text_t const >::Type infix_t;

    static
    typename Size< container_t >::Type
    rep_length( exposed_t it ) {
        return repLength( it );
    }

    static
    infix_t
    get_representative( exposed_t it ) {
        return representative( it );
    }

    static
    typename Size< container_t >::Type
    parent_edge_length( exposed_t it ) {
        if( isRoot( it ) ) {
            throw std::invalid_argument( "The root of the index has no parent edge." );
        }
        return parentEdgeLength( it );
    }

    static
    typename Infix< text_t const >::Type
    parent_edge_label( exposed_t it ) {
        if( isRoot( it ) ) {
            throw std::invalid_argument( "The root of the index has no parent edge." );
        }
        return parentEdgeLabel( it );
    }

    static
    typename Size< container_t >::Type
    count_occurrences( exposed_t it ) {
        return countOccurrences( it );
    }

    static
    bool
    go_down( exposed_t & it ) {
        return goDown( it );
    }

    static
    bool
    go_down_char( exposed_t & it, alphabet_t c ) {
        return goDown( it, c );
    }

    static
    bool
    go_right( exposed_t & it ) {
        return goRight( it );
    }

    static
    exposed_t
    __copy__( exposed_t it ) {
        return it;
    }

    static
    vertex_t
    value( exposed_t it ) {
        return seqan::value( it );
    }

    static
    bool
    is_root( exposed_t it ) {
        return isRoot( it );
    }

    static
    void
    expose() {
        namespace py = boost::python;

        py::class_<
            exposed_t
        > _class(
            "TopDownIterator",
            "Wrapper for C++ SeqAn top down iterater.",
            py::init< container_t & >(
                py::args( "index" ),
                "Construct the iterator from the index."
            )[ py::with_custodian_and_ward< 1, 2 >() ]
        );
        _class.add_property( "repLength", rep_length, "The length of the representative substring of this iterator." );
        _class.add_property( "representative", get_representative, "A representative substring of this iterator." );
        _class.add_property( "parentEdgeLength", parent_edge_length, "The length of the label of the edge from the parent node to this node." );
        _class.add_property( "parentEdgeLabel", parent_edge_label, "The label of the edge from the parent node to this node." );
        _class.add_property( "countOccurrences", count_occurrences, "Number of occurrences of the prefix this iterator represents." );
        _class.add_property( "value", value, "0 <= value < 2*len(index). Can be used to assign properties to nodes with a property map." );
        _class.add_property( "isRoot", is_root, "Does the iterator point at the root of the index?" );
        _class.def( "goDown", go_down, "Iterates down one edge or a path in a tree." );
        _class.def( "goDownChar", go_down_char, "Iterates down one edge or a path in a tree which starts with given character." );
        _class.def( "goRight", go_right, "Iterates to the next sibling in a tree." );
        _class.def( "__copy__", __copy__, "Returns a copy of this iterator." );
    }
};





} // namespace python
} // namespace seqan

#endif //PYSEQAN_ITERATOR_JR_14FEB2013_DEFS_H_
