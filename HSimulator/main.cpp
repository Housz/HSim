#include <app/app.h>
#include <scene/scene_graph.h>
#include <HSim/box3.h>

HSim::SceneGraph_ptr createScene()
{
	// HSim::SceneGraph sg;
	auto sg = std::make_shared<HSim::SceneGraph>();

	auto root = std::make_shared<HSim::GameObject>();
	sg->root = root;

	auto box1 = std::make_shared<HSim::Box3<float>>();
	HSim::Vec3<float> lower(1, 1, 1);
	HSim::Vec3<float> upper(5, 5, 5);
	auto box2 = std::make_shared<HSim::Box3<float>>(lower, upper);
	// box1->transform.translation = {2, 0, 0};
	HSim::Vec3f axis(0, 0, 1);
	float angle = PI_QUARTER;
	HSim::Quaternionf orientation(axis, angle);
	// box1->transform.orientation = orientation;

	auto go1 = std::make_shared<HSim::GameObject>();
	auto go2 = std::make_shared<HSim::GameObject>();
	auto go3 = std::make_shared<HSim::GameObject>();

	go1->surface_ptr = box1;

	go3->surface_ptr = box2;

	root->drawable = false;
	go1->drawable = true;
	// go2->surface_ptr = box;
	go2->drawable = false;
	go3->drawable = true;

	// go3->surface_ptr = box;

	root->children.push_back(go1);
	root->children.push_back(go2);
	root->children.push_back(go3);

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