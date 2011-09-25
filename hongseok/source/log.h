#ifndef __LOG_H__
#define	__LOG_H__

#include "types.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <boost/thread.hpp>

namespace hongseok {

using namespace hongseok::types;

class log
{
public:
	enum level
	{
		normal,
		notification,
		warning,
		error,
		critical,
	};// enum level

	enum output
	{
		console	= 0x01,
		file	= 0x02,
		network	= 0x04,
	};// enum output

	enum _end_marker
	{
		endl,
		flush,
	};// enum _flush

	log();
	~log();

	// all no throw
	log&	operator<<(const hs_bool8& val);
	log&	operator<<(const hs_int8& val);
	log&	operator<<(const hs_int16& val);
	log&	operator<<(const hs_int32& val);
	log&	operator<<(const hs_int64& val);
	log&	operator<<(const hs_uint8& val);
	log&	operator<<(const hs_uint16& val);
	log&	operator<<(const hs_uint32& val);
	log&	operator<<(const hs_uint64& val);
	log&	operator<<(const hs_float32& val);
	log&	operator<<(const hs_float64& val);
//	log&	operator<<(const hs_float96& val);
	log&	operator<<(const hs_char8* val);
	log&	operator<<(const std::string& val);
	log&    operator<<(const _end_marker& em);

	log&	lv(const level& l)	{ _level = l; return *this; }
	log&	lv_normal()			{ _level = normal; return *this; }
	log&	lv_notification()	{ _level = notification; return *this; }
	log&	lv_warning()		{ _level = warning; return *this; }
	log&	lv_error()			{ _level = error; return *this; }
	log&	lv_critical()		{ _level = critical; return *this; }
	void	set_global_level(const level& l)	{ _global_level = l; }

	bool	set_output(const output& o, const hs_char8* filename = 0, 
				const hs_char8* host = 0, const hs_char8* port = 0);	// no throw
	void	unset_output(const output& o);	// no throw

private:
	log&    _flush();	// no throw

private:
	std::ostringstream	_oss;
	level				_level;
	level				_global_level;
	hs_uint32			_output;

	boost::mutex		_mutex;

	std::ostream&		_console;
	std::ofstream		_file;
};// class log

extern log	g_log;

};// namespace hongseok

#define	L_NOR  	hongseok::g_log.lv_normal()
#define L_NOT   hongseok::g_log.lv_notification()
#define L_WAR   hongseok::g_log.lv_warning()
#define L_ERR   hongseok::g_log.lv_error()
#define L_CRI   hongseok::g_log.lv_critical()
#define	L_ENDL		hongseok::log::endl
#define	L_FLUSH		hongseok::log::flush

#endif // __LOG_H__
