/*
 * CFile1.c
 *
 * Created: 6/13/2013 3:03:03 PM
 *  Author: mfolz
 */ 

#include "Communications.h"
#include "../AtmegaLibs/Serial_Atmel328p.h"
#include "../AtmegaLibs/SPI.h"
#include "../NRF/NRF24.h"
#define F_CPU 16000000
#include <util/delay.h>


#define LengthofServerName	5
#define LengthofClient1Name	5


void Client_Application(void){
	 char com_state;
	 char *data= "PONG";
	 Client_Start((sizeof(data)), "clie1");
	 printf("initialized Client: %s \r It will be put into send mode", "serv1");
	 printf("\r Welcome to the client module I am controlled by the server using the nrf wireless device");
	 printf("\r Please choose one of the following options: ");
	 printf("\r 1. Put Client in send state and receive string. ");
	 printf("\r 2. Put Client in receive state and send string. ");
	 printf("\r 3. Send custom string out. ");
	 
	 
	 
	 while(1){
		 printf("\r please choose a state: ");
		 com_state= USART_receive();
		 
		 switch (com_state){
			 
			 case '1':
			 data[0]= 'P';
			 printf("\r Sending out: %s", data);
			 Client_SendData("serv1",  (char*)*&data); /// send data first then delay
			 break;
			 case '2':
			 Client_RecieveData("serv1", (char*)*&data);
			 printf("\r Received: %s", data);
			 break;
			 case '3':
			 data[0]= 'P';
			 printf("\r Sending out: %s", data);
			 Client_SendData("serv1",  (char*)*&data); /// send data first then delay
			 Client_RecieveData("serv1", (char*)*&data);
			 printf("\r Received: %s", data);
			 _delay_ms(100);
			 break;
		 }
	 }

	 
 }

 void Server_Application(void){
	 char *data= "PING";
	 char com_state;
	 Server_Start(sizeof(data), "serv1");
	 printf("\r Initialized Server: %s ", "serv1");
	 
	 while(1){
		Server_RecieveData("clie1", (char*)*&data);
		printf("\r Received: %s", data);
		 		 
		data[0]= 'D';
		printf("\r Sending out: %s", data);
		Server_SendData("clie1", (char*)*&data);

			
	}
 
 }

void Client_Start(char payloadsize, char *Client_Name){
	//	struct WirelessCom *Point_Data = &Data;
	/// Start Client Process
	printf("\rClient start, read the data from server");
	if (!NRF24_Init())
	printf("\rNRF24 init failed");
	// Defaults after init are 2.402 GHz (channel 2)
	// Now be compatible with Mirf ping_server
	if (!NRF24_setChannel(1))
	printf("\rsetChannel failed");
	// Set the address for the client!
	if (!NRF24_setThisAddress((uint8_t*)Client_Name, LengthofClient1Name))
	printf("\rsetThisAddress failed");
	// Set the payload size max = 32
	if (!NRF24_setPayloadSize(payloadsize))
	printf("\rsetPayloadSize failed");
	// Set 2mbps at 0dB
	if (!NRF24_setRF(1,3))
	printf("\rsetRF failed");
}

void Client_RecieveData(char *ServerName, char *buffer){
	///	struct WirelessCom *Point_Data = &Data;
	// Send some data to the server
	if (!NRF24_setTransmitAddress((uint8_t*)ServerName, LengthofServerName))
	printf("\rsetTransmitAddress failed");
	
	if (NRF24_waitAvailableTimeout(100))
	{
		// Server should send the same time back to us
		//printf("available");
		if (!NRF24_recv((uint8_t*)buffer, sizeof(buffer))) // receive data + 1
		printf("\rread failed");	
	}
	else
	printf("No reply from server");
	
}
void Client_SendData(char *ServerName, char *data){
	//	struct WirelessCom *Point_Data = &Data;
	// Send some data to the server
	if (!NRF24_setTransmitAddress((uint8_t*)ServerName, LengthofServerName))
		printf("\rsetTransmitAddress failed");

	if (!NRF24_send((uint8_t*)data, sizeof(data)))
		printf("\rsend failed");
	if (!NRF24_waitPacketSent())
		printf("\rwaitPacketSent failed");

}



void Server_Start(char payloadsize, char *ServerName){
	uint8_t length = sizeof(ServerName);
	if (!NRF24_Init())
	printf("NRF24 init failed");
	
	// Defaults after init are 2.402 GHz (channel 2)
	// Now be compatible with Mirf ping_client
	if (!NRF24_setChannel(1))
	printf("\rsetChannel failed");
	// Set Address
	if (!NRF24_setThisAddress((uint8_t*)ServerName, LengthofServerName))
	//if (!NRF24_setThisAddress((uint8_t*)ServerName, 5))
	printf("\rsetThisAddress failed");
	/// Set payload size
	if (!NRF24_setPayloadSize(payloadsize))
	printf("setPayloadSize failed");
	
	if (!NRF24_setRF(1,3))	/// 2mbps at 0db
	printf("\rsetRF failed");
}

void Server_RecieveData(char *ClientName, char *buffer){
	char length = sizeof(buffer);
	// Wait till Server is available
	NRF24_waitAvailable();
	// ping_client sends us an unsigned long containing its timestamp
	if (!NRF24_recv((uint8_t*)buffer, length))
	printf("\rread failed");
	
}

void Server_SendData(char *ClientName, char *data){
	// Now send the same data
	// Need to set the address of the detination each time, since auto-ack changes the TX address
	// if (!NRF24_setTransmitAddress((uint8_t*)ClientName, 5))
	if (!NRF24_setTransmitAddress((uint8_t*)ClientName, LengthofClient1Name))
	printf("\rsetTransmitAddress failed");
	// Send the same data after address has been transmitted
	if (!NRF24_send((uint8_t*)data, sizeof(data)))
	printf("\rsend failed");

	// wait till packet has been successfully sent, move on
	if (!NRF24_waitPacketSent())
	printf("waitPacketSent failed");
}



 
 /*
#include "Application.h"
#include "../AtmegaLibs/Serial_AtmegaLibs328p.h"
#include "../AtmegaLibs/SPI.h"
#include "../NRF/NRF24.h"
#define F_CPU 16000000
#include <util/delay.h>


#define LengthofServerName	5 
#define LengthofClient1Name	5

long int data_test= 0; 


void Client_Application(void){
	
	long int Data_final=0; 
	Client_Start((sizeof(Data_final)), "clie1");
	while(1){
		Client_SendData("serv1", Data_final);
		Client_RecieveData("serv1", Data_final);
		_delay_ms(100);
	}

}

void Server_Application(void){
	long int Data_final=0; 
	Server_Start(sizeof(Data_final), "serv1");
	while(1){
		Server_RecieveData("clie1", Data_final);
		Server_SendData("clie1",Data_final);
		_delay_ms(100);
	}
}


 void start_Server(char *servername){

	 uint8_t len = sizeof(data_test);
	 int x=0;
	 // nrf24_ping_server.pde
	 // -*- mode: C++ -*-
	 // Example sketch showing how to create a simple messaging server
	 // with the NRF24 class.
	 // It is designed to work with the example nrf24_ping_client.
	 // It also works with ping_client from the Mirf library
	 printf("SERVER START, will print out the time and sent");
	 if (!NRF24_Init())
	 printf("NRF24 init failed");
	 // Defaults after init are 2.402 GHz (channel 2)
	 // Now be compatible with Mirf ping_client
	 if (!NRF24_setChannel(1))
		printf("\rsetChannel failed");
	 if (!NRF24_setThisAddress((uint8_t*)"serv1", LengthofServerName))
		 printf("\rsetThisAddress failed");
	 if (!NRF24_setPayloadSize(sizeof(unsigned long)))
		 printf("setPayloadSize failed");
	 if (!NRF24_setRF(1,3))
		 printf("\rsetRF failed");
	
	 printf("\rinitialised");
	 	 
	 while(1)
	 {
		 printf("\rwaiting..........");
		 NRF24_waitAvailable();
		 // ping_client sends us an unsigned long containing its timestamp
		 if (!NRF24_recv((uint8_t*)&data_test, len))
		   printf("\rread failed");
		   
		   
		 else{
			printf("\rData Read: %d", data_test);
			 } 
		 
		 printf("\rSending Data %d + 1", data_test++);

		 // Now send the same data back
		 // Need to set the address of the detination each time, since auto-ack changes the TX address
		 if (!NRF24_setTransmitAddress((uint8_t*)"clie1", LengthofClient1Name))
		 printf("\rsetTransmitAddress failed");
		 // Send the same data back
		 
		 if (!NRF24_send((uint8_t*)&data_test, sizeof(data_test)))
		 printf("\rsend failed");
		 if (!NRF24_waitPacketSent())
		 {
			 printf("waitPacketSent failed");
		 }
	 }

 }

void start_Client(void) 
{
	// nrf24_ping_client.pde
	// -*- mode: C++ -*-
	// Example sketch showing how to create a simple messageing client
	// with the NRF24 class.
	// It is designed to work with the example nrf24_ping_server
	// It also works with ping_server from the Mirf library
	long int value = sizeof(long int);
	if (!NRF24_Init())
	printf("NRF24 init failed");
	// Defaults after init are 2.402 GHz (channel 2)
	// Now be compatible with Mirf ping_server
	if (!NRF24_setChannel(1))
		printf("setChannel failed");
	if (!NRF24_setThisAddress((uint8_t*)"clie1", LengthofClient1Name))
		printf("setThisAddress failed");
	if (!NRF24_setPayloadSize(sizeof(unsigned long)))
		printf("setPayloadSize failed");
	 if (!NRF24_setRF(1,3))
		printf("setRF failed");

	printf("initialized");


	// With printing commented and delay removed, this can achieve about 666 round trips per second
	while(1)
	{
		//  printf("send");
		
		// Send some data to the server
		if (!NRF24_setTransmitAddress((uint8_t*)"serv1", LengthofServerName))
		printf("setTransmitAddress failed");


		if (!NRF24_send((uint8_t*)&data_test, sizeof(long int)))
		printf("send failed");
		if (!NRF24_waitPacketSent())
		printf("waitPacketSent failed");
		//     printf("Sent");
	
		if (NRF24_waitAvailableTimeout(1000))
		{
			// Server should send the same time back to us

			//    printf("available");
			if (!NRF24_recv((uint8_t*)&data_test, value))
				printf("read failed");

			_delay_ms(20); 
			printf("Ping: ");
			printf("%d \r" , data_test);
			// Compute the round trip time. This is the time to send one (acknowledged) message (with data)
			// to the server and
			// for the server to send one (acknowledged) reply (with data) back to us
			// Typical reported RTT is 1- 2 ms
			
		}
		else
		printf("No reply from server");

		_delay_ms(1000);
	}

}




void Client_Start(char payloadsize, char *Client_Name){
//	struct WirelessCom *Point_Data = &Data;
	/// Start Client Process
	printf("\rClient start, read the data from server");
	if (!NRF24_Init())
		printf("\rNRF24 init failed");
	// Defaults after init are 2.402 GHz (channel 2)
	// Now be compatible with Mirf ping_server
	if (!NRF24_setChannel(1))
		printf("\rsetChannel failed");
	// Set the address for the client!	
	if (!NRF24_setThisAddress((uint8_t*)Client_Name, LengthofClient1Name))
		printf("\rsetThisAddress failed");
	// Set the payload size max = 32	
	if (!NRF24_setPayloadSize(payloadsize))
		printf("\rsetPayloadSize failed");
	// Set 2mbps at 0dB	
	if (!NRF24_setRF(1,3))
		printf("\rsetRF failed");
	printf("\rinitialized");	
}

void Client_RecieveData(char *ServerName, char *buffer){	
///	struct WirelessCom *Point_Data = &Data;
	// Send some data to the server
	if (!NRF24_setTransmitAddress((uint8_t*)ServerName, LengthofServerName))
		printf("\rsetTransmitAddress failed");
		
	if (NRF24_waitAvailableTimeout(100))
	{
		// Server should send the same time back to us
        //printf("available");
		if (!NRF24_recv((uint8_t*)&data_test, sizeof(data_test))) // receive data + 1
			printf("\rread failed");
	
		printf("- Data Received: %d", data_test);
		_delay_ms(1000);
	}
	else
	printf("No reply from server");
	
}
void Client_SendData(char *ServerName, char *stuff){
//	struct WirelessCom *Point_Data = &Data;
	printf("\rsend");
	// Send some data to the server
	if (!NRF24_setTransmitAddress((uint8_t*)ServerName, LengthofServerName))
	printf("\rsetTransmitAddress failed");

	if (!NRF24_send((uint8_t*)&data_test, sizeof(data_test)))
	printf("\rsend failed");
	if (!NRF24_waitPacketSent())
	printf("\rwaitPacketSent failed");
	printf("\r Sent: %d", data_test); /// send 0
}


 
 void Server_Start(char payloadsize, char *ServerName){
		
	 printf("SERVER START, will print out the time and sent");
	 if (!NRF24_Init())
		 printf("NRF24 init failed");
	 
	 // Defaults after init are 2.402 GHz (channel 2)
	 // Now be compatible with Mirf ping_client
	 if (!NRF24_setChannel(1))
		 printf("\rsetChannel failed");
	 // Set Address
	  if (!NRF24_setThisAddress((uint8_t*)ServerName, LengthofServerName))
	 //if (!NRF24_setThisAddress((uint8_t*)ServerName, 5))
		 printf("\rsetThisAddress failed");
	 /// Set payload size
	 if (!NRF24_setPayloadSize(payloadsize))
		 printf("setPayloadSize failed");
	 
	 if (!NRF24_setRF(1,3))	/// 2mbps at 0db
		printf("\rsetRF failed");
	 printf("\rinitialised");
 }
 
 void Server_RecieveData(char *ClientName, char *buffer){
	char length = sizeof(buffer); 
	 // Wait till Server is available
	 NRF24_waitAvailable();
	 // ping_client sends us an unsigned long containing its timestamp
	 if (!NRF24_recv((uint8_t*)&data_test, length))
		 printf("\rread failed");
	 printf("\rData Read: %d",data_test); 
	
 }
 
 void Server_SendData(char *ClientName, char *data){
	 
	 data_test++; 
	 printf("\rSending Data: %d", data_test);
	 // Now send the same data

	 // Need to set the address of the detination each time, since auto-ack changes the TX address
	// if (!NRF24_setTransmitAddress((uint8_t*)ClientName, 5))
	 if (!NRF24_setTransmitAddress((uint8_t*)ClientName, LengthofClient1Name))	
		 printf("\rsetTransmitAddress failed");
	 // Send the same data after address has been transmitted
	 if (!NRF24_send((uint8_t*)&data_test, sizeof(data_test)))
		 printf("\rsend failed");

	 // wait till packet has been successfully sent, move on
	 if (!NRF24_waitPacketSent())
		 printf("waitPacketSent failed");
 }
 
 
 
 
 */
 
 /*
 
 
 void Client_Application(void){
	 char com_state;
	 char *data= "PONG";
	 Client_Start((sizeof(data)), "clie1");
	 printf("initialized Client: %s \r It will be put into send mode", "serv1");
	 printf("\r Welcome to the client module I am controlled by the server using the nrf wireless device");
	 printf("\r Please choose one of the following options: ");
	 printf("\r 1. Put Client in send state and receive string. ");
	 printf("\r 2. Put Client in receive state and send string. ");
	 printf("\r 3. Send custom string out. ");
	 
	 
	 
	 while(1){
		 printf("\r please choose a state: ");
		 com_state= USART_receive();
		 
		 switch (com_state){
			 
			 case '1':
			 data[0]= 'P';
			 printf("\r Sending out: %s", data);
			 Client_SendData("serv1",  (char*)*&data); /// send data first then delay
			 break;
			 case '2':
			 Client_RecieveData("serv1", (char*)*&data);
			 printf("\r Received: %s", data);
			 break;
			 case '3':
			 data[0]= 'P';
			 printf("\r Sending out: %s", data);
			 Client_SendData("serv1",  (char*)*&data); /// send data first then delay
			 Client_RecieveData("serv1", (char*)*&data);
			 printf("\r Received: %s", data);
			 _delay_ms(100);
			 break;
		 }
	 }

	 
 }

 void Server_Application(void){
	 char *data= "PING";
	 char com_state;
	 Server_Start(sizeof(data), "serv1");
	 printf("\r Initialized Server: %s ", "serv1");
	 printf("\r Welcome to the server module I am controlled via RS232 communications using bluetooth technology");
	 printf("\r Please choose one of the following options: ");
	 printf("\r 1. Put Server in receive state and receive string. ");
	 printf("\r 2. Put Server in send state and send string. ");
	 printf("\r 3. Send custom string out. ");
	 //printf("I will put it into receive mode", "serv1");
	 
	 while(1){
		 printf("\r please choose a state: ");
		 com_state= USART_receive();
		 
		 switch (com_state){
			 
			 case '1':
			 Server_RecieveData("clie1", (char*)*&data);
			 printf("\r Received: %s", data);
			 break;
			 case '2':
			 data[0]= 'D';
			 printf("\r Sending out: %s", data);
			 Server_SendData("clie1", (char*)*&data);
			 break;
			 case '3':
			 Server_RecieveData("clie1", (char*)*&data);
			 printf("\r Received: %s", data);
			 data[0]= 'D';
			 printf("\r Sending out: %s", data);
			 Server_SendData("clie1", (char*)*&data);
			 break;
		 }
	 }

	 
 }
 
 */ 