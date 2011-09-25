#ifndef __IOBUFFER_H__
#define __IOBUFFER_H__

#include "error.h"
#include "types.h"

#include <exception>
#include <string>
#include <string.h>

using namespace hongseok::types;

namespace hongseok {

#define	READ_BUFFER(T)	\
virtual ibuffer& operator>>(T& val)\
{\
	if (_pos + sizeof(T) > _size)	throw user_error_code::make(0, "ibuffer no data");\
	val	= *(reinterpret_cast<const T*>(_data + _pos));\
	_pos	+= sizeof(T);\
	return *this;\
}

namespace {
	class _null_class
	{
	};// class _null_class
};// namespace

template <class P = _null_class>
class ibuffer : public P
{
public:
	ibuffer(const void* data, const hs_uint32& size)
	: _data(static_cast<const hs_uint8*>(data)), _size(size), _pos(0)
	{
	}

	virtual ~ibuffer()
	{
	}

	READ_BUFFER(hs_bool8)
	READ_BUFFER(hs_int8)
	READ_BUFFER(hs_int16)
	READ_BUFFER(hs_int32)
	READ_BUFFER(hs_int64)
	READ_BUFFER(hs_uint8)
	READ_BUFFER(hs_uint16)
	READ_BUFFER(hs_uint32)
	READ_BUFFER(hs_uint64)
	READ_BUFFER(hs_float32)
	READ_BUFFER(hs_float64)
//	READ_BUFFER(hs_float96)

	virtual ibuffer&	operator>>(hs_char8* val)
	{
		size_t	len	= strlen(reinterpret_cast<const hs_char8*>(_data + _pos)) + 1;
		if (_pos + len > _size)	throw user_error_code::make(0, "ibuffer no data");
		memcpy(val, _data + _pos, len);
		_pos	+= len;
		return *this;
	}

	virtual ibuffer&	operator>>(std::string& val)
	{
		size_t	len	= strlen(reinterpret_cast<const hs_char8*>(_data + _pos)) + 1;
		if (_pos + len > _size)	throw user_error_code::make(0, "ibuffer no data");
		val	= reinterpret_cast<const hs_char8*>(_data + _pos);
		_pos	+= len;
		return *this;
	}

protected:
	const hs_uint8*	_data;
	const hs_uint32	_size;
	hs_uint32		_pos;
};// class ibuffer

#define	WRITE_BUFFER(T)	\
virtual obuffer& operator<<(const T& val) \
{\
	if (_pos + sizeof(T) > _size)	throw user_error_code::make(0, "obuffer no buffer");\
	memcpy(_data + _pos, &val, sizeof(T));\
	_pos	+= sizeof(T);\
	return *this;\
}

template <class P = _null_class>
class obuffer : public P
{
public:
	obuffer(void* data, const hs_uint32& size)
	: _data(static_cast<hs_uint8*>(data)), _size(size), _pos(0)
	{
	}

	virtual ~obuffer()
	{
	}

	// all throw boost::system::error_code
	WRITE_BUFFER(hs_bool8)
	WRITE_BUFFER(hs_int8)
	WRITE_BUFFER(hs_int16)
	WRITE_BUFFER(hs_int32)
	WRITE_BUFFER(hs_int64)
	WRITE_BUFFER(hs_uint8)
	WRITE_BUFFER(hs_uint16)
	WRITE_BUFFER(hs_uint32)
	WRITE_BUFFER(hs_uint64)
	WRITE_BUFFER(hs_float32)
	WRITE_BUFFER(hs_float64)
//	WRITE_BUFFER(hs_float96)

	virtual obuffer&	operator<<(const hs_char8* val)	// throw boost::system::error_code
	{
		size_t	len	= strlen(val) + 1;
		if (_pos + len > _size)	throw user_error_code::make(0, "obuffer no buffer");
		memcpy(_data + _pos, val, len);
		_pos	+= len;
		return *this;
	}

	virtual obuffer&	operator<<(const std::string& val)	// throw boost::system::error_code
	{
		size_t	len	= val.size() + 1;
		if (_pos + len > _size)	throw user_error_code::make(0, "obuffer no buffer");
		memcpy(_data + _pos, val.c_str(), len);
		_pos	+= len;
		return *this;
	}

protected:
	hs_uint8*		_data;
	const hs_uint32	_size;
	hs_uint32		_pos;
};// class obuffer

};// namespace hongseok

#endif // __IOBUFFER_H__
