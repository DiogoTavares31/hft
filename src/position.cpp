#include "position.h"
#include <cmath>
#include <algorithm>

Position::Position() 
    : symbol(""), quantity(0), avg_price(0), unrealized_pnl(0), realized_pnl(0) {
}

Position::Position(const std::string& sym) 
    : symbol(sym), quantity(0), avg_price(0), unrealized_pnl(0), realized_pnl(0) {
}

void Position::updatePosition(double qty, double price) {
    if (quantity == 0) {
        quantity = qty;
        avg_price = price;
    } else if ((quantity > 0 && qty > 0) || (quantity < 0 && qty < 0)) {
        // Same direction - update average price
        avg_price = (avg_price * quantity + price * qty) / (quantity + qty);
        quantity += qty;
    } else {
        // Opposite direction - realize P&L
        double close_qty = std::min(std::abs(qty), std::abs(quantity));
        double pnl = close_qty * (price - avg_price) * (quantity > 0 ? 1 : -1);
        realized_pnl += pnl;
        
        if (std::abs(qty) >= std::abs(quantity)) {
            // Position closed or reversed
            double remaining_qty = std::abs(qty) - std::abs(quantity);
            quantity = (qty > 0 ? remaining_qty : -remaining_qty);
            avg_price = (quantity != 0) ? price : 0;
        } else {
            // Partial close
            quantity += qty;
        }
    }
}

void Position::updateUnrealizedPnL(double current_price) {
    if (quantity != 0) {
        unrealized_pnl = quantity * (current_price - avg_price);
    } else {
        unrealized_pnl = 0;
    }
}