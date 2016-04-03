
UNIT_TEST_T(Matrix4f::ZERO()             == Matrix4f(0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0));
UNIT_TEST_T(Matrix4f::IDENTITY()         == Matrix4f(1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1));
UNIT_TEST_T(Matrix4f::SCALE(3,4,5)       == Matrix4f(3,0,0,0, 0,4,0,0, 0,0,5,0, 0,0,0,1));
UNIT_TEST_T(Matrix4f::TRANSLATION(3,4,5) == Matrix4f(1,0,0,3, 0,1,0,4, 0,0,1,5, 0,0,0,1));
Matrix4f m = Matrix4f::FROM_AXES(Vector3f(1,2,3), Vector3f(4,5,6), Vector3f(7,8,9));
UNIT_TEST_T(m == Matrix4f(1,4,7,0, 2,5,8,0, 3,6,9,0, 0,0,0,1));
UNIT_TEST_T(m.transpose() == Matrix4f(1,2,3,0, 4,5,6,0, 7,8,9,0, 0,0,0,1))

Matrix4f sequential(1,2,3,4, 5,6,7,8, 9,10,11,12, 13,14,15,16);
Matrix4f const & const_sequential = sequential;
UNIT_TEST_T(const_sequential.transpose() == Matrix4f(1,5,9,13, 2,6,10,14, 3,7,11,15, 4,8,12,16));
UNIT_TEST_T(sequential == Matrix4f(1,2,3,4, 5,6,7,8, 9,10,11,12, 13,14,15,16));
UNIT_TEST_F(sequential.is_affine());
UNIT_TEST_T(Matrix4f::IDENTITY().is_affine());

m = Matrix4f::ZERO();
m(0, 0) = 3;
m(0, 1) = 0.5;
m(0, 2) = 0.5;
m(0, 3) = 0.5;
m(1, 1) = 5;
m(2, 2) = 7;
m(3, 3) = 9;
UNIT_TEST_T(m == Matrix4f(3,0.5,0.5,0.5, 0,5,0,0, 0,0,7,0, 0,0,0,9));
UNIT_TEST_T(m[0] == 3);
// UNIT_TEST_T(m(15) == 9);
UNIT_TEST_T(m(0,0) == 3);

std::stringstream("1,2,3,4, 5,6,7,8, 9,10,11,12, 13,14,15,16") >> sequential;
UNIT_TEST_T(sequential.row2(0) == Vector2f(1,2));
UNIT_TEST_T(sequential.row3(0) == Vector3f(1,2,3));
UNIT_TEST_T(sequential.row4(0) == Vector4f(1,2,3,4));
UNIT_TEST_T(sequential.row2(1) == Vector2f(5,6));
UNIT_TEST_T(sequential.row3(1) == Vector3f(5,6,7));
UNIT_TEST_T(sequential.row4(1) == Vector4f(5,6,7,8));
UNIT_TEST_T(sequential.row2(2) == Vector2f(9,10));
UNIT_TEST_T(sequential.row3(2) == Vector3f(9,10,11));
UNIT_TEST_T(sequential.row4(2) == Vector4f(9,10,11,12));
UNIT_TEST_T(sequential.row2(3) == Vector2f(13,14));
UNIT_TEST_T(sequential.row3(3) == Vector3f(13,14,15));
UNIT_TEST_T(sequential.row4(3) == Vector4f(13,14,15,16));

UNIT_TEST_T(sequential.column2(0) == Vector2f(1,5));
UNIT_TEST_T(sequential.column3(0) == Vector3f(1,5,9));
UNIT_TEST_T(sequential.column4(0) == Vector4f(1,5,9,13));
UNIT_TEST_T(sequential.column2(1) == Vector2f(2,6));
UNIT_TEST_T(sequential.column3(1) == Vector3f(2,6,10));
UNIT_TEST_T(sequential.column4(1) == Vector4f(2,6,10,14));
UNIT_TEST_T(sequential.column2(2) == Vector2f(3,7));
UNIT_TEST_T(sequential.column3(2) == Vector3f(3,7,11));
UNIT_TEST_T(sequential.column4(2) == Vector4f(3,7,11,15));
UNIT_TEST_T(sequential.column2(3) == Vector2f(4,8));
UNIT_TEST_T(sequential.column3(3) == Vector3f(4,8,12));
UNIT_TEST_T(sequential.column4(3) == Vector4f(4,8,12,16));

std::stringstream("160,150,140,130, 120,110,100,90, 80,70,60,50, 40,30,20,10") >> sequential;
UNIT_TEST_T(Matrix4f(160,150,140,130, 120,110,100,90, 80,70,60,50, 40,30,20,10) == sequential);

sequential.set_row(0, Vector2f(1,1));         UNIT_TEST_T(sequential == Matrix4f(1,1,0,0, 120,110,100,90, 80,70,60,50, 40,30,20,10));
sequential.set_row(0, Vector3f(2,2,2));       UNIT_TEST_T(sequential == Matrix4f(2,2,2,0, 120,110,100,90, 80,70,60,50, 40,30,20,10));
sequential.set_row(0, Vector4f(3,3,3,3));     UNIT_TEST_T(sequential == Matrix4f(3,3,3,3, 120,110,100,90, 80,70,60,50, 40,30,20,10));

sequential.set_row(1, Vector2f(4,4));         UNIT_TEST_T(sequential == Matrix4f(3,3,3,3, 4,4,0,0,        80,70,60,50, 40,30,20,10));
sequential.set_row(1, Vector3f(5,5,5));       UNIT_TEST_T(sequential == Matrix4f(3,3,3,3, 5,5,5,0,        80,70,60,50, 40,30,20,10));
sequential.set_row(1, Vector4f(6,6,6,6));     UNIT_TEST_T(sequential == Matrix4f(3,3,3,3, 6,6,6,6,        80,70,60,50, 40,30,20,10));

sequential.set_row(2, Vector2f(7,7));         UNIT_TEST_T(sequential == Matrix4f(3,3,3,3, 6,6,6,6,        7,7,1,0,     40,30,20,10));
sequential.set_row(2, Vector3f(8,8,8));       UNIT_TEST_T(sequential == Matrix4f(3,3,3,3, 6,6,6,6,        8,8,8,0,     40,30,20,10));
sequential.set_row(2, Vector4f(9,9,9,9));     UNIT_TEST_T(sequential == Matrix4f(3,3,3,3, 6,6,6,6,        9,9,9,9,     40,30,20,10));

sequential.set_row(3, Vector2f(10,10));       UNIT_TEST_T(sequential == Matrix4f(3,3,3,3, 6,6,6,6,        9,9,9,9,     10,10,0,1));
sequential.set_row(3, Vector3f(11,11,11));    UNIT_TEST_T(sequential == Matrix4f(3,3,3,3, 6,6,6,6,        9,9,9,9,     11,11,11,1));
sequential.set_row(3, Vector4f(12,12,12,12)); UNIT_TEST_T(sequential == Matrix4f(3,3,3,3, 6,6,6,6,        9,9,9,9,     12,12,12,12));

std::stringstream("160,150,140,130, 120,110,100,90, 80,70,60,50, 40,30,20,10") >> sequential;
sequential.set_column(0, Vector2f(1,1));         UNIT_TEST_T(sequential == Matrix4f(1,150,140,130, 1,110,100,90, 0,70,60,50, 0,30,20,10));
sequential.set_column(0, Vector3f(2,2,2));       UNIT_TEST_T(sequential == Matrix4f(2,150,140,130, 2,110,100,90, 2,70,60,50, 0,30,20,10));
sequential.set_column(0, Vector4f(3,3,3,3));     UNIT_TEST_T(sequential == Matrix4f(3,150,140,130, 3,110,100,90, 3,70,60,50, 3,30,20,10));

sequential.set_column(1, Vector2f(4,4));         UNIT_TEST_T(sequential == Matrix4f(3,4,140,130, 3,4,100,90, 3,0,60,50, 3,0,20,10));
sequential.set_column(1, Vector3f(5,5,5));       UNIT_TEST_T(sequential == Matrix4f(3,5,140,130, 3,5,100,90, 3,5,60,50, 3,0,20,10));
sequential.set_column(1, Vector4f(6,6,6,6));     UNIT_TEST_T(sequential == Matrix4f(3,6,140,130, 3,6,100,90, 3,6,60,50, 3,6,20,10));

sequential.set_column(2, Vector2f(7,7));         UNIT_TEST_T(sequential == Matrix4f(3,6,7,130, 3,6,7,90, 3,6,1,50, 3,6,0,10));
sequential.set_column(2, Vector3f(8,8,8));       UNIT_TEST_T(sequential == Matrix4f(3,6,8,130, 3,6,8,90, 3,6,8,50, 3,6,0,10));
sequential.set_column(2, Vector4f(9,9,9,9));     UNIT_TEST_T(sequential == Matrix4f(3,6,9,130, 3,6,9,90, 3,6,9,50, 3,6,9,10));

sequential.set_column(3, Vector2f(10,10));       UNIT_TEST_T(sequential == Matrix4f(3,6,9,10, 3,6,9,10, 3,6,9,0,  3,6,9,1));
sequential.set_column(3, Vector3f(11,11,11));    UNIT_TEST_T(sequential == Matrix4f(3,6,9,11, 3,6,9,11, 3,6,9,11, 3,6,9,1));
sequential.set_column(3, Vector4f(12,12,12,12)); UNIT_TEST_T(sequential == Matrix4f(3,6,9,12, 3,6,9,12, 3,6,9,12, 3,6,9,12));


m = Matrix4f::IDENTITY();
UNIT_TEST_T(m.scale() == Vector3f(1,1,1));
UNIT_TEST_T(m.translation() == Vector3f(0,0,0));
m = Matrix4f(2,1,1,9, 1,3,1,8, 1,1,4,7, 0,0,0,1);
UNIT_TEST_T(m.scale() == Vector3f(2,3,4));
UNIT_TEST_T(m.translation() == Vector3f(9,8,7));

std::stringstream("1,2,3,4, 5,6,7,8, 9,10,11,12, 13,14,15,16") >> m;
UNIT_TEST_T((m + 1) == Matrix4f(2,3,4,5, 6,7,8,9, 10,11,12,13, 14,15,16,17));
UNIT_TEST_T((m - 1) == Matrix4f(0,1,2,3, 4,5,6,7, 8,9,10,11, 12,13,14,15));
UNIT_TEST_T((m * 2) == Matrix4f(2,4,6,8, 10,12,14,16, 18,20,22,24, 26,28,30,32));
UNIT_TEST_T((m / 3) == Matrix4f(1.0/3,2.0/3,1,4.0/3, 5.0/3,6.0/3,7.0/3,8.0/3, 9.0/3,10.0/3,11.0/3,12.0/3, 13.0/3,14.0/3,15.0/3,16.0/3));

std::stringstream("1,2,3,4, 5,6,7,8, 9,10,11,12, 13,14,15,16") >> m;
UNIT_TEST_T((m += 1) == Matrix4f(2,3,4,5, 6,7,8,9, 10,11,12,13, 14,15,16,17));
UNIT_TEST_T((m -= 1) == Matrix4f(1,2,3,4, 5,6,7,8, 9,10,11,12, 13,14,15,16));
UNIT_TEST_T((m *= 2) == Matrix4f(2,4,6,8, 10,12,14,16, 18,20,22,24, 26,28,30,32));
UNIT_TEST_T((m /= 3) == Matrix4f(2.0/3,4.0/3,6.0/3,8.0/3, 10.0/3,12.0/3,14.0/3,16.0/3, 18.0/3,20.0/3,22.0/3,24.0/3, 26.0/3,28.0/3,30.0/3,32.0/3));

// matrix vector multiplication
std::stringstream("1,2,3,4, 5,6,7,8, 9,10,11,12, 13,14,15,16") >> m;
Vector4f v4(4,3,2,1);
UNIT_TEST_T(m * v4 == Vector4f(20, 60, 100, 140));
v4 = Vector4f(4,3,2,5);
UNIT_TEST_T(m * v4 == Vector4f(36, 92, 148, 204));
Vector3f v3(4,3,2);
UNIT_TEST_T(m * v3 == Vector3f(0.14285714285714, 0.42857142857143, 0.71428571428571));
std::stringstream("1,2,3,4, 5,6,7,8, 9,10,11,12, 0,0,0,1") >> m;
UNIT_TEST_T(m * v3 == Vector3f(20, 60, 100));
