#pragma once

#include <concepts>

#include "sxt/algorithm/base/mapper.h"
#include "sxt/algorithm/base/reducer.h"
#include "sxt/base/macro/cuda_callable.h"

namespace sxt::algb {
//--------------------------------------------------------------------------------------------------
// accumulate
//--------------------------------------------------------------------------------------------------
template <algb::reducer Reducer, algb::mapper Mapper>
  requires std::same_as<typename Reducer::value_type, typename Mapper::value_type>
CUDA_CALLABLE void accumulate(typename Reducer::value_type& res, typename Reducer::value_type& e,
                              Mapper mapper, unsigned int i) noexcept
  requires(!requires { Reducer::accumulate_inplace(res, e, mapper, i); })
{
  mapper.map_index(e, i);
  Reducer::accumulate_inplace(res, e);
}

template <algb::reducer Reducer, algb::mapper Mapper>
  requires std::same_as<typename Reducer::value_type, typename Mapper::value_type>
CUDA_CALLABLE void accumulate(typename Reducer::value_type& res, typename Reducer::value_type& e,
                              Mapper mapper, unsigned int i) noexcept
  requires(requires { Reducer::accumulate_inplace(res, e, mapper, i); })
{
  // support specialized accumulate that might be more efficient
  Reducer::accumulate_inplace(res, e, mapper, i);
}
} // namespace sxt::algb
