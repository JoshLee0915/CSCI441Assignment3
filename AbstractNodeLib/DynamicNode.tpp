#include "DynamicNode.h"

template <class nodeT>
int DynamicNode<nodeT>::getNumOfConnections() const
{
	throw "Not Implimented";
}

template <class nodeT>
DynamicNode<nodeT>::DynamicNode()
{}

template <class nodeT>
DynamicNode<nodeT>::~DynamicNode()
{}

template <class nodeT>
DynamicNode<nodeT>::~DynamicNode()
{}

template <class nodeT>
nodeT* DynamicNode<nodeT>::operator[](const int pos)
{
	throw "Not Implimented";
}

template <class nodeT>
nodeT* DynamicNode<nodeT>::getNode(int pos)
{
	throw "Not Implimented";
}

template <class nodeT>
void DynamicNode<nodeT>::copyAttachedNodes(const nodeT& srcNode, nodeT& dstNode) const
{
	throw "Not Implimented";
}

template <class nodeT>
void DynamicNode<nodeT>::attachNode(nodeT* node, int pos)
{
	throw "Not Implimented";
}

template <class nodeT>
void DynamicNode<nodeT>::removeAttachedNode(int pos)
{}

template <class nodeT>
void DynamicNode<nodeT>::destroyNode()
{}

template <class nodeT>
void DynamicNode<nodeT>::removeAllConnections()
{}