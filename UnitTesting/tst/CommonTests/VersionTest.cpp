#include "CommonTestHeaders.h"
#include "Version.h"

namespace Spock::Common::UnitTesting
{
	TEST(VersionTest, TestVersionsEqual) {
		auto v1 = Version(1, 0, 0);
		auto v2 = Version(1, 0, 0);
		EXPECT_EQ(v1, v2);
	}

	TEST(VersionTest, TestVersionsNotEqual) {
		auto v1 = Version(1, 0, 1);
		auto v2 = Version(1, 0, 0);
		EXPECT_NE(v1, v2);
	}

	TEST(VersionTest, TestVersionGreater) {
		auto v1 = Version(1, 0, 1);
		auto v2 = Version(1, 0, 0);
		EXPECT_GT(v1, v2);

		auto v3 = Version(1, 1, 0);
		EXPECT_GT(v3, v2);

		auto v4 = Version(2, 0, 0);
		EXPECT_GT(v4, v2);
	}

	TEST(VersionTest, TestVersionLesser) {
		auto v1 = Version(1, 0, 1);
		auto v2 = Version(1, 1, 0);
		EXPECT_LT(v1, v2);

		auto v3 = Version(0, 1, 0);
		EXPECT_LT(v3, v2);

		auto v4 = Version(1, 1, 1);
		EXPECT_LT(v3, v4);
	}
}