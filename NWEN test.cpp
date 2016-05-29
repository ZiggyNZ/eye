//Test code for NWEN gate
#include <stdio.>
#include <opengate.h>
#include <time.h>

//External imports
extern "C" int init(int d_lev);
extern "C" int connect_to_server( char server_addr[15],int port);
extern "C" int send_to_server(char message[24]);
extern "C" int receive_from_server(char message[24]);

int openGate(char serverIP){ //Wiki says 192.168.1.2
	init(1);
	connect_to_server(serverIP, 1024);
	send_to_server("IP Please");
	char message[24];
	receive_from_server(message);
	printf("%s", message);
return 0;}























