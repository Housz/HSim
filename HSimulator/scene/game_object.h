#pragma once

#include <HSim/transform3.h>
#include <HSim/surface3.h>
#include <HSim/field3.h>

#include <config/numerical_config.h>

namespace HSim
{

	using GameObject_ptr = std::shared_ptr<GameObject>;

	class GameObject
	{
	public:
		GameObject();
		~GameObject();

	public:
		GameObject_ptr parent;
		std::vector<GameObject_ptr> children;

	public:
		void traverse(std::function<void(GameObject&)> callback);

	public:
		Transform3<PRECISION> transform;

	public:
		std::shared_ptr<Surface3<PRECISION>> surface_ptr;

		// field3 ptr		

	};

	// using GameObject_ptr = std::shared_ptr<GameObject>;
	
	
	
} // namespace HSim
