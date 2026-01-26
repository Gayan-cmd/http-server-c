#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdio.h>
#define PORT 8080

int main(){

    WSADATA wsaData;

    int WSA_result = WSAStartup(MAKEWORD(2,2),&wsaData);

    if (WSA_result != 0){
        printf("WSAStartup failed: %d\n",  WSAGetLastError());
        return 1;
    }

    printf("Winsock initialized successfully!\n");

    SOCKET server_socket = socket(AF_INET, SOCK_STREAM, 0);

    if(server_socket == INVALID_SOCKET){
        printf("Socket creation failed: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    printf("Socket Created Successfuly .\n");

    struct sockaddr_in server_addr;

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    int socket_bind_result = bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));

    if (socket_bind_result == SOCKET_ERROR){
        printf("Socket Binding Failed: %d \n", WSAGetLastError());
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    printf("Socket bound to port %d \n", PORT);

    int socket_listen_result = listen(server_socket, 5);

    if (socket_listen_result == SOCKET_ERROR){
        printf("Socket listing failed %d \n", WSAGetLastError());
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    printf("Server listening on port %d...\n", PORT);
    printf("Open your browser and go to: http://localhost:%d\n", PORT);

    printf("\nPress Enter to shut down the server...\n");
    getchar();

    closesocket(server_socket);
    WSACleanup();

    printf("Server shut down");

    return 0;
}