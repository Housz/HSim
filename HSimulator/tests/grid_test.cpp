#include <HSim/common.h>
#include <HSim/sphere3.h>
#include <HSim/cell_center_scalar_grid3.h>

#include <scene/game_object.h>
#include <config/numerical_config.h>
#include <app/app.h>
#include <IO/obj_reader.h>

void createScene(HSim::App &app)
{
	auto scene = std::make_shared<HSim::SceneGraph>();
	app.setScene(scene);

	auto root = scene->root;

	
}


int main()
{
	HSim::App app;

	app.init(1920, 1080);

	createScene(app);

	app.mainLoop();

	return 0;
}
