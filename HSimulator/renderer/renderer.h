#pragma once 

#include <HSim/common.h>
// #include <app/mtx.h>
#include <HSim/lock.h>

#include <scene/scene_graph.h>
#include <scene/helper_object.h>
#include <renderer/render_params.h>
#include <renderer/shader.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


namespace HSim
{

	class Renderer
	{
	public:
		Renderer();
		~Renderer();

	public:		
		void init();

		void setScene(SceneGraph_ptr scene_);

	public: 
		void draw(const RenderParams& renderParams);

	// drawable objects
	public:
		SceneGraph_ptr scene;

		// HSim::GroundHelper ground;
		// HSim::SphereHelper sphere;

	// shader
	public:
		HSim::Shader shader;

		// HSim::Shader shader2;

	};

	using Renderer_ptr = std::shared_ptr<Renderer>;
	
} // namespace HSim
