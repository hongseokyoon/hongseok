#include "buffer.h"

#include <memory.h>
#include <algorithm>

namespace hongseok {

buffer::buffer()
: _begin(0), _end(0)
{
}

const hs_void* buffer::readable_data() const
{
	return _buffer + _begin;
}

hs_void* buffer::writable_data()
{
	return _buffer + _end;
}

hs_uint32 buffer::readable_size() const
{
	return _end - _begin;
}

hs_uint32 buffer::writable_size() const
{
	return capacity - _end;
}

const hs_void* buffer::read(const hs_uint32& len)
{
	if (readable_size() < len)	return 0;

	_begin	+= len;
	return _buffer + _begin - len;
}

hs_bool8 buffer::write(const hs_void* data, const hs_uint32& len)
{
	if (writable_size() < len)	return false;

	memcpy(_buffer + _end, data, len);

	_end	+= len;

	return true;
}

hs_bool8 buffer::write(const hs_uint32& len)
{
	if (writable_size() < len) return false;

	_end	+= len;

	return true;
}

hs_void buffer::normalize()
{
	memcpy(_buffer, _buffer + _begin, readable_size());
	_end	-= _begin;
	_begin	= 0;
}

};// namespace hongseok
