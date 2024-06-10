#include <HSim/common.h>

#include <app/app.h>
#include <scene/game_object.h>
#include <config/configs.h>
#include <IO/obj_reader.h>

void createScene(HSim::App &app)
{
	auto scene = std::make_shared<HSim::SceneGraph>();
	app.setScene(scene);

	scene->addGround(10);

	auto root = scene->root;

	auto mesh = std::make_shared<HSim::TriangleMesh3<PRECISION>>();
	HSim::readOBJtoTriangleMesh(mesh, "./resources/models/spot_triangulated.obj");

	mesh->buildBVH();

	auto matMesh = std::make_shared<HSim::BasicMaterial>();
	matMesh->color = {0.2, 0.8, 0.2};
	matMesh->renderingType = HSim::RenderingType::FLAT;
	// matMesh->wireframe = true;

	auto meshGObject = std::make_shared<HSim::TriangleMesh3GObject>(mesh, matMesh);
	auto meshRenderable = std::make_shared<HSim::Renderable>(mesh, meshGObject);

	auto goMesh = std::make_shared<HSim::GameObject>();
	goMesh->renderable = meshRenderable;
	root->addChild(goMesh);

	HSim::Vec3f target(0.2, 0.4, 0.3);

	auto p = mesh->closestPositionLocal(target);

	auto n = mesh->closestNormalLocal(target);

	auto sphere1 = std::make_shared<HSim::Sphere3<PRECISION>>();
	sphere1->center = target;
	sphere1->radius = 0.01;

	auto sphere2 = std::make_shared<HSim::Sphere3<PRECISION>>();
	sphere2->center = p;
	sphere2->radius = 0.005;

	auto material1 = std::make_shared<HSim::BasicMaterial>();
	material1->color = {0.8, 0.4, 0.4};
	material1->wireframe = true;

	auto material2 = std::make_shared<HSim::BasicMaterial>();
	material2->color = {0.8, 0.4, 0.4};
	material2->wireframe = true;

	auto sphereGraphicsObject1 = std::make_shared<HSim::Sphere3GObject>(sphere1, material1);
	auto sphereGraphicsObject2 = std::make_shared<HSim::Sphere3GObject>(sphere2, material2);
	auto sphereRenderable1 = std::make_shared<HSim::Renderable>(sphere1, sphereGraphicsObject1);
	auto sphereRenderable2 = std::make_shared<HSim::Renderable>(sphere2, sphereGraphicsObject2);

	auto go1 = std::make_shared<HSim::GameObject>();
	go1->renderable = sphereRenderable1;
	auto go2 = std::make_shared<HSim::GameObject>();
	go2->renderable = sphereRenderable2;

	root->addChild(go1);
	root->addChild(go2);


	scene->addLine(p, target);

	scene->addLine(p, p + n);
}

// return scene;

int main()
{
	HSim::App app;

	app.init(1920, 1080);

	createScene(app);

	app.mainLoop();

	return 0;
}
