#pragma once
#include "market_data.h"
#include "order.h"
#include "position.h"
#include "trading_strategy.h"
#include <map>
#include <vector>
#include <string>

class TradingEngine {
private:
    std::map<std::string, Position> positions;
    std::vector<Order> orders;
    std::map<std::string, TradingStrategy> strategies;
    std::map<std::string, double> current_prices;
    int next_order_id;
    
public:
    TradingEngine();
    
    void addStrategy(const std::string& symbol);
    void processMarketData(const MarketData& data);
    void processSignal(const std::string& symbol, const std::string& signal, double price);
    void placeOrder(const std::string& symbol, OrderSide side, double quantity, double price);
    void fillOrder(size_t order_index, double fill_price);
    void printStatus();
};