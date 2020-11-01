/*
    Message.h
    1. Contains structs of various message types.
    2. Implementation of member functions, mainly for serialisation of object 
        into bytes and deserialisation back to object, can be found in Message.cpp.
*/
#include <stdint.h>
#include <iostream>
#include <unistd.h>
#ifndef MESSAGE_H
#define MESSAGE_H

struct Threshold
{
    uint64_t buy_threshold;
    uint64_t sell_threshold;

    Threshold() {}
    Threshold(uint64_t buy_threshold_, uint64_t sell_threshold_)
        : buy_threshold(buy_threshold_), sell_threshold(sell_threshold_) {}
    bool serialize(uint8_t *buffer, uint8_t start_idx = 0) const;
    bool deserialize(uint8_t const *buffer, uint8_t start_idx = 0);
    friend std::ostream &operator<<(std::ostream &out, Threshold const &th);
    /* Get threshold from cli. */
    void get_threshold();
} __attribute__((__packed__));

struct Header
{
    uint16_t version;        // Protocol version
    uint16_t payloadSize;    // Payload size in bytes
    uint32_t sequenceNumber; // Sequence number for this package
    uint64_t timestamp;      // Timestamp, number of nanoseconds from Unix epoch.

    Header() {}
    Header(uint16_t version_, uint16_t payloadSize_, uint32_t sequenceNumber_);

    bool serialize(uint8_t *buffer, uint8_t start_idx = 0) const;
    bool deserialize(uint8_t const *buffer, uint8_t start_idx = 0);
    friend std::ostream &operator<<(std::ostream &out, Header const &h);

} __attribute__((__packed__));

struct NewOrder
{
    static constexpr uint16_t MESSAGE_TYPE = 1;
    uint16_t messageType = MESSAGE_TYPE; // Message type of this message
    uint64_t listingId;                  // Financial instrument id associated to this message
    uint64_t orderId;                    // Order id used for further order changes
    uint64_t orderQuantity;              // Order quantity
    uint64_t orderPrice;                 // Order price, the price contains 4 implicit decimals
    char side;                           // The side of the order, 'B' for buy and 'S' for sell
    NewOrder() {}
    NewOrder(uint64_t listingId_, uint64_t orderId_, uint64_t orderQuantity_, uint64_t orderPrice_, char side_)
        : listingId(listingId_), orderId(orderId_), orderQuantity(orderQuantity_), orderPrice(orderPrice_), side(side_) {}
    bool serialize(uint8_t *buffer, uint8_t start_idx = 16) const;
    bool deserialize(uint8_t const *buffer, uint8_t start_idx = 16);
    friend std::ostream &operator<<(std::ostream &out, NewOrder const &no);
} __attribute__((__packed__));

struct OrderResponse
{
    static constexpr uint16_t MESSAGE_TYPE = 5;
    // enum class Status : uint16_t
    // {
    //     ACCEPTED = 0,
    //     REJECTED = 1,
    // };
    uint16_t status;
    uint16_t messageType = MESSAGE_TYPE; // Message type of this message
    uint64_t orderId;                    // Order id that refers to the original order id Status status; // Status of the order
    friend std::ostream &operator<<(std::ostream &out, OrderResponse const &order_response);
    bool serialize(uint8_t *buffer, uint8_t start_idx = 16) const;
    bool deserialize(uint8_t const *buffer, uint8_t start_idx = 16);
} __attribute__((__packed__));

struct DeleteOrder
{
    static constexpr uint16_t MESSAGE_TYPE = 2;
    uint16_t messageType = MESSAGE_TYPE; // Message type of this message
    uint64_t orderId;                    // Order id that refers to the original order id
    DeleteOrder() {}
    DeleteOrder(uint64_t orderId_) : orderId(orderId_) {}
} __attribute__((__packed__));

struct ModifyOrderQuantity
{
    static constexpr uint16_t MESSAGE_TYPE = 3;
    uint16_t messageType = MESSAGE_TYPE; // Message type of this message
    uint64_t orderId;                    // Order id that refers to the original order id
    uint64_t newQuantity;                // The new quantity
    ModifyOrderQuantity() {}
    ModifyOrderQuantity(uint64_t orderID_, uint64_t newQuantity_)
        : orderId(orderID_), newQuantity(newQuantity_) {}

} __attribute__((__packed__));

struct Trade
{
    static constexpr uint16_t MESSAGE_TYPE = 4;
    uint16_t messageType = MESSAGE_TYPE; // Message type of this message
    uint64_t listingId;                  // Financial instrument id associated to this message
    uint64_t tradeId;                    // Order id that refers to the original order id
    uint64_t tradeQuantity;              // Trade quantity
    uint64_t tradePrice;                 // Trade price, the price contains 4 implicit decimals
    Trade() {}
    Trade(uint64_t listingId_, uint64_t tradeId_, uint64_t tradeQuantity_, uint64_t tradePrice_)
        : listingId(listingId_), tradeId(tradeId_), tradeQuantity(tradeQuantity_), tradePrice(tradePrice_) {}
} __attribute__((__packed__));

#endif // !MESSAGE_H