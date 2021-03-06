// Copyright 2013, QT Inc.
// All rights reserved.
//
// Author: wndproc@gmail.com (Ray Ni)
//
// routines and classes for strategy runner


#ifndef STRATEGY_RUNNER_H
#define STRATEGY_RUNNER_H

#include "common.h"
#include "util.h"
#include "time.h"
#include "asset.h"
#include "trade.h"
#include "strategy.h"
#include "screener.h"
#include <string>
#include <vector>
#include <map>

namespace prism {
		
	class RunnerObserver
	{
	public:
		RunnerObserver();
		~RunnerObserver();
	public:
		void OnTransaction(const Transaction& trans);
		void OnStart(Strategy* strategy);
		void OnCycleBegin(time_t cursor);
		void OnCycleEnd(time_t cursor);
		void OnFinished();
		void Clear() { performance_.clear(); }
	public:
		DoubleTimeList& performance() { return performance_; }
	private:
		DoubleTimeList performance_;
	};

	// the strategy runner
	class StrategyRunner
	{
	public:
		StrategyRunner(const std::shared_ptr<Strategy>& strategy,
			const std::shared_ptr<AssetsProvider>& assets_provider,
			const std::shared_ptr<PortfolioManager>& portfolio_manager,
			const std::shared_ptr<TransactionManager>& transaction_manager,
			const std::shared_ptr<RunnerObserver>& runner_observer): 
			strategy_(strategy), assets_provider_(assets_provider), 
			portfolio_manager_(portfolio_manager), transaction_manager_(transaction_manager),
			runner_observer_(runner_observer)
		{
			attach_screener_ = std::make_unique<Screener>();
			detach_screener_ = std::make_unique<Screener>();
			cash_box_ = std::make_unique<CashBox>();
		}
		~StrategyRunner(){}
	public:
		// if reload = true, the assets_provider will re-load assets according to the strategy->stocks
		// return false if there are no assets to test against
		bool Init(bool reload = false);
		void Run();
		void Clear();
	private:
		void InitIndexer();
		void MoveToCursor();
		void ForwardToCursor();
		void Screen();
		void Trade();
		void Detach(const std::string& symbol);
		bool Attach(double money);
	private:
		std::shared_ptr<Strategy> strategy_;
		std::shared_ptr<AssetsProvider> assets_provider_;
		std::shared_ptr<PortfolioManager> portfolio_manager_;
		std::shared_ptr<TransactionManager> transaction_manager_;
		std::shared_ptr<RunnerObserver> runner_observer_;
		AssetIndexerList asset_indexer_list_;
		time_t cursor_;
		double cash_;
		std::unique_ptr<Screener> attach_screener_;
		std::unique_ptr<Screener> detach_screener_;
		std::vector<int> to_attach_;
		std::vector<int> to_detach_;
		std::unique_ptr<CashBox> cash_box_;
	};

}

#endif