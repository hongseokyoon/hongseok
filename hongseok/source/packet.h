#ifndef __PACKET_H__
#define	__PACKET_H__

#include "types.h"

namespace hongseok {

using namespace hongseok::types;

class packet
{
public:
	virtual ~packet()	{}

	virtual const hs_void*	data() const	= 0;
	virtual hs_uint32	size() const	= 0;
};// class packet

class hs_packet : public packet
{
public:
	hs_packet(const hs_void* data, const hs_uint32& size)
	: _data(data), _size(size)
	{}

	virtual const hs_void*	data() const	{	return _data;	}
	virtual hs_uint32	size() const	{	return _size;	}

protected:
	const hs_void*		_data;
	const hs_uint32	_size;
};// class hs_packet : public packet

};// namespace hongseok

#endif // __PACKET_H__
