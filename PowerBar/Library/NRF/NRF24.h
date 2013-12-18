// NRF24.h
// Author: Mike McCauley
// Copyright (C) 2012 Mike McCauley
// $Id: NRF24.h,v 1.1 2012/12/12 11:17:16 mikem Exp mikem $
//
/// \mainpage NRF24 library for Arduino
///
/// This is the Arduino NRF24 library.
/// It provides an object-oriented interface for sending and receiving data messages with Nordic nRF24L01
/// based radio modules, such as the sparkfun WRL-00691 http://www.sparkfun.com/products/691
///
/// The nRF24L01 (http://www.sparkfun.com/datasheets/Wireless/Nordic/nRF24L01P_Product_Specification_1_0.pdf)
/// is a low-cost 2.4GHz ISM transceiver module. It supports a number of channel frequencies in the 2.4GHz band
/// and a range of data rates.
///
/// This library provides functions for sending and receiving messages of up to 32 octets on any
/// frequency supported by the nRF24L01, at a selected data rate.
///
/// Up to 2 nRF24L01 modules can be connected to an Arduino, permitting the construction of translators
/// and frequency changers, etc.
///
/// This library provides classes for
/// - NRF24: addressed, reliable, retransmitted, acknowledged messages (using nrf24 Enhanced Shockburst)
///
/// Example Arduino programs are included to show the main modes of use.
///
/// The version of the package that this documentation refers to can be downloaded
/// from http://www.airspayce.com/mikem/arduino/NRF24/NRF24-1.9.zip
/// You can find the latest version at http://www.airspayce.com/mikem/arduino/NRF24
///
/// You can also find online help and disussion at http://groups.google.com/group/NRF24-arduino
/// Please use that group for all questions and discussions on this topic.
/// Do not contact the author directly, unless it is to discuss commercial licensing.
///
/// Tested on Arduino Diecimila, Duemilanove and Mega with arduino 1.0
/// on OpenSuSE 12.1 with avr-libc-1.7.1-1.1, cross-avr-binutils-2.19.1-33.1
/// and cross-avr-gcc-4.3.3_20100125-28.1, and with arduino-0021 on the same platforms
///
/// \par Packet Format
///
/// All messages sent and received by this NRF24 library must conform to this packet format, as specified by
/// the nRF24L01 product specificaiton:
///
/// - 1 octets PREAMBLE
/// - 4 octets ADDRESS
/// - 9 bits packet control field
/// - 0 to 32 octets PAYLOAD
/// - 2 octets CRC
///
/// \par Connecting nRF24L01 to Arduino
///
/// The physical connection between the nRF24L01 and the Arduino require 3.3V, the 3 x SPI pins (SCK, SDI, SDO),
/// a Chip Enable pin and a Slave Select pin.
/// If you are using the Sparkfun WRL-00691 module, it has a voltage regulator on board and
/// can be run with 5V VCC
/// The examples below assume the Sparkfun WRL-00691 module
///
/// Connect the nRF24L01 to most Arduino's like this (Caution, Arduino Mega has different pins for SPI,
/// see below),
/// \code
///                 Arduino      Sparkfun WRL-00691
///                 GND----------GND   (ground in)
///                 3V3 or 5V----VCC   (3.3V to 7V in)
///             pin D8-----------CE    (chip enable in)
///          SS pin D10----------CSN   (chip select in)
///         SCK pin D13----------SCK   (SPI clock in)
///        MOSI pin D11----------SDI   (SPI Data in)
///        MISO pin D12----------SDO   (SPI data out)
/// \endcode
/// For an Arduino Mega:
/// \code
///                 Mega         Sparkfun WRL-00691
///                 GND----------GND  (ground in)
///                 3V3 or 5V----VCC   (3.3V to 7V in)
///             pin D8-----------CE    (chip enable in)
///          SS pin D53----------CSN   (chip select in)
///         SCK pin D52----------SCK   (SPI clock in)
///        MOSI pin D51----------SDI   (SPI Data in)
///        MISO pin D50----------SDO   (SPI data out)
/// \endcode
/// and you can then use the default constructor NRF24().
/// You can override the default settings for the CSN and CE pins
/// in the NRF24() constructor if you wish to connect the slave select CSN to other than the normal one for your
/// Arduino (D10 for Diecimila, Uno etc and D53 for Mega)
///
/// Caution: on some Arduinos such as the Mega 2560, if you set the slave select pin to be other than the usual SS
/// pin (D53 on  Mega 2560), you may need to set the usual SS pin to be an output to force the Arduino into SPI
/// master mode.
///
/// It is possible to have 2 radios conected to one arduino, provided each radio has its own
/// CSN and CE line (SCK, SDI and SDO are common to both radios)
///
/// \par Example programs
///
/// The following example programs are provided:
/// -nrf24_ping_client, nrf24_pin_server. This is a matched pair. The client sends (acknowledged)
///  4 byte timestamp to the server which replies (acknowledged).
///  The client measures the round-trip time and prints it. Typical RTT is 1-2 msec.
///  These are compatible radio wise with the ping_client and ping_server programs that come with the Mirf library
///   though the electrical connections are different
/// -nrf24_specan. Example sketch showing how to create a primitive spectrum analyser
///  with the NRF24 class. The nRF24L01 received power detector is only one bit, but
///  this will show which channels have more than -64dBm present.
/// -nrf24_audio_tx, nrf24_audio_rx. This is a matched pair. The clinet sends a stream of audio samples measured
///  from analog input 0 to the receiver, which reconstructs them on output D6. See comments in those files for
///  electrical requirements. The pair demonstrates the use of NRF24 in NOACK modefor improved performance
///  (but no reliability). Can achieve 6.4kHz sample rate. Dont expect good quality audio!
///
/// \par Radio Performance
///
/// The performance of this radio seems to be very good. I was able to build ping client/server
/// that was able to achieve over 800 round trips per second
/// (at 0dBm power, 2Mbps, channel 1, 4 byte payload each way, 1 checksum byte) when the radios were next to each other.
/// This rate could still be achieved at 15m distance, but the orientation of the radios and
/// obstructions became critical. The human body can easily block these signals.
/// Best response was when the chip antennas were broadside to each other.
///
/// It is possible to get even better streaming performance using NOACK mode (see the nrf24_audio_tx sample)
/// at the cost of nop reliability.
/// In NOACK mode, at 2Mbps, 32 byte payload, can get about 1900 packets per second: 60800 bytes of payload per second
///
/// Frequency accuracy may be debatable. For nominal frequency of 2401.000 MHz (ie channel 1),
/// my Yaesu VR-5000 receiver indicated the center frequency for my test radios
/// was 2401.121 MHz. Its not clear to me if the Yaesu
/// is the source of the error, but I tend to believe it, which would make the nRF24l01 frequency out by 121kHz.
///
/// \par Radio operating strategy and defaults
///
/// The radio is enabled all the time, and switched between TX and RX modes depending on
/// whether there is any data to send. Sending data sets the radio to TX mode.
/// After data is sent, the radion automatically returns to Standby II mode. Calling waitAvailable() or
/// waitAvailableTimeout() starts the radio in RX mode.
///
/// The radio is configured by default to Channel 2, 2Mbps, 0dBm power, 5 bytes address, payload width 1, CRC enabled
/// 1 byte CRC, Auto-Ack mode. Enhanced shockburst is used.
/// P1 is the receive pipe. P0 is set to the transmit address to enable autoack.
///
/// \par Memory
///
/// Memory usage of this program is minimal. The compiled ping client and server programs are about 4000 bytes.
/// RAM requirements of the library are minimal.
///
/// \par Installation
///
/// Install in the usual way: unzip the distribution zip file to the libraries
/// sub-folder of your sketchbook.
///
/// This software is Copyright (C) 2012 Mike McCauley. Use is subject to license
/// conditions. The main licensing options available are GPL V2 or Commercial:
///
/// \par Open Source Licensing GPL V2
///
/// This is the appropriate option if you want to share the source code of your
/// application with everyone you distribute it to, and you also want to give them
/// the right to share who uses it. If you wish to use this software under Open
/// Source Licensing, you must contribute all your source code to the open source
/// community in accordance with the GPL Version 2 when your application is
/// distributed. See http://www.gnu.org/copyleft/gpl.html
///
/// \par Commercial Licensing
///
/// This is the appropriate option if you are creating proprietary applications
/// and you are not prepared to distribute and share the source code of your
/// application. Contact info@airspayce.com for details.
///
/// \par Revision History
///
/// \version 1.0 Initial release
///
/// \version 1.1 Changed default value for slave slect pin in constructor to be SS, ie the normal one for
/// the compiled Arduino (D10 for Diecimila, Uno etc and D53 for Mega). This is because some Arduinos such as Mega 2560
/// reportedly use the type of the SS pin to determine whether to run in slave or master mode. Therfore it
/// is preferred that the slave select pin actually be the normal SS pin.
///
/// \version 1.2 Updated documentation about what happens during init, and that SPI is initialised to 8MHz
/// (but can be set to other frequencies after calling init()
/// \version 1.3 Fixed error in title of main page
/// \version 1.4 Fixed typo in nrf24_test.pde, reported by TOM.
/// \version 1.6 Fixed an error NRF24::setRF in setting data rate to 250kbps. Reported by Shiu Kumar.
/// \version 1.7 Improvements to waitPacketSent() so if the chip is not in transmit mode, it wont wait forever.
///              Improvements to isSending() so it returns false if the chip is not in transmit mode.
/// \version 1.8 Fixed a conflict between 2 definitions of NRF24_TX_FULL. The one in 07 STATUS is changed to
///              NRF24_STATUS_TX_FULL. Reported by Charles-Henri Hallard.
///              Updated author and distribution location details to airspayce.com
/// \version 1.9 Improvements to waitAvailableTimeout to make it robust on millis() rollover.
///
/// \author  Mike McCauley (mikem@airspayce.com) DO NOT CONTACT THE AUTHOR DIRECTLY: USE THE LISTS

#ifndef NRF24_h_INCLUDED
#define NRF24_h_INCLUDED


#define F_CPU 16000000
#include <util/delay.h>
#include <stdbool.h>
#include <stdio.h>
#include <avr/io.h>


// This is the bit in the SPI address that marks it as a write
#define NRF24_SPI_WRITE_MASK 0x80

// This is the maximum message length that can be supported by this library. Limited by
// the suported message lengths oin the nRF24
// Can be pre-defined to a smaller size (to save SRAM) prior to including this header
#ifndef NRF24_MAX_MESSAGE_LEN
#define NRF24_MAX_MESSAGE_LEN 32
#endif

// Keep track of the mode the NRF24 is in
#define NRF24_MODE_IDLE         0
#define NRF24_MODE_RX           1
#define NRF24_MODE_TX           2

// These values we set for FIFO thresholds are actually the same as the POR values
#define NRF24_TXFFAEM_THRESHOLD 4
#define NRF24_RXFFAFULL_THRESHOLD 55

// This is the default node address,
#define NRF24_DEFAULT_NODE_ADDRESS 0x00000000

// This address in the TO addreess signifies a broadcast
#define NRF24_BROADCAST_ADDRESS 0xffffffffff

// SPI Command names
#define NRF24_COMMAND_R_REGISTER                        0x00
#define NRF24_COMMAND_W_REGISTER                        0x20
#define NRF24_COMMAND_R_RX_PAYLOAD                      0x61
#define NRF24_COMMAND_W_TX_PAYLOAD                      0xa0
#define NRF24_COMMAND_FLUSH_TX                          0xe1
#define NRF24_COMMAND_FLUSH_RX                          0xe2
#define NRF24_COMMAND_REUSE_TX_PL                       0xe3
#define NRF24_COMMAND_R_RX_PL_WID                       0x60
#define NRF24_COMMAND_W_ACK_PAYLOAD                     0xa8
#define NRF24_COMMAND_W_TX_PAYLOAD_NOACK                0xb0
#define NRF24_COMMAND_NOP                               0xff

// Register names
#define NRF24_REGISTER_MASK                             0x1f
#define NRF24_REG_00_CONFIG                             0x00
#define NRF24_REG_01_EN_AA                              0x01
#define NRF24_REG_02_EN_RXADDR                          0x02
#define NRF24_REG_03_SETUP_AW                           0x03
#define NRF24_REG_04_SETUP_RETR                         0x04
#define NRF24_REG_05_RF_CH                              0x05
#define NRF24_REG_06_RF_SETUP                           0x06
#define NRF24_REG_07_STATUS                             0x07
#define NRF24_REG_08_OBSERVE_TX                         0x08
#define NRF24_REG_09_RPD                                0x09
#define NRF24_REG_0A_RX_ADDR_P0                         0x0a
#define NRF24_REG_0B_RX_ADDR_P1                         0x0b
#define NRF24_REG_0C_RX_ADDR_P2                         0x0c
#define NRF24_REG_0D_RX_ADDR_P3                         0x0d
#define NRF24_REG_0E_RX_ADDR_P4                         0x0e
#define NRF24_REG_0F_RX_ADDR_P5                         0x0f
#define NRF24_REG_10_TX_ADDR                            0x10
#define NRF24_REG_11_RX_PW_P0                           0x11
#define NRF24_REG_12_RX_PW_P1                           0x12
#define NRF24_REG_13_RX_PW_P2                           0x13
#define NRF24_REG_14_RX_PW_P3                           0x14
#define NRF24_REG_15_RX_PW_P4                           0x15
#define NRF24_REG_16_RX_PW_P5                           0x16
#define NRF24_REG_17_FIFO_STATUS                        0x17
#define NRF24_REG_1C_DYNPD                              0x1c
#define NRF24_REG_1D_FEATURE                            0x1d

// These register masks etc are named wherever possible
// corresponding to the bit and field names in the nRF24L01 Product Specification
// #define NRF24_REG_00_CONFIG                             0x00
#define NRF24_MASK_RX_DR                                0x40
#define NRF24_MASK_TX_DS                                0x20
#define NRF24_MASK_MAX_RT                               0x10
#define NRF24_EN_CRC                                    0x08
#define NRF24_CRCO                                      0x04
#define NRF24_PWR_UP                                    0x02
#define NRF24_PRIM_RX                                   0x01

// #define NRF24_REG_01_EN_AA                              0x01
#define NRF24_ENAA_P5                                   0x20
#define NRF24_ENAA_P4                                   0x10
#define NRF24_ENAA_P3                                   0x08
#define NRF24_ENAA_P2                                   0x04
#define NRF24_ENAA_P1                                   0x02
#define NRF24_ENAA_P0                                   0x01

// #define NRF24_REG_02_EN_RXADDR                          0x02
#define NRF24_ERX_P5                                    0x20
#define NRF24_ERX_P4                                    0x10
#define NRF24_ERX_P3                                    0x08
#define NRF24_ERX_P2                                    0x04
#define NRF24_ERX_P1                                    0x02
#define NRF24_ERX_P0                                    0x01

// #define NRF24_REG_03_SETUP_AW                           0x03
#define NRF24_AW_3_BYTES                                0x01
#define NRF24_AW_4_BYTES                                0x02
#define NRF24_AW_5_BYTES                                0x03

// #define NRF24_REG_04_SETUP_RETR                         0x04
#define NRF24_ARD                                       0xf0
#define NRF24_ARC                                       0x0f

// #define NRF24_REG_05_RF_CH                              0x05
#define NRF24_RF_CH                                     0x7f

// #define NRF24_REG_06_RF_SETUP                           0x06
#define NRF24_CONT_WAVE                                 0x80
#define NRF24_RF_DR_LOW                                 0x20
#define NRF24_PLL_LOCK                                  0x10
#define NRF24_RF_DR_HIGH                                0x08
#define NRF24_PWR                                       0x06
#define NRF24_PWR_m18dBm                                0x00
#define NRF24_PWR_m12dBm                                0x02
#define NRF24_PWR_m6dBm                                 0x04
#define NRF24_PWR_0dBm                                  0x06

// #define NRF24_REG_07_STATUS                             0x07
#define NRF24_RX_DR                                     0x40
#define NRF24_TX_DS                                     0x20
#define NRF24_MAX_RT                                    0x10
#define NRF24_RX_P_NO                                   0x0e
#define NRF24_STATUS_TX_FULL                            0x01


// #define NRF24_REG_08_OBSERVE_TX                         0x08
#define NRF24_PLOS_CNT                                  0xf0
#define NRF24_ARC_CNT                                   0x0f

// #define NRF24_REG_09_RPD                                0x09
#define NRF24_RPD                                       0x01

// #define NRF24_REG_17_FIFO_STATUS                        0x17
#define NRF24_TX_REUSE                                  0x40
#define NRF24_TX_FULL                                   0x20
#define NRF24_TX_EMPTY                                  0x10
#define NRF24_RX_FULL                                   0x02
#define NRF24_RX_EMPTY                                  0x01

// #define NRF24_REG_1C_DYNPD                              0x1c
#define NRF24_DPL_P5                                    0x20
#define NRF24_DPL_P4                                    0x10
#define NRF24_DPL_P3                                    0x08
#define NRF24_DPL_P2                                    0x04
#define NRF24_DPL_P1                                    0x02
#define NRF24_DPL_P0                                    0x01

// #define NRF24_REG_1D_FEATURE                            0x1d
#define NRF24_EN_DPL                                    0x04
#define NRF24_EN_ACK_PAY                                0x02
#define NRF24_EN_DYN_ACK                                0x01


/////////////////////////////////////////////////////////////////////
/// \class NRF24 NRF24.h <NRF24.h>
/// \brief Send and receive addressed, reliable, acknowledged datagrams by nRF24L01.
///
/// This base class provides basic functions for sending and receiving addressed, reliable,
/// automatically acknowledged and retransmitted
/// datagrams via nRF24L01 of arbitrary length to 32 octets per packet.
/// Sender and receiver must each know the addreesses of the other, so arbitrary meshes and stars are
/// not possible at this level.
/// Directed replies (ie replies sent back to the original sender) are not possible
/// (the address of the sender is not carried in the message).
/// See subclasses for support for this.
///
/// Subclasses may use this class to implement streams,
/// mesh routers, repeaters, translators etc.
///
/// On transmission, the addresses of this node defaults to 0x0000000000, unless changed by a subclass.
///
/// The radio is configured to use Enhanced Shockburst with retransmits.
/// TX_ADDR and RX_ADDR_P0 are set to the transmit address (ie the address of the destination for the next message
/// RX_ADDR_P1 is set to the address of this node
/// RX_ADDR_P2 is set to RX_ADDR_P1 with the LSbyte set to 0xff, for use as a broadcast address
///
/// Naturally, for any 2 radios to communicate that must be configured to use the same frequency and
/// data rate, and with compatible addresses

#define SETBIT(ADDRESS,BIT) (ADDRESS |= (1<<BIT))
#define CLEARBIT(ADDRESS,BIT) (ADDRESS &= ~(1<<BIT))
#define FLIPBIT(ADDRESS,BIT) (ADDRESS ^= (1<<BIT))
#define CHECKBIT(ADDRESS,BIT) (ADDRESS & (1<<BIT))

#define DDR_SPI DDRB
#define SPI_MOSI PORTB3
#define SPI_MISO PINB4
#define SPI_CLK  PORTB5
#define SPI_SS  PORTB2
#define SPI_PORT PORTB


#define  SPI_CS_Low()		CLEARBIT(SPI_PORT , SPI_SS) // pull data low
#define  SPI_CS_High()		SETBIT(SPI_PORT , SPI_SS) // pull data high

// SWSPI
#define  SW_SPI_DDR  DDRC // Data I/O +
#define  SW_SPI_PORT  PORTC
#define  SW_SPI_CLK PINC1 //digital pin 11
#define  SW_SPI_Data_Out PINC2 //digital pin 11
#define  SW_SPI_Data_In PINC0 // digital pin 10
//
#define SW_SPI_CS_DDR DDRD
#define SW_SPI_CS_PORT PORTD
#define SW_SPI_CS PIND7

#define NRF24L01_CSN	PORTB2  /// Chip Select
#define NRF24L01_CE		PORTB0 // Chip Enable (Tx or Rx Mode)
#define NRF24L01_CSN_CE_DDR		DDRB
#define NRF24L01_CSN_CE_PORT	PORTB


// Chip Select Pin
#define NRF24L01_CSN_LOW	CLEARBIT(NRF24L01_CSN_CE_PORT,NRF24L01_CSN)
#define NRF24L01_CSN_HIGH	SETBIT(NRF24L01_CSN_CE_PORT,NRF24L01_CSN)
// TX - RX Pin
#define NRF24L01_CE_LOW		CLEARBIT(NRF24L01_CSN_CE_PORT,NRF24L01_CE)
#define NRF24L01_CE_HIGH	SETBIT(NRF24L01_CSN_CE_PORT,NRF24L01_CE)




// Default device config: 1 byte CRC
#define NRF24_DEFAULT_CONFIGURATION (NRF24_EN_CRC)



bool NRF24_Init(void);
uint8_t NRF24_spiWriteRegister(uint8_t reg, uint8_t val);
uint8_t NRF24_spiWrite(uint8_t command, uint8_t val);

bool NRF24_powerDown();
bool NRF24_powerUpRx();
bool NRF24_powerUpTx();
uint8_t NRF24_flushTx();
uint8_t NRF24_flushRx();

bool NRF24_setPayloadSize(uint8_t size);
bool NRF24_setThisAddress(uint8_t* address, uint8_t len);
bool NRF24_setChannel(uint8_t channel);
uint8_t NRF24_spiBurstWriteRegister(uint8_t reg, uint8_t* src, uint8_t len);
uint8_t NRF24_spiWriteRegister(uint8_t reg, uint8_t val);
uint8_t NRF24_spiBurstWrite(uint8_t command, uint8_t* src, uint8_t len);
bool NRF24_setRF(uint8_t data_rate, uint8_t power);
bool NRF24_setTransmitAddress(uint8_t* address, uint8_t len);
bool NRF24_send(uint8_t* data, uint8_t len);

bool NRF24_waitPacketSent();
uint8_t NRF24_spiReadRegister(uint8_t reg);
uint8_t NRF24_spiRead(uint8_t command);
uint8_t NRF24_statusRead();


bool NRF24_waitAvailableTimeout(uint16_t timeout);
bool NRF24_available();
bool NRF24_recv(uint8_t* buf, uint8_t len);
void NRF24_spiBurstRead(uint8_t command, uint8_t* dest, uint8_t len);
void NRF24_waitAvailable();


/// @example nrf24_audio_rx.pde
/// Example sketch showing how to create an audio digital receiver
/// with the NRF24 class.
/// Works with the nrf24_audio_tx sample transmitter
/// Connect audio output to pin 6, through a low pass filter consisting of a 1k resister and series followed by a
/// 0.0033 microfarad capacitor to ground (48kHz filter).
/// The audio quality is poor: dont expect hi-fi!
/// We have to change the PWM frequency to 62 kHz so we can get bandwidth reasonable
/// audio out through the low pass filter
/// Tested on UNO

/// @example nrf24_audio_tx.pde
/// Example sketch showing how to create an audio digital transmitter
/// with the NRF24 class.
/// Connect a 1Vp-p audio sigal to analog input 0, connected through a 1uF capacitor
/// Works with the nrf24_audio_rx sample receiver
/// The audio quality is poor: dont expect hi-fi!
///
/// This code sends about 250 messages per second, each with 32 8 bit samples from analog input 0
/// It uses the NRF4 in NOACK mode. The receiver never acknowledges or replies
/// Tested on UNO

/// @example nrf24_ping_client.pde
/// Example sketch showing how to create a simple messageing client
/// with the NRF24 class.
/// It is designed to work with the example nrf24_ping_server
/// It also works with ping_server from the Mirf library

/// @example nrf24_ping_server.pde
/// Example sketch showing how to create a simple messageing server
/// with the NRF24 class.
/// It is designed to work with the example nrf24_ping_client.
/// It also works with ping_client from the Mirf library

/// @example nrf24_specan.pde
/// Example sketch showing how to create a primitive spectrum analyser
/// with the NRF24 class.
/// The nRF24L01 received power detector is only one bit, but
/// this will show which channels have more than -64dBm present

/// @example nrf24_test.pde
/// Test suite for the NRF24 class.

#endif
