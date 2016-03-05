#pragma once
#include "BaseNode.h"

namespace abstractNodes
{

	template <typename nodeT> class SinglyLinkedNode;
	template <typename nodeT> class DoublyLinkedNode;

	template <class nodeT>
	class DynamicNode : public BaseNode
	{
		friend class SinglyLinkedNode<nodeT>;
		friend class DoublyLinkedNode<nodeT>;

	public:
		int getNumOfConnections() const;
		DynamicNode();
		virtual ~DynamicNode();
		nodeT* operator[](const int pos);
		nodeT* getNode(int pos);

	protected:
		void attachNode(nodeT* node, int pos);
		void copyAttachedNodes(const nodeT& srcNode, nodeT& dstNode) const;
		void destroyNode();
		void removeAttachedNode(int pos);

	private:
		virtual void removeAllConnections();
	};

#include "DynamicNode.tpp"

}