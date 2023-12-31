#include <iostream>
#include <HSim/vec2.h>

#define log(x) ( std::cout << x << std::endl )
// #define log ( std::cout << )


int main()
{
    HSim::Vec2<double> v2(1, 2);
    HSim::Vec2d vd(1, 2);
    HSim::Vec2f vf(1, 2);

    std::cout << v2 << vd << vf;

    std::cout << v2.add(2);
    auto vd1 = new HSim::Vec2d(3, 4);
    std::cout << *vd1 ;

    std::cout << vd.add( 1.2 );
    std::cout << vd.add( *(new HSim::Vec2d(1, 2)) );
    std::cout << vd.sub( 3.14 );
    std::cout << vd.sub( *(new HSim::Vec2d(1, 2)) );
    std::cout << vd.mul( 2 );

    vd.add_self( *(new HSim::Vec2d(1, 2)) );
    std::cout << vd;

    std::cout << vd[1] << std::endl;

    vd =  *vd1;  std::cout << vd;
    vd += *vd1; std::cout << vd;
    vd -= *vd1; std::cout << vd;
    vd *= *vd1; std::cout << vd;
    vd /= *vd1; std::cout << vd;

    std::cout << vd << *vd1;

    std::cout << vd + *vd1;
    std::cout << vd - *vd1 + vd;
    std::cout << vd / *vd1 + vd;
    std::cout << vd * 2 / 3;

    auto is = vd == (*vd1);

    HSim::Vec2d vlist = {1, 2}; std::cout << vlist;
    vlist.set({1, 3}); std::cout << vlist;
    vlist = {3, 4}; std::cout << vlist;
    
    

    return 0;
}