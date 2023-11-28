#include <iostream>
#include <HSim/vec.h>
#include <HSim/mat.h>
#include <HSim/parallel.h>


int main()
{
    HSim::Mat<float, 2, 2> mat22;

    // std::cout << mat22;

    // std::cout << mat22(0, 1) << std::endl;

    std::cout << mat22(1);

    return 0;
}