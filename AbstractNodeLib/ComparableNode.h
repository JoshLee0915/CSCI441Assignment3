#pragma once

namespace abstractNodes
{
	template<class T>
	class ComparableNode
	{
	public:
		ComparableNode() {}
		virtual ~ComparableNode() {}
		virtual int compareTo(T* node) = 0;
		virtual bool nodeIs(T* node) = 0;
	};
}