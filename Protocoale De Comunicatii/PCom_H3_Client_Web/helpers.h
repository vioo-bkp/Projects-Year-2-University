#ifndef _HELPERS_
#define _HELPERS_

/*
 * Macro de verificare a erorilor.
 * Exemplu:
 *     int fd = open(file_name, O_RDONLY);
 *     DIE(fd == -1, "open failed");
 * Preluat din laborator.
 */
#define DIE(condition, msg) \
	do { \
		if (condition) { \
			fprintf(stderr, "(%s:%d): %s\n", __FILE__, __LINE__, msg); \
			perror(""); \
			exit(1); \
		} \
	} while (0)

#define BUFLEN 4096				// dimensiunea maxima a calupului de date
#define LINELEN 1000			// lungimea maxima a unei linii
#define HOSTIP "34.118.48.238"  // adresa IP a serverului

// shows the current error
void error(const char *msg);

// adds a line to a string message
void compute_message(char *message, const char *line);

// opens a connection with server host_ip on port portno, returns a socket
int open_connection(char *host_ip, int portno, int ip_type, int socket_type, int flag);

// closes a server connection on socket sockfd
void close_connection(int sockfd);

// send a message to a server
void send_to_server(int sockfd, char *message);

// receives and returns the message from a server
char *receive_from_server(int sockfd);

// extracts and returns a JSON from a server response
char *basic_extract_json_response(char *str);

#endif
