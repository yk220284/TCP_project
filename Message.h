#include <stdint.h>
#include <iostream>
#include <stdexcept>
#ifndef MESSAGE_H
#define MESSAGE_H

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
    NewOrder(uint64_t listingId_, uint64_t orderId_, uint64_t orderQuantity_, uint64_t orderPrice_, char side_) : listingId(listingId_), orderId(orderId_), orderQuantity(orderQuantity_), orderPrice(orderPrice_), side(side_) {}
    bool serialize(uint8_t *buffer, uint8_t start_idx = 16) const;
    bool deserialize(uint8_t const *buffer, uint8_t start_idx = 16);
    friend std::ostream &operator<<(std::ostream &out, NewOrder const &no);
} __attribute__((__packed__));

#endif // !MESSAGE_H