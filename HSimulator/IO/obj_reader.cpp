#include <IO/obj_reader.h>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

void HSim::readOBJtoTriangleMesh(TriangleMesh3_Ptr<PRECISION> triMesh, const std::string &filename)
{
    tinyobj::ObjReaderConfig reader_config;
    reader_config.mtl_search_path = "./";

    tinyobj::ObjReader reader;

    if (!reader.ParseFromFile(filename, reader_config))
    {
        if (!reader.Error().empty())
        {
            std::cerr << "TinyObjReader: " << reader.Error();
        }
        // return false;
    }

    if (!reader.Warning().empty())
    {
        std::cout << "TinyObjReader: " << reader.Warning();
        // return false;
    }

    auto &attrib = reader.GetAttrib();
    auto &shapes = reader.GetShapes();
    auto &materials = reader.GetMaterials();

    // Read vertices
    for (size_t i = 0; i < attrib.vertices.size(); i += 3)
    {
        auto vx = attrib.vertices[i + 0];
        auto vy = attrib.vertices[i + 1];
        auto vz = attrib.vertices[i + 2];

        triMesh->addPoint({vx, vy, vz});
    }

    // Read normals
    for (size_t i = 0; i < attrib.normals.size(); i += 3)
    {
        auto nx = attrib.normals[i + 0];
        auto ny = attrib.normals[i + 1];
        auto nz = attrib.normals[i + 2];

        triMesh->addNormal({nx, ny, nz});
    }

    // Read UVs
    for (size_t i = 0; i < attrib.texcoords.size(); i += 2)
    {
        auto u = attrib.texcoords[i + 0];
        auto v = attrib.texcoords[i + 1];

        triMesh->addUV({u, v});
    }

    // Read faces (triangles)
    for (auto &shape : shapes)
    {
        size_t idx = 0;

        for (size_t f = 0; f < shape.mesh.num_face_vertices.size(); ++f)
        {
            const size_t fv = shape.mesh.num_face_vertices[f];

            if (fv == 3)
            {
                if (!attrib.vertices.empty())
                {
                    triMesh->addTrianglePointIndices(
                        {shape.mesh.indices[idx].vertex_index,
                         shape.mesh.indices[idx + 1].vertex_index,
                         shape.mesh.indices[idx + 2].vertex_index});
                }

                if (!attrib.normals.empty())
                {
                    triMesh->addTriangleNormalIndices(
                        {shape.mesh.indices[idx].normal_index,
                         shape.mesh.indices[idx + 1].normal_index,
                         shape.mesh.indices[idx + 2].normal_index});
                }

                if (!attrib.texcoords.empty())
                {
                    triMesh->addTriangleUVIndices({shape.mesh.indices[idx].texcoord_index,
                                          shape.mesh.indices[idx + 1].texcoord_index,
                                          shape.mesh.indices[idx + 2].texcoord_index});
                }
            } // if (fv == 3)

            idx += fv;
        }
    } // Read faces (triangles)
}

void HSim::fun(const std::string &filename)
{
    std::cout << filename;
}