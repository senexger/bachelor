// Broadcast mac 
// for broadcasts the addr needs to be ff:ff:ff:ff:ff:ff
// all devices on the same channel

#ifndef MACLIST_H
#define MACLIST_H

    static uint8_t BROADCAST_MAC[6]     = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF }; // M
    static uint8_t SLAVE_MAC_1[6]       = { 0xFC, 0xF5, 0xC4, 0x31, 0x69, 0x0C }; // I
    // static uint8_t SLAVE_MAC_2[6]       = { 0x24, 0x0A, 0xC4, 0x61, 0x19, 0x08 }; // II
    static uint8_t SLAVE_MAC_2[6]       = { 0x24, 0x62, 0xAB, 0xF9, 0x50, 0x5C }; // III
    static uint8_t SLAVE_MAC_3[6]       = { 0x24, 0x62, 0xAB, 0xF9, 0x50, 0x5C };
    static uint8_t SLAVE_MAC_4[6]       = { 0x24, 0x62, 0xAB, 0xF9, 0x50, 0x5C };
    static uint8_t SLAVE_MAC_5[6]       = { 0x24, 0x62, 0xAB, 0xF9, 0x50, 0x5C };
    static uint8_t SLAVE_MAC_6[6]       = { 0x24, 0x62, 0xAB, 0xF9, 0x50, 0x5C };
    static uint8_t SLAVE_MAC_7[6]       = { 0x24, 0x62, 0xAB, 0xF9, 0x50, 0x5C };
    static uint8_t SLAVE_MAC_8[6]       = { 0x24, 0x62, 0xAB, 0xF9, 0x50, 0x5C };
    static uint8_t SLAVE_MAC_9[6]       = { 0x24, 0x62, 0xAB, 0xF9, 0x50, 0x5C };
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

    // General Constants
    // print level
    static uint8_t VERBOSE              = 1   ;
    static uint8_t DEBUG                = 1   ;
    static uint8_t TIMESTAMP            = 0   ; // taking timestamps
    static uint8_t AIRTIME              = 0   ; // measuring airtime
    static uint8_t FULL_REPETITIONS     = 10  ; // how many times run the test

    // TODO SLAVE_CHANNEL
    static uint8_t MASTER_CHANNEL       = 7;
    static uint8_t SLAVE_CHANNEL        = 1;

    static uint8_t DMX_BROADCASTING     = 1   ; // 1 = Broadcast, 0 = Unicast
    static uint8_t CHANNEL_TOTAL        = 10  ; // Broadcast: total count of channels of all fixtures
    static uint8_t BROADCAST_FRAME_SIZE = 100 ; // Broadcast: Channel/Broadcast
    static uint8_t UNICAST_FRAME_SIZE   = 20  ; // Unicast: Channel/Unicast
    static uint8_t SEND_REPITITION      = 1   ; // Sending-Measuring Interval (how many casts)
    static uint8_t WAIT_AFTER_SEND      = 50  ; // delay between broadcast sendings - prevent errors?
    // * uint8_t can just up to 255
    static uint16_t WAIT_AFTER_REP_SEND = 1000; // delay between sendings - prevent errors?

#endif

// MASTER CONSTANTS:
#define MAX_BROADCAST_FRAME_SIZE          250
#define MAX_UNICAST_FRAME_SIZE            50     // could also be 250
// Global copy of slave
#define MAX_SLAVES                        20
// ++++++++++ TESTING +++++++++