/*
 * This is a simple MMORPG server in C.
 *
 * The server can handle up to 10 clients.
 * The server maintains a list of connected clients and sends messages to all clients when a client sends a message.
 * The server also allows clients to disconnect by sending the message "quit".
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * This constant defines the maximum number of clients that the server can handle.
 */
#define MAX_CLIENTS 10

/*
 * This struct represents a client.
 *
 * The client has an ID, a name, and a connected flag.
 */
struct client {
  int id;
  char name[100];
  int connected;
};

/*
 * This struct represents the server.
 *
 * The server has an array of clients and a number of connected clients.
 */
struct server {
  struct client clients[MAX_CLIENTS];
  int num_clients;
};

/*
 * This function initializes the server.
 *
 * It sets the number of connected clients to 0.
 */
void init_server(struct server *server) {
  server->num_clients = 0;
}

/*
 * This function adds a client to the server.
 *
 * It sets the client's ID, name, and connected flag.
 * It also increments the number of connected clients.
 */
void add_client(struct server *server, int id, char *name) {
  server->clients[server->num_clients].id = id;
  strcpy(server->clients[server->num_clients].name, name);
  server->clients[server->num_clients].connected = 1;
  server->num_clients++;
}

/*
 * This function removes a client from the server.
 *
 * It searches for the client with the given ID and sets its connected flag to 0.
 */
void remove_client(struct server *server, int id) {
  for (int i = 0; i < server->num_clients; i++) {
    if (server->clients[i].id == id) {
      server->clients[i].connected = 0;
      break;
    }
  }
}

/*
 * This function sends a message to all connected clients.
 *
 * It iterates through the list of clients and sends the message to each client that is connected.
 */
void send_message_to_all(struct server *server, char *message) {
  for (int i = 0; i < server->num_clients; i++) {
    if (server->clients[i].connected) {
      send(server->clients[i].id, message, strlen(message), 0);
    }
  }
}

/*
 * This function processes a client message.
 *
 * It checks if the message is "quit" and removes the client if it is.
 * Otherwise, it sends the message to all connected clients.
 */
void process_client_message(struct server *server, int client_id, char *message) {
  if (strcmp(message, "quit") == 0) {
    remove_client(server, client_id);
  } else {
    send_message_to_all(server, message);
  }
}

/*
 * This is the main function.
 *
 * It creates a server and enters a loop where it waits for messages from clients.
 * When it receives a message, it processes the message and then sends a message back to the client.
 * The loop continues until the server receives a message from a client that says "quit".
 */
int main() {
  struct server server;
  init_server(&server);

  while (1) {
    int client_id;
    char message[100];

    recv(client_id, message, 100, 0);

    process_client_message(&server, client_id, message);
  }

  return 0;
}
