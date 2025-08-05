#include "order.h"

Order::Order(int order_id, const std::string& sym, OrderSide s, double qty, double p)
    : id(order_id), symbol(sym), side(s), quantity(qty), price(p), 
      status(OrderStatus::PENDING), timestamp(std::chrono::system_clock::now()) {
}