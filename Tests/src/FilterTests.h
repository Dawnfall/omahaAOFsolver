#pragma once

#include "Filter/HandFilter.h"

std::array<uint8_t, 4> GetHand(const std::string& c1, const std::string& c2, const std::string& c3, const std::string& c4)
{
	return {
		static_cast<uint8_t>(Card(c1)),
		static_cast<uint8_t>(Card(c2)),
		static_cast<uint8_t>(Card(c3)),
		static_cast<uint8_t>(Card(c4))
	};
}

bool CheckResult(bool expected, bool result, const std::string invalidMsg)
{
	if (expected != result)
	{
		std::cout << invalidMsg << std::endl;
		return false;
	}
	return true;
}

void RankSimpleTest()
{
	const std::string filterStr = "AKK";
	auto filter = HandFilter::ParseFilter(filterStr);

	auto hand1 = GetHand("Qd", "Ks", "As", "Kd");
	auto hand2 = GetHand("3s", "As", "Kd", "7d");

	bool result1 = filter->Validate(hand1);
	CheckResult(true, result1, "Rank test: Result1 invalid");

	bool result2 = filter->Validate(hand2);
	CheckResult(false, result2, "Rank test: Result2 invalid");
}

void SuitSimpleTest()
{
	const std::string filterStr = "csc";
	auto filter = HandFilter::ParseFilter(filterStr);

	auto hand1 = GetHand("Qc", "Kd", "As", "Kc");
	auto hand2 = GetHand("3c", "As", "Kd", "7d");

	bool result1 = filter->Validate(hand1);
	CheckResult(true, result1, "Suit test: Result1 invalid");

	bool result2 = filter->Validate(hand2);
	CheckResult(false, result2, "Suit test: Result2 invalid");
}

void MixSimpleTest()
{
	const std::string filterStr = "cQss4";
	auto filter = HandFilter::ParseFilter(filterStr);

	auto hand1 = GetHand("Qc", "Qd", "Qs", "4s");
	auto hand2 = GetHand("Qc", "4s", "3s", "8d");

	bool result1 = filter->Validate(hand1);
	CheckResult(true, result1, "MixSimple test: Result1 invalid");

	bool result2 = filter->Validate(hand2);
	CheckResult(false, result2, "MisSimple test: Result2 invalid");
}

void NegateTest()
{
	const std::string filterStr = "!sQc";
	auto filter = HandFilter::ParseFilter(filterStr);

	auto hand1 = GetHand("Qc", "Qh", "3d", "2d");
	auto hand2 = GetHand("Qd", "Qc", "4d", "3s");

	bool result1 = filter->Validate(hand1);
	CheckResult(true, result1, "Negate test: Result1 invalid");

	bool result2 = filter->Validate(hand2);
	CheckResult(false, result2, "Negate test: Result2 invalid");
}

void OrTest()
{
	const std::string filterStr = "A,ss,QQ";
	auto filter = HandFilter::ParseFilter(filterStr);

	auto hand1 = GetHand("Qd", "4s", "Qd", "3s");
	auto hand2 = GetHand("Kd", "5s", "Qd", "3d");

	bool result1 = filter->Validate(hand1);
	CheckResult(true, result1, "Or test: Result1 invalid");

	bool result2 = filter->Validate(hand2);
	CheckResult(false, result2, "Or test: Result2 invalid");
}


void AndOrTest()
{
	const std::string filterStr = "T:ss,dd";
	auto filter = HandFilter::ParseFilter(filterStr);

	auto hand1 = GetHand("Qd", "4s", "Td", "3s");
	auto hand2 = GetHand("Kd", "5s", "Td", "3d");
	auto hand3 = GetHand("Kh", "5s", "Td", "3d");

	bool result1 = filter->Validate(hand1);
	CheckResult(true, result1, "AndOr test: Result1 invalid");

	bool result2 = filter->Validate(hand2);
	CheckResult(true, result2, "AndOr test: Result2 invalid");

	bool result3 = filter->Validate(hand3);
	CheckResult(false, result2, "AndOr test: Result3 invalid");
}

void DoubleNegateTest()
{
	const std::string filterStr = "!KK:!J";
	auto filter = HandFilter::ParseFilter(filterStr);

	auto hand1 = GetHand("Qd", "7s", "Kd", "4s");
	auto hand2 = GetHand("Kc", "7s", "Kd", "2s");
	auto hand3 = GetHand("8c", "7s", "Kd", "Js");


	bool result1 = filter->Validate(hand1);
	CheckResult(true, result1, "DoubleNegate test: Result1 invalid");
	bool result2 = filter->Validate(hand2);
	CheckResult(false, result2, "DoubleNegate test: Result2 invalid");
	bool result3 = filter->Validate(hand3);
	CheckResult(false, result3, "DoubleNegate test: Result3 invalid");
}

void Complex1Test()
{
	const std::string filterStr = "!KK:!J:ss:!As";
	auto filter = HandFilter::ParseFilter(filterStr);

	auto hand1 = GetHand("Qd", "7s", "Kd", "4s");
	auto hand2 = GetHand("Kc", "7s", "Kd", "2s");
	auto hand3 = GetHand("8c", "As", "Kd", "Js");
	auto hand4 = GetHand("8c", "8s", "Kd", "3d");



	bool result1 = filter->Validate(hand1);
	CheckResult(true, result1, "Complex1 test: Result1 invalid");
	bool result2 = filter->Validate(hand2);
	CheckResult(false, result2, "Complex1 test: Result2 invalid");
	bool result3 = filter->Validate(hand3);
	CheckResult(false, result3, "Complex1 test: Result3 invalid");
	bool result4 = filter->Validate(hand4);
	CheckResult(false, result4, "Complex1 test: Result4 invalid");
}