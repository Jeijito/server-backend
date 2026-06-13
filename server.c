//first prototype for communcation between laptop and pc


#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>  //SOCKET API
#include <netinet/in.h>  //INTERNET PROTOCOL
#include <unistd.h> //POSIX API
		   

//
//socket() is a simple intger where 0 is the standard input, 1 is the standard output and 2 is the standard error. The next number in the Kernel is 3 so it would be 3.
//
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
//socket() has 3 parameters:
//	domain: The protocol family or network layer space the socket will use to communicate.
//		AF_INET uses ipv4
//		AFINET6 uses ipv6
//		AF_UNIX or AF_LOCAL uses local IPs, same computer, cannot travel through WIFI
//	type: The behavioral rules of the data stream
//		SOCK_STREAM, fprces a quential error checked stream
//		SOCKDGRAM forces a quick unverified stream used for videogames or VC like discord
//			
//	protocol: The precise transport-layer mathematical protocol to back the socket type.
//		0 is Auto-matching which tells the kernel any type of socket is being used.
//		6 requires to use SOCK_STREAM as the socket type, and forces TCP (transmission control protocol), which splits the data being sent into different pieces. 
//		17 requires to use SOCK_DGRAM whic hises UDP which is used for streaming voice or video games
//		132 requires to use SOCK_STREAM or SOCK_SEQPACKET which is used for advabced multistream data transport
//		255 requires to use SOCK_RAW which is used for netowrk engineering, custom packets and sniffing
//
//
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------	
//
// struct sockaddr_in address is a built in structure to store IPv4 sockets.
//
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
//port is 8080 becuase port 80 is the standard HTTP port and it is easier for testing
//
//other examples for port numbers could be 443 for HTTPS secure web traffic and port  22 SSH secure remove access
//
//Ports from 1 to 1023 are known as well-known Ports. Only User Admin can bind a socket to a port below 1024. If I use 80 as the port I would need to run it as sudo ./server 
//
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//


int main(){
	int server_fd; // stores operation
		      
	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	


	if(server_fd < 0){
		perror("SOCKET CREATION FAILED");
		exit(EXIT_FAILURE);
	}

	printf("Socket created FD ID: %d\n", server_fd);

	close(server_fd);
	return 0;
}
	
