#include <math.h>
#include <float.h>
#include <stdexcept>
#include "typed_array.h"
#include "point.h"
#include "matrix.h"
#include "gtest/gtest.h"

namespace {

TEST(TypedArray, Construction) {
    TypedArray<Point> b;
    b.set(0, Point(1,2,3));
    b.set(1, Point(2,3,4));
    b.set(20, Point(3,4,5));
    EXPECT_EQ(b.get(0).x, 1);
    EXPECT_EQ(b.get(1).y, 3);
    EXPECT_EQ(b.get(20).z, 5);
}

TEST(TypedArray, Defaults) {
    TypedArray<Point> x;
    Point & y = x.get(3);
    EXPECT_DOUBLE_EQ(y.magnitude(), 0.0);
}

TEST(TypedArray, MatrixLike) {
    TypedArray<TypedArray<double>> m;
    for (int i=0; i<3; i++) {
        for (int j=0; j<3; j++) {
            m.get(i).set(j, 3*i + j);
        }
    }
    for (int i=0; i<3; i++) {
        for (int j=0; j<3; j++) {
            EXPECT_DOUBLE_EQ(m.get(i).get(j), 3*i + j);
        }
    }
}

TEST(TypedArray, CopyElementsInSet1) {
    TypedArray<Point> b;
    Point p(1,2,3);
    b.set(0, p);
    p.x = 4;
    EXPECT_DOUBLE_EQ(b.get(0).x, 1);
}

TEST(TypedArray, CopyElementsInSet2) {
    TypedArray<TypedArray<double>> m;
    TypedArray<double> x;
    x.set(0,0);
    m.set(0,x);
    x.set(0,-1);
    EXPECT_DOUBLE_EQ(m.get(0).get(0), 0.0);
}

TEST(TypedArray, PushPop) {
    TypedArray<int> a;
    a.push(1);
    a.push(2);
    a.push_front(0);
    EXPECT_EQ(a.size(), 3);
    EXPECT_EQ(a.safe_get(0), 0);
    EXPECT_EQ(a.safe_get(1), 1);
    EXPECT_EQ(a.safe_get(2), 2);
    EXPECT_EQ(a.pop(), 2);
    EXPECT_EQ(a.pop_front(), 0);
    EXPECT_EQ(a.pop(), 1);
    EXPECT_EQ(a.size(), 0);
}

TEST(TypedArray, PopEmptyThrows) {
    TypedArray<int> a;
    EXPECT_THROW(a.pop(), std::range_error);
    EXPECT_THROW(a.pop_front(), std::range_error);
}

TEST(TypedArray, ConcatPlusReverse) {
    TypedArray<int> a;
    a.push(1);
    a.push(2);
    TypedArray<int> b;
    b.push(3);
    b.push(4);

    TypedArray<int> c = a.concat(b);
    EXPECT_EQ(a.size(), 2);
    EXPECT_EQ(b.size(), 2);
    EXPECT_EQ(c.size(), 4);
    EXPECT_EQ(c.safe_get(0), 1);
    EXPECT_EQ(c.safe_get(3), 4);

    TypedArray<int> d = a + b + a;
    EXPECT_EQ(d.size(), 6);
    EXPECT_EQ(d.safe_get(0), 1);
    EXPECT_EQ(d.safe_get(1), 2);
    EXPECT_EQ(d.safe_get(2), 3);
    EXPECT_EQ(d.safe_get(3), 4);
    EXPECT_EQ(d.safe_get(4), 1);
    EXPECT_EQ(d.safe_get(5), 2);

    d.reverse();
    EXPECT_EQ(d.safe_get(0), 2);
    EXPECT_EQ(d.safe_get(5), 1);
}

TEST(Matrix, ConstructorsAndAccess) {
    Matrix a;
    EXPECT_TRUE(a.isEmpty());

    Matrix b(2,3);
    EXPECT_EQ(b.rows(), 2u);
    EXPECT_EQ(b.cols(), 3u);
    EXPECT_DOUBLE_EQ(b(1,2), 0.0);

    Matrix c(2,2,5.0);
    EXPECT_DOUBLE_EQ(c(0,0), 5.0);
    EXPECT_DOUBLE_EQ(c(1,1), 5.0);

    Matrix d{{1,2},{3,4}};
    EXPECT_DOUBLE_EQ(d(0,1), 2.0);
    EXPECT_DOUBLE_EQ(d(1,0), 3.0);

    EXPECT_THROW(d.at(10,0), std::out_of_range);
}

TEST(Matrix, CopySemantics) {
    Matrix a{{1,2},{3,4}};
    Matrix b = a;
    b(0,0) = 9;
    EXPECT_DOUBLE_EQ(a(0,0), 1.0);
    EXPECT_DOUBLE_EQ(b(0,0), 9.0);

    Matrix c;
    c = a;
    c(1,1) = 8;
    EXPECT_DOUBLE_EQ(a(1,1), 4.0);
    EXPECT_DOUBLE_EQ(c(1,1), 8.0);
}

TEST(Matrix, AddSubScalarUnary) {
    Matrix a{{1,2},{3,4}};
    Matrix b{{5,6},{7,8}};
    Matrix c = a + b;
    EXPECT_DOUBLE_EQ(c(0,0), 6.0);
    EXPECT_DOUBLE_EQ(c(1,1), 12.0);

    Matrix d = b - a;
    EXPECT_DOUBLE_EQ(d(0,0), 4.0);
    EXPECT_DOUBLE_EQ(d(1,1), 4.0);

    Matrix e = a * 2.0;
    EXPECT_DOUBLE_EQ(e(1,0), 6.0);

    Matrix f = 2.0 * a;
    EXPECT_DOUBLE_EQ(f(0,1), 4.0);

    Matrix g = -a;
    EXPECT_DOUBLE_EQ(g(1,1), -4.0);

    EXPECT_THROW(a + Matrix(3,3), std::invalid_argument);
}

TEST(Matrix, Multiply) {
    Matrix a{{1,2,3},{4,5,6}};
    Matrix b{{7,8},{9,10},{11,12}};
    Matrix c = a * b;
    EXPECT_EQ(c.rows(), 2u);
    EXPECT_EQ(c.cols(), 2u);
    EXPECT_DOUBLE_EQ(c(0,0), 58.0);
    EXPECT_DOUBLE_EQ(c(0,1), 64.0);
    EXPECT_DOUBLE_EQ(c(1,0), 139.0);
    EXPECT_DOUBLE_EQ(c(1,1), 154.0);

    EXPECT_THROW(b * a, std::invalid_argument);
}

TEST(Matrix, TransposeTraceDiagonalNormFillFactories) {
    Matrix a{{1,2},{3,4}};
    Matrix t = a.transpose();
    EXPECT_DOUBLE_EQ(t(0,1), 3.0);
    EXPECT_DOUBLE_EQ(t(1,0), 2.0);

    EXPECT_DOUBLE_EQ(a.trace(), 5.0);
    EXPECT_THROW(Matrix(2,3).trace(), std::logic_error);

    Matrix d = a.diagonal();
    EXPECT_EQ(d.rows(), 2u);
    EXPECT_EQ(d.cols(), 1u);
    EXPECT_DOUBLE_EQ(d(0,0), 1.0);
    EXPECT_DOUBLE_EQ(d(1,0), 4.0);

    Matrix z = Matrix::zeros(2,2
