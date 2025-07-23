#include "C:\Users\diogo\Desktop\Personal\Code\hft\include\market_data.h"

MarketData::MarketData(const std::string& sym, double p, double v) 
    : symbol(sym), price(p), volume(v), timestamp(std::chrono::system_clock::now()) {
}