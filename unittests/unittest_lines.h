Colour _c1_l = Colours::red,
       _c2_l = Colours::blue;
ZBuffer_AlwaysAllow __zba_l;

int __y_l = 0;
scan_line(__y_l,   0, 1, _c1_l, Vector2d(),   width, 1, _c2_l, Vector2d(),   __zba_l, image); __y_l+=2;
scan_line(__y_l,   width, 1, _c2_l, Vector2d(),   0, 1, _c1_l, Vector2d(),   __zba_l, image); __y_l+=2;
line(0, __y_l, 1, _c1_l,   width, __y_l, 1, _c2_l,   __zba_l, image); __y_l+=2;
line(width, __y_l, 1, _c2_l,   0, __y_l, 1, _c1_l,   __zba_l, image); __y_l+=2;