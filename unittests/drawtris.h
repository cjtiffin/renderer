// anticlockwise anticlockwise - bottom left quarter
triangle(Vertex(bl, _c1_ct), Vertex(mm, _c2_ct), Vertex(tl, _c3_ct), __zba_ct, image);
triangle(Vertex(tl, _c1_ct), Vertex(mm, _c2_ct), Vertex(tr, _c3_ct), __zba_ct, image);
triangle(Vertex(tr, _c1_ct), Vertex(mm, _c2_ct), Vertex(br, _c3_ct), __zba_ct, image);
triangle(Vertex(br, _c1_ct), Vertex(mm, _c2_ct), Vertex(bl, _c3_ct), __zba_ct, image);

// clockwise clockwise - bottom right quarter
triangle(Vertex(tl+ow, _c1_ct), Vertex(mm+ow, _c2_ct), Vertex(bl+ow, _c3_ct), __zba_ct, image);
triangle(Vertex(tr+ow, _c1_ct), Vertex(mm+ow, _c2_ct), Vertex(tl+ow, _c3_ct), __zba_ct, image);
triangle(Vertex(br+ow, _c1_ct), Vertex(mm+ow, _c2_ct), Vertex(tr+ow, _c3_ct), __zba_ct, image);
triangle(Vertex(bl+ow, _c1_ct), Vertex(mm+ow, _c2_ct), Vertex(br+ow, _c3_ct), __zba_ct, image);

// top right quarter
triangle(Vertex(tl+ob, _c1_ct), Vertex(mm+ob, _c2_ct), Vertex(tm+ob, _c3_ct), __zba_ct, image);
triangle(Vertex(tr+ob, _c1_ct), Vertex(mm+ob, _c2_ct), Vertex(tm+ob, _c3_ct), __zba_ct, image);
triangle(Vertex(tr+ob, _c1_ct), Vertex(mm+ob, _c2_ct), Vertex(mr+ob, _c3_ct), __zba_ct, image);
triangle(Vertex(br+ob, _c1_ct), Vertex(mm+ob, _c2_ct), Vertex(mr+ob, _c3_ct), __zba_ct, image);
triangle(Vertex(br+ob, _c1_ct), Vertex(mm+ob, _c2_ct), Vertex(bm+ob, _c3_ct), __zba_ct, image);
triangle(Vertex(bl+ob, _c1_ct), Vertex(mm+ob, _c2_ct), Vertex(bm+ob, _c3_ct), __zba_ct, image);
triangle(Vertex(bl+ob, _c1_ct), Vertex(mm+ob, _c2_ct), Vertex(ml+ob, _c3_ct), __zba_ct, image);
triangle(Vertex(tl+ob, _c1_ct), Vertex(mm+ob, _c2_ct), Vertex(ml+ob, _c3_ct), __zba_ct, image);

// clockwise clockwise - top left quarter
triangle(Vertex(bl+oh, _c1_ct), Vertex(mm+oh, _c2_ct), Vertex(tl+oh, _c3_ct), __zba_ct, image);
triangle(Vertex(tr+oh, _c1_ct), Vertex(mm+oh, _c2_ct), Vertex(tl+oh, _c3_ct), __zba_ct, image);
triangle(Vertex(tr+oh, _c1_ct), Vertex(mm+oh, _c2_ct), Vertex(br+oh, _c3_ct), __zba_ct, image);
triangle(Vertex(bl+oh, _c1_ct), Vertex(mm+oh, _c2_ct), Vertex(br+oh, _c3_ct), __zba_ct, image);