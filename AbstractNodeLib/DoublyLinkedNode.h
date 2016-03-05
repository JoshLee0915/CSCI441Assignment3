#pragma once
#include "BaseNode.h"

namespace abstractNodes
{
	template <typename nodeT> class StaticNode;

	template <class nodeT>
	class DoublyLinkedNode : virtual public BaseNode<nodeT>
	{
	public:
		DoublyLinkedNode();
		virtual ~DoublyLinkedNode();
		void addNode(nodeT* node, int pos);
		void addNode(nodeT* node, int pos, int connectBackPos);
		void removeNode(int pos);
		void removeNode(int pos, int connectBackPos);

	private:
		virtual int getConnectBackIndex(int pos) = 0;
	};

#include "DoublyLinkedNode.tpp"
}