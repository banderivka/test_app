#pragma once

template < class T >
class MyElement
{
private:
	T _data;
	size_t _ownerId;

public:
	MyElement< T >(size_t id, T data):_data(data), _ownerId(id) {}

	~MyElement< T >() {}

	size_t getOwnerId() { return this->_ownerId; }
};
