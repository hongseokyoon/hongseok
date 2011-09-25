#ifndef __ODBC_H__
#define	__ODBC_H__

#include "types.h"

// unixODBC
#include <sql.h>
#include <sqlext.h>
#include <sqltypes.h>

#include <queue>

#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <boost/thread/condition.hpp>

using namespace hongseok::types;

namespace hongseok {
namespace odbc {

class environment;
class statement;

class result
{
public:
	result(boost::shared_ptr<statement> stmt, 
			const std::string& query);
	~result();

	const std::string&	get_query() const;
	hs_int32			row_count();
	hs_int32			col_count();
	bool				next_row();

	template <typename T>
	T	get_data(hs_uint16 col_no);

	template <typename T>
	void	bind_col(hs_uint16 col_no, 
				T* val, hs_int32 val_size = 0);

	hs_int32	get_last_error() const;
	std::string	get_error_str() const;

private:
	boost::shared_ptr<statement>	_stmt;
	std::string						_query;
};// class result

class manager
{
public:
	typedef	void(*request_handler)(result, void*);

	manager(const std::string& dns, 
			const std::string& id, 
			const std::string& pwd, 
			const hs_uint32 thread_num);
	~manager();

	void	execute(const std::string& query, 
					request_handler handler, 
					void* user_data);

    void    execute(const std::string& query);

private:
	struct _request_data
	{
		std::string		query;
		request_handler	handler;
		void*			user_data;
	};// struct _request_data

	void	_worker();

private:
	boost::shared_ptr<environment>	_env;

	std::string						_dns;
	std::string						_id;
	std::string						_pwd;

	boost::thread_group				_thread_group;

	std::queue<_request_data>		_requests;
	boost::mutex					_requests_mutex;
	boost::condition				_requests_condition;

	hs_bool8						_stop_worker;
};// class manager

};// namespace odbc
};// namespace hongseokW

#endif//__ODBC_H__
