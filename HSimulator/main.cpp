#include <iostream>

#include <app/app.h>
#include <scene/scene_graph.h>
#include <HSim/box3.h>
#include <HSim/sphere3.h>
#include <HSim/cell_center_scalar_grid3.h>
#include <HSim/triangle_mesh3.h>

HSim::SceneGraph_ptr createScene()
{
	// HSim::SceneGraph sg;
	auto sg = std::make_shared<HSim::SceneGraph>();

	auto root = std::make_shared<HSim::GameObject>();
	sg->root = root;

	// auto box1 = std::make_shared<HSim::Box3<float>>();
	HSim::Vec3<float> lower(1, 1, 1);
	HSim::Vec3<float> upper(5, 5, 5);

	auto sphere = std::make_shared<HSim::Sphere3<float>>();

	auto box2 = std::make_shared<HSim::Box3<float>>(lower, upper);
	// box1->transform.translation = {2, 0, 0};
	HSim::Vec3f axis(0, 0, 1);
	float angle = PI_QUARTER;
	HSim::Quaternionf orientation(axis, angle);
	// box1->transform.orientation = orientation;

	auto sphere1 = std::make_shared<HSim::Sphere3<float>>();

	size_t n = 10;
	auto grid = std::make_shared<HSim::CellCenterScalarGrid3<float>>(n, n, n);

	auto mesh = std::make_shared<HSim::TriangleMesh3f>();
	// mesh->readOBJ("spot_triangulated.obj");
	mesh->readOBJ("bunny.obj");
	// mesh->readOBJ("cube.obj");


	// std::default_random_engine generator;
	// std::uniform_int_distribution<int> distribution(-10, 10);
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
	auto go2 = std::make_shared<HSim::GameObject>();
	auto go3 = std::make_shared<HSim::GameObject>();
	auto go4 = std::make_shared<HSim::GameObject>();
	auto go5 = std::make_shared<HSim::GameObject>();

	go1->surface_ptr = sphere;
	go2->surface_ptr = sphere1;
	go3->surface_ptr = box2;
	go4->grid_ptr = grid;
	go5->surface_ptr = mesh;

	root->drawable = false;

	go1->drawable = false;
	go2->drawable = false;
	go3->drawable = false;
	go4->drawable = false;
	go5->drawable = true;

	root->children.push_back(go1);
	root->children.push_back(go2);
	root->children.push_back(go3);
	root->children.push_back(go4);
	root->children.push_back(go5);

	return sg;
}

int main()
{
	HSim::App app;

	app.init(1920, 1080);

	app.setScene(createScene());

	app.mainLoop();

	return 0;
}