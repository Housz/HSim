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

        FaceCenterGrid3(const FaceCenterGrid3<T> &faceCenterGrid3_)
            : VectorGrid3(faceCenterGrid3_),
              _dataU(faceCenterGrid3_._dataU),
              _dataV(faceCenterGrid3_._dataV),
              _dataW(faceCenterGrid3_._dataW),
              _dataOriginU(faceCenterGrid3_._dataOriginU),
              _dataOriginV(faceCenterGrid3_._dataOriginV),
              _dataOriginW(faceCenterGrid3_._dataOriginW)
        {
        }

        FaceCenterGrid3(size_t x, size_t y, size_t z)
            : VectorGrid3<T>(x, y, z)
        {
            _dataU.resize(x + 1, y, z);
            _dataV.resize(x, y + 1, z);
            _dataW.resize(x, y, z + 1);

            _dataOriginU = gridOrigin + Vec3<T>(0, deltaY() / 2, deltaZ() / 2);
            _dataOriginV = gridOrigin + Vec3<T>(deltaX() / 2, 0, deltaZ() / 2);
            _dataOriginW = gridOrigin + Vec3<T>(deltaX() / 2, deltaY() / 2, 0);
        }

        // FaceCenterGrid3(const Size3& resolution)
        //     : VectorGrid3<T>(resolution)
        // {
        //     _dataU.resize(resolution.x + 1, resolution.y, resolution.z);
        //     _dataV.resize(resolution.x, resolution.y + 1, resolution.z);
        //     _dataW.resize(resolution.x, resolution.y, resolution.z + 1);

        //     _dataOriginU = gridOrigin + Vec3<T>(0, deltaY() / 2, deltaZ() / 2);
        //     _dataOriginV = gridOrigin + Vec3<T>(deltaX() / 2, 0, deltaZ() / 2);
        //     _dataOriginW = gridOrigin + Vec3<T>(deltaX() / 2, deltaY() / 2, 0);
        // }



        FaceCenterGrid3(const Size3& resolution, const Vec3<T>& origin = {0, 0, 0}, const Vec3<T>& gridSpacing = {1, 1, 1})
            : VectorGrid3<T>(resolution, origin, gridSpacing)
        {
            _dataU.resize(resolution.x + 1, resolution.y, resolution.z);
            _dataV.resize(resolution.x, resolution.y + 1, resolution.z);
            _dataW.resize(resolution.x, resolution.y, resolution.z + 1);

            _dataOriginU = gridOrigin + Vec3<T>(0, deltaY() / 2, deltaZ() / 2);
            _dataOriginV = gridOrigin + Vec3<T>(deltaX() / 2, 0, deltaZ() / 2);
            _dataOriginW = gridOrigin + Vec3<T>(deltaX() / 2, deltaY() / 2, 0);
        }

        ~FaceCenterGrid3() {}

        void clone(std::shared_ptr<SpaceObject3<T>> &ptr) override
        {
            ptr = std::make_shared<FaceCenterGrid3<T>>(*this);
        }

        // setter getter
    public:
        Array3<T> &dataU() { return _dataU; }
        Array3<T> &dataV() { return _dataV; }
        Array3<T> &dataW() { return _dataW; }

        Vec3i dataSizeU() const { return _dataU.size; }
        Vec3i dataSizeV() const { return _dataV.size; }
        Vec3i dataSizeW() const { return _dataW.size; }

        Vec3<T> &dataOriginU() { return _dataOriginU; }
        Vec3<T> &dataOriginV() { return _dataOriginV; }
        Vec3<T> &dataOriginW() { return _dataOriginW; }

        T &u(size_t i, size_t j, size_t k) { return _dataU(i, j, k); }
        T &v(size_t i, size_t j, size_t k) { return _dataV(i, j, k); }
        T &w(size_t i, size_t j, size_t k) { return _dataW(i, j, k); }

        T u(size_t i, size_t j, size_t k) const { return _dataU(i, j, k); }
        T v(size_t i, size_t j, size_t k) const { return _dataV(i, j, k); }
        T w(size_t i, size_t j, size_t k) const { return _dataW(i, j, k); }

        // cell center data by index i, j, k
        Vec3<T> dataAtCellCenter(size_t i, size_t j, size_t k) const;
        Vec3<T> dataAtCellCenter(Vec3i) const;

        Vec3<T> dataAtFaceU(size_t i, size_t j, size_t k) const;
        Vec3<T> dataAtFaceV(size_t i, size_t j, size_t k) const;
        Vec3<T> dataAtFaceW(size_t i, size_t j, size_t k) const;

        Vec3<T> positionAt(size_t i, size_t j, size_t k) const;

        Vec3<T> positionAtFaceU(size_t i, size_t j, size_t k) const;
        Vec3<T> positionAtFaceV(size_t i, size_t j, size_t k) const;
        Vec3<T> positionAtFaceW(size_t i, size_t j, size_t k) const;


        Vec3<T> sample(const Vec3<T> &p) const;

        // divergence at index i, j, k
        T divergenceAtCellCenter(size_t, size_t, size_t);
        T divergenceAtCellCenter(Vec3i);
        // T divergenceAt(T, T, T);
        // T divergenceAt(Vec3<T>);

        // Vec3<T> curlAtCellCenter(size_t, size_t, size_t);
        // Vec3<T> curlAtCellCenter(Vec3i);
        // Vec3<T> curlAt(T, T, T);
        // Vec3<T> curlAt(Vec3<T>);

        void swap(FaceCenterGrid3& other)
        {
            _dataU.swap(other._dataU);
            _dataV.swap(other._dataV);
            _dataW.swap(other._dataW);
        }



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
    Vec3<T> FaceCenterGrid3<T>::dataAtCellCenter(size_t i, size_t j, size_t k) const
    {
        assert(i < sizeX() && j < sizeY() && k < sizeZ());

        Vec3<T> data;
        data.x = 0.5 * (u(i, j, k) + u(i + 1, j, k));
        data.y = 0.5 * (v(i, j, k) + v(i, j + 1, k));
        data.z = 0.5 * (w(i, j, k) + w(i, j, k + 1));
        return data;
    }
    template <typename T>
    Vec3<T> FaceCenterGrid3<T>::dataAtCellCenter(Vec3i position) const
    {
        return dataAtCellCenter(position.x, position.y, position.z);
    }

    template <typename T>
    Vec3<T> FaceCenterGrid3<T>::dataAtFaceU(size_t i, size_t j, size_t k) const
    {
        // todo
        return Vec3<T>();
    }

    template <typename T>
    Vec3<T> FaceCenterGrid3<T>::dataAtFaceV(size_t i, size_t j, size_t k) const
    {
        // todo
        return Vec3<T>();
    }

    template <typename T>
    Vec3<T> FaceCenterGrid3<T>::dataAtFaceW(size_t i, size_t j, size_t k) const
    {
        // todo
        return Vec3<T>();
    }

    template <typename T>
    Vec3<T> FaceCenterGrid3<T>::positionAt(size_t i, size_t j, size_t k) const
    {
        return gridOrigin + gridSpacing * 0.5 + Vec3<T>(i * gridSpacing.x, j * gridSpacing.y, k * gridSpacing.z);
    }

    template <typename T>
    Vec3<T> FaceCenterGrid3<T>::positionAtFaceU(size_t i, size_t j, size_t k) const
    {
        return _dataOriginU + Vec3<T>(i * gridSpacing.x, j * gridSpacing.y, k * gridSpacing.z);
    }

    template <typename T>
    Vec3<T> FaceCenterGrid3<T>::positionAtFaceV(size_t i, size_t j, size_t k) const
    {
        return _dataOriginV + Vec3<T>(i * gridSpacing.x, j * gridSpacing.y, k * gridSpacing.z);
    }

    template <typename T>
    Vec3<T> FaceCenterGrid3<T>::positionAtFaceW(size_t i, size_t j, size_t k) const
    {
        return _dataOriginW + Vec3<T>(i * gridSpacing.x, j * gridSpacing.y, k * gridSpacing.z);
    }

    template <typename T>
    Vec3<T> FaceCenterGrid3<T>::sample(const Vec3<T> &p) const
    {
        size_t i = (size_t)(p.x / deltaX());
        size_t j = (size_t)(p.y / deltaY());
        size_t k = (size_t)(p.z / deltaZ());

        auto positionAtVertex = gridOrigin + Vec3<T>(i, j, k) * gridSpacing;
        auto t = p - positionAtVertex;

        Vec3<T> result = {
            lerp(u(i, j, k), u(i + 1, j, k), t.x),
            lerp(v(i, j, k), v(i, j + 1, k), t.y),
            lerp(w(i, j, k), w(i, j, k + 1), t.z)};

        return result;
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

    template <typename T>
    using FaceCenterGrid3_Ptr = std::shared_ptr<FaceCenterGrid3<T>>;

    template <typename T>
    using MACGrid3 = FaceCenterGrid3<T>;
    template <typename T>
    using MACGrid3_Ptr = std::shared_ptr<MACGrid3<T>>;

    template <typename T>
    using StaggeredGrid3 = FaceCenterGrid3<T>;
    template <typename T>
    using StaggeredGrid3_Ptr = std::shared_ptr<StaggeredGrid3<T>>;

} // namespace HSim
