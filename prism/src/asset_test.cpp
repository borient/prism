#include <gtest/gtest.h>
#include "asset.h"
#include "store.h"
#include "util.h"
#include "indicator.h"
#include <iostream>

using namespace prism;

const std::string kDataPath = "D:\\project\\prism\\data\\";

class AssetTest : public testing::Test
{
public:
	KCStore store;
	virtual void SetUp()
	{
		bool ret = store.Open(kDataPath + "TestData8.kch");
		EXPECT_TRUE(ret);
	}

	virtual void TearDown()
	{
		store.Close();
	}
};

TEST_F(AssetTest, testLoad)
{
	std::string symbols;
	bool ret = store.GetBlock("�������\\A�ɰ��", symbols);
	EXPECT_TRUE(ret);

	std::vector<std::string> elems;
	kyotocabinet::strsplit(symbols, '\n', &elems);
	
	AssetsProvider loader((IStore*)&store);
	int count = loader.LoadAssets(elems, 2011, 2013);
	std::cout << "symbols loaded: " << count << std::endl;
}

TEST_F(AssetTest, testLoadAll)
{
	AssetsProvider loader((IStore*)&store);
	int count = loader.LoadAll();

	std::cout << "symbols loaded: " << count << std::endl;
	
}

TEST_F(AssetTest, testLoadPatterns)
{
	AssetsProvider loader((IStore*)&store);
	// load symbols in the blocks and match the patterns...
	int count = loader.LoadAssets("patterns=SH600\\d{3},SZ001\\d{3};blocks=��ҵ\\ҽҩ,��ҵ\\�����", 2010, 2012);

	std::cout << "symbols loaded: " << count << std::endl;

}

TEST_F(AssetTest, testScales)
{
	AssetsProvider loader((IStore*)&store);

	std::string symbol = "SH600198";
	std::string another_symbol = "SZ002015";
	std::vector<std::string> elems;
	elems.push_back(symbol);
	elems.push_back(another_symbol);
	int count = loader.LoadAssets(elems, 2011, 2013);

	Asset* asset = loader.Get(symbol);
	EXPECT_TRUE(asset != nullptr);
	AssetScale* daily = asset->scales(DATA_TYPE_DAILY, 1);
	EXPECT_TRUE(daily != nullptr);

	AssetScale* two_days = asset->scales(DATA_TYPE_DAILY, 2);
	EXPECT_TRUE(two_days != nullptr);
	AssetScale* weekly = asset->scales(DATA_TYPE_WEEKLY, 1);
	EXPECT_TRUE(weekly != nullptr);
	AssetScale* two_weeks = asset->scales(DATA_TYPE_WEEKLY, 2);
	EXPECT_TRUE(two_weeks != nullptr);
	AssetScale* monthly = asset->scales(DATA_TYPE_MONTHLY, 1);
	EXPECT_TRUE(monthly != nullptr);
	AssetScale* two_months = asset->scales(DATA_TYPE_MONTHLY, 2);
	EXPECT_TRUE(two_months != nullptr);

	MACD* macd = (MACD*)daily->indicators("MACD_12_26_9");
	EXPECT_TRUE(macd != nullptr);
	EMA* ema = (EMA*)weekly->indicators("EMA_10");
	EXPECT_TRUE(ema != nullptr);
	macd = (MACD*)weekly->indicators("MACD_12_26_9");
	EXPECT_TRUE(macd != nullptr);
	CR* cr = (CR*)two_days->indicators("CR_20");
	EXPECT_TRUE(cr != nullptr);

	// try another symbol
	asset = loader.Get(another_symbol);
	EXPECT_TRUE(asset != nullptr);
	daily = asset->scales(DATA_TYPE_DAILY, 1);
	EXPECT_TRUE(daily != nullptr);
	ema = (EMA*)daily->indicators("EMA_30");
	EXPECT_TRUE(ema != nullptr);

}

TEST_F(AssetTest, testAssetIndexer)
{
	AssetsProvider loader((IStore*)&store);

	std::string symbol = "SH600198";
	std::string another_symbol = "SZ002015";
	std::vector<std::string> elems;
	elems.push_back(symbol);
	elems.push_back(another_symbol);
	int count = loader.LoadAssets(elems, 2011, 2013);

	Asset* asset = loader.Get(symbol);
	AssetIndexer *asset_indexer = new AssetIndexer(asset);
	time_t day = StringToDate("2012-12-21", "%d-%d-%d");
	for (int i = 0; i < 33; i++)
	{
		day = day + 24 * 3600;		
		asset_indexer->ForwardTo(day);
		time_t date = asset_indexer->GetIndexTime();
		std::string date_str = TimeToString(date, "%Y-%m-%d");
		std::cout << date_str << std::endl;
	}

	// try multiple scales
	AssetScaleIndexer* scale_indexer_two_days = asset_indexer->scale_indexers(DATA_TYPE_DAILY, 2);
	AssetScaleIndexer* scale_indexer_weekly = asset_indexer->scale_indexers(DATA_TYPE_WEEKLY, 1);
	
	asset_indexer->ForwardTo(day);
	std::string date_str = TimeToString(day, "%Y-%m-%d");
	std::cout << "move to: " << date_str << std::endl;
	
	time_t date = scale_indexer_two_days->GetIndexTime();
	date_str = TimeToString(date, "%Y-%m-%d");
	std::cout << date_str << std::endl;

	date = scale_indexer_weekly->GetIndexTime();
	date_str = TimeToString(date, "%Y-%m-%d");
	std::cout << date_str << std::endl;

}