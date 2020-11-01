#include "Message.h"
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <ctime>
#include <array>
#include <iostream>

/* Helper Function */
std::array<uint8_t, 8> int64_to_bytes(uint64_t v)
{
    std::array<uint8_t, 8> bytes;
    bytes[0] = (v & 0xFF00000000000000) >> 56;
    bytes[1] = (v & 0x00FF000000000000) >> 48;
    bytes[2] = (v & 0x0000FF0000000000) >> 40;
    bytes[3] = (v & 0x000000FF00000000) >> 32;
    bytes[4] = (v & 0x00000000FF000000) >> 24;
    bytes[5] = (v & 0x0000000000FF0000) >> 16;
    bytes[6] = (v & 0x000000000000FF00) >> 8;
    bytes[7] = (v & 0x00000000000000FF);
    return bytes;
}
std::array<uint8_t, 4> int32_to_bytes(uint32_t v)
{
    std::array<uint8_t, 4> bytes;
    bytes[0] = (v & 0xFF000000) >> 24;
    bytes[1] = (v & 0x00FF0000) >> 16;
    bytes[2] = (v & 0x0000FF00) >> 8;
    bytes[3] = (v & 0x000000FF);
    return bytes;
}

uint64_t bytes_to_int64(uint8_t const *bytes, uint8_t start_idx, uint8_t size)
{
    uint64_t v = 0;
    uint8_t i = 0;
    for (i = 0; i < size - 1; i++)
    {
        v |= bytes[start_idx + i];
        v <<= 8;
    }
    v |= bytes[start_idx + i];
    return (uint64_t)v;
}

template <class T>
void copy_to_buffer(uint8_t *buffer, uint8_t &start_idx, const T &data)
{
    memcpy(buffer + start_idx, int64_to_bytes(data).data() + (8 - sizeof(data)), sizeof(data));
    start_idx += sizeof(data);
    std::cout << (int)start_idx << std::endl;
}

template <class T>
void decode_from_buffer(uint8_t const *buffer, uint8_t &start_idx, T &data)
{
    data = bytes_to_int64(buffer, start_idx, sizeof(data));
    start_idx += sizeof(data);
}

/* Implementation of Header class */
Header::Header(uint16_t version_, uint16_t payloadSize_, uint32_t sequenceNumber_) : version(version_), payloadSize(payloadSize_), sequenceNumber(sequenceNumber_)
{
    timestamp = std::time(nullptr);
}

bool Header::serialize(uint8_t *buffer, uint8_t start_idx) const
{
    copy_to_buffer(buffer, start_idx, version);
    copy_to_buffer(buffer, start_idx, payloadSize);
    copy_to_buffer(buffer, start_idx, sequenceNumber);
    copy_to_buffer(buffer, start_idx, timestamp);
    return true;
}

bool Header::deserialize(uint8_t const *buffer, uint8_t start_idx)
{
    decode_from_buffer(buffer, start_idx, version);
    decode_from_buffer(buffer, start_idx, payloadSize);
    decode_from_buffer(buffer, start_idx, sequenceNumber);
    decode_from_buffer(buffer, start_idx, timestamp);
    return true;
}

std::ostream &operator<<(std::ostream &out, Header const &h)
{
    out << "version: " << h.version << std::endl
        << "payloadSize: " << h.payloadSize << std::endl
        << "sequenceNumber: " << h.sequenceNumber << std::endl
        << "timestamp: " << h.timestamp << std::endl;
    return out;
}

/* Implementation of NewOrder class */
bool NewOrder::serialize(uint8_t *buffer, uint8_t start_idx) const
{
    copy_to_buffer(buffer, start_idx, messageType);
    copy_to_buffer(buffer, start_idx, listingId);
    copy_to_buffer(buffer, start_idx, orderId);
    copy_to_buffer(buffer, start_idx, orderQuantity);
    copy_to_buffer(buffer, start_idx, orderPrice);
    copy_to_buffer(buffer, start_idx, side);
    return true;
}
bool NewOrder::deserialize(uint8_t const *buffer, uint8_t start_idx)
{

    decode_from_buffer(buffer, start_idx, messageType);
    decode_from_buffer(buffer, start_idx, listingId);
    decode_from_buffer(buffer, start_idx, orderId);
    decode_from_buffer(buffer, start_idx, orderQuantity);
    decode_from_buffer(buffer, start_idx, orderPrice);
    decode_from_buffer(buffer, start_idx, side);
    return true;
}
std::ostream &operator<<(std::ostream &out, NewOrder const &no)
{
    out << "messageType: " << no.messageType << std::endl
        << "listingId: " << no.listingId << std::endl
        << "orderId: " << no.orderId << std::endl
        << "orderQuantity: " << no.orderQuantity << std::endl
        << "orderPrice: " << no.orderPrice << std::endl
        << "side: " << no.side << std::endl;
    return out;
}