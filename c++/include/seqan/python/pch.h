/** Copyright John Reid 2013
 *
 * \file
 * \brief Files to be included in pre-compiled header for pyseqan build.
 *
 */

#ifndef PYSEQAN_PCH_JR_2JUN2013_DEFS_H_
#define PYSEQAN_PCH_JR_2JUN2013_DEFS_H_

#include <boost/python.hpp>

#include <myrrh/defs.h>
#include <myrrh/python/boost_function.h>
#include <myrrh/python/registry.h>
#include <myrrh/seqan_boost_range_adaptors.h>

#include <boost/config.hpp>
#include <boost/python/detail/target.hpp>
#include <boost/python/signature.hpp>
#include <boost/python/object/pointer_holder.hpp>
#include <boost/python/suite/indexing/vector.hpp>
#include <boost/range/adaptor/reversed.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/type_traits/config.hpp>
#include <boost/type_traits/cv_traits.hpp>
#include <boost/type_traits/detail/template_arity_spec.hpp>
#include <boost/type_traits/detail/bool_trait_def.hpp>
#include <boost/type_traits/detail/bool_trait_undef.hpp>
#include <boost/utility.hpp>

#include <seqan/basic.h>
#include <seqan/file.h>
#include <seqan/index.h>
#include <seqan/seq_io.h>
#include <seqan/sequence.h>
#include <seqan/find.h>

#include <stddef.h>
#include <cstddef>
#include <iostream>
#include <string>
#include <vector>

#endif //PYSEQAN_PCH_JR_2JUN2013_DEFS_H_
