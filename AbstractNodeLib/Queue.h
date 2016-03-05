#pragma once

namespace Pathfinding
{
	namespace Util
	{
		template <class T>
		class Queue
		{
		protected:
			int size;

		public:
			int getSize() const
			{
				return size;
			}
			virtual bool isEmpty() const = 0;
			Queue() {};
			virtual ~Queue() {};
			virtual T& front() const = 0;
			virtual T& back() const = 0;
			virtual void push(T* value) = 0;
			virtual T& pop() = 0;
		};
	}
}
