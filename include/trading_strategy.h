#pragma once
#include "technical_indicators.h"
#include <string>

class TradingStrategy {
private:
    TechnicalIndicators indicators;
    double last_signal_price;
    bool in_position;
    
public:
    TradingStrategy();
    
    void updateData(double price);
    std::string generateSignal(double current_price);
};