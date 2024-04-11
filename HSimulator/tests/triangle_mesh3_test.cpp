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
	// mesh->readOBJ("spot_triangulated.obj");
	mesh->readOBJ("cube.obj");
	mesh->buildBVH();

	auto go1 = std::make_shared<HSim::GameObject>();
	go1->setSurface(mesh);
	go1->enableDraw();

	root->addChild(go1);

	HSim::Vec3f target(0, 0.1, -1);

	auto p = mesh->closestPositionLocal(target);
	std::cout << p;

	auto sphere1 = std::make_shared<HSim::Sphere3<float>>();
	sphere1->center = target;
	sphere1->radius = 0.05;
	auto sphere2 = std::make_shared<HSim::Sphere3<float>>();
	sphere2->center = p;
	sphere2->radius = 0.05;


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
