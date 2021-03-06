// prism_test.cpp : Defines the entry point for the console application.
//
#include <gtest/gtest.h>

int main(int argc, char* argv[])
{
	::testing::GTEST_FLAG(filter) = "TestIndicator.*:TestHLOCSeries.*:TestTimeSeries.*:TestPattern.*";//TestIndicator.*:TestPattern.*:TestHLOCSeries.*:
	//::testing::GTEST_FLAG(filter) = "TestIndicator.testCR";
	//::testing::GTEST_FLAG(filter) = "StrategyTest.testRun";
	//::testing::GTEST_FLAG(filter) = "StrategyTest.testScreener";
	//::testing::GTEST_FLAG(filter) = "StrategyTest.testMACDScreener";
	//::testing::GTEST_FLAG(filter) = "LibTest.*";
	//::testing::GTEST_FLAG(filter) = "ImportTest.testImportBlock";
	//::testing::GTEST_FLAG(filter) = "TestTimeSeries.testFindTurningPoints";
	//::testing::GTEST_FLAG(filter) = "ImportTest.testImportDir";
	//::testing::GTEST_FLAG(filter) = "ScreenerTest.*";

	::testing::GTEST_FLAG(filter) = "TradeTest.*:ScreenerTest.*:AssetTest.*";
	::testing::GTEST_FLAG(filter) = "StrategyTest.testParse";
	::testing::GTEST_FLAG(filter) = "StrategyRunnerTest.testSimple";
	//::testing::GTEST_FLAG(filter) = "AssetTest.testAssetIndexer";

	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

