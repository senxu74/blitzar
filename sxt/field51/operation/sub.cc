#include "sxt/field51/operation/sub.h"

namespace sxt::f51o {
//--------------------------------------------------------------------------------------------------
// sub_impl
//--------------------------------------------------------------------------------------------------
template <class T1, class T2, class T3>
CUDA_CALLABLE void sub_impl(T1& h, const T2& f, const T3& g) noexcept {
  const uint64_t mask = 0x7ffffffffffffULL;
  uint64_t h0, h1, h2, h3, h4;

  h0 = g[0];
  h1 = g[1];
  h2 = g[2];
  h3 = g[3];
  h4 = g[4];

  h1 += h0 >> 51;
  h0 &= mask;
  h2 += h1 >> 51;
  h1 &= mask;
  h3 += h2 >> 51;
  h2 &= mask;
  h4 += h3 >> 51;
  h3 &= mask;
  h0 += 19ULL * (h4 >> 51);
  h4 &= mask;

  h0 = (f[0] + 0xfffffffffffdaULL) - h0;
  h1 = (f[1] + 0xffffffffffffeULL) - h1;
  h2 = (f[2] + 0xffffffffffffeULL) - h2;
  h3 = (f[3] + 0xffffffffffffeULL) - h3;
  h4 = (f[4] + 0xffffffffffffeULL) - h4;

  h[0] = h0;
  h[1] = h1;
  h[2] = h2;
  h[3] = h3;
  h[4] = h4;
}

//--------------------------------------------------------------------------------------------------
// sub
//--------------------------------------------------------------------------------------------------
CUDA_CALLABLE
void sub(f51t::element& h, const f51t::element& f, const f51t::element& g) noexcept {
  sub_impl(h, f, g);
}

CUDA_CALLABLE
void sub(volatile f51t::element& h, const f51t::element& f, const f51t::element& g) noexcept {
  sub_impl(h, f, g);
}

CUDA_CALLABLE
void sub(volatile f51t::element& h, const volatile f51t::element& f,
         const volatile f51t::element& g) noexcept {
  sub_impl(h, f, g);
}
} // namespace sxt::f51o
