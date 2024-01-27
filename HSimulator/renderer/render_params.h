#pragma once

#include <glm/glm.hpp>

namespace HSim
{
	struct RenderParams
	{
		struct Transform
		{
			glm::mat4 model = glm::mat4{1.f}; // model transform
			glm::mat4 view = glm::mat4{1.f};  // view transform
			glm::mat4 proj = glm::mat4{1.f};  // projection transform
											  // glm::mat4 normal = glm::mat4{ 1.f }; // normal transform
		} transforms;

		struct Light
		{
			// ambient light
			glm::vec3 ambientColor = glm::vec3(0.05f);
			float ambientScale = 1.f;

			// directional light
			glm::vec3 mainLightColor = glm::vec3(1.f);
			float mainLightScale = 5.f;
			glm::vec3 mainLightDirection = glm::vec3(0.4f, 0.6f, 0.8f);
			float mainLightShadow = 1.f; // 0 - disable shadow; otherwise enable shadow

			// camera light
			glm::vec3 cameraLightColor = glm::vec3(0.1f);
			float cameraLightScale = 1.f;
		} light;

		// image size
		int width = 0;
		int height = 0;

		// index
		int index = -1;

		// render mode
		// 0 - Opacity
		// 1 - Shadow map generation
		// 2 = Transparency
		int mode = 0;
	};

} // namespace HSim
