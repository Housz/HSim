#pragma once

// readOBJ(trimesh, ".obj")
#include <config/numerical_config.h>
#include <HSim/triangle_mesh3.h>

namespace HSim
{   
    void readOBJtoTriangleMesh(TriangleMesh3_Ptr<PRECISION> triMesh, const std::string &filename);

} // namespace HSim
