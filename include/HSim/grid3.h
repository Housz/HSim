#pragma once

#include <HSim/common.h>
#include <HSim/space_object3.h>
#include <HSim/vec3.h>
#include <HSim/aabb3.h>
#include <HSim/parallel.h>
#include <HSim/math_utils.h>

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
        Grid3() 
        {
            spaceObjectType = SpaceObjectType::GRID;
        }

        Grid3(const Grid3<T>& grid3_) 
            : gridResolution(grid3_.gridResolution),
              gridOrigin(grid3_.gridOrigin),
              gridSpacing(grid3_.gridSpacing)
        {
            spaceObjectType = SpaceObjectType::GRID;

            aabb = std::make_shared<AABB3<T>>();
            computeAABB();
        }

        /**
         * @brief Construct a new Grid3 object by a grid resolution.
         *
         * @param x resolution.x
         * @param y resolution.y
         * @param z resolution.z
         */
        Grid3(size_t x, size_t y, size_t z)
        {
            spaceObjectType = SpaceObjectType::GRID;

            gridResolution = {x, y, z};
            computeAABB();
        }
        // Grid3(const Size3& resolution)
        // {
        //     spaceObjectType = SpaceObjectType::GRID;

        //     gridResolution = resolution;
        //     computeAABB();
        // }

        /**
         * @brief Construct a new Grid3 object by gird resolution, grid origin and grid spacing.
         *
         * @param resolution
         * @param origin
         * @param gridSpacing
         */
        Grid3(const Size3& resolution, const Vec3<T>& origin = {0, 0, 0}, const Vec3<T>& gridSpacing = {1, 1, 1}) 
            : gridResolution(resolution), gridOrigin(origin), gridSpacing(gridSpacing)
        {
            spaceObjectType = SpaceObjectType::GRID;

            gridResolution = resolution;
            
            computeAABB();
        }

        ~Grid3() {}

    public:
        /**
         * @brief gird resolution
         *
         * @return Size3
         */
        // Size3 gridResolution() { return gridResolution; }
        void setGridResolution(const Size3& r) { gridResolution = r; }
        size_t sizeX() const { return gridResolution.x; }
        size_t sizeY() const { return gridResolution.y; }
        size_t sizeZ() const { return gridResolution.z; }

        /**
         * @brief grid origin, the position (x, y, z) of the grid origin
         *
         * @return Vec3<T>
         */
        // Vec3<T> gridOrigin() { return gridOrigin; }
        void setGridOrigin(const Vec3<T>& origin) { gridOrigin = origin; }

        /**
         * @brief The spacing of the grid in the x, y and z directions
         *
         * @return Vec3<T>
         */
        // Vec3<T> gridSpacing() { return gridSpacing; }
        void setGridSpacing(const Vec3<T>& gs) { gridSpacing = gs; }
        T deltaX() const { return gridSpacing.x; }
        T deltaY() const { return gridSpacing.y; }
        T deltaZ() const { return gridSpacing.z; }

        void forEachCell(const std::function<void(size_t, size_t, size_t)> &func)
        {
            for (size_t k = 0; k < gridResolution.z; k++)
            {
                for (size_t j = 0; j < gridResolution.y; j++)
                {
                    for (size_t i = 0; i < gridResolution.x; i++)
                    {
                        func(i, j, k);
                    }
                }
            }
        }

        void parallelForEachCell(const std::function<void(size_t, size_t, size_t)> &func)
        {
            tbb::parallel_for(tbb::blocked_range3d<size_t>(0, gridResolution.x, 0, gridResolution.y, 0, gridResolution.z),
                              [&](tbb::blocked_range3d<size_t> r)
                              {
                                  for (size_t i = r.pages().begin(); i < r.pages().end(); i++)
                                  {
                                      for (size_t j = r.rows().begin(); j < r.rows().end(); j++)
                                      {
                                          for (size_t k = r.cols().begin(); k < r.cols().end(); k++)
                                          {
                                              func(i, j, k);
                                          }
                                      }
                                  }
                              });
        }

    // data
    public:
        Size3 gridResolution;
        Vec3<T> gridOrigin = Vec3<T>(0, 0, 0);
        Vec3<T> gridSpacing = Vec3<T>(1, 1, 1);

        AABB3_Ptr<T> aabb = std::make_shared<AABB3<T>>();

        void computeAABB()
        {
            // std::cout << "aabb:\n";
            // std::cout << gridOrigin;
            // std::cout << gridOrigin + gridResolution * gridSpacing;

            aabb->setLowerCorner(gridOrigin);
            aabb->setUpperCorner(gridOrigin + gridResolution * gridSpacing);
        }

        // for rendering
    public:
        virtual void drawBoundary() {}
        virtual void drawGrid() {}
        virtual void drawData() {}
        bool updated = false;
    };

} // namespace HSim
