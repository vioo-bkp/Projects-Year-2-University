#ifndef _HELPERS_H
#define _HELPERS_H 1

#include <arpa/inet.h>
#include <math.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include "list.h"
#include "queue.h"

/*
 * Macro de verificare a erorilor.
 * Exemplu:
 *     int fd = open(file_name, O_RDONLY);
 *     DIE(fd == -1, "open failed");
 * Preluat din laborator.
 */
#define DIE(assertion, call_description)				\
	do {								\
		if (assertion) {					\
			fprintf(stderr, "(%s, %d): ",			\
					__FILE__, __LINE__);		\
			perror(call_description);			\
			exit(EXIT_FAILURE);				\
		}							\
	} while(0)

#define BUFLEN 1500 		// dimensiunea maxima a calupului de date
#define IDLEN 11			// lungimea maxima a id-ului unui client
#define TOPICLEN 50			// lungimea maxima a numelui unui topic
#define SUBSCRIBTIONLEN	11 	// lungimea maxima a unei actiuni/comenzi de abonare/dezabonare
#define MAX_CLIENTS	5		// numarul maxim de clienti in asteptare
#define CLIENT_ON 1			// client online
#define CLIENT_OFF 0		// client offline

#define INT_TYPE 0			// tipul de date INT
#define SHORT_REAL_TYPE 1	// tipul de date SHORT_REAL
#define FLOAT_TYPE 2		// tipul de date FLOAT
#define STRING_TYPE 3		// tipul de date STRING

// Structura pentru datele unui client tcp.
typedef struct tcp_client {
	char id[IDLEN];
	int sockfd;
	int status;
	queue queued_messages;
} tcp_client;

// Structura pentru abonarea unui client tcp la un topic.
typedef struct subscriber {
	tcp_client *client;
	int sf;
} subscriber;

// Structura pentru topic.
typedef struct topic {
	char name[TOPICLEN];
	list subscribers;
} topic;

// Structura pentru mesajele de tip udp.
typedef struct udp_message {
	char topic[TOPICLEN];
	char data_type;
	char payload[BUFLEN];
} udp_message;

// Structura pentru mesajele de tip tcp.
typedef struct tcp_message {
	struct sockaddr_in udp_client;
	char topic[TOPICLEN];
	char data_type;
	char payload[BUFLEN];
} tcp_message;

// Structura pentru mesajele de abonare/dezabonare ale clientilor la topics.
typedef struct subscribtion_message {
	char subscribe[SUBSCRIBTIONLEN];
	char topic[TOPICLEN];
	int sf;
} subscribtion_message;

#endif
