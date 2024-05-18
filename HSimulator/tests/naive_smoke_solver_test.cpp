#include <app/app.h>

void createScene(HSim::App &app)
{
    auto scene = std::make_shared<HSim::SceneGraph>();
    app.setScene(scene);

    auto root = scene->root;

    scene->addGround(100);

    
	/**************************************************
	 * scene
	*/
    const size_t n = 64;
    auto grid = std::make_shared<HSim::CellCenterScalarGrid3<float>>(n, n, n);

	
    /**************************************************/


	/**************************************************
	 * solver
	*/


	/**************************************************/
}

int main()
{
    HSim::App app;

    app.init(1920, 1080);

    createScene(app);

    app.mainLoop();

    return 0;
}