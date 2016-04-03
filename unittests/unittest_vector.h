
const Vector2f v1(1, 0);
Vector2f v2, v3(1, 2);

// test assignment ctr, copy ctr, and assignment
const Vector2f v4 = v3;
const Vector2f v5(v3);
v2 = v1;

// comparison
UNIT_TEST_T(v1 == v1);
UNIT_TEST_T(v1 == v2);
UNIT_TEST_T(v2 == v1);
UNIT_TEST_F(v1 == v3);
UNIT_TEST_F(v3 == v1);
UNIT_TEST_T(v1 != v3);
UNIT_TEST_F(v1 != v1);
UNIT_TEST_T(v1 <= v3);
UNIT_TEST_T(v3 >= v1);
UNIT_TEST_F(v1 >= v3);
UNIT_TEST_F(v3 <= v1);
UNIT_TEST_T(v1 < v3);
UNIT_TEST_T(v3 > v1);
UNIT_TEST_F(v1 > v3);
UNIT_TEST_F(v3 < v1);

// arithmetic with float
UNIT_TEST_T((v1 + 3) == Vector2f(4, 3));
UNIT_TEST_T((v1 - 3) == Vector2f(-2, -3));
UNIT_TEST_T((v4 * 3) == Vector2f(3, 6));
UNIT_TEST_T((v4 / 2) == Vector2f(0.5, 1));
Vector2f v1a = v1, v1b = v1;
Vector2f v4a = v4, v4b = v4;
UNIT_TEST_T((v1a += 3) == Vector2f(4, 3));
UNIT_TEST_T((v1b -= 3) == Vector2f(-2, -3));
UNIT_TEST_T((v4a *= 3) == Vector2f(3, 6));
UNIT_TEST_T((v4b /= 2) == Vector2f(0.5, 1));

// arithmetic with vector
PRINTLN("MISSING v x v, v x= v");

// subscript
UNIT_TEST_T(v3[0] == 1);
UNIT_TEST_T(v3[1] == 2);
UNIT_TEST_T(v3(0) == 1);
UNIT_TEST_T(v3(1) == 2);
v3[0] = 5; UNIT_TEST_T(v3[0] == 5);
v3[1] = 2; UNIT_TEST_T(v3[1] == 2);
v3(0) = 9; UNIT_TEST_T(v3(0) == 9);
v3(1) = 8; UNIT_TEST_T(v3(1) == 8);

// vector 2 functions
UNIT_TEST_T(is_equal(v3.angle(), 0.726642));
UNIT_TEST_T(v1.rotate(1.5708) == Vector2f(0, 1));
UNIT_TEST_T(v3.rotate(1.5708) == Vector2f(-8.00003,8.99997));
UNIT_TEST_T(is_equal(v3.angle(), 2.29744));
UNIT_TEST_T(is_equal(v3.angle().rad(), deg_to_rad(v3.angle().deg())));
UNIT_TEST_T(v1.rotate(Angle(-90, Angle::TYPE_DEG)) == Vector2f(0, -1));
UNIT_TEST_T(v3.rotate(Angle::DEG(-90)) == Vector2f(9, 8));
UNIT_TEST_T(is_equal(v3.angle(), 0.726642));

// vector 3 functions
UNIT_TEST_T(Vector3f(1, 0, 0).cross(Vector3f(0, 1, 0)) == Vector3f(0, 0, 1));
UNIT_TEST_T(Vector3f(0, 1, 0).cross(Vector3f(1, 0, 0)) == Vector3f(0, 0, -1));

// all vector functions
UNIT_TEST_T(is_equal(v3.dot(v4), 25));
UNIT_TEST_T(is_equal(v3.squared_length(), 145.0));
UNIT_TEST_T(is_equal(v3.length(), 12.0416));
UNIT_TEST_T(v3.normalize() == Vector2f(0.747409, 0.664364));
UNIT_TEST_T(v4.normalize() == Vector2f(0.447214, 0.894427));
UNIT_TEST_T(v4 == Vector2f(1, 2)); // check v4 hasn't changed
UNIT_TEST_T(v3.length() == 1);
UNIT_TEST_T(v3.is_unit());
UNIT_TEST_T(v3.squared_distance(v1));
UNIT_TEST_T(v3.distance(v1));
