// NRF24.cpp
//
// Copyright (C) 2012 Mike McCauley
// $Id: NRF24.cpp,v 1.1 2012/12/12 11:17:16 mikem Exp mikem $

#include "NRF24.h"



bool NRF24_setPayloadSize(uint8_t size)
{
	NRF24_spiWriteRegister(NRF24_REG_11_RX_PW_P0, size);
	NRF24_spiWriteRegister(NRF24_REG_12_RX_PW_P1, size);
	return true;
}


bool NRF24_setThisAddress(uint8_t* address, uint8_t len)
{
	// Set RX_ADDR_P1 for this address
	NRF24_spiBurstWriteRegister(NRF24_REG_0B_RX_ADDR_P1, address, len);
	// RX_ADDR_P2 is set to RX_ADDR_P1 with the LSbyte set to 0xff, for use as a broadcast address
	return true;
}


bool NRF24_setChannel(uint8_t channel)
{
	NRF24_spiWriteRegister(NRF24_REG_05_RF_CH, channel & NRF24_RF_CH);
	return true;
}

uint8_t NRF24_spiBurstWriteRegister(uint8_t reg, uint8_t* src, uint8_t len)
{
	return NRF24_spiBurstWrite((reg & NRF24_REGISTER_MASK) | NRF24_COMMAND_W_REGISTER, src, len);
}

uint8_t NRF24_spiBurstWrite(uint8_t command, uint8_t* src, uint8_t len)
{
	NRF24L01_CSN_LOW;
	uint8_t status = SPI_Master_Transmit_Recieve(command);
	while (len--)
	SPI_Master_Transmit_Recieve(*src++);
	NRF24L01_CSN_HIGH;
	return status;
}

bool NRF24_setRF(uint8_t data_rate, uint8_t power)
{
	uint8_t value = (power << 1) & NRF24_PWR;
	// Ugly mapping of data rates to noncontiguous 2 bits:
	if (data_rate == 2)
	value |= NRF24_RF_DR_LOW;
	else if (data_rate == 1)
	value |= NRF24_RF_DR_HIGH;
	// else NRF24DataRate1Mbps, 00
	NRF24_spiWriteRegister(NRF24_REG_06_RF_SETUP, value);
	return true;
}


bool NRF24_setTransmitAddress(uint8_t* address, uint8_t len)
{
	// Set both TX_ADDR and RX_ADDR_P0 for auto-ack with Enhanced shockwave
	NRF24_spiBurstWriteRegister(NRF24_REG_0A_RX_ADDR_P0, address, len);
	NRF24_spiBurstWriteRegister(NRF24_REG_10_TX_ADDR, address, len);
	return true;
}

bool NRF24_send(uint8_t* data, uint8_t len)
{
	NRF24_powerUpTx();
	NRF24_spiBurstWrite(0 ? NRF24_COMMAND_W_TX_PAYLOAD_NOACK : NRF24_COMMAND_W_TX_PAYLOAD, data, len);
	// Radio will return to Standby II mode after transmission is complete
	return true;
}

bool NRF24_waitPacketSent()
{
	// If we are currently in receive mode, then there is no packet to wait for
	if (NRF24_spiReadRegister(NRF24_REG_00_CONFIG) & NRF24_PRIM_RX)
	return false;

	// Wait for either the Data Sent or Max ReTries flag, signalling the
	// end of transmission
	uint8_t status;
	while (!((status = NRF24_statusRead()) & (NRF24_TX_DS | NRF24_MAX_RT)));

	// Must clear NRF24_MAX_RT if it is set, else no further comm
	NRF24_spiWriteRegister(NRF24_REG_07_STATUS, NRF24_TX_DS | NRF24_MAX_RT);
	if (status & NRF24_MAX_RT)
	NRF24_flushTx();
	// Return true if data sent, false if MAX_RT
	return status & NRF24_TX_DS;
}

uint8_t NRF24_spiReadRegister(uint8_t reg)
{
	return NRF24_spiRead((reg & NRF24_REGISTER_MASK) | NRF24_COMMAND_R_REGISTER);
}

// Read and write commands
uint8_t NRF24_spiRead(uint8_t command)
{
	NRF24L01_CSN_LOW;
	SPI_Master_Transmit_Recieve(command); // Send the address, discard status
	uint8_t val = SPI_Master_Transmit_Recieve(0); // The MOSI value is ignored, value is read
	NRF24L01_CSN_HIGH;
	return val;
}

uint8_t NRF24_statusRead()
{
	return NRF24_spiReadRegister(NRF24_REG_07_STATUS);
}

bool NRF24_waitAvailableTimeout(uint16_t timeout)
{
	unsigned long starttime = 0;
	NRF24_powerUpRx();
	
	while ((starttime) < timeout){
		if (NRF24_available()) return true;
		starttime++;
		_delay_ms(10);
		return false;
	}
}


bool NRF24_available()
{
	if (NRF24_spiReadRegister(NRF24_REG_17_FIFO_STATUS) & NRF24_RX_EMPTY)
	return false;
	// Manual says that messages > 32 octets should be discarded
	if (NRF24_spiRead(NRF24_COMMAND_R_RX_PL_WID) > 32){
		NRF24_flushRx();
		return false;
	}
	return true;
}

bool NRF24_recv(uint8_t* buf, uint8_t len)
{
	// Clear read interrupt
	NRF24_spiWriteRegister(NRF24_REG_07_STATUS, NRF24_RX_DR);

	// 0 microsecs @ 8MHz SPI clock
	if (!NRF24_available())
	return false;
	// 32 microsecs (if immediately available)
	len = NRF24_spiRead(NRF24_COMMAND_R_RX_PL_WID);
	// 44 microsecs
	NRF24_spiBurstRead(NRF24_COMMAND_R_RX_PAYLOAD, buf, len);
	// 140 microsecs (32 octet payload)

	return true;
}

void NRF24_spiBurstRead(uint8_t command, uint8_t* dest, uint8_t len)
{
	NRF24L01_CSN_LOW;
	SPI_Master_Transmit_Recieve(command); // Send the start address, discard status
	while (len--)
	*dest++ = SPI_Master_Transmit_Recieve(0); // The MOSI value is ignored, value is read
	NRF24L01_CSN_HIGH;
	// 300 microsecs for 32 octet payload
}

void NRF24_waitAvailable()
{
	NRF24_powerUpRx();
	while (!NRF24_available());
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Low level commands for interfacing with the device
uint8_t NRF24_spiCommand(uint8_t command)
{
	uint8_t status;
	NRF24L01_CSN_LOW;
	status = SPI_Master_Transmit_Recieve(command);
	NRF24L01_CSN_HIGH;
	return status;
}

uint8_t NRF24_flushTx()
{
	return NRF24_spiCommand(NRF24_COMMAND_FLUSH_TX);
}

uint8_t NRF24_flushRx()
{
	return NRF24_spiCommand(NRF24_COMMAND_FLUSH_RX);
}


uint8_t NRF24_spiWriteRegister(uint8_t reg, uint8_t val)
{
	return NRF24_spiWrite((reg & NRF24_REGISTER_MASK) | NRF24_COMMAND_W_REGISTER, val);
}

uint8_t NRF24_spiWrite(uint8_t command, uint8_t val)
{
	NRF24L01_CSN_LOW;
	uint8_t status = SPI_Master_Transmit_Recieve(command);
	SPI_Master_Transmit_Recieve(val); // New register value follows
	NRF24L01_CSN_HIGH;
	return status;
}

bool NRF24_powerDown()
{
	NRF24_spiWriteRegister(NRF24_REG_00_CONFIG, NRF24_DEFAULT_CONFIGURATION);
	NRF24L01_CE_LOW;
	return true;
}

bool NRF24_powerUpRx()
{
	NRF24_spiWriteRegister(NRF24_REG_00_CONFIG, NRF24_DEFAULT_CONFIGURATION | NRF24_PWR_UP | NRF24_PRIM_RX);
	NRF24L01_CE_HIGH;
	return true;
}

bool NRF24_powerUpTx()
{
	// Its the pulse high that puts us into TX mode
	NRF24L01_CE_LOW;
	NRF24_spiWriteRegister(NRF24_REG_00_CONFIG, NRF24_DEFAULT_CONFIGURATION | NRF24_PWR_UP);
	NRF24L01_CE_HIGH;
	return true;
}


bool NRF24_Init(void)
{
	// Initialise the slave select pin
	// start the SPI library:
	// Note the NRF24 wants mode 0, MSB first and default to 1 Mbps
	//SPI_MasterInit();
	SPI_MasterInit();
	//SPI.begin();
	//SPI.setDataMode(SPI_MODE0);
	//SPI.setBitOrder(MSBFIRST);
	//SPI.setClockDivider(SPI_2XCLOCK_MASK); // 1 MHz SPI clock
	//SPI.setClockDivider(SPI_CLOCK_DIV2); // 8MHz SPI clock */
	_delay_ms(100);
	NRF24L01_CSN_CE_DDR	|= (1 << NRF24L01_CSN) | (1 << NRF24L01_CE);

	NRF24L01_CE_LOW;
	NRF24L01_CSN_HIGH;
	// Clear interrupts
	NRF24_spiWriteRegister(NRF24_REG_07_STATUS, NRF24_RX_DR | NRF24_TX_DS | NRF24_MAX_RT);

	// Make sure we are powered down
	NRF24_powerDown();

	// Flush FIFOs
	NRF24_flushTx();
	NRF24_flushRx();
	//
	NRF24_powerUpRx();

	return true;
}

