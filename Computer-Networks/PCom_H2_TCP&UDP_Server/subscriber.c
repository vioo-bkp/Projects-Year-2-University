#include "helpers.h"

// Functie preluata din laborator.
void usage(char *file) {
  fprintf(stderr, "Usage: %s server_address server_port\n", file);
  exit(0);
}

/*
   Functie pentru procesarea unui mesaj tcp primit de client de la server.

   Mesajul tcp poate contine date de tip INT, SHORT_REAL, FLOAT sau STRING.

   Se realizeaza afisarea corecta a datelor in functie de tipul lor.
*/
void process_tcp_message(tcp_message *tcp_msg) {
  // Octetul de semn.
  char sign = 0;

  // Date de tip INT.
  if (tcp_msg->data_type == INT_TYPE) {
    uint32_t module;

    // Se copiaza octetul de semn si modulul numarului in sign, respectiv module.
    memcpy(&sign, &(tcp_msg->payload), 1);
    memcpy(&module, &(tcp_msg->payload[1]), sizeof(uint32_t));

    /* 
       Se realizeaza conversia modului de la network byte order la host byte 
       order si se adauga semnul numarului.
    */
    int32_t value = ntohl(module);
    if (sign) {
      value *= -1;
    }

    printf("%s:%d - %s - INT - %d\n", inet_ntoa(tcp_msg->udp_client.sin_addr),
           ntohs(tcp_msg->udp_client.sin_port), tcp_msg->topic, value);
  } else if (tcp_msg->data_type == SHORT_REAL_TYPE) {
  	// Date de tip SHORT-REAL.

    uint16_t module;

    // Se copiaza modulul numarului in module.
    memcpy(&module, &(tcp_msg->payload), sizeof(uint16_t));

    /* 
       Se realizeaza conversia modului de la network byte order la host byte 
       order.
    */    
    float value = ntohs(module);

    printf("%s:%d - %s - SHORT_REAL - %.2f\n",
           inet_ntoa(tcp_msg->udp_client.sin_addr),
           ntohs(tcp_msg->udp_client.sin_port), tcp_msg->topic, value / 100);
  } else if (tcp_msg->data_type == FLOAT_TYPE) {
  	// Date de tip FLOAT.

    uint32_t module;
    uint8_t pow_module;

    /* 
       Se copiaza octetul de semn, modulul numarului si modulul puterii 
       negative a lui 10 in sign, module, respectiv in pow_module.
    */
    memcpy(&sign, &(tcp_msg->payload), 1);
    memcpy(&module, &(tcp_msg->payload[1]), sizeof(uint32_t));
    memcpy(&pow_module, &(tcp_msg->payload[5]), sizeof(uint8_t));

    /* 
       Se realizeaza conversia modului de la network byte order la host byte 
       order, se delimiteaza partea intreaga de cea fractionara si se adauga 
       semnul numarului.
    */    
    double value = ntohl(module);
    value /= pow(10, pow_module);
    if (sign) {
      value *= -1;
    }

    printf("%s:%d - %s - FLOAT - %lf\n",
           inet_ntoa(tcp_msg->udp_client.sin_addr),
           ntohs(tcp_msg->udp_client.sin_port), tcp_msg->topic, value);
  } else if (tcp_msg->data_type == STRING_TYPE) {
  	// Date de tip STRING.

    printf(
        "%s:%d - %s - STRING - %s\n", inet_ntoa(tcp_msg->udp_client.sin_addr),
        ntohs(tcp_msg->udp_client.sin_port), tcp_msg->topic, tcp_msg->payload);
  }
}

/* 
   Functie pentru procesarea unei cereri de subscribe/unsubscribe a clientului 
   la un topic.

   Daca mesajul nu respecta standardele:
      - subscribe nume_topic sf
      - unsubscribe nume_topic
      nu se va trimite cererea catre server.
*/
void process_client_request(char *buffer, int sockfd) {
  int ret;

  subscribtion_message subscribtion_msg;
  memset(&subscribtion_msg, 0, sizeof(subscribtion_message));

  // Se extrage comanda de subscribe/unsubscribe.
  char *cmd = strtok(buffer, " ");
  if (!cmd) {
    return;
  }

  // Se extrage numele topicului la care se doreste abonarea/dezabonarea.
  char *name = strtok(NULL, " \n\0");
  if (!name) {
    return;
  }

  // Se verifica daca este o cerere de subscribe la un topic.
  if (strcmp(cmd, "subscribe") == 0) {
  	/* 
  	   Se completeaza cererea catre server cu actiunea de abonare (subscribe) 
  	   si cu numele topicului la care se doreste abonarea.
  	*/
    strcpy(subscribtion_msg.subscribe, "subscribe");
    strcpy(subscribtion_msg.topic, name);

    // Se extrage campul sf.
    char *sf = strtok(NULL, "\0\n ");
    if (!sf) {
      return;
    }

   	/* 
   	   Daca acesta este setat la o valoare diferita de 0 (nu se doreste sf) 
   	   sau 1 (se doreste sf) nu se va trimite cererea catre server.
   	*/
    if (strncmp(sf, "1", 1) == 0 || strncmp(sf, "0", 1) == 0) {
      // Se completeaza cererea cu campul sf.
      subscribtion_msg.sf = atoi(sf);

      // Se trimite cererea de subscribe catre server.
      ret = send(sockfd, &subscribtion_msg, sizeof(subscribtion_message), 0);
      DIE(ret < 0, "send");
      printf("Subscribed to topic.\n");
    }
  } else if (strcmp(cmd, "unsubscribe") == 0) {
  	// Este o cerere de unsubscribe de la un topic.

  	/* 
  	   Se completeaza cererea catre server cu actiunea de dezabonare 
  	   (unsubscribe) si cu numele topicului de la care se doreste dezabonarea.
  	*/  	
    strcpy(subscribtion_msg.subscribe, "unsubscribe");
    strncpy(subscribtion_msg.topic, name, strlen(name) - 1);

    // Se trimite cererea de unsubscribe catre server.
    ret = send(sockfd, &subscribtion_msg, sizeof(subscribtion_message), 0);
    DIE(ret < 0, "send");
    printf("Unsubscribed from topic.\n");
  }
}

int main(int argc, char *argv[]) {
  // Se dezactiveaza buffering-ul la afisare.
  setvbuf(stdout, NULL, _IONBF, BUFSIZ);

  int sockfd, ret;
  struct sockaddr_in serv_addr;
  char buffer[BUFLEN];

  // Tipul de mesaj pe care il poate primi clientul tcp.
  tcp_message tcp_msg;

  fd_set read_fds;	// Multimea de citire folosita in select().
  fd_set tmp_fds;	// Multime folosita temporar.
  int fdmax;		// Valoare maxima fd din multimea read_fds.

  /* 
     Se verifica faptul ca a fost dat un numar corect de argumente la pornirea 
     clientului.
  */
  if (argc < 4) {
    usage(argv[0]);
  }

  /* 
     Se goleste multimea de descriptori de citire (read_fds) si multimea
     temporara (tmp_fds).
  */
  FD_ZERO(&tmp_fds);
  FD_ZERO(&read_fds);

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  DIE(sockfd < 0, "socket");

  // Se dezactiveaza algoritmul lui Nagle.
  int disable = 1;
  ret = setsockopt(sockfd, IPPROTO_TCP, TCP_NODELAY, &disable, sizeof(int));
  DIE(ret < 0, "setsockopt");  

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(atoi(argv[3]));
  ret = inet_aton(argv[2], &serv_addr.sin_addr);
  DIE(ret == 0, "inet_aton");

  /* 
     Se adauga file descriptorii (socketii pe care se asculta conexiuni) in 
     multimea read_fds.
  */
  FD_SET(sockfd, &read_fds);
  FD_SET(STDIN_FILENO, &read_fds);
  fdmax = sockfd;

  // Se conecteaza clientul la server.
  ret = connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
  DIE(ret < 0, "connect");

  // Se trimite id-ul cu care se doreste conectarea.
  ret = send(sockfd, argv[1], strlen(argv[1]) + 1, 0);
  DIE(ret < 0, "send");

  /* 
     Se primeste raspuns cu accept (daca nu exista deja un alt client conectat 
     cu acest id)/reject (in caz contrar).
  */
  ret = recv(sockfd, buffer, BUFLEN, 0);
  DIE(ret < 0, "recv");
  if (strncmp(buffer, "reject", strlen("reject")) == 0) {
    close(sockfd);
    return 0;
  }

  while (1) {
    tmp_fds = read_fds;

    ret = select(fdmax + 1, &tmp_fds, NULL, NULL, NULL);
    DIE(ret < 0, "select");

    // S-au primit date de la STDIN.
    if (FD_ISSET(STDIN_FILENO, &tmp_fds)) {
      memset(buffer, 0, sizeof(buffer));
      fgets(buffer, sizeof(buffer), stdin);

      
      // Daca se primeste comanda exit se inchide clientul.
      if (strncmp(buffer, "exit", 4) == 0) {
        break;
      } else {
      	/* 
      	   S-a primit o cerere de abonare/dezabonare la un topic si trebuie 
      	   procesata.
      	*/
        process_client_request(buffer, sockfd);
      }
    } else if (FD_ISSET(sockfd, &tmp_fds)) {
      // S-au primit date de la server.

      memset(&tcp_msg, 0, sizeof(tcp_msg));

      ret = recv(sockfd, &tcp_msg, sizeof(tcp_message), 0);
      DIE(ret < 0, "recv");
      if (ret == 0) {
        break;
      }			

      /* 
         Se proceseaza datele de la server si se afiseaza informatiile 
      	 prelucrate.
      */
      process_tcp_message(&tcp_msg);
    }
  }

  close(sockfd);
  return 0;
}
