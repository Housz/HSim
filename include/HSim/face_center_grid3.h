#pragma once

#include <HSim/vector_grid3.h>
#include <HSim/array3.h>

namespace HSim
{
    template <typename T>
    class FaceCenterGrid3 : public VectorGrid3<T>
    {
    public:
        FaceCenterGrid3() {}
        ~FaceCenterGrid3() {}

        FaceCenterGrid3(size_t x, size_t y, size_t z)
            : VectorGrid3<T>(x, y, z)
        {
            _dataU.resize(x + 1, y, z);
            _dataV.resize(x, y + 1, z);
            _dataW.resize(x, y, z + 1);

            _dataOriginU = _girdOrigin + Vec3<T>(0, deltaY() / 2, deltaZ() / 2);
            _dataOriginV = _girdOrigin + Vec3<T>(deltaX() / 2, 0, deltaZ() / 2);
            _dataOriginW = _girdOrigin + Vec3<T>(deltaX() / 2, deltaY() / 2, 0);
        }
        FaceCenterGrid3(Vec3i resolution, Vec3<T> origin = {0, 0, 0}, Vec3<T> gridSpacing = {1, 1, 1})
            : VectorGrid3<T>(resolution, origin, gridSpacing)
        {
        }

        // setter getter
    public:
        Array3<T> &dataU() { return _dataU; }
        Array3<T> &dataV() { return _dataV; }
        Array3<T> &dataW() { return _dataW; }

        Vec3i dataSizeU() { return _dataU.size(); }
        Vec3i dataSizeV() { return _dataV.size(); }
        Vec3i dataSizeW() { return _dataW.size(); }

        Vec3<T> &dataOriginU() { return _dataOriginU; }
        Vec3<T> &dataOriginV() { return _dataOriginV; }
        Vec3<T> &dataOriginW() { return _dataOriginW; }

        T &u(size_t i, size_t j, size_t k) { return _dataU(i, j, k); }
        T &v(size_t i, size_t j, size_t k) { return _dataV(i, j, k); }
        T &w(size_t i, size_t j, size_t k) { return _dataW(i, j, k); }

        // cell center data by interpolation
        Vec3<T> dataAtCellCenter(size_t, size_t, size_t);
        Vec3<T> dataAtCellCenter(Vec3i);

        T divergenceAtCellCenter(size_t, size_t, size_t);
        T divergenceAtCellCenter(Vec3i);
        T divergenceAt(T, T, T);
        T divergenceAt(Vec3<T>);

        // Vec3<T> curlAtCellCenter(size_t, size_t, size_t);
        // Vec3<T> curlAtCellCenter(Vec3i);
        // Vec3<T> curlAt(T, T, T);
        // Vec3<T> curlAt(Vec3<T>);

        // data
    public:
        Array3<T> _dataU;
        Array3<T> _dataV;
        Array3<T> _dataW;

        Vec3<T> _dataOriginU;
        Vec3<T> _dataOriginV;
        Vec3<T> _dataOriginW;
    };

    /**
     * @brief data at center of cell (i,j,k)
     *
     * data.x = [ u(i, j, k) + u(i+1, j, k) ] / 2
     *
     */
    template <typename T>
    Vec3<T> FaceCenterGrid3<T>::dataAtCellCenter(size_t i, size_t j, size_t k)
    {
        assert(i < sizeX() && j < sizeY() && k < sizeZ());

        Vec3<T> data;
        data.x = 0.5 * (u(i, j, k) + u(i + 1, j, k));
        data.y = 0.5 * (v(i, j, k) + v(i, j + 1, k));
        data.z = 0.5 * (w(i, j, k) + w(i, j, k + 1));
        return data;
    }
    template <typename T>
    Vec3<T> FaceCenterGrid3<T>::dataAtCellCenter(Vec3i position)
    {
        return dataAtCellCenter(position.x, position.y, position.z);
    }

    /**
     * @brief divergence at center of cell (i,j,k)
     *
     * divergence f = df/dx + df/dy + df/dz
     * df/dx = u(i+1,j,k)-u(i,j,k) / deltaX
     */
    template <typename T>
    T FaceCenterGrid3<T>::divergenceAtCellCenter(size_t i, size_t j, size_t k)
    {
        assert(i < sizeX() && j < sizeY() && k < sizeZ());

        T ULeft = u(i, j, k);
        T URight = u(i + 1, j, k);
        T VBottom = v(i, j, k);
        T VTop = v(i, j + 1, k);
        T WBack = w(i, j, k);
        T WFront = w(i, j, k + 1);

        T divergence = (URight - ULeft) / deltaX() +
                       (VTop - VBottom) / deltaY() +
                       (WFront - WBack) / deltaZ();
        return divergence;
    }
    template <typename T>
    T FaceCenterGrid3<T>::divergenceAtCellCenter(Vec3i position)
    {
        return divergenceAtCellCenter(position.x, position.y, position.z);
    }

} // namespace HSim
