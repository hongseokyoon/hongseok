#include <gtest/gtest.h>
#include "util.h"
#include "types.h"
#include <boost/thread.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <iostream>

namespace {

class util_test : public ::testing::Test
{
};// class util_test : public ::testing::Test

TEST_F(util_test, time_duration)
{
	using namespace hongseok::types;

	hs_int64	duration;
	hs_int64	total_duration	= 0;

	const int	test_count	= 1000;
	for (int i = 0; i < test_count; ++i)
	{
		TIME_DURATION(duration);
		total_duration	+= duration;
	}

	ASSERT_LE(total_duration / test_count, 5);

	TIME_DURATION(duration);

	ASSERT_GE(duration, 0);

	TIME_DURATION(duration)
		boost::this_thread::sleep(boost::posix_time::microseconds(1000));
	
	ASSERT_GE(duration, 1000);

	TIME_DURATION(duration)
		boost::this_thread::sleep(boost::posix_time::microseconds(2000));

	ASSERT_GE(duration, 2000);
}

class func_class
{
public:
    hongseok::types::hs_int32 func1()
    {
        return 1;
    }

    hongseok::types::hs_int32 func2(hongseok::types::hs_int32 i)
    {
        return i;
    }

    hongseok::types::hs_int32 func3(hongseok::types::hs_int32 a,
                                    hongseok::types::hs_int32 b)
    {
        return b;
    }
};// class func_class

TEST_F(util_test, invoker)
{
    func_class  fc;

    using namespace hongseok::util;

    ASSERT_EQ(invoker(&func_class::func1)(&fc), 1);
    ASSERT_EQ(invoker(&func_class::func2, 3)(&fc), 3);
    ASSERT_EQ(invoker(&func_class::func3, 2, 3)(&fc), 3);
}

}
