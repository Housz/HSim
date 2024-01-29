#include <thread>

#include <string>

#include <HSim/common.h>

#include <scene/scene_graph.h>
#include <GUI/render_window.h>
#include <simulator/simulator.h>

namespace HSim
{
	
	class App
	{
	public:
		App();
		~App();

	public:
		void init(size_t width, size_t height);
		void mainLoop();

		void setScene(SceneGraph_ptr scene_);

	public:
		void setWindowTitle(const std::string& title_);

	public:
		SceneGraph_ptr scene = nullptr;
		
		Simulator_ptr simulator = nullptr;
		RenderWindow_ptr window = nullptr;
	};
	
	

} // namespace HSim
