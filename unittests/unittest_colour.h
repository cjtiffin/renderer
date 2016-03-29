typedef  ColourBase<short>  ColourS;

const ColourUC orig_uc(255, 127, 63, 0);
const ColourD orig_d(1, 0.5, 0.25, 0);
const ColourF orig_f(1, 0.5, 0.25, 0);
const ColourS orig_s(32767, 16383, 8191, 0);

const ColourD comp_d(1, 0.498039, 0.247059, 0);
const ColourF comp_f(1, 0.498039, 0.247059, 0);
const ColourS comp_s(32767, 16319, 8095, 0);

// #undef UNIT_TEST_T
// #define UNIT_TEST_T(x)

{
ColourUC uc = orig_uc; UNIT_TEST_T(uc == orig_uc);
ColourD d = orig_uc;   UNIT_TEST_T(d == comp_d);
ColourF f = orig_uc;   UNIT_TEST_T(f == comp_f);
ColourS s = orig_uc;   UNIT_TEST_T(s == comp_s);
}

{
ColourUC uc = orig_d; UNIT_TEST_T(uc == orig_uc);
ColourD d = orig_d;   UNIT_TEST_T(d == orig_d);
ColourF f = orig_d;   UNIT_TEST_T(f == orig_f);
ColourS s = orig_d;   UNIT_TEST_T(s == orig_s);
}

{
ColourUC uc = orig_f; UNIT_TEST_T(uc == orig_uc);
ColourD d = orig_f;   UNIT_TEST_T(d == orig_d);
ColourF f = orig_f;   UNIT_TEST_T(f == orig_f);
ColourS s = orig_f;   UNIT_TEST_T(s == orig_s);
}

{
ColourUC uc = orig_s; UNIT_TEST_T(uc == orig_uc);
ColourD d = orig_s;   UNIT_TEST_T(d == orig_d);
ColourF f = orig_s;   UNIT_TEST_T(f == orig_f);
ColourS s = orig_s;   UNIT_TEST_T(s == orig_s);
}