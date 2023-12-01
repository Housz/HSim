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
        // typename std::vector<T>::iterator begin() { return container.begin(); }
        // typename std::vector<T>::iterator end() { return container.end(); }

        template <typename T1, size_t M1, size_t N1>
        friend std::ostream &operator<<(std::ostream &, Mat<T1, M1, N1> &);

        // Mat(): container(M*N, 0) {}
        Mat(): container(M, std::vector<T>(N, 0)){ }
        Mat(T value): container(M, std::vector<T>(N, value)){ }
		Mat(const std::initializer_list<std::initializer_list<T>>& list): container(M, std::vector<T>(N, 0)) 
        {
             set(list); 
        }
        template <typename T1>
        Mat(const Mat<T1, M, N>& m_): container(M, std::vector<T>(N, 0)) 
        {
            set(m_);
        }
		
		void set(const std::initializer_list<std::initializer_list<T>>& list)
		{
            for (auto iter = list.begin(); iter < list.end(); iter++)
            {
                container[iter-list.begin()] = *iter;
            }
		}

        template <typename T1>
        void set(const Mat<T1, M, N>& m_)
        {
            for (size_t i = 0; i < M; i++)
            {
                for (size_t j = 0; j < N; j++)
                {
                    container[i][j] = (T)m_.container[i][j];
                }
            }
        }

        void set(T value)
        {
            parallelFor(size_t(0), M, [&](size_t i){
                std::fill(container[i].begin(), container[i].end(), value);
            });
        }

        template <typename T1>
        void set_row(size_t i, std::vector<T1> row_)
        {
            parallelFor(size_t(0), N, [&](size_t j){
                container[i][j] = T(row_[j]);
            });
        }

        template <typename T1>
        void set_row(size_t i, T1 value)
        {
            parallelFor(size_t(0), N, [&](size_t j){
                container[i][j] = T(value);
            });
        }

        template <typename T1>
        void set_col(size_t j, std::vector<T1> col_)
        {
            parallelFor(size_t(0), M, [&](size_t i){
                container[i][j] = col_[i];
            });
        }

        template <typename T1>
        void set_col(size_t j, T1 value)
        {
            parallelFor(size_t(0), N, [&](size_t i){
                container[i][j] = T(value);
            });
        }

        template <typename T1>
        void set_diag(T1 value)
        {
            assert(M == N);
            parallelFor(size_t(0), M, [&](size_t i){
                container[i][i] = T(value);
            });
        }
        
        size_t size_row()
        {
            return M;
        }

        size_t size_col()    
        {
            return N;
        }

        template <typename T1>
        Mat<T, M, N> add(T1 value)
        {
            Mat<T, M, N> m;

            parallelFor(size_t(0), M, [&](size_t i){
                parallelFor(size_t(0), N, [&](size_t j){
                    m[i][j] = container[i][j] + T(value);
                });
            });

            return m;
        }

        template <typename T1>
        Mat<T, M, N> add(Mat<T1, M, N> &m_)
        {
            Mat<T, M, N> m;
            parallelFor(size_t(0), M, [&](size_t i){
                parallelFor(size_t(0), N, [&](size_t j){
                    m[i][j] = container[i][j] + T(m_[i][j]);
                });
            });

            return m;
        }       

        template <typename T1>
        void add_self(T1 value)
        {
            parallelFor(size_t(0), M, [&](size_t i){
                for(auto &ij: container[i])
                {
                    ij += value;
                }
            });
        }

        template <typename T1>
        void add_self(Mat<T1, M, N> &m_)
        {
            parallelFor(size_t(0), M, [&](size_t i){
                parallelFor(size_t(0), N, [&](size_t j){
                    container[i][j] += (T)m_[i][j];
                });
            });
        }

        template <typename T1>
        Mat<T, M, N> sub(T1 value)
        {
            Mat<T, M, N> m;

            parallelFor(size_t(0), M, [&](size_t i){
                parallelFor(size_t(0), N, [&](size_t j){
                    m[i][j] = container[i][j] - T(value);
                });
            });

            return m;
        }

        template <typename T1>
        Mat<T, M, N> sub(Mat<T1, M, N> &m_)
        {
            Mat<T, M, N> m;
            parallelFor(size_t(0), M, [&](size_t i){
                parallelFor(size_t(0), N, [&](size_t j){
                    m[i][j] = container[i][j] - T(m_[i][j]);
                });
            });

            return m;
        }   

        template <typename T1>
        void sub_self(T1 value)
        {
            parallelFor(size_t(0), M, [&](size_t i){
                for(auto &ij: container[i])
                {
                    ij -= value;
                }
            });
        }

        template <typename T1>
        void sub_self(Mat<T1, M, N> &m_)
        {
            parallelFor(size_t(0), M, [&](size_t i){
                parallelFor(size_t(0), N, [&](size_t j){
                    container[i][j] -= (T)m_[i][j];
                });
            });
        }

        template <typename T1>
        Mat<T, M, N> mul(T1 value)
        {
            Mat<T, M, N> m;

            parallelFor(size_t(0), M, [&](size_t i){
                parallelFor(size_t(0), N, [&](size_t j){
                    m[i][j] = container[i][j] * T(value);
                });
            });

            return m;
        }

        template <typename T1>
        Mat<T, M, N> mul(Mat<T1, M, N> &m_)
        {
            Mat<T, M, N> m;
            parallelFor(size_t(0), M, [&](size_t i){
                parallelFor(size_t(0), N, [&](size_t j){
                    m[i][j] = container[i][j] * T(m_[i][j]);
                });
            });

            return m;
        }   

        template <typename T1>
        void mul_self(T1 value)
        {
            parallelFor(size_t(0), M, [&](size_t i){
                for(auto &ij: container[i])
                {
                    ij *= value;
                }
            });
        }

        template <typename T1>
        void mul_self(Mat<T1, M, N> &m_)
        {
            parallelFor(size_t(0), M, [&](size_t i){
                parallelFor(size_t(0), N, [&](size_t j){
                    container[i][j] *= (T)m_[i][j];
                });
            });
        }

        template <typename T1>
        Mat<T, M, N> div(T1 value)
        {
            Mat<T, M, N> m;

            parallelFor(size_t(0), M, [&](size_t i){
                parallelFor(size_t(0), N, [&](size_t j){
                    m[i][j] = container[i][j] / T(value);
                });
            });

            return m;
        }

        template <typename T1>
        Mat<T, M, N> div(Mat<T1, M, N> &m_)
        {
            Mat<T, M, N> m;
            parallelFor(size_t(0), M, [&](size_t i){
                parallelFor(size_t(0), N, [&](size_t j){
                    m[i][j] = container[i][j] / T(m_[i][j]);
                });
            });

            return m;
        }   

        template <typename T1>
        void div_self(T1 value)
        {
            parallelFor(size_t(0), M, [&](size_t i){
                for(auto &ij: container[i])
                {
                    ij /= value;
                }
            });
        }

        template <typename T1>
        void div_self(Mat<T1, M, N> &m_)
        {
            parallelFor(size_t(0), M, [&](size_t i){
                parallelFor(size_t(0), N, [&](size_t j){
                    container[i][j] /= (T)m_[i][j];
                });
            });
        }


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
        
		std::vector<T>& operator[] (size_t i)
		{
			return container[i];
		}

        template <typename T1>
        Mat<T, M, N>& operator=(T1 value)
        {
            set(value);
            return *this;
        }

        template <typename T1>
        Mat<T, M, N>& operator=(const Mat<T1, M, N>& m_)
        {
            set(m_);
            return *this;
        }

        template <typename T1>
        Mat<T, M, N> operator+(T1 value)
        {
            return add(value);
        }

        template <typename T1>
        Mat<T, M, N> operator+(Mat<T1, M, N> &m_)
        {
            return add(m_);
        }

        template <typename T1>
        Mat<T, M, N> operator-(T1 value)
        {
            return sub(value);
        }

        template <typename T1>
        Mat<T, M, N> operator-(Mat<T1, M, N> &m_)
        {
            return sub(m_);
        }

        template <typename T1>
        Mat<T, M, N> operator*(T1 value)
        {
            return mul(value);
        }

        template <typename T1>
        Mat<T, M, N> operator*(Mat<T1, M, N> &m_)
        {
            return mul(m_);
        }

        template <typename T1>
        Mat<T, M, N> operator/(T1 value)
        {
            return div(value);
        }

        template <typename T1>
        Mat<T, M, N> operator/(Mat<T1, M, N> &m_)
        {
            return div(m_);
        }

        template <typename T1>
        Mat<T, M, N>& operator+=(T1 value)
        {
            add_self(value);
            return *this;
        }

        template <typename T1>
        Mat<T, M, N>& operator+=(Mat<T1, M, N> &m_)
        {
            add_self(m_);
            return *this;
        }

        template <typename T1>
        Mat<T, M, N>& operator-=(T1 value)
        {
            sub_self(value);
            return *this;
        }

        template <typename T1>
        Mat<T, M, N>& operator-=(Mat<T1, M, N> &m_)
        {
            sub_self(m_);
            return *this;
        }

        template <typename T1>
        Mat<T, M, N>& operator*=(T1 value)
        {
            mul_self(value);
            return *this;
        }

        template <typename T1>
        Mat<T, M, N>& operator*=(Mat<T1, M, N> &m_)
        {
            mul_self(m_);
            return *this;
        }

        template <typename T1>
        Mat<T, M, N>& operator/=(T1 value)
        {
            div_self(value);
            return *this;
        }        

        template <typename T1>
        Mat<T, M, N>& operator/=(Mat<T1, M, N> &m_)
        {
            div_self(m_);
            return *this;
        }  

    };

    template <typename T1, size_t M1, size_t N1>
    std::ostream &operator<<(std::ostream &os, Mat<T1, M1, N1> &m_)
    {
        for (size_t i = 0; i < M1; i++)
        {
            for (size_t j = 0; j < N1; j++)
            {
                os << m_[i][j] << " ";
            }
            os << std::endl;
        }
        os << std::endl;
        return os;
    }

    typedef Mat<float, 2, 2> Mat22f;
    typedef Mat<double, 2, 2> Mat22d;

	typedef Mat<float, 3, 3> Mat33f;
    typedef Mat<double, 3, 3> Mat33d;

	typedef Mat<float, 4, 4> Mat44f;
    typedef Mat<double, 4, 4> Mat44d;

} // namespace HSim
