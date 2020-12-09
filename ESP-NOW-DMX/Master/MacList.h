// Broadcast mac 
// for broadcasts the addr needs to be ff:ff:ff:ff:ff:ff
// all devices on the same channel

#ifndef MACLIST_H
#define MACLIST_H

static uint8_t BROADCAST_MAC[]      = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
static uint8_t SLAVE_MAC_1[6]       = { 0xFC, 0xF5, 0xC4, 0x31, 0x69, 0x0C };
static uint8_t SLAVE_MAC_2[6]       = { 0x24, 0x0A, 0xC4, 0x61, 0x19, 0x08 };
static uint8_t SLAVE_MAC_3[6]       = { 0xFC, 0xF5, 0xC4, 0x31, 0x69, 0x0C };
static uint8_t SLAVE_MAC_4[6]       = { 0xFC, 0xF5, 0xC4, 0x31, 0x69, 0x0C };
static uint8_t SLAVE_MAC_5[6]       = { 0xFC, 0xF5, 0xC4, 0x31, 0x69, 0x0C };
static uint8_t SLAVE_MAC_6[6]       = { 0xFC, 0xF5, 0xC4, 0x31, 0x69, 0x0C };
static uint8_t SLAVE_MAC_7[6]       = { 0xFC, 0xF5, 0xC4, 0x31, 0x69, 0x0C };
static uint8_t SLAVE_MAC_8[6]       = { 0xFC, 0xF5, 0xC4, 0x31, 0x69, 0x0C };
static uint8_t SLAVE_MAC_9[6]       = { 0xFC, 0xF5, 0xC4, 0x31, 0x69, 0x0C };
// static uint8_t SLAVE_MAC_2[6]       = { 0xFC, 0xF5, 0xC4, 0x32, 0x69, 0x0d };
// static uint8_t SLAVE_MAC_3[6]       = { 0xFC, 0xF5, 0xC4, 0x33, 0x69, 0x0d };
// static uint8_t SLAVE_MAC_4[6]       = { 0xFC, 0xF5, 0xC4, 0x34, 0x6a, 0x0e };
// static uint8_t SLAVE_MAC_5[6]       = { 0xFC, 0xF5, 0xC4, 0x35, 0x6d, 0x0C };
// static uint8_t SLAVE_MAC_6[6]       = { 0xFC, 0xF5, 0xC4, 0x39, 0x6b, 0x0C };
// static uint8_t SLAVE_MAC_7[6]       = { 0xFC, 0xF5, 0xC4, 0x36, 0x6b, 0x0C };
// static uint8_t SLAVE_MAC_8[6]       = { 0xFC, 0xF5, 0xC4, 0x34, 0x6a, 0x0C };
// static uint8_t SLAVE_MAC_9[6]       = { 0xFC, 0xF5, 0xC4, 0x33, 0x6f, 0x0C };

static uint8_t slave_offset         = 30;
static uint8_t slave_broadcastId    = 1;

#endif