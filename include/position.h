#pragma once
#include <string>

class Position {
public:
    std::string symbol;
    double quantity;
    double avg_price;
    double unrealized_pnl;
    double realized_pnl;
    
    // Default constructor
    Position();
    Position(const std::string& sym);
    
    void updatePosition(double qty, double price);
    void updateUnrealizedPnL(double current_price);
};