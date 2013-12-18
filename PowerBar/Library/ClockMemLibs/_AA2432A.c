

#include "_AA2432A.h"

void _24AA32A_SendByte(int byte_address, char byte){
	I2C_StartWrite();
	I2C_SendByte(Device_Address); 
	I2C_SendByte((int) (byte_address>>8));  // MSB 
	I2C_SendByte((int)(byte_address & 0xFF));
	I2C_SendByte(byte);
	I2C_TerminateTransmission();
	DataInDelay();
}

// must be under 32 bytes if over 32 bytes the device writes over the data in the 
// starting address and moves forward from there. 

void _24AA32A_Send32Bytes(int byte_address, char length, char *byte){
	I2C_StartWrite();
	I2C_SendByte(Device_Address);
	I2C_SendByte((int) (byte_address>>8));  // MSB
	I2C_SendByte((int)(byte_address & 0xFF));
	for (int x=0; x< length; x++){
		I2C_SendByte(byte[x]);
	}	
	I2C_TerminateTransmission();
	DataInDelay();
}

// i dont like this module as its very slow and in-efficient.
// input the address for the memory to be read, how long you wish to read, then a char * to the storage. 
void _24AA32A_ReadMultipleBytes_Slow(int byte_address, int length_read, char *store_data){
	//char readbytes[100];
	// Send the desired byte address
	for(int read=0; read <= length_read; read++){
		I2C_StartWrite();
		I2C_SendByte(Device_Address);
		I2C_SendByte((int) ((byte_address+read)>>8));  // MSB
		I2C_SendByte((int)((byte_address+read) & 0xFF));
		// Restart the device, place device in read mode
		I2C_TerminateTransmission();
		I2C_StartWrite();
		I2C_SendByte(0xA1);
		store_data[read] = I2C_ReadByte();
	//	USART_send(store_data[read]);
		I2C_TerminateTransmission();
		DataOutDelay(); 
	}
	store_data[length_read] = 0x00;
}


void _24AA32A_SendMultipleBytes_Slow(int byte_address, char total_length, char *byte){
	
	for (int i=0; i < total_length; i++)
	{
	I2C_StartWrite();
	I2C_SendByte(Device_Address);
	I2C_SendByte((int) (byte_address + i>>8));  // MSB
	I2C_SendByte((int)(byte_address + i & 0xFF));
	I2C_SendByte(byte[i]);
	I2C_TerminateTransmission();
	DataInDelay(); 
	}
	
	
}

char _24AA32A_ReadByte(int byte_address){
	char readbyte =0x00;
	// Send the desired byte address
	I2C_StartWrite();
	I2C_SendByte(Device_Address);
	I2C_SendByte((int) (byte_address>>8));  // MSB
	I2C_SendByte((int)(byte_address & 0xFF));
	// Restart the device, place device in read mode
	I2C_TerminateTransmission();
	I2C_StartWrite();
	I2C_SendByte(0xA1);
	
	readbyte= I2C_ReadByte();	
	I2C_TerminateTransmission();
	DataOutDelay();
	return readbyte; 
}







/* _24AA32A_H_ 


// LIBRARIES THAT DONT WORK PROPERLY 
/*
void _24AA32A_SendMultipleBytes(int byte_address, char total_length, char *byte){
	char _32Bitmem[WriteBit];
	int byte32count, copystring, send32bytes=0, remainderbytes=0, bytelength=0;
	send32bytes = (total_length/WriteBit);
	remainderbytes = total_length%WriteBit;
	
	for (byte32count=0; byte32count<= (send32bytes); byte32count++){
		
		if (byte32count == send32bytes) bytelength=remainderbytes;
		else bytelength = WriteBit;
		
		for (copystring=0; copystring < bytelength; copystring++){
			_32Bitmem[copystring] = byte[copystring+(byte32count*WriteBit)];
		}
		_24AA32A_Send32Bytes(byte_address + (byte32count*WriteBit), bytelength, _32Bitmem);
		DataInDelay();
	//for (int clear=0; clear < 32 ; clear++){_32Bitmem[clear]= 0x00;}
	}
}


char* _24AA32A_ReadMultipleBytes(int byte_address, int fucksake){
	char readbytes[100];
	// Send the desired byte address
	I2C_StartWrite();
	I2C_SendByte(Device_Address);
	I2C_SendByte((int) (byte_address>>8));  // MSB
	I2C_SendByte((int)(byte_address & 0xFF));
	// Restart the device, place device in read mode
	I2C_TerminateTransmission();
	I2C_StartWrite();
	I2C_SendByte(0xA1);
	
	for(int read=0; read <= fucksake; read++){
		readbytes[read] = I2C_ReadByte();
		TWCR|=(1<<TWEA);  // have the I2C master transmit an ack
		USART_send(readbytes[read]);
	}
	I2C_TerminateTransmission();
	return readbytes;
}



 */
/*


void _24AA32A_SendMultipleBytes(int byte_address, char total_length, char *byte){
	
	for (int byte32count=0; byte32count< 1 + (total_length/32); byte32count++){
		I2C_StartWrite();
		I2C_SendByte(Device_Address);
		I2C_SendByte((int) ((byte_address+(byte32count*32))>>8));  // MSB
		I2C_SendByte((int)((byte_address+(byte32count*32)) & 0xFF));
		
		for (int x=0; x< 32; x++){
			I2C_SendByte(byte[x+ (byte32count*32)]);
		}
		
		I2C_TerminateTransmission();
		DataInDelay();
	}
}



void _24AA32A_SendMultipleBytes(int byte_address, char total_length, char *byte){
	char *_32Bitmem;
	for (int byte32count=0; byte32count< 1 + (total_length/32); byte32count++){
		
		for(int copystring=0; copystring< 32; copystring++){
			_32Bitmem[copystring] = byte[(32*byte32count)+copystring];
		}
		
		_24AA32A_Send32Bytes((byte_address+(byte32count*32)), strlen(_32Bitmem), _32Bitmem);
	}
}


void _24AA32A_SendMultipleBytes2(int byte_address, char start_byte, char end_byte,  char *byte){
	
	I2C_StartWrite();
	I2C_SendByte(Device_Address);
	I2C_SendByte((int) ((byte_address)>>8));  // MSB
	I2C_SendByte((int)((byte_address) & 0xFF));
	
	for (int x=start_byte; x< end_byte; x++){
		I2C_SendByte(byte[x]);
	}
	I2C_TerminateTransmission();
	DataInDelay();
	
}
*/