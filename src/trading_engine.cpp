#include "trading_engine.h"
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>

TradingEngine::TradingEngine() : next_order_id(1) {
}

void TradingEngine::addStrategy(const std::string& symbol) {
    strategies[symbol] = TradingStrategy();
    positions[symbol] = Position(symbol);
}

void TradingEngine::processMarketData(const MarketData& data) {
    current_prices[data.symbol] = data.price;
    
    if (strategies.find(data.symbol) != strategies.end()) {
        strategies[data.symbol].updateData(data.price);
        std::string signal = strategies[data.symbol].generateSignal(data.price);
        
        if (signal != "HOLD") {
            processSignal(data.symbol, signal, data.price);
        }
        
        // Update unrealized P&L
        positions[data.symbol].updateUnrealizedPnL(data.price);
    }
}

void TradingEngine::processSignal(const std::string& symbol, const std::string& signal, double price) {
    if (signal == "BUY") {
        placeOrder(symbol, OrderSide::BUY, 100, price);
    } else if (signal == "SELL") {
        placeOrder(symbol, OrderSide::SELL, 100, price);
    } else if (signal == "CLOSE") {
        // Close position
        double pos_qty = positions[symbol].quantity;
        if (pos_qty > 0) {
            placeOrder(symbol, OrderSide::SELL, pos_qty, price);
        } else if (pos_qty < 0) {
            placeOrder(symbol, OrderSide::BUY, -pos_qty, price);
        }
    }
}

void TradingEngine::placeOrder(const std::string& symbol, OrderSide side, double quantity, double price) {
    Order order(next_order_id++, symbol, side, quantity, price);
    orders.push_back(order);
    
    // Simulate immediate fill for simplicity
    fillOrder(orders.size() - 1, price);
    
    // Simplified order notification
    std::cout << "🔔 " << (side == OrderSide::BUY ? "BOUGHT" : "SOLD") << " " 
              << std::setprecision(0) << quantity << " " << symbol 
              << " @ $" << std::fixed << std::setprecision(2) << price << std::endl;
}

void TradingEngine::fillOrder(size_t order_index, double fill_price) {
    Order& order = orders[order_index];
    order.status = OrderStatus::FILLED;
    
    double trade_qty = (order.side == OrderSide::BUY) ? order.quantity : -order.quantity;
    positions[order.symbol].updatePosition(trade_qty, fill_price);
}

void TradingEngine::printStatus() {
    system("cls"); // Clear screen on Windows (use "clear" on Linux/Mac)
    
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    auto local_tm = *std::localtime(&time_t);
    
    std::cout << "\n╔══════════════════════════════════════════════════════════════════════════════╗" << std::endl;
    std::cout << "║                              TRADING DASHBOARD                               ║" << std::endl;
    std::cout << "╠══════════════════════════════════════════════════════════════════════════════╣" << std::endl;
    std::cout << "║ Time: " << std::put_time(&local_tm, "%Y-%m-%d %H:%M:%S") << std::string(52, ' ') << "║" << std::endl;
    std::cout << "╠══════════════════════════════════════════════════════════════════════════════╣" << std::endl;
    
    // Market Data Header
    std::cout << "║ SYMBOL │   PRICE   │  CHANGE  │ POSITION │ AVG COST │ UNREALIZED │ REALIZED ║" << std::endl;
    std::cout << "╠════════╪═══════════╪══════════╪══════════╪══════════╪════════════╪══════════╣" << std::endl;
    
    double total_realized = 0, total_unrealized = 0;
    
    for (const auto& [symbol, pos] : positions) {
        double current_price = current_prices.count(symbol) ? current_prices.at(symbol) : 0;
        double price_change = current_price - pos.avg_price;
        double change_pct = pos.avg_price > 0 ? (price_change / pos.avg_price) * 100 : 0;
        
        // Color coding for positive/negative values
        std::string change_color = change_pct >= 0 ? "+" : "";
        std::string pnl_color = pos.unrealized_pnl >= 0 ? "+" : "";
        std::string realized_color = pos.realized_pnl >= 0 ? "+" : "";
        
        std::cout << "║ " << std::left << std::setw(6) << symbol << " │";
        std::cout << " $" << std::right << std::setw(8) << std::fixed << std::setprecision(2) << current_price << " │";
        std::cout << " " << change_color << std::setw(7) << std::setprecision(1) << change_pct << "% │";
        std::cout << " " << std::setw(8) << std::setprecision(0) << pos.quantity << " │";
        std::cout << " $" << std::setw(7) << std::setprecision(2) << pos.avg_price << " │";
        std::cout << " " << pnl_color << "$" << std::setw(8) << std::setprecision(2) << pos.unrealized_pnl << " │";
        std::cout << " " << realized_color << "$" << std::setw(7) << std::setprecision(2) << pos.realized_pnl << " ║" << std::endl;
        
        total_realized += pos.realized_pnl;
        total_unrealized += pos.unrealized_pnl;
    }
    
    std::cout << "╠════════╪═══════════╪══════════╪══════════╪══════════╪════════════╪══════════╣" << std::endl;
    
    double total_pnl = total_realized + total_unrealized;
    std::string total_color = total_pnl >= 0 ? "+" : "";
    
    std::cout << "║ TOTALS │           │          │          │          │";
    std::cout << " " << (total_unrealized >= 0 ? "+" : "") << "$" << std::setw(8) << std::setprecision(2) << total_unrealized << " │";
    std::cout << " " << (total_realized >= 0 ? "+" : "") << "$" << std::setw(7) << std::setprecision(2) << total_realized << " ║" << std::endl;
    
    std::cout << "╠══════════════════════════════════════════════════════════════════════════════╣" << std::endl;
    std::cout << "║ TOTAL P&L: " << total_color << "$" << std::setw(12) << std::setprecision(2) << total_pnl;
    std::cout << " │ ORDERS: " << std::setw(8) << orders.size() << " │ STATUS: RUNNING     ║" << std::endl;
    std::cout << "╚══════════════════════════════════════════════════════════════════════════════╝" << std::endl;
    
    // Recent activity
    if (!orders.empty()) {
        std::cout << "\n┌─ RECENT ACTIVITY ────────────────────────────────────────────────────────────┐" << std::endl;
        
        // Show last 3 orders
        int start_idx = std::max(0, (int)orders.size() - 3);
        for (int i = start_idx; i < orders.size(); ++i) {
            const auto& order = orders[i];
            auto order_time = std::chrono::system_clock::to_time_t(order.timestamp);
            auto order_tm = *std::localtime(&order_time);
            
            std::cout << "│ [" << std::put_time(&order_tm, "%H:%M:%S") << "] ";
            std::cout << (order.side == OrderSide::BUY ? "BUY " : "SELL") << " ";
            std::cout << std::setw(3) << std::setprecision(0) << order.quantity << " ";
            std::cout << std::setw(6) << order.symbol << " @ $";
            std::cout << std::setw(8) << std::setprecision(2) << order.price;
            std::cout << std::string(35, ' ') << "│" << std::endl;
        }
        
        std::cout << "└──────────────────────────────────────────────────────────────────────────────┘" << std::endl;
    }
    
    std::cout << "\nPress 'q' + Enter to stop..." << std::endl;
}