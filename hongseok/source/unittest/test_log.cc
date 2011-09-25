#include "log.h"
#include <gtest/gtest.h>

namespace {

class log_test : public ::testing::Test
{
};// class log_test : public ::testing::Test

TEST_F(log_test, genearl)
{
	using namespace hongseok;
	using namespace hongseok::types;

	hongseok::log	l;
//	l.flush();
}

}
