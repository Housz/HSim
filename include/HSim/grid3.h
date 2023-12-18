#pragma once

#include <HSim/common.h>
#include <HSim/vec3.h>
#include <HSim/parallel.h>

namespace HSim
{
    /**
     * @brief 3D cartesian grid base class
     *
     * @tparam T: float or double
     */
    template <typename T>
    class Grid3
    {
    public:
        Grid3() {}
        ~Grid3() {}

        Grid3(size_t x, size_t y, size_t z) { _resolution = {x, y, z}; }
        Grid3(Vec3i resolution, Vec3<T> origin={0, 0, 0}, Vec3<T> gridSpacing={1, 1, 1}):
        _resolution(resolution), _origin(origin), _gridSpacing(gridSpacing)
        {}
        

    public:
        Vec3i resolution() { return _resolution; }
        void setResolution(Vec3i r) { _resolution = r; }
        size_t sizeX() { return _resolution.x; }
        size_t sizeY() { return _resolution.y; }
        size_t sizeZ() { return _resolution.z; }

        Vec3<T> origin() { return _origin; }
        void setOrigin(Vec3<T> origin) { _origin = origin; }

        Vec3<T> gridSpacing() { return _gridSpacing; }
        void setGridSpacing(Vec3<T> gs) { _gridSpacing = gs; }
        T deltaX() { return _gridSpacing.x; }
        T deltaY() { return _gridSpacing.y; }
        T deltaZ() { return _gridSpacing.z; }

        virtual void setData(std::vector<T> &sourceData) = 0;
        virtual void getData(std::vector<T> &targetData) = 0;

        void forEachCell(const std::function<void(size_t, size_t, size_t)> &func)
        {
            for (size_t k = 0; k < _resolution.z; k++)
            {
                for (size_t j = 0; j < _resolution.y; j++)
                {
                    for (size_t i = 0; i < _resolution.x; i++)
                    {
                        func(i, j, k);
                    }
                }
            }
        }

        void parallelForEachCell(const std::function<void(size_t, size_t, size_t)> &func)
        {
            tbb::parallel_for(tbb::blocked_range3d<size_t>(0, _resolution.x, 0, _resolution.y, 0, _resolution.z),
                              [&](tbb::blocked_range3d<size_t> r)
                              {
                                  for (size_t k = r.pages().begin(); k < r.pages().end(); k++)
                                  {
                                      for (size_t j = r.cols().begin(); j < r.cols().end(); j++)
                                      {
                                          for (size_t i = r.rows().begin(); i < r.rows().end(); i++)
                                          {
                                              func(i, j, k);
                                          }
                                      }
                                  }
                              });
        }

    public:
        Vec3i _resolution;
        Vec3<T> _origin = Vec3<T>(0, 0, 0);
        Vec3<T> _gridSpacing = Vec3<T>(1, 1, 1);
    };

} // namespace HSim
