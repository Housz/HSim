#include <iostream>
#include <HSim/vec2.h>

int main()
{
    HSim::Vec2<double> v2(1, 2);
    HSim::vec2d vd(1, 2);
    HSim::vec2f vf(1, 2);
    std::cout << v2.x << std::endl;
    
    return 0;
}