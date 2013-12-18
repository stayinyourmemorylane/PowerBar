/*
 * Application.h
 *
 * Created: 6/13/2013 3:03:56 PM
 *  Author: mfolz
 */ 


#ifndef APPLICATION_H_INCLUDED
#define APPLICATION_H_INCLUDED


void Client_Application(void);
void Server_Application(void);
void Client_Start(char payloadsize, char *Client_Name);
void Client_RecieveData(char *ServerName, char *buffer);
void Client_SendData(char *ServerName, char *data);
void Server_Start(char payloadsize, char *ServerName);
void Server_RecieveData(char *ClientName, char *buffer);
void Server_SendData(char *ClientName, char *data);




#endif /* APPLICATION_H_ */