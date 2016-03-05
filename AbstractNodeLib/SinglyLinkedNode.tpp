#include "SinglyLinkedNode.h"


template <class nodeT>
SinglyLinkedNode<nodeT>::SinglyLinkedNode()
{}

template <class nodeT>
SinglyLinkedNode<nodeT>::~SinglyLinkedNode()
{}

template <class nodeT>
void SinglyLinkedNode<nodeT>::addNode(nodeT* node, int pos)
{
	// attach the node to the correct pos
	this->attachNode(node, pos);
}

template <class nodeT>
void SinglyLinkedNode<nodeT>::removeNode(int pos)
{
	nodeT* tmpNode = this->getNode(pos);

	if (tmpNode != 0)
	{
		// remove the pointer to that node
		this->removeAttachedNode(pos);
	}
}
