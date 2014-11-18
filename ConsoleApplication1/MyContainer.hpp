#pragma once

#include <iostream>
#include <deque>
#include <list>
#include <map>
#include <mutex>

#include "MyElement.hpp"

template < typename T >
class MyContainer
{
private:
	std::deque< MyElement < T > > _myQueue;
	size_t _maxSize;
	size_t _fixedMaximum;

public:
	MyContainer(size_t maxsize):_fixedMaximum(0), _maxSize(maxsize) {}
	
	~MyContainer() {}

	void add(MyElement < T > elem)
	{
		_myQueue.push_back(elem);

		if (_fixedMaximum < _myQueue.size())
			_fixedMaximum = _myQueue.size();

		if ( ( _myQueue.front().getOwnerId() != elem.getOwnerId() ) || ( _myQueue.size() > _maxSize ) )
			_myQueue.pop_front();
	}

	size_t getCountOfElementsById(size_t threadId)
	{
		size_t k = 0;
		
		for (std::deque< MyElement< T > >::iterator it = _myQueue.begin(); it != _myQueue.end(); ++it)
			if (it->getOwnerId() == threadId)
				k++;

		return k;
	}

	size_t getCurrentSize()
	{
		return this->_myQueue.size();
	}

	size_t getFixedMaximum()
	{
		return this->_fixedMaximum;
	}
};

