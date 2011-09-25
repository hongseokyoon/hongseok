#define	private	public
#define	protected public
#include "iobuffer.h"
#include <gtest/gtest.h>
#include <string>

namespace {

class iobuffer_test : public ::testing::Test
{
public:
	iobuffer_test()
	{
	}

	void	set()
	{
		_b8		= false;
		_i8		= -10;
		_i16	= -100;
		_i32	= -1000;
		_i64	= -10000;
		_ui8	= 10;
		_ui16	= 100;
		_ui32	= 1000;
		_ui64	= 10000;
		_f32	= -123.123;
		_f64	= 1234.1234;
//		_f96	= 1.23456789;
		strcpy(_cstr, "Hongseok Yoon");
		_str	= "Karo Ha";
	}

	void	unset()
	{
		_b8		= true;
		_i8		= 0;
		_i16	= 0;
		_i32	= 0;
		_i64	= 0;
		_ui8	= 0;
		_ui16	= 0;
		_ui32	= 0;
		_ui64	= 0;
		_f32	= 0;
		_f64	= 0;
//		_f96	= 0;
		strcpy(_cstr, "");
		_str	= "";
	}

private:
	hs_bool8	_b8;
	hs_int8		_i8;
	hs_int16	_i16;
	hs_int32	_i32;
	hs_int64	_i64;
	hs_uint8	_ui8;
	hs_uint16	_ui16;
	hs_uint32	_ui32;
	hs_uint64	_ui64;
	hs_float32	_f32;
	hs_float64	_f64;
//	hs_float96	_f96;
	hs_char8	_cstr[64];
	std::string	_str;
};// class buffer_test : public ::testing::Test

TEST_F(iobuffer_test, general)
{
	using namespace hongseok;
	using namespace hongseok::types;

	hs_uint8	buffer[1024];

	set();
	obuffer<>	ob(buffer, 1024);
	ASSERT_EQ(buffer, ob._data);
	ASSERT_EQ(1024, ob._size);
	ASSERT_EQ(0, ob._pos);
	ob	<< _b8 
		<< _i8 << _i16 << _i32 << _i64
		<< _ui8 << _ui16 << _ui32 << _ui64
		<< _f32 << _f64// << _f96
		<< _cstr << _str;

	ASSERT_EQ(65, ob._pos);

	unset();
	ibuffer<>	ib(buffer, 65);
	ASSERT_EQ(buffer, ib._data);
	ASSERT_EQ(65, ib._size);
	ASSERT_EQ(0, ib._pos);
	ib	>> _b8 
		>> _i8 >> _i16 >> _i32 >> _i64
		>> _ui8 >> _ui16 >> _ui32 >> _ui64
		>> _f32 >> _f64// >> _f96
		>> _cstr >> _str;
	//ASSERT_EQ(false, _b8);
    ASSERT_FALSE(_b8);
	ASSERT_EQ(-10, _i8);
	ASSERT_EQ(-100, _i16);
	ASSERT_EQ(-1000, _i32);
	ASSERT_EQ(-10000, _i64);
	ASSERT_EQ(10, _ui8);
	ASSERT_EQ(100, _ui16);
	ASSERT_EQ(1000, _ui32);
	ASSERT_EQ(10000, _ui64);
	ASSERT_FLOAT_EQ(-123.123, _f32);
	ASSERT_DOUBLE_EQ(1234.1234, _f64);
//	ASSERT_DOUBLE_EQ(1.23456789, _f96);
	ASSERT_STREQ("Hongseok Yoon", _cstr);
	ASSERT_EQ("Karo Ha", _str);
	ASSERT_EQ(65, ib._pos);
}

}
