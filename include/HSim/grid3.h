#pragma once

#include <HSim/common.h>
#include <HSim/space_object3.h>
#include <HSim/vec3.h>
#include <HSim/aabb3.h>
#include <HSim/parallel.h>

namespace HSim
{
    /**
     * @brief 3D cartesian grid base class
     *
     * @tparam T: float or double
     */
    template <typename T>
    class Grid3 : public SpaceObject3<T>
    {
    public:
        Grid3() {}
        ~Grid3() {}

        /**
         * @brief Construct a new Grid3 object by a grid resolution.
         *
         * @param x resolution.x
         * @param y resolution.y
         * @param z resolution.z
         */
        Grid3(size_t x, size_t y, size_t z)
        {
            _gridResolution = {x, y, z};
            computeAABB();
        }
        /**
         * @brief Construct a new Grid3 object by gird resolution, grid origin and grid spacing.
         *
         * @param resolution
         * @param origin
         * @param gridSpacing
         */
        Grid3(Vec3i resolution, Vec3<T> origin = {0, 0, 0}, Vec3<T> gridSpacing = {1, 1, 1}) : _gridResolution(resolution), _girdOrigin(origin), _gridSpacing(gridSpacing)
        {
            computeAABB();
        }

    public:
        /**
         * @brief gird resolution
         *
         * @return Vec3i
         */
        Vec3i gridResolution() { return _gridResolution; }
        void setGridResolution(Vec3i r) { _gridResolution = r; }
        size_t sizeX() { return _gridResolution.x; }
        size_t sizeY() { return _gridResolution.y; }
        size_t sizeZ() { return _gridResolution.z; }

        /**
         * @brief grid origin, the position (x, y, z) of the grid origin
         *
         * @return Vec3<T>
         */
        Vec3<T> gridOrigin() { return _girdOrigin; }
        void setGridOrigin(Vec3<T> origin) { _girdOrigin = origin; }

        /**
         * @brief The spacing of the grid in the x, y and z directions
         *
         * @return Vec3<T>
         */
        Vec3<T> gridSpacing() { return _gridSpacing; }
        void setGridSpacing(Vec3<T> gs) { _gridSpacing = gs; }
        T deltaX() { return _gridSpacing.x; }
        T deltaY() { return _gridSpacing.y; }
        T deltaZ() { return _gridSpacing.z; }

        void forEachCell(const std::function<void(size_t, size_t, size_t)> &func)
        {
            for (size_t k = 0; k < _gridResolution.z; k++)
            {
                for (size_t j = 0; j < _gridResolution.y; j++)
                {
                    for (size_t i = 0; i < _gridResolution.x; i++)
                    {
                        func(i, j, k);
                    }
                }
            }
        }

        void parallelForEachCell(const std::function<void(size_t, size_t, size_t)> &func)
        {
            tbb::parallel_for(tbb::blocked_range3d<size_t>(0, _gridResolution.x, 0, _gridResolution.y, 0, _gridResolution.z),
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
        Vec3i _gridResolution;
        Vec3<T> _girdOrigin = Vec3<T>(0, 0, 0);
        Vec3<T> _gridSpacing = Vec3<T>(1, 1, 1);

        AABB3_Ptr<T> aabb = std::make_shared<AABB3<T>>();

        void computeAABB()
        {
            std::cout << "aabb:\n";
            std::cout << _girdOrigin;
            std::cout << _girdOrigin + _gridResolution * _gridSpacing;

            aabb->setLowerCorner(_girdOrigin);
            aabb->setUpperCorner(_girdOrigin + _gridResolution * _gridSpacing);
        }

        // for rendering
    public:
        virtual void drawBoundary() {}
        virtual void drawGrid() {}
        virtual void drawData() {}
        bool updated = false;
    };

} // namespace HSim
