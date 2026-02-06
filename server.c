#include <WinSock2.h>  
#include <WS2tcpip.h>
#include <stdio.h>
#include <string.h> 

#define PORT 8080

int main()
{

  WSADATA wsaData;

  int WSA_result = WSAStartup(MAKEWORD(2, 2), &wsaData);

  if (WSA_result != 0)
  {
    printf("WSAStartup failed: %d\n", WSAGetLastError());
    return 1;
  }

  printf("Winsock initialized successfully!\n");

  SOCKET server_socket = socket(AF_INET, SOCK_STREAM, 0);

  if (server_socket == INVALID_SOCKET)
  {
    printf("Socket creation failed: %d\n", WSAGetLastError());
    WSACleanup();
    return 1;
  }

  printf("Socket Created Successfuly .\n");

  struct sockaddr_in server_addr;

  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(PORT);

  int socket_bind_result =
      bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));

  if (socket_bind_result == SOCKET_ERROR)
  {
    printf("Socket Binding Failed: %d \n", WSAGetLastError());
    closesocket(server_socket);
    WSACleanup();
    return 1;
  }

  printf("Socket bound to port %d \n", PORT);

  int socket_listen_result = listen(server_socket, 5);

  if (socket_listen_result == SOCKET_ERROR)
  {
    printf("Socket listing failed %d \n", WSAGetLastError());
    closesocket(server_socket);
    WSACleanup();
    return 1;
  }

  printf("Server listening on port %d...\n", PORT);
  printf("Open your browser and go to: http://localhost:%d\n", PORT);

  int request_count = 0;

  while (1)
  {

    struct sockaddr_in client_addr;
    int client_addr_size = sizeof(client_addr);

    printf("\nWaiting for a connection...\n");

    SOCKET client_socket =
        accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_size);

    if (client_socket == INVALID_SOCKET)
    {
      printf("Accepting connection failed: %d \n", WSAGetLastError());
      closesocket(server_socket);
      WSACleanup();
      return 1;
    }

    request_count++;
    printf("\n=== Request #%d from %s ===\n", request_count, inet_ntoa(client_addr.sin_addr));

    printf("Client connected!\n");

    printf("Client IP: %s\n", inet_ntoa(client_addr.sin_addr));
    printf("Server IP: %s\n", inet_ntoa(server_addr.sin_addr));

    char buffer[4096];
    int bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);

    if (bytes_received == SOCKET_ERROR)
    {
      printf("Recv failed: %d\n", WSAGetLastError());
      closesocket(client_socket);
      closesocket(server_socket);
      WSACleanup();
      return 1;
    }

    buffer[bytes_received] = '\0';

    printf("\n--- Received HTTP Request ---\n");
    printf("%s", buffer);
    printf("--- End of Request ---\n");

    const char *html_content = "<h1>Hello World</h1>";

    const int html_content_lenght = strlen(html_content);
    char response[4096];

    sprintf(response,
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html\r\n"
            "Content-Length: %d\r\n"
            "Connection: close\r\n"
            "\r\n"
            "%s",
            html_content_lenght, html_content);

    int bytes_sent = send(client_socket, response, strlen(response), 0);

    if (bytes_sent == SOCKET_ERROR)
    {
      printf("Failed to send response: %d\n", WSAGetLastError());
      closesocket(client_socket);
      closesocket(server_socket);
      WSACleanup();
      return 1;
    }

    printf("Response sent successfully! Waiting for next connection...\n");

    closesocket(client_socket);
  }

  closesocket(server_socket);
  WSACleanup();
  printf("Server shut down");

  return 0;
}