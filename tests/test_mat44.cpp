#include <HSim/mat44.h>
#include <HSim/vec4.h>

int main()
{
	HSim::Mat44f m({1,2,3,4,
					   5,6,7,8,
					   9,10,11,12,
					   13,14,15,16});

	
	std::cout << m;

	HSim::Mat44f m1(1);
	HSim::Mat44f m2(m);
	auto m3(m2);

    HSim::Mat33f m33 = { -1, -1, -1, -1, -1, -1, -1, -1, -1 };
    HSim::Mat44f m4(m33);
    std::cout << m4;

// mul
// #define MUL
#ifdef MUL
    std::cout << m * 2;
    std::cout << 2 * m;
    std::cout << m1 * m;
    m2 *= 2;
    std::cout << m2;
    m2 *= m1;
    std::cout << m2;
#endif 

// add
// #define ADD
#ifdef ADD
    std::cout << m2 + m3;
    m2 += m3;
    std::cout << m2;
    m2 += 1;
    std::cout << m2;
    std::cout << m2 + 1;
    std::cout << 1 + m2;
#endif 

// sub
// #define SUB
#ifdef SUB
    std::cout << m2 - m3;
    m2 -= m3;
    std::cout << m2;
    m2 -= 1;
    std::cout << m2;
    std::cout << m2 - 1;
    std::cout << 1 - m2;
    std::cout << -m2;
    std::cout << -m2 * 2;
#endif 

// mul v
// #define VEC
#ifdef VEC  
    HSim::Vec4f v(1, 2, 3, 4);
    std::cout << -m2 * v;
    std::cout << -v * m2;

#endif 

// trans
// #define TRANS
#ifdef TRANS
    m2.setIdentity();
    std::cout << m2;
    m2.transpose();
    std::cout << m2;
    std::cout << m2.determinant() << std::endl;
    m2.invert();
    std::cout << m2;
#endif 


// set
// #define SET
#ifdef SET
    std::cout << m;
    m.setCol(0, {-1, -1, -1, -1});
    std::cout << m;
    m.setRow(0, {-2, -2, -2, -2});
    std::cout << m;
#endif

	return 0;
}