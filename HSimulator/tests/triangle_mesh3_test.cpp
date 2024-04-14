#include <iostream>

#include <app/app.h>
#include <scene/scene_graph.h>
#include <HSim/box3.h>
#include <HSim/sphere3.h>
#include <HSim/cell_center_scalar_grid3.h>
#include <HSim/triangle_mesh3.h>

HSim::SceneGraph_ptr createScene()
{

	auto scene = std::make_shared<HSim::SceneGraph>();

	auto root = std::make_shared<HSim::GameObject>();
	scene->root = root;

	auto mesh = std::make_shared<HSim::TriangleMesh3f>();
	mesh->readOBJ("spot_triangulated.obj");
	// mesh->readOBJ("cube.obj");
	// mesh->readOBJ("bunny.obj");
	// mesh->readOBJ("grid.obj");

	// std::default_random_engine generator;
	// std::uniform_int_distribution<int> distribution(-2, 2);
	// for (size_t i = 0; i < 9; i++)
	// {
	// 	// HSim::Vec3f offset = {distribution(generator), distribution(generator), distribution(generator)};
	// 	// HSim::Vec3f offset = {10, 1, 1};

	// 	auto offset = distribution(generator);

	// 	std::cout << offset;

	// 	HSim::Triangle3f tri;
	// 	tri.points[0] = {1+i, 0+i%2, 0+i%4};
	// 	tri.points[1] = {0+i, 1+i%3, 0+i%3};
	// 	tri.points[2] = {0+i, 0+i%4, 1+i%2};

	// 	HSim::Vec3 triNormal = {1, 1, 1};
	// 	triNormal.normalize();
	// 	tri.normals[0] = triNormal;
	// 	tri.normals[1] = triNormal;
	// 	tri.normals[2] = triNormal;

	// 	mesh->addTriangle(tri);
	// }

	mesh->buildBVH();

	auto go1 = std::make_shared<HSim::GameObject>();
	go1->setSurface(mesh);
	go1->enableDraw();

	root->addChild(go1);

	HSim::Vec3f target(0.2, 0.5, 0);
	// HSim::Vec3f target(0, 1, 0);

	auto p = mesh->closestPositionLocal(target);
	std::cout << "numTrianlges: " << mesh->numTrianlges() << "\n";
	std::cout << p;

	auto sphere1 = std::make_shared<HSim::Sphere3<float>>();
	sphere1->center = target;
	sphere1->radius = 0.01;
	auto sphere2 = std::make_shared<HSim::Sphere3<float>>();
	sphere2->center = p;
	sphere2->radius = 0.01;

	auto goTarget = std::make_shared<HSim::GameObject>();
	goTarget->setSurface(sphere1);
	auto goClosest = std::make_shared<HSim::GameObject>();
	goClosest->setSurface(sphere2);

	root->addChild(goTarget);
	root->addChild(goClosest);
	goTarget->enableDraw();
	goClosest->enableDraw();

	return scene;
}

int main()
{
	HSim::App app;

	app.init(1920, 1080);

	app.setScene(createScene());

	app.mainLoop();

	return 0;
}
