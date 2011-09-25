#include "buffer.h"
#include <gtest/gtest.h>

namespace {

class buffer_test : public ::testing::Test
{
};

TEST_F(buffer_test, general)
{
	using namespace hongseok;

	const hs_uint8	sample[]	= "0123456789";
	const hs_uint32	sample_size	= 10;

	buffer	b;

	ASSERT_EQ(0, b.readable_size());
	ASSERT_EQ(buffer::capacity, b.writable_size());

	ASSERT_EQ(0, b.read(1));

	ASSERT_TRUE(b.write(sample, sample_size));
	ASSERT_EQ(sample_size, b.readable_size());
	ASSERT_EQ(buffer::capacity - sample_size, b.writable_size());
	
	ASSERT_TRUE(0 != b.read(1));
	ASSERT_EQ(sample_size - 1, b.readable_size());
	ASSERT_EQ(buffer::capacity - sample_size, b.writable_size());

	b.normalize();
	ASSERT_EQ(sample_size - 1, b.readable_size());
	ASSERT_EQ(buffer::capacity - sample_size + 1, b.writable_size());

	ASSERT_EQ(0, b.read(10));
	ASSERT_EQ(sample_size - 1, b.readable_size());
	ASSERT_EQ(buffer::capacity - sample_size + 1, b.writable_size());

	ASSERT_TRUE(NULL != b.read(9));
	ASSERT_EQ(0, b.readable_size());
	ASSERT_EQ(buffer::capacity - sample_size + 1, b.writable_size());

	b.normalize();
	ASSERT_EQ(0, b.readable_size());
	ASSERT_EQ(buffer::capacity, b.writable_size());
}

}
