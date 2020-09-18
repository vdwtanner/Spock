#include "CommonTestHeaders.h"

#include "Common/Functional/Optional.h"

namespace Spock::Common::UnitTesting
{
	struct Nullable {};

	struct HelperMock
	{
		MOCK_METHOD(void, DoApply, (int), ());
		MOCK_METHOD(int, DoMap, (int), ());
		MOCK_METHOD(void, DoApplyNullable, (Nullable), ());
		MOCK_METHOD(int, DoMapNullable, (Nullable), ());
		MOCK_METHOD(int, DoMapNullablePointer, (Nullable*), ());
	};

	TEST(OptionTest, Empty_Apply_NotExecuted) {
		HelperMock mock;
		auto sut = Optional<int>::empty();
		EXPECT_CALL(mock, DoApply).Times(0);

		auto lambda = [&](int x) {mock.DoApply(x); };
		sut.Apply(lambda);
	}

	TEST(OptionTest, NonEmpty_Apply_Executed) {
		HelperMock mock;
		auto sut = Optional<int>::of(1);
		EXPECT_CALL(mock, DoApply).Times(1);

		auto lambda = [&](int x) {mock.DoApply(x); };
		sut.Apply(lambda);
	}

	TEST(OptionTest, Empty_InstanceApply_NotExecuted) {
		HelperMock mock;
		auto sut = Optional<int>::empty();
		EXPECT_CALL(mock, DoApply).Times(0);

		sut.Apply(&mock, &HelperMock::DoApply);
	}

	TEST(OptionTest, NonEmpty_InstanceApply_Executed) {
		HelperMock mock;
		auto sut = Optional<int>::of(1);
		EXPECT_CALL(mock, DoApply).Times(1);

		sut.Apply(&mock, &HelperMock::DoApply);
	}

	TEST(OptionTest, Empty_InstanceMap_NotExecuted) {
		HelperMock mock;
		auto sut = Optional<int>::empty();
		EXPECT_CALL(mock, DoMap).Times(0);

		sut.Map<int>(&mock, &HelperMock::DoMap);
	}

	TEST(OptionTest, NonEmpty_InstanceMapNotExecuted) {
		HelperMock mock;
		auto sut = Optional<int>::of(1);
		EXPECT_CALL(mock, DoMap).Times(1);

		sut.Map<int>(&mock, &HelperMock::DoMap);
	}

	TEST(OptionTest, OfNullable_InstanceMap_NotExecuted) {
		HelperMock mock;
		auto sut = Optional<Nullable>::ofNullable(nullptr);

		EXPECT_CALL(mock, DoMapNullable).Times(0);

		sut.Map<int>(&mock, &HelperMock::DoMapNullable);
	}

	TEST(OptionTest, OfNullable_InstanceMap_Executed) {
		HelperMock mock;
		Nullable n;
		auto sut = Optional<Nullable>::ofNullable(&n);

		EXPECT_CALL(mock, DoMapNullable).Times(1);

		sut.Map<int>(&mock, &HelperMock::DoMapNullable);
	}

	TEST(OptionTest, Empty_Get_throws) {
		HelperMock mock;
		auto sut = Optional<int>::empty();

		EXPECT_ANY_THROW(sut.Get());
	}

	TEST(OptionTest, NonEmpty_Get_ReturnsValue) {
		auto sut = Optional<int>::of(6);

		EXPECT_EQ(6, sut.Get());
	}

	TEST(OptionTest, Empty_GetOr_ReturnsOr) {
		auto sut = Optional<int>::empty();

		EXPECT_EQ(1, sut.GetOr(1));
	}

	TEST(OptionTest, NonEmpty_GetOr_ReturnsValue) {
		auto sut = Optional<int>::of(6);

		EXPECT_EQ(6, sut.GetOr(1));
	}

	TEST(OptionTest, ChainedMapCalls) {
		auto mult2 = [](int x) {return x * 2; };
		auto castToFloat = [](int x) {return (float)x; };
		auto toString = [](float x) {return "Final value is " + std::to_string(x); };
		auto sut = Optional<int>::of(5);

		auto str = sut.Map<int>(mult2)
			.Map<float>(castToFloat)
			.Map<std::string>(toString);

		EXPECT_EQ(std::string("Final value is 10.000000"), str.Get());
	}

	TEST(OptionTest, Empty_Assignment_ofValue) {
		auto sut = Optional<int>::empty();
		sut = 6;

		EXPECT_EQ(6, sut.Get());
	}

	TEST(OptionTest, Empty_Assignment_ofOptional) {
		auto sut = Optional<int>::empty();
		sut = Optional<int>::of(6);

		EXPECT_EQ(6, sut.Get());
	}

	TEST(OptionTest, NonEmpty_Comparisons_rhsIsOptional) {
		auto sut = Optional<int>::of(5);

		EXPECT_EQ(sut, Optional<int>::of(5));
		EXPECT_NE(sut, Optional<int>::of(6));
		EXPECT_NE(sut, Optional<int>::empty());
		EXPECT_LT(sut, Optional<int>::of(6));
		EXPECT_GT(sut, Optional<int>::of(4));
		EXPECT_LE(sut, Optional<int>::of(6));
		EXPECT_LE(sut, Optional<int>::of(5));
		EXPECT_GE(sut, Optional<int>::of(4));
		EXPECT_GE(sut, Optional<int>::of(5));
	}

	TEST(OptionTest, NonEmpty_Comparisons_rhsIsValue) {
		auto sut = Optional<int>::of(5);

		EXPECT_EQ(sut, 5);
		EXPECT_NE(sut, 4);
		EXPECT_LT(sut, 6);
		EXPECT_GT(sut, 4);
		EXPECT_LE(sut, 6);
		EXPECT_LE(sut, 5);
		EXPECT_GE(sut, 4);
		EXPECT_GE(sut, 5);
	}

	TEST(OptionTest, Empty_Comparisions) {
		auto sut = Optional<int>::empty();

		EXPECT_EQ(sut, Optional<int>::empty());
		EXPECT_NE(sut, Optional<int>::of(1));
		EXPECT_NE(sut, 1);
		EXPECT_LT(sut, 0);
		EXPECT_LT(sut, Optional<int>::of(1));
		EXPECT_LE(sut, 0);
		EXPECT_LE(sut, Optional<int>::of(1));
		EXPECT_LE(sut, Optional<int>::empty());
		EXPECT_GE(sut, Optional<int>::empty());
	}
}