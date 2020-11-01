#include "Portfolio.h"
#include <algorithm>
#include <cassert>

OrderResponse Portfolio::place_order(NewOrder const &new_order)
{
    OrderResponse resp;
    resp.orderId = new_order.orderId;
    if (stocks.find(new_order.listingId) == stocks.end())
    {
        // First time ordering this instrument.

        stocks[new_order.listingId] = Stock(new_order.listingId);
    }
    Stock &target_stock = stocks[new_order.listingId];

    if (new_order.side == 'b')
    {
        uint64_t buy_hypo_worst = std::max(target_stock.buy_qty + new_order.orderQuantity + target_stock.net_pos,
                                           target_stock.buy_qty + new_order.orderQuantity);
        if (buy_hypo_worst > buy_threshold)
        {
            //     resp.status = OrderResponse::Status::REJECTED;
            resp.status = 1;
        }
        else
        {
            // resp.status = OrderResponse::Status::ACCEPTED;
            resp.status = 0;
            target_stock.buy_qty += new_order.orderQuantity;
            orders[new_order.orderId] = new_order;
        }
    }
    else
    {
        assert(new_order.side == 's');
        uint64_t sell_hypo_worst = std::max(target_stock.sell_qty + new_order.orderQuantity - target_stock.net_pos,
                                            target_stock.sell_qty + new_order.orderQuantity);
        if (sell_hypo_worst > sell_threshold)
        {
            // resp.status = OrderResponse::Status::REJECTED;
            resp.status = 1;
        }
        else
        {
            // resp.status = OrderResponse::Status::ACCEPTED;
            resp.status = 0;
            target_stock.sell_qty += new_order.orderQuantity;
            orders[new_order.orderId] = new_order;
        }
    }
    return resp;
}

void Portfolio::delete_order(DeleteOrder const &delete_order)
{
    if (orders.find(delete_order.orderId) != orders.end())
    {
        NewOrder &order_to_delet = orders[delete_order.orderId];
        if (order_to_delet.side == 'b')
        {
            stocks[order_to_delet.listingId].buy_qty -= order_to_delet.orderQuantity;
        }
        else
        {
            stocks[order_to_delet.listingId].sell_qty -= order_to_delet.orderQuantity;
        }
        orders.erase(delete_order.orderId);
    }
}

void Portfolio::modify_order(ModifyOrderQuantity const &modify_order)
{
    if (orders.find(modify_order.orderId) == orders.end())
    {
        // Non-exiting order.
        return;
    }
    NewOrder &order_to_modify = orders[modify_order.orderId];
    Stock &target_stock = stocks[order_to_modify.listingId];
    if (order_to_modify.orderQuantity >= modify_order.newQuantity)
    {
        // Make quantity smaller, always ok.
        target_stock.buy_qty += modify_order.newQuantity - order_to_modify.orderQuantity;
        order_to_modify.orderQuantity = modify_order.newQuantity;
        return;
    }
    // Making the original quantity bigger.
    if (order_to_modify.side == 'b')
    {
        // Additng the difference to the buy_qty
        uint64_t buy_hypo_worst = std::max(modify_order.newQuantity - order_to_modify.orderQuantity + target_stock.net_pos + target_stock.buy_qty,
                                           modify_order.newQuantity - order_to_modify.orderQuantity + target_stock.buy_qty);
        if (buy_hypo_worst <= buy_threshold)
        {
            target_stock.buy_qty += modify_order.newQuantity - order_to_modify.orderQuantity;
            order_to_modify.orderQuantity = modify_order.newQuantity;
        }
    }
    else
    {
        uint64_t sell_hypo_worst = std::max(modify_order.newQuantity - order_to_modify.orderQuantity - target_stock.net_pos + target_stock.sell_qty,
                                            modify_order.newQuantity - order_to_modify.orderQuantity + target_stock.sell_qty);
        if (sell_hypo_worst < buy_threshold)
        {
            target_stock.buy_qty += modify_order.newQuantity - order_to_modify.orderQuantity;
            order_to_modify.orderQuantity = modify_order.newQuantity;
        }
    }
}

void Portfolio::confirm_trade(Trade const &trade)
{
    NewOrder &target_order = orders[trade.tradeId];
    Stock &target_stock = stocks[trade.listingId];
    if (target_order.side == 'b')
        target_stock.net_pos += trade.tradeQuantity;
    else
        target_stock.net_pos -= trade.tradeQuantity;
}
