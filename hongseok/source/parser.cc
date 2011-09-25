#include "parser.h"
#include <string.h>

namespace hongseok {

parser::range_t parser::read_all(const void* data, const hs_uint32& size)
{
	return std::make_pair(0, size);
}

parser::range_t parser::read_begin_with(const void* data, const hs_uint32& size,
						const void* header, const hs_uint32& header_size)
{
	range_t	ret	= std::make_pair(0, 0);

	if (size >= sizeof(header_size))
	{
		const hs_char8*	d	= static_cast<const hs_char8*>(data);

		hs_uint32	i = 0;
		for (; i < size - header_size; ++i)
			if (memcmp(d + i, header, header_size) == 0)	break;
	
		ret.first	= i;
		ret.second	= i;

		i	= i + header_size;
		for (; i < size - header_size; ++i)
			if (memcmp(d + i, header, header_size) == 0)	break;

		if (i < size - header_size)
			ret.second	= i - 1;
	}

	return ret;
}

parser::range_t parser::read_end_with(const void* data, const hs_uint32& size,
						const void* trailer, const hs_uint32& trailer_size)
{
	range_t	ret	= std::make_pair(0, 0);

	if (size >= sizeof(trailer_size))
	{
		const hs_char8*	d	= static_cast<const hs_char8*>(data);

		hs_uint32	i	= 0;
		for (; i < size - trailer_size; ++i)
			if (memcmp(d + i, trailer, trailer_size) == 0)	break;

		if (i < size - trailer_size)
			ret.second	= i + trailer_size -1;	
	}

	return ret;
}

hs_int64 parser::find(const hs_void* data, const hs_uint32& size,
						const hs_void* find, const hs_uint32& find_size)
{
	hs_int64	ret	= -1;

	if (size >= find_size)
	{
		const hs_uint8*	d	= static_cast<const hs_uint8*>(data);

		hs_uint32	i	= 0;
		for (; i < size - find_size; ++i)
			if (memcmp(d + i, find, find_size) == 0)	break;

		if (i < size - find_size)	ret	= i;
	}

	return ret;
}

};// namespace hongseok
