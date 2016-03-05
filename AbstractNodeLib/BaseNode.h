#pragma once
#pragma warning( disable : 4250)

namespace abstractNodes
{
	template <class nodeT>
	class BaseNode
	{
	public:
		virtual int getNumOfConnections() const = 0;

		BaseNode();
		virtual ~BaseNode();
		virtual nodeT* operator[](const int pos) = 0;
		virtual nodeT* getNode(int pos) = 0;
		virtual void addNode(nodeT* node, int pos) = 0;
		virtual void removeNode(int pos) = 0;
		virtual nodeT* cloneNode() const = 0;

	protected:
		virtual void attachNode(nodeT* node, int pos) = 0;
		virtual void destroyNode() = 0;
		virtual void removeAttachedNode(int pos) = 0;
	};

#include "BaseNode.tpp"
	
}

