#include <HSim/triangle_mesh3.h>

#include <HSim/triangle3.h>

// #define TRIANGLE3

#define TRIMESH

int main()
{
	// test triangle3

#ifdef TRIANGLE3
	HSim::Triangle3<float> tri;

	tri.points[0] = {0, 0, 0};
	tri.points[1] = {1, 0, 0};
	tri.points[2] = {0, 1., 0};

	tri.normals[0] = {0, 0, 1};
	tri.normals[1] = {0, 0, 1};
	tri.normals[2] = {0, 0, 1};

	// std::cout << tri.area() << std::endl;
	// std::cout << tri.faceNormal();

	HSim::Vec3f p_local({.5, 0, 0});
	// std::cout << tri.closestDistanceLocal(p_local);
	// std::cout << tri.closestNormalLocal(p_local);
	// std::cout << tri.barycentricCoords(p_local);

	// std::cout << tri.AABBLocal().lowerCorner;
	// std::cout << tri.AABBLocal().upperCorner;

	// intersection
	HSim::Ray3f ray({0.5, 0.5, 1.}, {0, 0, -1});
	// HSim::Ray3f ray({0.5, 0.5, 1.}, {1, 0, 0});
	auto intersectionInfo = tri.interactLocal(ray);
	std::cout << intersectionInfo;
#endif

#ifdef TRIMESH

	HSim::TriangleMesh3f mesh;

	// mesh.readOBJ("spot_triangulated.obj");
	mesh.readOBJ("cube.obj");

	
	/**
	 * test show
	 */
	std::cout << mesh.points[0];

	// std::cout << mesh.pointIndices[0];
	// std::cout << mesh.uvIndices[0];
	// std::cout << mesh.points[mesh.pointIndices[0][0]];
	// std::cout << mesh.points[mesh.pointIndices[0][1]];
	// std::cout << mesh.points[mesh.pointIndices[0][2]];
	// std::cout << mesh.uvs[mesh.uvIndices[0][0]];
	// std::cout << mesh.uvs[mesh.uvIndices[0][1]];
	// std::cout << mesh.uvs[mesh.uvIndices[0][2]];

	/**
	 * test addTriangle
	 */
	// HSim::Triangle3<float> tri;

	// tri.points[0] = {0, 0, 0};
	// tri.points[1] = {1, 0, 0};
	// tri.points[2] = {0, 1, 0};

	// tri.uvs[0] = {0, 0};
	// tri.uvs[1] = {1, 0};
	// tri.uvs[2] = {0, 1};

	// tri.normals[0] = {0, 0, 1};
	// tri.normals[1] = {0, 0, 1};
	// tri.normals[2] = {0, 0, 1};

	// mesh.addTriangle(tri);

	/**
	 * show the last triangle abc
	 */
	// std::cout << std::endl;
	// std::cout << mesh.pointIndices[mesh.pointIndices.size()-1];
	// std::cout << mesh.uvIndices[mesh.pointIndices.size()-1];
	// std::cout << mesh.points[mesh.pointIndices[mesh.pointIndices.size()-1][0]];
	// std::cout << mesh.points[mesh.pointIndices[mesh.pointIndices.size()-1][1]];
	// std::cout << mesh.points[mesh.pointIndices[mesh.pointIndices.size()-1][2]];
	// std::cout << mesh.uvs[mesh.uvIndices[mesh.pointIndices.size()-1][0]];
	// std::cout << mesh.uvs[mesh.uvIndices[mesh.pointIndices.size()-1][1]];
	// std::cout << mesh.uvs[mesh.uvIndices[mesh.pointIndices.size()-1][2]];
	// std::cout << std::endl;

	// auto flattenedPoints = mesh.flatten(mesh.points);
	// std::cout << flattenedPoints[0] << std::endl;
	// auto flattenedPointIndices = mesh.flatten(mesh.pointIndices);
	// std::cout << flattenedPointIndices[0] << std::endl;
	
	
	/**
	 * test bvh
	 */
	mesh.buildBVH();
	// std::cout << mesh.bvh.rootAABB.lowerCorner;
	// std::cout << mesh.bvh.rootAABB.upperCorner;

#endif

	return 0;
}