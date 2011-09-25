#include "odbc.h"
#include <gtest/gtest.h>

namespace {

class odbc_test : public ::testing::Test
{
public:
	odbc_test()
	{
	}
};

TEST_F(odbc_test, type_size)
{
	using namespace hongseok;
	using namespace hongseok::types;

	::testing::StaticAssertTypeEq<SQLCHAR, hs_uint8>();
//	::testing::StaticAssertTypeEq<SQLINTEGER, hs_int32>();
	::testing::StaticAssertTypeEq<SQLSMALLINT, hs_int16>();
	::testing::StaticAssertTypeEq<SQLREAL, hs_float32>();
	::testing::StaticAssertTypeEq<SQLDOUBLE, hs_float64>();

	ASSERT_EQ(sizeof(SQLCHAR), sizeof(hs_uint8));
	ASSERT_EQ(sizeof(SQLSMALLINT), sizeof(hs_int16));
	ASSERT_EQ(sizeof(SQLINTEGER), sizeof(hs_int32));
	ASSERT_EQ(sizeof(SQLREAL), sizeof(hs_float32));
	ASSERT_EQ(sizeof(SQLDOUBLE), sizeof(hs_float64));
}

}
