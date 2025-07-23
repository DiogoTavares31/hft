#pragma once
#include <string>
#include <chrono>

enum class OrderSide { BUY, SELL };
enum class OrderStatus { PENDING, FILLED, CANCELLED };

class Order {
public:
    int id;
    std::string symbol;
    OrderSide side;
    double quantity;
    double price;
    OrderStatus status;
    std::chrono::system_clock::time_point timestamp;
    
    Order(int order_id, const std::string& sym, OrderSide s, double qty, double p);
};