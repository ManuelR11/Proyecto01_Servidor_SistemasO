#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include "chat_protocol.pb.h" // Importa el protocolo generado por protobuf

using namespace std;

// Función para manejar la comunicación con un cliente en un hilo separado
void handleClient(int clientSocket) {
    // Implementa la lógica de manejo de cliente aquí
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        cerr << "Uso: " << argv[0] << " <puerto>" << endl;
        return 1;
    }

    int port = atoi(argv[1]);

    // Crea el socket del servidor
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        cerr << "Error al crear el socket del servidor" << endl;
        return 1;
    }

    // Configura la dirección del servidor
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    // Enlaza el socket a la dirección del servidor
    if (bind(serverSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) == -1) {
        cerr << "Error al enlazar el socket a la dirección del servidor" << endl;
        return 1;
    }

    // Escucha por conexiones entrantes
    if (listen(serverSocket, 10) == -1) {
        cerr << "Error al poner el servidor en modo escucha" << endl;
        return 1;
    }

    cout << "Servidor de chat iniciado en el puerto " << port << endl;

    // Acepta conexiones entrantes y maneja cada una en un hilo separado
    while (true) {
        struct sockaddr_in clientAddr;
        socklen_t clientAddrLen = sizeof(clientAddr);
        int clientSocket = accept(serverSocket, (struct sockaddr *) &clientAddr, &clientAddrLen);
        if (clientSocket == -1) {
            cerr << "Error al aceptar la conexión entrante" << endl;
            continue;
        }

        cout << "Nueva conexión aceptada" << endl;

        // Crea un hilo para manejar la comunicación con el cliente
        thread clientThread(handleClient, clientSocket);
        clientThread.detach(); // Desconecta el hilo principal del hilo del cliente
    }

    close(serverSocket);
    return 0;
}
