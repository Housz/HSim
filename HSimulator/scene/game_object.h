#pragma once

#include <HSim/transform3.h>
#include <HSim/surface3.h>
#include <HSim/field3.h>

#include <config/numerical_config.h>

namespace HSim
{
	
	class GameObject : public std::enable_shared_from_this<GameObject>
	{
	public:
		GameObject();
		~GameObject();

	// topology
	public:
		std::shared_ptr<GameObject> parent = nullptr;
		std::vector<std::shared_ptr<GameObject>> children;

		bool isLeaf();	

		void addChild(const std::shared_ptr<GameObject> child);


		// todo
		// std::unordered_set<std::shared_ptr<GameObject>> children;
		// void removeChild()

	
	// iteration
	public:
		void traverseChildren(std::function<void(std::shared_ptr<GameObject>)>& callback);


	// geometrical elements
	public:
		bool drawable = false;
		std::shared_ptr<Surface3<PRECISION>> surface_ptr = nullptr;

		// field3 ptr		

	// local transform
	public:
		Transform3<PRECISION> transform;
	};

	using GameObject_ptr = std::shared_ptr<GameObject>;
	
	
	
} // namespace HSim
