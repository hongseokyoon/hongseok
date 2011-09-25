#include "odbc.h"

#include "log.h"

namespace hongseok {
namespace odbc {

// Inner class declaration /////////////////////////////////////////////////////
// User cannot use handle, environment, connection and statement classes directly.
class handle
{
public:
	handle(SQLSMALLINT handle_type);
	virtual ~handle();

	hs_int32	get_last_error() const;
	std::string	get_error_str() const;

protected:
	SQLSMALLINT	_handle_type;
	SQLHANDLE	_handle;
	SQLRETURN	_last_error;
};// class handle

class connection;
class environment : public handle
{
friend class connection;

public:
	environment();
	~environment();
};// class environment

class statement;
class connection : public handle
{
friend class statement;

public:
	connection(boost::shared_ptr<environment> e);
	~connection();

	void	connect(std::string& dns, std::string& id, std::string& pwd, hs_int32 timeout = 5);
	void	disconnect();
};// class connection

class statement : public handle
{
public:
	statement(boost::shared_ptr<connection> c);
	~statement();

	void	execute(const std::string& query);
	void	get_data(SQLUSMALLINT column_number, 
					SQLSMALLINT target_type, 
					SQLPOINTER target_value, 
					SQLLEN buffer_length, 
					SQLLEN* indicator);
	void	bind_col(SQLUSMALLINT column_number,
					SQLSMALLINT target_type,
					SQLPOINTER target_value, 
					SQLLEN buffer_length, 
					SQLLEN* indicator);
	hs_int32	row_count();
	hs_int32	col_count();
	bool		next_row();
};// class statement

////////////////////////////////////////////////////////////////////////////////

handle::handle(SQLSMALLINT handle_type)
: _handle_type(handle_type), _last_error(SQL_SUCCESS)
{
}

handle::~handle()
{
}

hs_int32 handle::get_last_error() const
{
	return static_cast<hs_int32>(_last_error);
}

std::string handle::get_error_str() const
{
	SQLCHAR		state[1024];
	SQLINTEGER	native_error_code;
	SQLCHAR		message[1024];
	SQLSMALLINT	message_length;
	SQLRETURN	ret	= SQLGetDiagRec(_handle_type, _handle, 1,
						state, &native_error_code, 
						message, sizeof(message), &message_length);

	std::ostringstream	oss;
	oss	<< "[[RETURN: " << _last_error << "(" << ret << ")"
		<< " STATE: " << state 
		<< " CODE: " << native_error_code
		<< " MESSAGE: " << message
		<< "]]";

	return oss.str();
}

#define	SQLDBGMSG	if (!SQL_SUCCEEDED(_last_error)) L_NOR << get_error_str() << L_ENDL

environment::environment()
: handle(SQL_HANDLE_ENV)
{
	_last_error	= SQLAllocHandle(_handle_type, SQL_NULL_HANDLE, &_handle);
	SQLDBGMSG;
	_last_error	= SQLSetEnvAttr(_handle, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0);
	SQLDBGMSG;
}

environment::~environment()
{
	_last_error	= SQLFreeHandle(_handle_type, _handle);
	SQLDBGMSG;
}

connection::connection(boost::shared_ptr<environment> e)
: handle(SQL_HANDLE_DBC)
{
	_last_error	= SQLAllocHandle(_handle_type, e->_handle, &_handle);
	SQLDBGMSG;
}

connection::~connection()
{
	_last_error	= SQLFreeHandle(_handle_type, _handle);
	SQLDBGMSG;
}

void connection::connect(
	std::string& dns, std::string& id, std::string& pwd,
	hs_int32 timeout)
{
	_last_error	= SQLSetConnectAttr(_handle, SQL_LOGIN_TIMEOUT, (SQLPOINTER*)timeout, 0);
	SQLDBGMSG;
	_last_error	= SQLConnect(_handle,
					(SQLCHAR*)dns.c_str(), SQL_NTS,
					(SQLCHAR*)id.c_str(), SQL_NTS,
					(SQLCHAR*)pwd.c_str(), SQL_NTS);
	SQLDBGMSG;
}

void connection::disconnect()
{
	_last_error	= SQLDisconnect(_handle);
	SQLDBGMSG;
}

statement::statement(boost::shared_ptr<connection> c)
: handle(SQL_HANDLE_STMT)
{
	_last_error	= SQLAllocHandle(_handle_type, c->_handle, &_handle);
	SQLDBGMSG;
}

statement::~statement()
{
	_last_error	= SQLFreeHandle(_handle_type, _handle);
	SQLDBGMSG;
}

void statement::execute(const std::string& query)
{
	_last_error	= SQLExecDirect(_handle, (SQLCHAR*)"select * from sf_attendance", SQL_NTS);
	SQLDBGMSG;
}

void statement::get_data(SQLUSMALLINT column_number,
						SQLSMALLINT target_type, 
						SQLPOINTER target_value, 
						SQLLEN buffer_length, 
						SQLLEN* indicator)
{
	_last_error	= SQLGetData(_handle, column_number, 
							target_type, target_value, buffer_length, 
							indicator);
	SQLDBGMSG;
}

void statement::bind_col(SQLUSMALLINT column_number,
						SQLSMALLINT target_type,
						SQLPOINTER target_value, 
						SQLLEN buffer_length, 
						SQLLEN* indicator)
{
	_last_error	= SQLBindCol(_handle, column_number,
							target_type, target_value, buffer_length, 
							indicator);
	SQLDBGMSG;
}

hs_int32 statement::row_count()
{
	SQLLEN  row_count	= 0;
	_last_error	= SQLRowCount(_handle, &row_count);
	if (!SQL_SUCCEEDED(_last_error))	return -1;

	return row_count;
}

hs_int32 statement::col_count()
{
	SQLSMALLINT	col_count	= 0;
	_last_error	= SQLNumResultCols(_handle, &col_count);
	if (!SQL_SUCCEEDED(_last_error))	return -1;

	return col_count;
}

bool statement::next_row()
{
	_last_error	= SQLFetch(_handle);
	return SQL_SUCCEEDED(_last_error);
}

result::result(boost::shared_ptr<statement> stmt, const std::string& query)
: _stmt(stmt), _query(query)
{
}

result::~result()
{
}

const std::string& result::get_query() const
{
	return _query;
}

hs_int32 result::row_count()
{
	return _stmt->row_count();
}

hs_int32 result::col_count()
{
	return _stmt->col_count();
}

bool result::next_row()
{
	return _stmt->next_row();
}

hs_int32 result::get_last_error() const
{
	return _stmt->get_last_error();
}

std::string result::get_error_str() const
{
	return _stmt->get_error_str();
}

template <>
std::string result::get_data<std::string>(hs_uint16 col_no)
{
	SQLCHAR	val[1024];
	_stmt->get_data(col_no, SQL_C_CHAR, val, sizeof(val), 0);
	return reinterpret_cast<hs_char8*>(val);
}

#define	_GETDATA(RET_TYPE, SQL_GETDATA_TYPE, SQL_TYPE)	\
template<>\
RET_TYPE result::get_data<RET_TYPE>(hs_uint16 col_no)\
{\
	SQL_TYPE	val;\
	_stmt->get_data(col_no, SQL_GETDATA_TYPE, &val, 0, 0);\
	return val;\
}

_GETDATA(hs_int16, SQL_C_SHORT, SQLSMALLINT)
_GETDATA(hs_int32, SQL_C_LONG, SQLINTEGER)
_GETDATA(hs_float32, SQL_C_FLOAT, SQLREAL)
_GETDATA(hs_float64, SQL_C_DOUBLE, SQLDOUBLE)

#define _BINDCOL(RET_TYPE, SQL_GETDATA_TYPE, SQL_TYPE)	\
template<>\
void result::bind_col<RET_TYPE>(hs_uint16 col_no, RET_TYPE* val, hs_int32 val_size)\
{\
	_stmt->bind_col(col_no, SQL_GETDATA_TYPE, val, val_size, 0);\
}

_BINDCOL(hs_char8, SQL_C_CHAR, SQLCHAR);
_BINDCOL(hs_int16, SQL_C_SHORT, SQLSMALLINT)
_BINDCOL(hs_int32, SQL_C_LONG, SQLINTEGER)
_BINDCOL(hs_float32, SQL_C_FLOAT, SQLREAL)
_BINDCOL(hs_float64, SQL_C_DOUBLE, SQLDOUBLE)

manager::manager(const std::string& dns, const std::string& id, const std::string& pwd, hs_uint32 thread_num)
:_env(new environment()),
_dns(dns), _id(id), _pwd(pwd), 
_stop_worker(false)
{
	for (hs_uint32 i = 0; i < thread_num; ++i)
		_thread_group.create_thread(boost::bind(&manager::_worker, this));
}

manager::~manager()
{
	// Process remained requests before exit.
	while (true)
	{
		boost::mutex::scoped_lock	l(_requests_mutex);
		if (!_requests.empty())	_requests_condition.notify_one();
		else
		{
			_stop_worker	= true;
			_requests_condition.notify_all();
			break;
		}
	}
	_thread_group.join_all();
}

void manager::execute(const std::string& query, request_handler handler, void* user_data)
{
	_request_data	r = {query, handler, user_data};

	boost::mutex::scoped_lock	l(_requests_mutex);

	_requests.push(r);
	_requests_condition.notify_one();
}

void manager::execute(const std::string& query)
{
    //boost::shared_ptr<statement>    stmt(new statememt(dbc));
    //stmt->execute(query);
}

void manager::_worker()
{
	boost::shared_ptr<connection>	dbc(new connection(_env));
	dbc->connect(_dns, _id, _pwd);

	while (!_stop_worker)
	{
		_request_data	r;
		{
			boost::mutex::scoped_lock	l(_requests_mutex);
			if (_requests.empty())
			{
				_requests_condition.wait(l);
				if (_stop_worker)	break;
			}
			r	= _requests.front();
			_requests.pop();
		}

		// process query
		boost::shared_ptr<statement>	stmt(new statement(dbc));
		stmt->execute(r.query);
		if (r.handler != 0)
		{
			r.handler(result(stmt, r.query), r.user_data);
		}
	}

	dbc->disconnect();
}


};// namespace odbc
};// namespace hongseok
