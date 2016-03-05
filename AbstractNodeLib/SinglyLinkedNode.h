#pragma once
#include "BaseNode.h"

namespace abstractNodes
{
	template <typename nodeT> class StaticNode;

	template <class nodeT>
	class SinglyLinkedNode : virtual public BaseNode<nodeT>
	{
	public:
		SinglyLinkedNode();
		virtual ~SinglyLinkedNode();
		void addNode(nodeT* node, int pos);
		void removeNode(int pos);
	};

#include "SinglyLinkedNode.tpp"
}
