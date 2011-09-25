#ifndef __BUFFER_H__
#define	__BUFFER_H__

#include "types.h"

namespace hongseok {

using namespace hongseok::types;

#ifndef	BUFFER_SIZE
#define	BUFFER_SIZE	16384	// 16K
#endif

class buffer
{
public:
	enum	{ capacity = BUFFER_SIZE };

	buffer();

	const hs_void*	readable_data() const;	// no throw
	hs_void*		writable_data();		// no throw

	hs_uint32		readable_size() const;	// no throw
	hs_uint32		writable_size() const;	// no throw

	const hs_void*	read(const hs_uint32& len);	// no throw
	hs_bool8		write(const hs_void* data, 
						const hs_uint32& len);	// no throw
	hs_bool8		write(const hs_uint32& len);// no throw

	hs_void			normalize();			// no throw

private:
	hs_uint8	_buffer[capacity];
	hs_uint32	_begin, _end;
};// class buffer

};// namespace hongseok

#endif // __BUFFER_H__
