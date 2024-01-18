#pragma once

#include <HSim/transform3.h>
#include <HSim/surface3.h>
#include <HSim/field3.h>

#include <config/numerical_config.h>

namespace HSim
{
	
	class GameObject
	{
	public:
		GameObject();
		~GameObject();

	public:
		std::shared_ptr<GameObject> parent;
		std::vector<std::shared_ptr<GameObject>> children;

		bool isLeaf();

	public:
		void traverseChildren(std::function<void(std::shared_ptr<GameObject>)>& callback);

	public:
		Transform3<PRECISION> transform;

	public:
		std::shared_ptr<Surface3<PRECISION>> surface_ptr;

		// field3 ptr		

	};

	using GameObject_ptr = std::shared_ptr<GameObject>;
	
	
	
} // namespace HSim
