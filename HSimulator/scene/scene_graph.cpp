#include <scene/scene_graph.h>
#include "scene_graph.h"

HSim::SceneGraph::SceneGraph()
{
	root = std::make_shared<HSim::GameObject>();
}

HSim::SceneGraph::~SceneGraph()
{
}

void HSim::SceneGraph::traverse(std::function<void(GameObject_ptr)> &callback)
{
	traverse(callback, root);
}

void HSim::SceneGraph::traverse(const std::function<void(GameObject_ptr)> &callback)
{
	traverse(callback, root);
}

void HSim::SceneGraph::traverse(std::function<void(GameObject_ptr)> &callback, GameObject_ptr go)
{
	callback(go);

	if (go->isLeaf())
	{
		return;
	}
	else
	{
		for (auto child : go->children)
		{
			traverse(callback, child);
		}
	}
}

void HSim::SceneGraph::traverse(const std::function<void(GameObject_ptr)> &callback, const GameObject_ptr go)
{
	callback(go);

	if (go->isLeaf())
	{
		return;
	}
	else
	{
		for (auto child : go->children)
		{
			traverse(callback, child);
		}
	}
}

// void HSim::SceneGraph::serialize()
// {
// 	// float r = 0;

// 	std::function<void(HSim::GameObject_ptr)> callback_serialize = [&](HSim::GameObject_ptr go)
// 	{
// 		if (go->surface_ptr != nullptr && go->drawable)
// 		{
// 			// std::cout << "callback_serialize" << std::endl;

// 			auto surface = go->surface_ptr;

// 			// update transform
// 			// surface->transform.translation.x = r;
// 			// r+=0.001;

// 			surface->serialize();
// 		}
// 	};

// 	this->traverse(callback_serialize);
// }

void HSim::SceneGraph::serialize()
{
	// todo
}

void HSim::SceneGraph::snapshot(SceneGraph& sg)
{
	sg.root = root;
	// sg.travese 
	// static -> do nothing
	// dynamic -> copy construct a new obj
}

void HSim::SceneGraph::draw()
{

	std::function<void(HSim::GameObject_ptr)> callback_draw = [&](HSim::GameObject_ptr go)
	{
		if (go->surface_ptr != nullptr && go->drawable)
		{
			// std::cout << "callback_draw" << std::endl;

			auto surface = go->surface_ptr;

			surface->draw();
			surface->drawBoundary();
		}

		if (go->grid_ptr != nullptr && go->drawable)
		{
			// std::cout << "callback_draw" << std::endl;
			auto grid = go->grid_ptr;
			grid->drawBoundary();

			grid->drawData();
		}

		if (go->renderable != nullptr && go->renderable->visible)
		{
		}
	};

	this->traverse(callback_draw);
}

void HSim::SceneGraph::draw(const RenderParams &renderParams)
{
	std::function<void(HSim::GameObject_ptr)> callback_draw = [&](HSim::GameObject_ptr go)
	{
		if (go->renderable != nullptr && go->renderable->visible)
		{
			go->renderable->draw(renderParams);
		}
	};

	this->traverse(callback_draw);
}

void HSim::SceneGraph::addGround(size_t slices)
{
	auto groudHelper = std::make_shared<HSim::GroundHelper>(slices);
	auto mat = std::make_shared<HSim::BasicMaterial>();
	mat->color = {0.2, 0.2, 0.2};
	auto groundHelperGObject = std::make_shared<HSim::GroundHelperGObject>(groudHelper, mat);
	auto groundHelperRenderable = std::make_shared<HSim::Renderable>(groudHelper, groundHelperGObject);
	auto go = std::make_shared<HSim::GameObject>();
	go->renderable = groundHelperRenderable;

	root->addChild(go);
}

void HSim::SceneGraph::addSphere(const Vec3f &center, const float radius, const Vec3f &color)
{
	auto sphere = std::make_shared<HSim::Sphere3<PRECISION>>();
	sphere->setCenter(center);
	sphere->setRadius(radius);

	auto sphereMat = std::make_shared<HSim::BasicMaterial>();
	sphereMat->color = {color[0], color[1], color[2]};

	auto sphereGraphicsObject = std::make_shared<HSim::Sphere3GObject>(sphere, sphereMat);
	auto sphereRenderable = std::make_shared<HSim::Renderable>(sphere, sphereGraphicsObject);

	auto go = std::make_shared<HSim::GameObject>();
	go->renderable = sphereRenderable;

	root->addChild(go);
}

void HSim::SceneGraph::addBox(const Vec3f &lowerCorner, const Vec3f &upperCorner, const Vec3f &color)
{
	auto box = std::make_shared<HSim::Box3<PRECISION>>(lowerCorner, upperCorner);
	auto boxMat = std::make_shared<HSim::BasicMaterial>();
	boxMat->color = {color[0], color[1], color[2]};
	
	auto boxGraphicsObject = std::make_shared<HSim::Box3GraphicsObject>(box, boxMat);

	auto boxRenderable = std::make_shared<HSim::Renderable>(box, boxGraphicsObject);
	
	auto go = std::make_shared<HSim::GameObject>();
	go->renderable = boxRenderable;

	root->addChild(go);
}

void HSim::SceneGraph::addLine(const Vec3f &start, const Vec3f &end, const Vec3f &color)
{
	auto lineHelper = std::make_shared<HSim::LineHelper>(start, end);
	auto lineMat = std::make_shared<HSim::LineMaterial>();
	lineMat->color = {color[0], color[1], color[2]};

	auto lineHelperGObject = std::make_shared<HSim::LineHelperGObject>(lineHelper, lineMat);

	auto lineHelperRenderable = std::make_shared<HSim::Renderable>(lineHelper, lineHelperGObject);

	auto go = std::make_shared<HSim::GameObject>();
	go->renderable = lineHelperRenderable;

	root->addChild(go);
}

void HSim::SceneGraph::addCylinder(const float radiusTop, const float radiusBottom, const float height, const Vec3f &color)
{
	auto cylinder = std::make_shared<HSim::Cylinder3<PRECISION>>(radiusTop, radiusBottom, height);
	auto cylinderMat = std::make_shared<HSim::BasicMaterial>();
	cylinderMat->color = {color[0], color[1], color[2]};
	// cylinderMat->wireframe = true;
	
	auto cylinderGraphicsObject = std::make_shared<HSim::CylinderGObject>(cylinder, cylinderMat);

	auto cylinderRenderable = std::make_shared<HSim::Renderable>(cylinder, cylinderGraphicsObject);

	auto go = std::make_shared<HSim::GameObject>();
	go->renderable = cylinderRenderable;

	root->addChild(go);
}
