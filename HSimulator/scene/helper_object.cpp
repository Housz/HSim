#include <scene/helper_object.h>

HSim::Helper::Helper()
{
}

HSim::Helper::~Helper()
{
}

HSim::GroundHelper::GroundHelper()
{
}

HSim::GroundHelper::GroundHelper(size_t slices_) : slices(slices_)
{
}

HSim::GroundHelper::~GroundHelper()
{
}

HSim::LineHelper::LineHelper()
{
}

HSim::LineHelper::LineHelper(const Vec3f &start_, const Vec3f &end_)
:start(start_), end(end_)
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
