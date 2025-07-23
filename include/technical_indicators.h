#pragma once
#include <vector>

class TechnicalIndicators {
private:
    std::vector<double> prices;
    size_t max_size;
    
public:
    TechnicalIndicators(size_t window_size = 50);
    
    void addPrice(double price);
    double getMA(int period) const;
    double getRSI(int period = 14) const;
    std::pair<double, double> getBollingerBands(int period = 20, double std_dev = 2.0) const;
};