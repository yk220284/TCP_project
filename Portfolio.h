/*
    Portfolio.h
    1. Portfolio class with member functions that enable action regarding messages.
    2. Stock class to store information about position in the market regarding a particular stock.
    3. Note that buy_hypo_worst and sell_hypo_worst are not stored as they will be derived.
*/

#include "Message.h"
#include <unordered_map>
#include <vector>
#include <stdint.h>

#ifndef PORTFOLIO_H
#define PORTFOLIO_H
struct Stock
{
    uint64_t listingId;
    int64_t net_pos = 0;
    uint64_t buy_qty = 0;
    uint64_t sell_qty = 0;
    Stock() {}
    Stock(uint64_t listingId_) : listingId(listingId_) {}
};
struct Portfolio
{
    uint64_t buy_threshold;
    uint64_t sell_threshold;
    std::unordered_map<uint64_t, Stock> stocks;
    std::unordered_map<uint64_t, NewOrder> orders;
    Portfolio(uint64_t buy_threshold_, uint64_t sell_threshold_) : buy_threshold(buy_threshold_), sell_threshold(sell_threshold_) {}
    OrderResponse place_order(NewOrder const &new_order);
    void delete_order(DeleteOrder const &delete_order);
    void modify_order(ModifyOrderQuantity const &modify_order);
    void confirm_trade(Trade const &trade);
};

#endif // !PORTFOLIO_H