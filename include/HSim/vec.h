#pragma once

#include <HSim/common.h>
#include <HSim/parallel.h>

// todo: warp of std::vector, add some funcs

namespace HSim
{

    /**
     * @brief n-D vector
     *
     * @tparam T : element type
     */
    template <typename T>
    class Vec
    {
    public:
        std::vector<T> container;
        typename std::vector<T>::iterator begin() { return container.begin(); }
        typename std::vector<T>::iterator end() { return container.end(); }

        template <typename T1>
        friend std::ostream &operator<<(std::ostream &, Vec<T1> &);

        Vec() : {}
        Vec(size_t n) : container(n, 0) {}
        Vec(size_t n, T value) : container(n, value) {}
        template <typename T1>
        Vec(Vec<T1> &v_) : container(v_.size(), 0) { set(v_); }
        template <typename T1>
        Vec(const std::initializer_list<T1> &list) { set(list); }

        size_t size() { return container.size(); }

        template <typename T1>
        void set(const std::initializer_list<T1> &list) { container = list; }

        void set(T value)
        {
            // HSim::parallelFill(begin(), end(), value);
            std::fill(begin(), end(), value);
        }

        template <typename T1>
        void set(Vec<T1> &v_)
        {
            assert(v_.size() == size());

            HSim::parallelFor(size_t(0), size(), [&](size_t i)
                              { container[i] = T(v_[i]); });
        }

        void setZero()
        {
            set(T(0));
        }

        void push_back(T value)
        {
            container.push_back(value);
        }

        void swap(Vec<T> &v_)
        {
            std::swap(v_.container, container);
        }

        template <typename T1>
        T dot(Vec<T1> &v_)
        {
            assert(v_.size() == size());
            T sum = parallelReduce(
                begin(), end(),

                T(0),

                [&](tbb::blocked_range<size_t> r, T local_sum)
                {
                    for (size_t i = r.begin(); i < r.end(); i++)
                    {
                        local_sum += container[i] * v_[i];
                    }
                    return local_sum;
                },

                std::plus<T>());

            return sum;
        }

        T *data() { return container.data(); }

        T sum()
        {
            T sum = parallelReduce(
                begin(), end(),

                T(0),

                [&](tbb::blocked_range<size_t> r, T local_sum)
                {
                    for (size_t i = r.begin(); i < r.end(); i++)
                    {
                        local_sum += container[i];
                    }
                    return local_sum;
                },

                std::plus<T>());

            return sum;
        }

        double avg()
        {
            return sum() / size();
        }

        T min()
        {
            T min = parallelReduce(
                begin(), end(),

                T(0),

                [&](tbb::blocked_range<size_t> r, T local_min)
                {
                    local_min = container[r.begin()];
                    for (size_t i = r.begin() + 1; i < r.end(); i++)
                    {
                        if(container[i] < local_min)
                            local_min = container[i];
                    }
                    return local_min;
                },

                std::min<T>()
                
                );

            return min;
        }

        T max()
        {
        }

        template <typename T1>
        Vec<T> add(const Vec<T1> v_)
        {
        }

        template <typename T1>
        void add_self(const Vec<T1> v_)
        {
        }

        template <typename T1>
        Vec<T> sub(const Vec<T1> v_)
        {
        }

        template <typename T1>
        void sub_self(const Vec<T1> v_)
        {
        }

        template <typename T1>
        Vec<T> mul(const Vec<T1> v_)
        {
        }

        template <typename T1>
        void mul_self(const Vec<T1> v_)
        {
        }

        template <typename T1>
        Vec<T> div(const Vec<T1> v_)
        {
        }

        template <typename T1>
        void div_self(const Vec<T1> v_)
        {
        }

        T &operator[](size_t i)
        {
            return container[i];
        }

        template <typename U>
        Vec<T> &operator=(const std::initializer_list<U> &list)
        {
        }

        Vec<T> &operator=(Vec<T> &v_)
        {
        }

        template <typename T1>
        Vec<T> &operator+=(T1 value)
        {
        }

        template <typename T1>
        Vec<T> &operator+=(Vec<T1> &v_)
        {
        }

        template <typename T1>
        Vec<T> &operator-=(T1 value)
        {
        }

        template <typename T1>
        Vec<T> &operator-=(Vec<T1> &v_)
        {
        }

        template <typename T1>
        Vec<T> &operator*=(T1 value)
        {
        }

        template <typename T1>
        Vec<T> &operator*=(Vec<T1> &v_)
        {
        }

        template <typename T1>
        Vec<T> &operator/=(T1 value)
        {
        }

        template <typename T1>
        Vec<T> &operator/=(Vec<T1> &v_)
        {
        }

        template <typename T1>
        bool isEqual(Vec<T1> &v_)
        {
        }

        template <typename T1>
        bool operator==(Vec<T1> &v_)
        {
        }

        template <typename T1>
        bool operator!=(Vec<T1> &v_)
        {
        }

        template <typename Callback>
        void forEach(Callback func)
        {
        }

        template <typename Callback>
        void forEachParallel(Callback func)
        {
            HSim::parallelFor(size_t(0), size(), func);
        }
    };

    typedef Vec<int> Veci;
    typedef Vec<float> Vecf;
    typedef Vec<double> Vecd;

    template <typename T1>
    std::ostream &operator<<(std::ostream &os, Vec<T1> &v_)
    {
        for (auto &&i : v_)
        {
            os << i << " ";
        }
        os << std::endl;

        return os;
    }

} // namespace HSim
