#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdio.h>

int main(){

    WSADATA wsaData;

    int result = WSAStartup(MAKEWORD(2,2),&wsaData);

    if (result != 0){
        printf("WSAStartup failed: %d\n",  WSAGetLastError());
        return 1;
    }

    printf("Winsock initialized successfully!\n");
    printf("Ready to build our HTTP server!\n");

    WSACleanup();

    return 0;
}