#include <scene/helper_object.h>
#include "helper_object.h"

HSim::Helper::Helper()
{
    spaceObjectType = SpaceObjectType::HELPER;
}

HSim::Helper::~Helper()
{
}

HSim::GroundHelper::GroundHelper()
{
}

HSim::GroundHelper::GroundHelper(size_t slices_)
    : slices(slices_)
{
}

HSim::GroundHelper::GroundHelper(const GroundHelper &groundHelper_)
    : slices(groundHelper_.slices)
{
}

HSim::GroundHelper::~GroundHelper()
{
}

HSim::LineHelper::LineHelper()
{
}

HSim::LineHelper::LineHelper(const Vec3f &start_, const Vec3f &end_)
    : start(start_), end(end_)
{
}

HSim::LineHelper::LineHelper(const LineHelper &lineHelper_)
    : start(lineHelper_.start), end(lineHelper_.end)
{
}

HSim::LineHelper::~LineHelper()
{
}

// HSim::SphereHelper::SphereHelper()
// {
// }

// HSim::SphereHelper::SphereHelper(const Vec3f &center_, const float radius_)
// {
// }

// HSim::SphereHelper::~SphereHelper()
// {
// }

HSim::ArrowHelper::ArrowHelper()
{
}

HSim::ArrowHelper::ArrowHelper(const Vec3f &start_, const Vec3f &end_)
    : start(start_), end(end_)
{
}

HSim::ArrowHelper::ArrowHelper(const ArrowHelper &arrowHelper_)
    : start(arrowHelper_.start), end(arrowHelper_.end)
{
}

HSim::ArrowHelper::~ArrowHelper()
{
}
