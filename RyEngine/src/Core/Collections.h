#pragma once

namespace RE
{
	template<typename T>
	class Heap
	{
	public:
		//TODO: maybe don't do this?
	private:
		
	};

	template<typename T>
	class Node
	{
	public:
		bool operator==(const T& other) { return _mVal == otehr._mVal; }
	private:
		T _mVal;
		Node<T>* _mLeft;
		Node<T>* _mRight;
	};
}