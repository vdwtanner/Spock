#include "CppUnitTest.h"
#include "Version.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Spock::Common;

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	template<> static std::wstring ToString<Version>(const Version& v) { 
		RETURN_WIDE_STRING("Version(" << v.major << ", " << v.minor << ", " << v.patch << ")");
	}
}
namespace UnitTesting::Common
{
	TEST_CLASS(VersionTest)
	{
	public:

		TEST_METHOD(TestVersionsEqual) {
			auto v1 = Version(1,0,0);
			auto v2 = Version(1, 0, 0);
			Assert::AreEqual(v1, v2);
		}

		TEST_METHOD(TestVersionsNotEqual) {
			auto v1 = Version(1, 0, 1);
			auto v2 = Version(1, 0, 0);
			Assert::IsTrue(v1 != v2);
		}

		TEST_METHOD(TestVersionGreater) {
			auto v1 = Version(1, 0, 1);
			auto v2 = Version(1, 0, 0);
			Assert::IsTrue(v1 > v2);
		}

		TEST_METHOD(TestVersionLesser) {
			auto v1 = Version(1, 0, 1);
			auto v2 = Version(1, 1, 0);
			Assert::IsTrue(v1 < v2);
		}
	};
}