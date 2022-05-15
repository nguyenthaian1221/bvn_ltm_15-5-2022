// MultithreadServer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>
#pragma comment(lib, "ws2_32")
#pragma warning(disable : 4996)
SOCKET clients[10];
int numClients = 0;

/*
DWORD WINAPI ClientThread(LPVOID lpParam)
{
    SOCKET client = *(SOCKET*)lpParam;
    int ret;
    char buf[256];
    // Nhan du lieu tu client va in ra man hinh
    while (1)
    {
        ret = recv(client, buf, sizeof(buf), 0);
        if (ret <= 0)
            break;
        buf[ret] = 0;
        printf("Du lieu nhan duoc: %s\n", buf);
    }
    closesocket(client);
}
*/

/*
 DWORD WINAPI SendAllClientThread(LPVOID lpParam)
{
    SOCKET client = *(SOCKET*)lpParam;
    int ret;
    char buf[256];
    // Nhan du lieu tu client va in ra man hinh
    while (1)
    {
        ret = recv(client, buf, sizeof(buf), 0);
        if (ret <= 0)
            break;
        buf[ret] = 0;
        printf("Du lieu nhan duoc: %s\n", buf);

        // gui du lieu
        for (SOCKET idxClient : clients)
        {
            // khong gui truong hop client la chinh no (client gui)
            if(client != idxClient)
                send(idxClient, buf, ret, 0);
        }
        
            
        
    }
    closesocket(client);
}
*/



DWORD WINAPI SendByClientSocketThread(LPVOID lpParam)
{
    SOCKET client = *(SOCKET*)lpParam;
    int ret;
    char buf[256];
    // Nhan du lieu tu client va in ra man hinh
    while (1)
    {
        ret = recv(client, buf, sizeof(buf), 0);
        if (ret <= 0)
            break;
        buf[ret] = 0;
        printf("Du lieu nhan duoc: %s\n", buf);

/*
        // gui du lieu cho toan Clients tru ban than thang gui
        for (SOCKET idxClient : clients)
        {
            // khong gui truong hop client la chinh no (client gui)
            if (client != idxClient  )
                send(idxClient, buf, ret, 0);
        }

*/
        // Tach lay all hoac la ma toi
        char* str = buf;
        char* token = strtok(str, " ");
        
        // tach lay noi dung can gui;
        int numbersocket = strlen(token);


        char* message = &str[numbersocket] + 1;
        char messageafterconcat[100];
        sprintf(messageafterconcat, "%d: %s", client, message);
        int sizeofmessage = strlen(messageafterconcat);


        for (SOCKET idxClient : clients)
        {
            // khong gui truong hop client la chinh no (client gui)
            if (client != idxClient && strcmp(token, "all") == 0)
                send(idxClient, messageafterconcat, sizeofmessage, 0);
            // gui cho client dung ma
            else if (client != idxClient && atoi(token) == idxClient )
                send(idxClient, messageafterconcat, sizeofmessage, 0);
        }
        


    }
    closesocket(client);
}




int main()
{
    // Khoi tao thu vien
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);
    // Tao socket
    SOCKET listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    // Khai bao dia chi server
    SOCKADDR_IN addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(8000);
    // Gan cau truc dia chi voi socket
    bind(listener, (SOCKADDR*)&addr, sizeof(addr));
    // Chuyen sang trang thai cho ket noi
    listen(listener, 5);
    while (1)
    {
        SOCKET client = accept(listener, NULL, NULL);
        clients[numClients] = client;
        numClients++;

        printf("Client moi ket noi: %d\n", client);
        CreateThread(0, 0, SendByClientSocketThread, &client, 0, 0);
        
        

    }
}