#include "parser.h"
#include <gtest/gtest.h>

namespace {

class parser_test : public ::testing::Test
{
};// class parser_test : publci ::testing::Test

TEST_F(parser_test, general)
{
	using namespace hongseok;
	using namespace hongseok::types;
//                             0          1         2         3
//                             012345678901234567890123456789012345678
	hs_char8	source[]	= "1234header.......trailer......header...";
/*
	connection::range_t	r;
	
	r	= parser::read_all(source, sizeof(source));
	ASSERT_EQ(0, r.first);
	ASSERT_EQ(40, r.second);

	r	= parser::read_begin_with(source, sizeof(source), "header", 6);
	ASSERT_EQ(4, r.first);
	ASSERT_EQ(29, r.second);

	r	= parser::read_end_with(source, sizeof(source), "trailer", 7);
	ASSERT_EQ(0, r.first);
	ASSERT_EQ(23, r.second);

	r	= parser::read_begin_with(source, sizeof(source), "qq", 2);
	ASSERT_EQ(38, r.first);
	ASSERT_EQ(38, r.second);

	r	= parser::read_end_with(source, sizeof(source), "qq", 2);
	ASSERT_EQ(0, r.first);
	ASSERT_EQ(0, r.second);
*/
}

}
