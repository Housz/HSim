#include <iostream>
#include <HSim/vec2.h>

int main()
{
    HSim::Vec2<double> v2(1, 2);
    HSim::Vec2d vd(1, 2);
    HSim::Vec2f vf(1, 2);
    std::cout << v2.x << std::endl;
    
    return 0;
}