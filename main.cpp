/**
 * TCP server, this server binds to the port 54000 of the localhost
 * when a client connects and sends a message the server receives it and
 * send the reversed string back to the client.
 * at this stage it is possible to connect using telnet on linux terminal but the experience
 * is not great
 */

#include <sys/socket.h>
#include <cstring>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string>
#include <bits/stdc++.h>


int main() {

    //create a socket
    int listeningSocket = socket(AF_INET, SOCK_STREAM, 0);

    if(listeningSocket == -1){

        printf("%s", "can't create socket");
        return -1;

    }

    //bind it to any IP/port that we can find
    sockaddr_in hint; // hint for the server side
    hint.sin_family = AF_INET;
    hint.sin_port = htons(54000);
    inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr);

    if(bind(listeningSocket, reinterpret_cast<const sockaddr *>(&hint), sizeof(hint)) == -1){

        printf("%s", "can't bind to IP/port");
        return -2;

    }

    //Mark the socket for listening in
    if(listen(listeningSocket, SOMAXCONN) == -1){

        printf("%s", "can't listen");
        return -3;

    }

    //accept a call
    sockaddr_in client{};
    socklen_t  clientSize = sizeof(client);
    char host[NI_MAXHOST];
    char svc[NI_MAXSERV];

    int clientSocket = accept(listeningSocket, (sockaddr*)&client, &clientSize);

    if(clientSocket == -1){

        printf("%s", "problem with client connection");
        return -4;

    }

    //Close the listening socket
    close(listeningSocket);

    memset(host, 0, NI_MAXHOST);
    memset(svc, 0, NI_MAXSERV);

    int result = getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, svc, NI_MAXSERV, 0);

    if(result){

        printf("%s%s%s%s", host, " connected ", svc, "\n");

    }else{

        inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
        printf("%s%s%i%s", host, " connected on ", ntohs(client.sin_port), "\n");

    }

    // While receiving display and echo the message
    char buf[4096];
    while(true){

        //clear the buffer
        memset(buf, 0, 4096);
        //wait for message
        int byteRecv = recv(clientSocket, buf, 4096, 0);
        if(byteRecv == -1){

            printf("%s", "there was an error in byteRecv");
            break;

        }

        if(byteRecv == 0){

            printf("%s", "client disconnected");
            break;

        }

        std::string message = std::string(buf, 0, byteRecv);
        reverse(message.begin(), message.end());

        //display message
        printf("%s%s%s%s%s", "  ", std::string(buf, 0, byteRecv).c_str(),"******", message.c_str(), "\n");

        //send inverted message
        send(clientSocket, message.c_str(), message.size()+1, 0);

    }

    //close the socket
    close(clientSocket);


    return 0;
}