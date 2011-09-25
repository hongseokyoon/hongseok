#ifndef	__PARSER_H__
#define	__PARSER_H__

#include "types.h"
#include <utility>

using namespace hongseok::types;

namespace hongseok {

class parser
{
public:
	typedef	std::pair<hs_uint32, hs_uint32>	range_t;
	typedef	range_t(*parser_t)(const hs_void*, const hs_uint32&);

	static range_t	read_all(const void* data, const hs_uint32& size);		// no throw

	static range_t	read_begin_with(const void* data, const hs_uint32& size, 
						const void* header, const hs_uint32& header_size);	// no throw
	static range_t	read_end_with(const void* data, const hs_uint32& size,
						const void* trailer, const hs_uint32& trailer_size);// no throw

	static hs_int64	find(const hs_void* data, const hs_uint32& size,
						const hs_void* find, const hs_uint32& find_size);	// no throw
};// class parser

};// namespace hongseok

#endif//__PARSER_H__
