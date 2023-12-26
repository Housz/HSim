#include <HSim/mat33.h>

int main()
{
    HSim::Mat33f m33f;
    std::cout << m33f.container[0] << std::endl;

    HSim::Mat33f m(m33f);
    HSim::Mat33f m1(1);
    HSim::Mat33f m2({1,2,3,4,5,6,7,8,9});
    std::cout << m2;
    auto m3(m2);
    std::cout << m3;

    std::cout << m3 * 2;
    std::cout << 2 * m3;
    // HSim::Mat33f m3 {{1,2,3},{4,5,6},{7,8,9}};
    // std::cout << m3;




    return 0;
}