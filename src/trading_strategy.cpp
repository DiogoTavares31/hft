#include "C:\Users\diogo\Desktop\Personal\Code\hft\include\trading_strategy.h"
#include <cmath>

TradingStrategy::TradingStrategy() : last_signal_price(0), in_position(false) {
}

void TradingStrategy::updateData(double price) {
    indicators.addPrice(price);
}

std::string TradingStrategy::generateSignal(double current_price) {
    double ma5 = indicators.getMA(5);
    double ma20 = indicators.getMA(20);
    double rsi = indicators.getRSI();
    auto [bb_lower, bb_upper] = indicators.getBollingerBands();
    
    if (ma5 == 0 || ma20 == 0) return "HOLD";
    
    // Mean reversion strategy with multiple indicators
    if (!in_position && current_price < bb_lower && rsi < 30 && ma5 < ma20) {
        in_position = true;
        last_signal_price = current_price;
        return "BUY";
    } else if (!in_position && current_price > bb_upper && rsi > 70 && ma5 > ma20) {
        in_position = true;
        last_signal_price = current_price;
        return "SELL";
    } else if (in_position && std::abs(current_price - last_signal_price) / last_signal_price > 0.02) {
        // Take profit/stop loss at 2%
        in_position = false;
        return "CLOSE";
    }
    
    return "HOLD";
}