#pragma once
#include <string.h>
#include "BaseNode.h"

namespace abstractNodes
{
	template <typename nodeT> class SinglyLinkedNode;
	template <typename nodeT> class DoublyLinkedNode;

	template <class nodeT>
	class StaticNode : virtual public BaseNode<nodeT>
	{
		friend class SinglyLinkedNode<nodeT>;
		friend class DoublyLinkedNode<nodeT>;

	private:
		int maxNumberOfConnections;
		nodeT** attachedNodes;

	public:
		int getNumOfConnections() const;

		StaticNode(int maxNumberOfConnections);
		virtual ~StaticNode();
		virtual nodeT* operator[](const int pos);
		virtual nodeT* getNode(int pos);

	protected:
		void attachNode(nodeT* node, int pos);
		void copyAttachedNodes(const nodeT& srcNode, nodeT& dstNode) const;
		void destroyNode();
		void removeAttachedNode(int pos);

	private:
		virtual void removeAllConnections();
	};

#include "StaticNode.tpp"

}
