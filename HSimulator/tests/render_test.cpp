#include <HSim/common.h>
#include <HSim/sphere3.h>

#include <scene/game_object.h>
#include <config/numerical_config.h>
#include <app/app.h>
#include <IO/obj_reader.h>

HSim::SceneGraph_ptr createScene()
{
	auto scene = std::make_shared<HSim::SceneGraph>();

	auto root = std::make_shared<HSim::GameObject>();
	scene->root = root;

	/**************************************************/

	auto groudHelper = std::make_shared<HSim::GroundHelper>();

	auto material0 = std::make_shared<HSim::BasicMaterial>();
	material0->color = {0.2, 0.2, 0.2};
	auto groundHelperGObject = std::make_shared<HSim::GroundHelperGObject>(groudHelper, material0);

	auto groundHelperRenderable = std::make_shared<HSim::Renderable>(groudHelper, groundHelperGObject);

	auto go0 = std::make_shared<HSim::GameObject>();
	go0->renderable = groundHelperRenderable;

	root->addChild(go0);

	/**************************************************/

	auto sphere = std::make_shared<HSim::Sphere3<PRECISION>>();

	auto material1 = std::make_shared<HSim::BasicMaterial>();
	material1->color = {0.8, 0.4, 0.4};
	material1->wireframe = true;

	auto sphereGraphicsObject = std::make_shared<HSim::Sphere3GObject>(sphere, material1);

	auto sphereRenderable = std::make_shared<HSim::Renderable>(sphere, sphereGraphicsObject);

	auto go1 = std::make_shared<HSim::GameObject>();
	go1->renderable = sphereRenderable;
	// go1->addRenderable( SphereRenderableCreator() )

	root->addChild(go1);

	/**************************************************/
	HSim::Vec3f start = {-5, -5, -5};
	HSim::Vec3f end = {0, 0, 0};
	auto lineHelper = std::make_shared<HSim::LineHelper>(start, end);

	auto material2 = std::make_shared<HSim::BasicMaterial>();
	material2->color = {0.2, 0.8, 0.2};
	auto lineHelperGObject = std::make_shared<HSim::LineHelperGObject>(lineHelper, material2);

	auto lineHelperRenderable = std::make_shared<HSim::Renderable>(lineHelper, lineHelperGObject);

	auto go2 = std::make_shared<HSim::GameObject>();
	go2->renderable = lineHelperRenderable;

	root->addChild(go2);

	/**************************************************/
	std::default_random_engine generator;
	std::uniform_int_distribution<int> distribution_i(-50, 50);
	std::uniform_real_distribution<float> distribution_f(0, 1);

	for (size_t i = 0; i < 100; i++)
	{
		auto x = distribution_i(generator);
		auto y = distribution_i(generator);
		auto z = distribution_i(generator);
		HSim::Vec3f lower = {x, y, z};
		HSim::Vec3f upper = {x + 1, y + 1, z + 1};
		auto box = std::make_shared<HSim::Box3<float>>(lower, upper);

		auto r = distribution_f(generator);
		auto g = distribution_f(generator);
		auto b = distribution_f(generator);
		auto boxMat = std::make_shared<HSim::BasicMaterial>();
		boxMat->color = {r, g, b};
		boxMat->wireframe = false;

		auto boxGraphicsObject = std::make_shared<HSim::Box3GraphicsObject>(box, boxMat);

		auto boxRenderable = std::make_shared<HSim::Renderable>(box, boxGraphicsObject);

		auto go3 = std::make_shared<HSim::GameObject>();
		go3->renderable = boxRenderable;

		root->addChild(go3);
	}

	/**************************************************/

	auto aabbGObject = std::make_shared<HSim::AABB3GraphicsObject>(sphere->AABB(), material2);
	auto aabbRenderable = std::make_shared<HSim::Renderable>(nullptr, aabbGObject);

	auto go4 = std::make_shared<HSim::GameObject>();
	go4->renderable = aabbRenderable;
	// go1->addRenderable( SphereRenderableCreator() )

	root->addChild(go4);

	/**************************************************/

	// mesh
	auto mesh = std::make_shared<HSim::TriangleMesh3<PRECISION>>();
	HSim::readOBJtoTriangleMesh(mesh, "spot_triangulated.obj");

	auto matMesh = std::make_shared<HSim::BasicMaterial>();
	matMesh->color = {0.2, 0.8, 0.2};
	matMesh->renderingType = HSim::RenderingType::FLAT;

	auto meshGObject = std::make_shared<HSim::TriangleMesh3GObject>(mesh, matMesh);
	auto meshRenderable = std::make_shared<HSim::Renderable>(mesh, meshGObject);

	auto go5 = std::make_shared<HSim::GameObject>();
	go5->renderable = meshRenderable;

	root->addChild(go5);

	// mesh AABB
	auto meshAABBGObject = std::make_shared<HSim::AABB3GraphicsObject>(mesh->AABB(), material2);
	auto meshAABBRenderable = std::make_shared<HSim::Renderable>(nullptr, meshAABBGObject);

	auto go6 = std::make_shared<HSim::GameObject>();
	go6->renderable = meshAABBRenderable;

	root->addChild(go6);

	// mesh BVH
	mesh->buildBVH();
	auto meshBVHGObject = std::make_shared<HSim::BVH3GraphicsObject>(mesh->bvh, material2);
	auto meshBVHRenderable = std::make_shared<HSim::Renderable>(nullptr, meshBVHGObject);

	auto go7 = std::make_shared<HSim::GameObject>();
	go7->renderable = meshBVHRenderable;

	root->addChild(go7);

	/**************************************************/

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
