#include <iostream>
#include <HSim/vec.h>
#include <HSim/mat.h>
#include <HSim/parallel.h>


int main()
{
    HSim::Matd<2, 2> mat22;
    HSim::Matd<3, 3> mat33 {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    std::cout << mat33;

    // auto i = mat33[0];

    HSim::Matf<3, 3> mat33f(mat33);
    std::cout << mat33f;

    mat33.set(1);
    std::cout << mat33;


    mat33.set_row(0, std::vector<int> {2, 2, 2});
    mat33.set_row(1, 3);
    std::cout << mat33;

    mat33.set_col(0, std::vector<float> {3.14, 3.15, 3.16});
    mat33.set_col(2, 0);
    std::cout << mat33;

    HSim::Matd<4, 4> mat44;
    mat44.set_diag(1);
    std::cout << mat44;
    mat44 += 1;
    std::cout << mat44;

    mat44 += mat44;
    std::cout << mat44;

    auto ma = mat44.add(1);
    std::cout << ma;
    // std::cout << mat44.add(1) << std::endl;
    // std::cout << mat44.add(mat44) << std::endl;

    std::cout << ma + ma;

    std::cout << mat44 * 2;
    // std::cout << ma * mat44;

    std::cout << 2 * mat44 * -1;


    // test op(mat, mat)
    HSim::Mat<float, 3, 4> m34(2);
    HSim::Mat<double, 4, 2> m42(3);
    HSim::Mat<float, 2, 1> m21(1);

    std::cout << m34;
    std::cout << m42;
    std::cout << m21;
    std::cout << m34 * m42;
    std::cout << m34 * m42 * m21;
    auto mulmat = m34 * m42 * m21;

    HSim::Mat<float, 1000, 1000> m1(1), m2(1);
    auto mm = m1*m2;
    // std::cout << (m1 * m2)[1][1];


    // auto mat10000 = new HSim::Mat<float, 10000, 10000>;
    

    return 0;
}