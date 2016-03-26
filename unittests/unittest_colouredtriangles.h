
double w = width/2, h = height/2, w2 = w/2, h2 = h/2;

auto bl = Vector3f(0,  0,  1);
auto bm = Vector3f(w2, 0,  1);
auto br = Vector3f(w,  0,  1);
auto ml = Vector3f(0,  h2, 1);
auto mm = Vector3f(w2, h2, 1);
auto mr = Vector3f(w,  h2, 1);
auto tl = Vector3f(0,  h,  1);
auto tm = Vector3f(w2, h,  1);
auto tr = Vector3f(w,  h,  1);

auto ow = Vector3f(w, 0, 0);
auto oh = Vector3f(0, h, 0);
auto ob = ow + oh;

#ifdef UT_WIREFRAME
ZBuffer_AlwaysAllow __zba;
#define triangle(a, b, c, zb, im) triangle(a, b, c, zb, im); line(a.point, b.point, __zba, im, Colours::white); line(a.point, c.point, __zba, im, Colours::white); line(b.point, c.point, __zba, im, Colours::white)
#endif

// anticlockwise anticlockwise - bottom left quarter
triangle(Vertex(bl, red), Vertex(mm, green), Vertex(tl, blue), zbuf, image);
triangle(Vertex(tl, red), Vertex(mm, green), Vertex(tr, blue), zbuf, image);
triangle(Vertex(tr, red), Vertex(mm, green), Vertex(br, blue), zbuf, image);
triangle(Vertex(br, red), Vertex(mm, green), Vertex(bl, blue), zbuf, image);

// clockwise clockwise - bottom right quarter
triangle(Vertex(tl+ow, red), Vertex(mm+ow, green), Vertex(bl+ow, blue), zbuf, image);
triangle(Vertex(tr+ow, red), Vertex(mm+ow, green), Vertex(tl+ow, blue), zbuf, image);
triangle(Vertex(br+ow, red), Vertex(mm+ow, green), Vertex(tr+ow, blue), zbuf, image);
triangle(Vertex(bl+ow, red), Vertex(mm+ow, green), Vertex(br+ow, blue), zbuf, image);

// top right quarter
triangle(Vertex(tl+ob, red), Vertex(mm+ob, green), Vertex(tm+ob, blue), zbuf, image);
triangle(Vertex(tr+ob, red), Vertex(mm+ob, green), Vertex(tm+ob, blue), zbuf, image);
triangle(Vertex(tr+ob, red), Vertex(mm+ob, green), Vertex(mr+ob, blue), zbuf, image);
triangle(Vertex(br+ob, red), Vertex(mm+ob, green), Vertex(mr+ob, blue), zbuf, image);
triangle(Vertex(br+ob, red), Vertex(mm+ob, green), Vertex(bm+ob, blue), zbuf, image);
triangle(Vertex(bl+ob, red), Vertex(mm+ob, green), Vertex(bm+ob, blue), zbuf, image);
triangle(Vertex(bl+ob, red), Vertex(mm+ob, green), Vertex(ml+ob, blue), zbuf, image);
triangle(Vertex(tl+ob, red), Vertex(mm+ob, green), Vertex(ml+ob, blue), zbuf, image);

// clockwise clockwise - top left quarter
triangle(Vertex(bl+oh, red), Vertex(mm+oh, green), Vertex(tl+oh, blue), zbuf, image);
triangle(Vertex(tr+oh, red), Vertex(mm+oh, green), Vertex(tl+oh, blue), zbuf, image);
triangle(Vertex(tr+oh, red), Vertex(mm+oh, green), Vertex(br+oh, blue), zbuf, image);
triangle(Vertex(bl+oh, red), Vertex(mm+oh, green), Vertex(br+oh, blue), zbuf, image);

#ifdef UT_WIREFRAME
#undef triangle
#endif