#include "DoublyLinkedNode.h"


	template <class nodeT>
	DoublyLinkedNode<nodeT>::DoublyLinkedNode()
	{}

	template <class nodeT>
	DoublyLinkedNode<nodeT>::~DoublyLinkedNode()
	{}

	template <class nodeT>
	void DoublyLinkedNode<nodeT>::addNode(nodeT* node, int pos)
	{
		// attach the node to the correct pos
		this->attachNode(node, pos);

		// check that the node is not null
		if(node)
			// attach this node to the passed node
			node->attachNode(dynamic_cast<nodeT*>(this), this->getConnectBackIndex(pos));
	}

	template <class nodeT>
	void DoublyLinkedNode<nodeT>::addNode(nodeT* node, int pos, int connectBackPos)
	{
		// attach the node to the correct pos
		this->attachNode(node, pos);

		// check that the node is not null
		if (node)
			// attach this node to the passed node
			node->attachNode(dynamic_cast<nodeT*>(this), connectBackPos);
	}

	template <class nodeT>
	void DoublyLinkedNode<nodeT>::removeNode(int pos)
	{
		nodeT* tmpNode = this->getNode(pos);

		if (tmpNode != 0)
		{
			// remove the pointer back to this node
			tmpNode->removeAttachedNode(this->getConnectBackIndex(pos));

			// remove the pointer to that node
			this->removeAttachedNode(pos);
		}
	}

	template <class nodeT>
	void DoublyLinkedNode<nodeT>::removeNode(int pos, int connectBackPos)
	{
		nodeT* tmpNode = this->getNode(pos);

		if (tmpNode != 0)
		{
			// remove the pointer back to this node
			tmpNode->removeAttachedNode(connectBackPos);

			// remove the pointer to that node
			this->removeAttachedNode(pos);
		}
	}
