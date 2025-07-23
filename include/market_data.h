#pragma once
#include <string>
#include <chrono>

class MarketData {
public:
    std::string symbol;
    double price;
    double volume;
    std::chrono::system_clock::time_point timestamp;
    
    MarketData(const std::string& sym, double p, double v);
};