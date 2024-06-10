#pragma once

#include <HSim/aabb3.h>

namespace HSim
{
	struct BVH3Node;
	using BVH3Node_Ptr = std::shared_ptr<BVH3Node>;

	struct BVH3Node
	{
		BVH3Node() {}

		BVH3Node(std::shared_ptr<BVH3Node> LChild_, std::shared_ptr<BVH3Node> RChild_)
			: LChild(LChild_), RChild(RChild_) {}

		BVH3Node_Ptr LChild = nullptr;
		BVH3Node_Ptr RChild = nullptr;

		AABB3<PRECISION> aabb;
		size_t primitiveIndex; // if isLeaf

		size_t depth;

		bool isLeaf()
		{
			return (LChild == nullptr) && (RChild == nullptr);
		}

	}; // struct BVH3Node

	struct BVH3NodeFlatten
	{

	};

	// closest
	struct ClosestPrimitiveInfo
	{
		size_t primitiveIndex;

		float distance = MAX_FLOAT;

		// Vec3f position;
		// Vec3f normal;

		inline friend std::ostream &operator<<(std::ostream &, ClosestPrimitiveInfo &);

	}; // struct ClosestInfo

	inline std::ostream &operator<<(std::ostream &os, ClosestPrimitiveInfo &info)
	{
		os << "primitiveIndex: " << info.primitiveIndex << "\n"
		   << "distance: " << info.distance << "\n";

		return os;
	}

	template <typename T>
	class BVH3
	{
	public:
		using indexIterType = std::vector<size_t>::iterator;

		BVH3()
		{
			rootNode = std::make_shared<BVH3Node>();
		}

		~BVH3() {}

		void reset()
		{
			rootNode = std::make_shared<BVH3Node>();
			rootAABB.reset();
		}

		void build(std::vector<size_t> &primitiveIndices_, const std::vector<AABB3<T>> &primitivesAABBs_)
		{
			if (primitiveIndices_.empty() || primitivesAABBs_.empty())
			{
				std::cout << "BVH build fault! primitiveIndices_ is empty!" << std::endl;
				return;
			}

			primitiveIndices = primitiveIndices_;
			primitivesAABBs = primitivesAABBs_;

			// compute root AABB
			rootAABB.reset();
			for (auto aabb : primitivesAABBs)
			{
				rootAABB.merge(aabb);
			}

			// debug
			// std::cout << "rootAABB";
			// std::cout << rootAABB.lowerCorner;
			// std::cout << rootAABB.upperCorner;

			rootNode->aabb = rootAABB;

			indexIterType indexBegin = primitiveIndices.begin();
			indexIterType indexEnd = primitiveIndices.end();

			auto depth = recursiveBuid(rootNode, indexBegin, indexEnd, 0);
			rootNode->depth = depth;

			// buildRenderingData();
		}

		//  primitiveIndices begin iter, end iter,

		// size_t recursiveBuid(BVH3Node_Ptr rootNode, std::vector<size_t> &primitiveIndices, size_t currDepth)
		size_t recursiveBuid(BVH3Node_Ptr currNode, indexIterType indexBegin, indexIterType indexEnd, size_t currDepth)
		{

			// leaf node
			// if (primitiveIndices.size() == 1)

			size_t currIndicesSize = indexEnd - indexBegin;

			// std::cout << "\ncurrIndicesSize: " << currIndicesSize << std::endl;

			if (currIndicesSize == 1)
			{
				currNode->primitiveIndex = primitiveIndices[*indexBegin];
				currNode->aabb = primitivesAABBs[primitiveIndices[*indexBegin]];

				currNode->LChild = nullptr;
				currNode->RChild = nullptr;

				return currDepth + 1;
			}

			// Compute node AABB by merging all children of the curr node.
			AABB3<T> nodeAABB;
			// for (size_t i = 0; i < primitiveIndices.size(); i++)
			// {
			// 	auto currPrimitiveIndex = primitiveIndices[i];
			// 	auto currPrimitivesAABB = primitivesAABBs[currPrimitiveIndex];
			// 	nodeAABB.merge(currPrimitivesAABB);
			// }

			for (auto iter = indexBegin; iter != indexEnd; iter++)
			{
				auto currPrimitiveIndex = *iter;
				auto currPrimitivesAABB = primitivesAABBs[currPrimitiveIndex];
				nodeAABB.merge(currPrimitivesAABB);
			}

			// debug
			// std::cout << "depth: " << currDepth;
			// std::cout << "\nnodeAABB\n" << nodeAABB.lowerCorner;
			// std::cout << nodeAABB.upperCorner;

			// Choose the axis with the longest span as the spliting axis
			auto nodeAABBSize = nodeAABB.upperCorner - nodeAABB.lowerCorner;
			size_t axis; // 0: x, 1: y, 2: z
			if (nodeAABBSize.x > nodeAABBSize.y && nodeAABBSize.x > nodeAABBSize.z)
			{
				axis = 0;
			}
			else
			{
				axis = (nodeAABBSize.y > nodeAABBSize.z) ? 1 : 2;
			}

			float pivotPosition = 0.5 * (nodeAABB.upperCorner[axis] + nodeAABB.lowerCorner[axis]);

			// debug
			// std::cout << "depth: " << currDepth << std::endl;
			// for (auto primitiveIndex : primitiveIndices)
			// {
			// 	std::cout << primitiveIndex << " ";
			// }

			// split
			auto splitIter = qsplit(indexBegin, indexEnd, pivotPosition, axis);

			// std::cout << "\nqsplited:\n";
			// for (auto primitiveIndex : primitiveIndices)
			// {
			// 	std::cout << primitiveIndex << " ";
			// }
			// std::cout << "\nsplitIter: " << splitIter - indexBegin << std::endl;

			// std::cout << indexBegin - indexBegin << " " << splitIter - indexBegin << " " << indexEnd - indexBegin << std::endl << std::endl;

			BVH3Node_Ptr LChildNode = std::make_shared<BVH3Node>();
			BVH3Node_Ptr RChildNode = std::make_shared<BVH3Node>();

			size_t depthL = recursiveBuid(LChildNode, indexBegin, splitIter, currDepth + 1);
			size_t depthR = recursiveBuid(RChildNode, splitIter, indexEnd, currDepth + 1);

			LChildNode->depth = currDepth;
			RChildNode->depth = currDepth;

			currNode->aabb = nodeAABB;
			currNode->LChild = LChildNode;
			currNode->RChild = RChildNode;

			return std::max(depthL, depthR);
		}

		indexIterType qsplit(indexIterType indexBegin, indexIterType indexEnd, float pivotPosition, size_t axis)
		{
			auto splitIter = indexBegin;
			for (auto iter = indexBegin; iter != indexEnd; iter++)
			{
				auto currPrimitiveIndex = *iter;
				auto currPrimitivesAABB = primitivesAABBs[currPrimitiveIndex];
				float center = 0.5 * (currPrimitivesAABB.lowerCorner[axis] + currPrimitivesAABB.upperCorner[axis]);

				if (center < pivotPosition)
				{
					std::swap(*iter, *splitIter);
					splitIter++;
				}
			}

			// ill condition
			if (splitIter == indexBegin || splitIter == indexEnd)
			{
				// Divide equally
				splitIter += (indexEnd - indexBegin) / 2;
			}

			return splitIter;
		}

		

		// traverse
	public:
		void traverse(const std::function<void(BVH3Node_Ptr)> &callback)
		{
			traverse(callback, rootNode);
		}

		void traverse(const std::function<void(BVH3Node_Ptr)> &callback, BVH3Node_Ptr node)
		{
			callback(node);

			if (node->isLeaf())
			{
				return;
			}
			else
			{
				auto LChild = node->LChild;
				auto RChild = node->RChild;
				traverse(callback, LChild);
				traverse(callback, RChild);
			}
		}

		// queries and operations
	public:
		ClosestPrimitiveInfo closestPrimitive(const Vec3<T> &position,
											  const std::function<T(const Vec3<T> &position, const size_t primitiveIndex)> &distanceFunction) const
		{
			ClosestPrimitiveInfo info;

			// info.distance = distanceFunction(position, 1);
			// info.primitiveIndex = 1;

			size_t overlap(0), both(0), onlyL(0), onlyR(0), other(0);
			size_t leaf(0);

			std::function<void(BVH3Node_Ptr node)> _traverse = [&](BVH3Node_Ptr node)
			{
				if (node->isLeaf())
				{
					leaf++;
					auto primitiveIndex = node->primitiveIndex;
					auto currDistance = distanceFunction(position, primitiveIndex);

					if (currDistance < info.distance)
					{
						info.distance = currDistance;
						info.primitiveIndex = primitiveIndex;
					}
				}
				else
				{
					auto LNode = node->LChild;
					auto RNode = node->RChild;

					auto currD = info.distance;
					// _traverse(LNode);
					// _traverse(RNode);return;
					auto distanceToLNodeAABB = position.distanceTo(clamp(position, LNode->aabb.lowerCorner, LNode->aabb.upperCorner));
					auto distanceToRNodeAABB = position.distanceTo(clamp(position, RNode->aabb.lowerCorner, RNode->aabb.upperCorner));

					// if (LNode->aabb.isOverlap(RNode->aabb)  && LNode->depth >= rootNode->depth - (size_t)rootNode->depth*.99)
					if (LNode->aabb.isOverlap(RNode->aabb))
					{
						overlap++;

						// std::cout << "LAABB\n" << LNode->aabb.lowerCorner << LNode->aabb.upperCorner;
						// std::cout << "RAABB\n" << RNode->aabb.lowerCorner << RNode->aabb.upperCorner;

						// std::cout << "currD " << currD << "\n"
						// 		<< "distanceToLNodeAABB " << distanceToLNodeAABB
						// 		<<  "\ndistanceToRNodeAABB " << distanceToRNodeAABB << "\n\n";

						_traverse(LNode);
						_traverse(RNode);

						return;
					}

					if (distanceToLNodeAABB < info.distance)
					{
						if (distanceToRNodeAABB < info.distance)
						{
							both++;
							if (distanceToLNodeAABB < distanceToRNodeAABB)
							{
								_traverse(LNode);
								if (distanceToRNodeAABB < info.distance)
									_traverse(RNode);
							}
							else
							{
								_traverse(RNode);
								if (distanceToLNodeAABB < info.distance)
									_traverse(LNode);
							}
						}
						else
						{
							onlyL++;
							_traverse(LNode);
						}
					}
					else if (distanceToRNodeAABB < info.distance)
					{
						onlyR++;
						_traverse(RNode);
					}
					else
					{
						other++;
					}

					// if (distanceToLNodeAABB < info.distance && distanceToRNodeAABB < info.distance)
					// {
					// 	_traverse(LNode);
					// 	_traverse(RNode);
					// 	// if (distanceToLNodeAABB < distanceToRNodeAABB)
					// 	// {
					// 	// 	_traverse(LNode);
					// 	// 	if (distanceToRNodeAABB < info.distance)
					// 	// 	{
					// 	// 		_traverse(RNode);
					// 	// 	}
					// 	// }
					// 	// else
					// 	// {
					// 	// 	_traverse(RNode);
					// 	// 	if (distanceToLNodeAABB < info.distance)
					// 	// 	{
					// 	// 		_traverse(LNode);
					// 	// 	}
					// 	// }
					// }
					// else if (distanceToLNodeAABB < info.distance)
					// {
					// 	// std::cout << "L\n";
					// 	_traverse(LNode);
					// }
					// else if (distanceToLNodeAABB > info.distance)
					// {
					// 	// std::cout << "R\n";
					// 	_traverse(RNode);
					// }
					// else
					// {
					// 	std::cout << "distanceToLNodeAABB " << distanceToLNodeAABB
					// 			  << "\ndistanceToRNodeAABB " << distanceToRNodeAABB
					// 			  << "\ninfo.distance " << info.distance << "\n\n";
					// 	return;
					// }
				}
			};

			_traverse(rootNode);

			printf("leaf: %zd\n", leaf);
			printf("overlap, both, onlyL, onlyR, other\n");
			printf("%zd %zd %zd %zd %zd\n", overlap, both, onlyL, onlyR, other);

			return info;
		}

		ClosestPrimitiveInfo closestPrimitiveFlatten(const Vec3<T> &position,
											  const std::function<T(const Vec3<T> &position, const size_t primitiveIndex)> &distanceFunction) const
		{
			// todo
			ClosestPrimitiveInfo info;
			retrun info;
		}	

		

		

		// data
	public:
		std::vector<size_t> primitiveIndices;
		std::vector<AABB3<T>> primitivesAABBs;

		// root Node
		BVH3Node_Ptr rootNode;
		// AABB of root
		AABB3<T> rootAABB;

/*
		// for rendering
	public:
		unsigned int vaoID = 0;
		unsigned int vboID = 0;

		std::vector<float> vertices;

		// void buildRenderingData()
		void buildRenderingData()
		{
			vertices.clear();
			// std::function<void(BVH3Node_Ptr)> callback = [&](BVH3Node_Ptr node)
			auto callback = [&](BVH3Node_Ptr node)
			{
				if (node->depth != 0)
				{
					return;
				}

				auto lowerCorner = node->aabb.lowerCorner;
				auto upperCorner = node->aabb.upperCorner;
				vertices.insert(vertices.end(), {lowerCorner[0], lowerCorner[1], lowerCorner[2], 0.0f, 0.0f, -1.0f,
												 upperCorner[0], lowerCorner[1], lowerCorner[2], 0.0f, 0.0f, -1.0f,
												 upperCorner[0], upperCorner[1], lowerCorner[2], 0.0f, 0.0f, -1.0f,

												 upperCorner[0], upperCorner[1], lowerCorner[2], 0.0f, 0.0f, -1.0f,
												 lowerCorner[0], upperCorner[1], lowerCorner[2], 0.0f, 0.0f, -1.0f,
												 lowerCorner[0], lowerCorner[1], lowerCorner[2], 0.0f, 0.0f, -1.0f,

												 lowerCorner[0], lowerCorner[1], upperCorner[2], 0.0f, 0.0f, 1.0f,
												 upperCorner[0], lowerCorner[1], upperCorner[2], 0.0f, 0.0f, 1.0f,
												 upperCorner[0], upperCorner[1], upperCorner[2], 0.0f, 0.0f, 1.0f,

												 upperCorner[0], upperCorner[1], upperCorner[2], 0.0f, 0.0f, 1.0f,
												 lowerCorner[0], upperCorner[1], upperCorner[2], 0.0f, 0.0f, 1.0f,
												 lowerCorner[0], lowerCorner[1], upperCorner[2], 0.0f, 0.0f, 1.0f,

												 lowerCorner[0], lowerCorner[1], lowerCorner[2], -1.0f, 0.0f, 0.0f,
												 upperCorner[0], lowerCorner[1], lowerCorner[2], -1.0f, 0.0f, 0.0f,
												 upperCorner[0], lowerCorner[1], upperCorner[2], -1.0f, 0.0f, 0.0f,

												 upperCorner[0], lowerCorner[1], upperCorner[2], -1.0f, 0.0f, 0.0f,
												 lowerCorner[0], lowerCorner[1], upperCorner[2], -1.0f, 0.0f, 0.0f,
												 lowerCorner[0], lowerCorner[1], lowerCorner[2], -1.0f, 0.0f, 0.0f,

												 lowerCorner[0], upperCorner[1], lowerCorner[2], 1.0f, 0.0f, 0.0f,
												 upperCorner[0], upperCorner[1], lowerCorner[2], 1.0f, 0.0f, 0.0f,
												 upperCorner[0], upperCorner[1], upperCorner[2], 1.0f, 0.0f, 0.0f,

												 upperCorner[0], upperCorner[1], upperCorner[2], 1.0f, 0.0f, 0.0f,
												 lowerCorner[0], upperCorner[1], upperCorner[2], 1.0f, 0.0f, 0.0f,
												 lowerCorner[0], upperCorner[1], lowerCorner[2], 1.0f, 0.0f, 0.0f,

												 lowerCorner[0], lowerCorner[1], lowerCorner[2], 0.0f, -1.0f, 0.0f,
												 lowerCorner[0], upperCorner[1], lowerCorner[2], 0.0f, -1.0f, 0.0f,
												 lowerCorner[0], upperCorner[1], upperCorner[2], 0.0f, -1.0f, 0.0f,

												 lowerCorner[0], upperCorner[1], upperCorner[2], 0.0f, -1.0f, 0.0f,
												 lowerCorner[0], lowerCorner[1], upperCorner[2], 0.0f, -1.0f, 0.0f,
												 lowerCorner[0], lowerCorner[1], lowerCorner[2], 0.0f, -1.0f, 0.0f,

												 upperCorner[0], lowerCorner[1], lowerCorner[2], 0.0f, 1.0f, 0.0f,
												 upperCorner[0], upperCorner[1], lowerCorner[2], 0.0f, 1.0f, 0.0f,
												 upperCorner[0], upperCorner[1], upperCorner[2], 0.0f, 1.0f, 0.0f,

												 upperCorner[0], upperCorner[1], upperCorner[2], 0.0f, 1.0f, 0.0f,
												 upperCorner[0], lowerCorner[1], upperCorner[2], 0.0f, 1.0f, 0.0f,
												 upperCorner[0], lowerCorner[1], lowerCorner[2], 0.0f, 1.0f, 0.0f});
			};

			traverse(callback);

			unsigned int vao;
			unsigned int vbo;

			glGenVertexArrays(1, &vao);
			glGenBuffers(1, &vbo);
			glBindVertexArray(vao);

			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferData(GL_ARRAY_BUFFER, (unsigned int)vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

			// layout 0: positions
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
			// layout 1: normals
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));

			glBindVertexArray(0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			vaoID = vao;
			vboID = vbo;
		}

		// void draw
		void draw()
		{
			glBindVertexArray(vaoID);

			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glLineWidth(3.0f);
			glDrawArrays(GL_TRIANGLES, 0, vertices.size());
			glLineWidth(1.0f);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

			// unbind
			glBindVertexArray(0);
		}
*/
	}; // class BVH3

	template <typename T>
	using BVH3_Ptr = std::shared_ptr<BVH3<T>>;

} // namespace HSim
