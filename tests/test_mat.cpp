#include <iostream>
#include <HSim/vec.h>
#include <HSim/mat.h>
#include <HSim/parallel.h>


int main()
{
    HSim::Mat22d mat22;
    HSim::Mat33d mat33 {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    std::cout << mat33;

    // auto i = mat33[0];

    HSim::Mat33f mat33f(mat33);
    std::cout << mat33f;

    mat33.set(1);
    std::cout << mat33;

    mat33.set_row(0, std::vector<int> {2, 2, 2});
    mat33.set_row(1, 3);
    std::cout << mat33;

    std::cout << mat22(1);

    mat22[0][1] = 1;
    std::cout << mat22[0][1] << std::endl;

    // auto mat10000 = new HSim::Mat<float, 10000, 10000>;
    

    return 0;
}