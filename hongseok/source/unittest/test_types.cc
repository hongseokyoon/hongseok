#include "types.h"
#include <gtest/gtest.h>

namespace {

class types_test : public ::testing::Test
{

};// class types_test : public ::testing::Test

TEST_F(types_test, types_size_check)
{
	using namespace hongseok::types;

	ASSERT_EQ(sizeof(hs_bool8),		1);
	ASSERT_EQ(sizeof(hs_int8),		1);
	ASSERT_EQ(sizeof(hs_int16),		2);
	ASSERT_EQ(sizeof(hs_int32),		4);
	ASSERT_EQ(sizeof(hs_int64),		8);
	ASSERT_EQ(sizeof(hs_uint8),		1);
	ASSERT_EQ(sizeof(hs_uint16),	2);
	ASSERT_EQ(sizeof(hs_uint32),	4);
	ASSERT_EQ(sizeof(hs_uint64),	8);
	ASSERT_EQ(sizeof(hs_float32),	4);
	ASSERT_EQ(sizeof(hs_float64),	8);
//	ASSERT_EQ(sizeof(hs_float96),	12);

	ASSERT_EQ(sizeof(hs_char8),		1);
}

}
