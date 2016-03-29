const double w = width/2, h = height/2, w2 = w/2, h2 = h/2;
 
const auto bl = Vector3f(0,  0,  1);
const auto bm = Vector3f(w2, 0,  1);
const auto br = Vector3f(w,  0,  1);
const auto ml = Vector3f(0,  h2, 1);
const auto mm = Vector3f(w2, h2, 1);
const auto mr = Vector3f(w,  h2, 1);
const auto tl = Vector3f(0,  h,  1);
const auto tm = Vector3f(w2, h,  1);
const auto tr = Vector3f(w,  h,  1);
 
const auto ow = Vector3f(w, 0, 0);
const auto oh = Vector3f(0, h, 0);
const auto ob = ow + oh;

Colour _c1_ct = Colours::red,
       _c2_ct = Colours::green,
       _c3_ct = Colours::blue;

ZBuffer_AlwaysAllow __zba_ct;

#include "drawtris.h"

#ifdef UT_WIREFRAME
_c1_ct = _c2_ct = _c3_ct = Colours::white;
#define triangle(a, b, c, zb, im) line(a, b, __zba_ct, im); line(a, c, __zba_ct, im); line(b, c, __zba_ct, im)
#include "drawtris.h"
#undef triangle
#endif