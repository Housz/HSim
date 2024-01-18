#include <string>

#include <scene/scene_graph.h>
#include <GUI/render_window.h>

namespace HSim
{
	
	class HSimulator
	{
	public:
		HSimulator();
		~HSimulator();

	public:
		void init(size_t width, size_t height);
		void mainLoop();

	public:
		void setWindowTitle(const std::string& title_);

	public:
		SceneGraph_ptr scene;
		RenderWindow_ptr window;
	};
	
	

} // namespace HSim
