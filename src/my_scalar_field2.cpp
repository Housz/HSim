#include <iostream>
#include <fstream>

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

    Vec2<T> gradient(const Vec2i& pos) override
    {
        Vec2<T> grad;
        return grad;
    }

    T laplacian(const Vec2i& pos) override
    {
        return 0;
    }

};

int main()
{
    size_t X = 100, Y = 100;
    MyScalarField2<float, 100, 100> sf;
    std::cout << sf.sample(Vec2i(1, 1));

    // out
    std::ofstream fs;
    std::ofstream outFile("a.txt");
    if (outFile.is_open()) {

        // outFile << "123" << std::endl;

        for (size_t i = 0; i < X; i++)
        {
            for (size_t j = 0; j < Y; j++)
            {
                auto s = sf.sample(Vec2i(i, j));
                
                outFile << s << " ";

            }
            outFile << std::endl;
        }
        

        outFile.close();

        std::cout << "ok a.txt" << std::endl;
    } else {
        std::cerr << "err a.txt" << std::endl;
    }

    return 0;
}