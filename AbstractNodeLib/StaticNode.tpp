#include "StaticNode.h"
template <class nodeT>
int StaticNode<nodeT>::getNumOfConnections() const
{
	return maxNumberOfConnections;
}

template <class nodeT>
StaticNode<nodeT>::StaticNode(int maxNumberOfConnections)
{
	this->maxNumberOfConnections = maxNumberOfConnections;

	attachedNodes = new nodeT*[maxNumberOfConnections];

	// Set all attched nodes to null
	memset(attachedNodes, 0, maxNumberOfConnections*sizeof(*attachedNodes));
}

template <class nodeT>
StaticNode<nodeT>::~StaticNode()
{}

template <class nodeT>
nodeT* StaticNode<nodeT>::operator[](const int pos)
{
	return getNode(pos);
}

template <class nodeT>
nodeT* StaticNode<nodeT>::getNode(int pos)
{
	nodeT* t = attachedNodes[pos];
	if (pos < maxNumberOfConnections)
		return attachedNodes[pos];
	else
		return 0;	// replace with an excption later
}

template <class nodeT>
void StaticNode<nodeT>::copyAttachedNodes(const nodeT& srcNode, nodeT& dstNode) const
{
	// copy all the nodes and their weights to the destination node
	for (int index = 0; index < maxNumberOfConnections; index++)
		dstNode.attachedNodes[index] = srcNode.attachedNodes[index];
	
}

template <class nodeT>
void StaticNode<nodeT>::attachNode(nodeT* node, int pos)
{
	if (pos < maxNumberOfConnections)
		// add the node and recored the weight
		attachedNodes[pos] = node;
	

	// add an excption later
}

template <class nodeT>
void StaticNode<nodeT>::removeAttachedNode(int pos)
{
	if (pos < maxNumberOfConnections)
		// remove the reffrence to the node and 0 the weight
		attachedNodes[pos] = 0;

}

template <class nodeT>
void StaticNode<nodeT>::destroyNode()
{

	this->removeAllConnections();

	// delete the arrays
	delete[] attachedNodes;
}

template <class nodeT>
void StaticNode<nodeT>::removeAllConnections()
{
	// remove the connetions of all attached nodes
	for (int index = 0; index < this->maxNumberOfConnections; index++)
		removeNode(index);
}