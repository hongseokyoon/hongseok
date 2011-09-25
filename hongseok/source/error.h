#ifndef __ERROR_H__
#define	__ERROR_H__

#include "types.h"

#include <string>

#include <boost/system/error_code.hpp>

using namespace hongseok::types;

namespace hongseok {

class user_error_code
{
private:
	class _error_category : public boost::system::error_category
	{
	public:
		_error_category(const hs_char8* message)
		: _message(message)
		{}

		const hs_char8*	name() const
		{
			return "hongseok.user_error";
		}

		std::string	message(hs_int32) const
		{
			return _message;
		}

	private:
		std::string	_message;
	};// class error_category

public:
	static boost::system::error_code	make(const hs_int32 value, const hs_char8* message)
	{
		return boost::system::error_code(value, _error_category(message));
	}

private:
	user_error_code()	{}
};// class user_error_code

};// namespace hongseok

#endif//__ERROR_H__
