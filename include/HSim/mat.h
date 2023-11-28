#pragma once

#include <HSim/common.h>
#include <HSim/vec.h>
#include <HSim/parallel.h>

namespace HSim
{

    template <typename T, size_t M, size_t N>
    class Mat
    {
    public:
        std::vector<std::vector<T>> container;
        typename std::vector<T>::iterator begin() { return container.begin(); }
        typename std::vector<T>::iterator end() { return container.end(); }

        template <typename T1, size_t M1, size_t N1>
        friend std::ostream &operator<<(std::ostream &, Mat<T1, M1, N1> &);

        // Mat(): container(M*N, 0) {}
        Mat(): container(M, std::vector<T>(N, 0)){ }

        T& operator()(size_t i, size_t j)
        {
            assert(i<M && j<N);
            return container[i][j];
        }

        T operator()(size_t i, size_t j) const
        {
            assert(i<M && j<N);
            return container[i][j];
        }

        Vec<T> operator()(size_t i)
        {
            assert(i<M);
            Vec<T> row_i(container[i]);
            return row_i;
        }
        

    };

    template <typename T1, size_t M1, size_t N1>
    std::ostream &operator<<(std::ostream &os, Mat<T1, M1, N1> &m_)
    {
        
        os << m_(1, 1) << " ";

        os << std::endl;

        return os;
    }

    

} // namespace HSim
