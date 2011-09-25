#ifndef	__UTIL_H__
#define	__UTIL_H__

#include "types.h"
#include <errno.h>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread.hpp>
//#include <string>

using namespace hongseok::types;

namespace hongseok {
namespace util {

class performance
{
public:

//private:
	class time_duration
	{
	public:
		time_duration(hongseok::types::hs_int64& duration);
		~time_duration();

		operator	hs_bool8() const
		{
			return true;
		}

	private:
		boost::posix_time::ptime	_begin;
		hongseok::types::hs_int64&	_duration;
	};// class time_duration
};// class performance

#define	TIME_DURATION(d)	if (hongseok::util::performance::time_duration __d = d)

class info
{
public:
	static const hs_char8*	thread_id();	// no throw
};// class info

#define	DBGINFO	'[' << util::info::thread_id() << '-' << (hs_uint32)this << ']'
/*
class memory 
{
public:
	static errno_t	copy(hs_void* dest, size_t dest_size, const hs_void* src, size_t count);
	static hs_void*	zero(hs_void* dest, size_t length);
};// class memory 
*/

namespace functor
{

template <typename T>
class releaser
{
public:
	void	operator()(T* p)
	{
		delete	p;
	}

};// class releaser

};// namespace functor

class scope_logger
{
public:
	scope_logger(const std::string& message);
	~scope_logger();

private:
	std::string	_indent(hs_uint32 idn) const;

	static hs_uint32			_idn;
    static boost::mutex         _mutex;
	const std::string			_message;
	boost::posix_time::ptime	_t;
};// class scope_logger

#ifndef SCOPE_LOGGER
#define	SCOPE_LOGGER	hongseok::util::scope_logger s##__LINE__(__FUNCTION__)
//#define SCOPE_LOGGER
#endif

namespace {
class invoker_null_type
{
};// class invoker_null_type
};// anonymous namespace

template <typename R, typename F, typename ARG1, typename ARG2>
class invoker_return
{
public:
    invoker_return(F f, ARG1 arg1, ARG2 arg2)
    :_f(f), _arg1(arg1), _arg2(arg2)
    {}

    template <typename C>
    R   operator()(C c)
    {
        return (c->*_f)(_arg1, _arg2);
    }

private:
    F       _f;
    ARG1    _arg1;
    ARG2    _arg2;
};// class invoker_return


template <typename R, typename F>
class invoker_return<R, F, invoker_null_type, invoker_null_type>
{
public:
    invoker_return(F f)
    :_f(f)
    {}

    template <typename C>
    R   operator()(C c)
    {
        return (c->*_f)();
    }

private:
    F   _f;
};// class invoker_return

template <typename R, typename F, typename ARG1>
class invoker_return<R, F, ARG1, invoker_null_type>
{
public:
    invoker_return(F f, ARG1 arg1)
    :_f(f), _arg1(arg1)
    {}

    template <typename C>
    R   operator()(C c)
    {
        return (c->*_f)(_arg1);
    }

private:
    F       _f;
    ARG1    _arg1;
};// class invoker_return

template <typename R, typename C, typename ARG1, typename ARG2>
class invoker_return_param
{
public:
    typedef R(C::*type0)();
    typedef R(C::*type1)(ARG1);
    typedef R(C::*type2)(ARG1, ARG2);
};// class invoker_type_param

template <typename R, typename C, typename ARG1 = invoker_null_type, typename ARG2 = invoker_null_type>
class invoker_return_type_shorter
{
public:
    typedef invoker_return<R, typename invoker_return_param<R, C, ARG1, ARG2>::type0, ARG1, ARG2>   type0;
    typedef invoker_return<R, typename invoker_return_param<R, C, ARG1, ARG2>::type1, ARG1, ARG2>   type1;
    typedef invoker_return<R, typename invoker_return_param<R, C, ARG1, ARG2>::type2, ARG1, ARG2>   type2;
};// class invoker_return_type_shorter

template <typename R, typename C>
typename invoker_return_type_shorter<R, C>::type0 invoker(R(C::*f)())
{
    return typename invoker_return_type_shorter<R, C>::type0(f);
}
template <typename R, typename C, typename ARG1>
typename invoker_return_type_shorter<R, C, ARG1>::type1 invoker(R(C::*f)(ARG1), ARG1 arg1)
{
    return typename invoker_return_type_shorter<R, C, ARG1>::type1(f, arg1);
}

template <typename R, typename C, typename ARG1, typename ARG2>
typename invoker_return_type_shorter<R, C, ARG1, ARG2>::type2 invoker(R(C::*f)(ARG1, ARG2), ARG1 arg1, ARG2 arg2)
{
    return typename invoker_return_type_shorter<R, C, ARG1, ARG2>::type2(f, arg1, arg2);
}

};// namespace util
};// namespace hongseok

#endif // __UTIL_H__
