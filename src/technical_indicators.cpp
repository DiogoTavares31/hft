#include "C:\Users\diogo\Desktop\Personal\Code\hft\include\technical_indicators.h"
#include <cmath>

TechnicalIndicators::TechnicalIndicators(size_t window_size) : max_size(window_size) {
}

void TechnicalIndicators::addPrice(double price) {
    prices.push_back(price);
    if (prices.size() > max_size) {
        prices.erase(prices.begin());
    }
}

double TechnicalIndicators::getMA(int period) const {
    if (prices.size() < period) return 0;
    
    double sum = 0;
    for (int i = prices.size() - period; i < prices.size(); ++i) {
        sum += prices[i];
    }
    return sum / period;
}

double TechnicalIndicators::getRSI(int period) const {
    if (prices.size() < period + 1) return 50;
    
    double gains = 0, losses = 0;
    for (int i = prices.size() - period; i < prices.size(); ++i) {
        double change = prices[i] - prices[i-1];
        if (change > 0) gains += change;
        else losses -= change;
    }
    
    if (losses == 0) return 100;
    double rs = gains / losses;
    return 100 - (100 / (1 + rs));
}

std::pair<double, double> TechnicalIndicators::getBollingerBands(int period, double std_dev) const {
    if (prices.size() < period) return {0, 0};
    
    double ma = getMA(period);
    double variance = 0;
    
    for (int i = prices.size() - period; i < prices.size(); ++i) {
        variance += std::pow(prices[i] - ma, 2);
    }
    
    double std = std::sqrt(variance / period);
    return {ma - std_dev * std, ma + std_dev * std};
}