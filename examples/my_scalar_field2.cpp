#include <iostream>
#include <fstream>
#include <cstdlib> 

#include <HSim/scalar_field2.h>

using namespace HSim;

template <typename T, size_t X, size_t Y>
class MyScalarField2 : public ScalarField2<T, X, Y>
{
public:

    T sample(const Vec2i& pos) override
    {
        const float scalar = 0.1;
        assert(pos.x < X && pos.y < Y);
        return std::sin(pos.x * scalar) * std::sin(pos.y * scalar);
    }

    T sample_scale(const Vec2i& pos, const T scalar) 
    {
        assert(pos.x < X && pos.y < Y);
        return std::sin(pos.x * scalar) * std::sin(pos.y * scalar);
    }

    Vec2<T> gradient(const Vec2i& pos) override
    {
        Vec2<T> grad;
        return grad;
    }

	// todo
    T laplacian(const Vec2i& pos) override
    {
        return 0;
    }

};

int main(int argc, char *argv[])
{
    float scalar = 1;
    if (argc == 2)
    {
        scalar = std::atof(argv[1]);
    }
    

    const size_t X = 100, Y = 100;
    MyScalarField2<float, X, Y> sf2;
    // std::cout << sf.sample(Vec2i(1, 1));

    // out
    std::ofstream fs;
    std::ofstream outFile("sf2sample.txt");
    if (outFile.is_open()) {

        for (size_t i = 0; i < X; i++)
        {
            for (size_t j = 0; j < Y; j++)
            {
                auto s = sf2.sample_scale(Vec2i(i, j), scalar);
        
                outFile << s << " ";

            }
            outFile << std::endl;
        }

        outFile.close();
        std::cout << "out done: a.txt" << std::endl;

    } else {
        std::cerr << "out error" << std::endl;
    }

    return 0;
}