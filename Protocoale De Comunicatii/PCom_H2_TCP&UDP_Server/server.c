#include "helpers.h"

// Functie preluata din laborator.
void usage(char *file) {
  fprintf(stderr, "Usage: %s server_port\n", file);
  exit(0);
}

/*
   Functie pentru cautarea unui client dupa id.

   Returneaza adresa clientului cu id-ul respectiv sau NULL daca nu exista.
*/
tcp_client *get_client_by_id(char *id, tcp_client *clients, int no_clients) {
  int i;

  for (i = 0; i < no_clients; i++) {
    if (strcmp(id, clients[i].id) == 0) {
      return &clients[i];
    }
  }

  return NULL;
}

/*
   Functie pentru cautarea unui client dupa socket.

   Returneaza adresa clientului cu socket-ul respectiv sau NULL daca nu exista.
*/
tcp_client *get_client_by_sockfd(int sockfd, tcp_client *clients,
                                 int no_clients) {
  int i;

  for (i = 0; i < no_clients; i++) {
    if (sockfd == clients[i].sockfd) {
      return &clients[i];
    }
  }

  return NULL;
}

/*
   Functie pentru crearea unui nou topic.

   Se aloca memorie, se completeaza campurile acestuia cu datele dorite si se 
   returneaza noul topic creat.
*/
topic *create_new_topic(char *name) {
  topic *cur_topic = (topic *)calloc(1, sizeof(topic));
  DIE(cur_topic == NULL, "calloc topic");

  strcpy(cur_topic->name, name);
  cur_topic->subscribers = NULL;

  return cur_topic;
}

/*
   Functie pentru cautarea unui topic dupa nume.

   Returneaza adresa topicului cu numele respectiv sau NULL daca nu exista.
*/
topic *get_topic_by_name(list topics, char *name) {
  list aux_topic = NULL;
  topic *cur_topic = NULL;

  for (aux_topic = topics; aux_topic != NULL; aux_topic = aux_topic->next) {
    cur_topic = (topic *)aux_topic->element;

    if (strcmp(cur_topic->name, name) == 0) {
      return cur_topic;
    }
  }

  return NULL;
}

/*
   Functie pentru procesarea unui mesaj de subscribe/unsubscribe la un topic 
   primit de la un client.
*/
void process_subscribtion_message(subscribtion_message *sub_msg,
                                  tcp_client *client, list *topics) {
  // Se cauta topicul in lista de topics dupa nume.
  topic *cur_topic = get_topic_by_name(*topics, sub_msg->topic);

  // Se verifica daca este o cerere de subscribe.
  if (strncmp(sub_msg->subscribe, "subscribe", strlen("subscribe")) == 0) {
  	/* 
  	   Se creeaza un nou subscriber pentru acest topic si se completeaza cu 
  	   datele clientului.
  	*/
    subscriber *sub = (subscriber *)calloc(1, sizeof(subscriber));
    DIE(sub == NULL, "calloc subscriber");

    sub->client = client;
    sub->sf = sub_msg->sf;

    /*
       Daca nu exista un topic cu numele respectiv se creeaza unul si se adauga la 
       lista de subscribers a acestuia noul subscriber pentru acest topic al 
       clientului.
    */
    if (cur_topic == NULL) {
      cur_topic = (topic *)calloc(1, sizeof(topic));
      DIE(cur_topic == NULL, "calloc topic");

      strcpy(cur_topic->name, sub_msg->topic);
      cur_topic->subscribers = cons(sub, NULL);

      *topics = cons(cur_topic, *topics);
    } else {
      /* 
         Altfel doar se adauga la lista de subscribers a topicului noul 
         subscriber pentru acest topic al clientului.
      */
      cur_topic->subscribers = cons(sub, cur_topic->subscribers);
    }

  } else if (strncmp(sub_msg->subscribe, "unsubscribe", strlen("unsubscribe")) == 0) {
  	// Este o cerere de unsubscribe.
    
    /* 
       Se verifica daca exista topicul respectiv (nu se poate dezabona de la un 
       topic inexistent).
    */
    if (cur_topic != NULL) {
      list subscribers = NULL, prev_sub = NULL;

      /* 
         Se cauta subscriberul corespunzator clientului care doreste sa se 
         dezaboneze si se elimina acesta din lista de subscribers a topicului 
         respectiv.
      */
      for (subscribers = cur_topic->subscribers; subscribers != NULL;
           subscribers = subscribers->next) {
        subscriber *sub = (subscriber *)(subscribers->element);

        if (strcmp(sub->client->id, client->id) == 0) {
          if (prev_sub == NULL) {
            cur_topic->subscribers = cur_topic->subscribers->next;
          } else {
            prev_sub->next = subscribers->next;
          }

          free(sub);
          break;
        }

        prev_sub = subscribers;
      }
    }
  }
}

/*
   Functie pentru procesarea unui mesaj tcp si trimiterea acestuia la clientii 
   tcp abonati la topicul respectiv.
*/
void send_tcp_message(tcp_message *tcp_msg, topic *cur_topic) {
  int ret;
  subscriber *sub = NULL;
  list aux_topic = NULL;
  
  // Pentru fiecare subscriber al topicului respectiv.
  for (aux_topic = cur_topic->subscribers; aux_topic != NULL;
       aux_topic = aux_topic->next) {
    sub = (subscriber *)aux_topic->element;

	/* 
	   Se verifica daca este online clientul corespunzator subscriber-ului si 
	   se trimite mesajul catre acesta in caz afirmativ.
	*/
    if (sub->client->status == CLIENT_ON) {
      ret = send(sub->client->sockfd, tcp_msg, sizeof(tcp_message), 0);
      DIE(ret < 0, "send");
    } else if (sub->sf == 1) {
      /* 
         Daca clientul corespunzator subscriber-ului respectiv nu este conectat 
         in momentul respectiv la server, dar este abonat la acest topic cu 
         sf = 1 se salveaza mesajul in coada de mesaje in asteptare a acestuia 
         si i se vor trimite atunci cand se va reconecta.
      */
      tcp_message *msg_copy = (tcp_message *)calloc(1, sizeof(tcp_message));
      DIE(msg_copy == NULL, "calloc tcp message");

      memcpy(msg_copy, tcp_msg, sizeof(tcp_message));
      queue_enq(sub->client->queued_messages, msg_copy);
    }
  }
}

// Functie pentru crearea unui nou client tcp.
tcp_client *create_new_client(char *id, int sockfd) {
  // Se aloca memorie pentru noul client.
  tcp_client *new_client = (tcp_client *)calloc(1, sizeof(tcp_client));
  DIE(new_client == NULL, "calloc client");

  // Se completeaza campurile acestuia cu datele corespunzatoare.
  memset(new_client->id, 0, IDLEN);
  memcpy(new_client->id, id, strlen(id));

  new_client->sockfd = sockfd;
  new_client->status = CLIENT_ON;
  new_client->queued_messages = queue_create();

  // Se returneaza noul client.
  return new_client;
}

// Functie pentru reconectarea unui client la server.
void reconnect_client(tcp_client *client, int sockfd) {
  int ret;

  // Se updateaza datele acestuia.
  client->status = CLIENT_ON;
  client->sockfd = sockfd;

  /* 
     Daca au fost trimise mesaje pe un topic la care clientul este abonat cu 
     sf = 1 cat timp acesta era offline i se vor trimite acum toate mesajele 
     pentru topicul respectiv.
  */
  while (!queue_empty(client->queued_messages)) {
    tcp_message *aux_msg = (tcp_message *)queue_deq(client->queued_messages);
    ret = send(client->sockfd, aux_msg, sizeof(tcp_message), 0);
    DIE(ret < 0, "send");
    free(aux_msg);
  }
}

int main(int argc, char **argv) {
  // Se dezactiveaza buffering-ul la afisare.
  setvbuf(stdout, NULL, _IONBF, BUFSIZ);

  int tcp_sockfd, udp_sockfd, newsockfd, portno, i, ret;
  struct sockaddr_in serv_addr, cli_addr;
  socklen_t clilen;
  char buffer[BUFLEN], id[IDLEN];

  // Tipurile de mesaje pe care le poate primi/trimite server-ul.
  udp_message udp_msg;
  subscribtion_message sub_msg;
  tcp_message tcp_msg;

  /* 
     Lista de clienti TCP a server-ului.
	 
	 Am incercat sa o implementez ca o lista precum cea de topics, dar luam 
	 uneori timeout si am preferat sa o las de tip array. Nu stiu daca e 
	 neaparat aceasta sursa problemei sau faceam eu altceva gresit.
  */
  int clients_capacity = 1, no_clients = 0;
  tcp_client *clients =
      (tcp_client *)calloc(clients_capacity, sizeof(tcp_client));
  DIE(clients == NULL, "calloc clients");

  // Lista de topics existente.
  list topics = NULL;

  fd_set read_fds; // Multimea de citire folosita in select().
  fd_set tmp_fds;  // Multime folosita temporar.
  int fdmax;       // Valoare maxima fd din multimea read_fds.

  /* 
     Se verifica faptul ca a fost dat un numar corect de argumente la pornirea 
     serverului.
  */
  if (argc < 2) {
    usage(argv[0]);
  }

  /* 
     Se goleste multimea de descriptori de citire (read_fds) si multimea
     temporara (tmp_fds).
  */
  FD_ZERO(&read_fds);
  FD_ZERO(&tmp_fds);

  udp_sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  DIE(udp_sockfd < 0, "socket udp_sockfd");

  tcp_sockfd = socket(AF_INET, SOCK_STREAM, 0);
  DIE(tcp_sockfd < 0, "socket tcp_sockfd");

  // Se dezactiveaza algoritmul lui Nagle.
  int disable = 1;
  ret = setsockopt(tcp_sockfd, IPPROTO_TCP, TCP_NODELAY, &disable, sizeof(int));
  DIE(ret < 0, "setsockopt");

  portno = atoi(argv[1]);
  DIE(portno == 0, "atoi");

  memset((char *)&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(portno);
  serv_addr.sin_addr.s_addr = INADDR_ANY;

  ret =
      bind(udp_sockfd, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr));
  DIE(ret < 0, "bind udp_sockfd");

  ret =
      bind(tcp_sockfd, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr));
  DIE(ret < 0, "bind tcp_sockfd");

  ret = listen(tcp_sockfd, MAX_CLIENTS);
  DIE(ret < 0, "listen");

  /* 
     Se adauga file descriptorii (socketii pe care se asculta conexiuni) in 
     multimea read_fds.
  */
  FD_SET(udp_sockfd, &read_fds);
  FD_SET(tcp_sockfd, &read_fds);
  FD_SET(STDIN_FILENO, &read_fds);

  fdmax = udp_sockfd < tcp_sockfd ? tcp_sockfd : udp_sockfd;

  while (1) {
    tmp_fds = read_fds;

    ret = select(fdmax + 1, &tmp_fds, NULL, NULL, NULL);
    DIE(ret < 0, "select");

    // S-au primit date de la STDIN.
    if (FD_ISSET(STDIN_FILENO, &tmp_fds)) {
      memset(buffer, 0, sizeof(buffer));
      fgets(buffer, sizeof(buffer), stdin);

      /* 
         Daca se primeste comanda exit se inchid serverul si toti clientii TCP 
         conectati la momentul respectiv.
      */
      if (strncmp(buffer, "exit", 4) == 0) {
        for (i = 0; i <= fdmax; ++i) {
          if (FD_ISSET(i, &read_fds)) {
            close(i);
          }
        }
        break;
      }
    }

    // S-au primit date pe socketul UDP.
    if (FD_ISSET(udp_sockfd, &tmp_fds)) {
      clilen = sizeof(cli_addr);

      memset(&udp_msg, 0, sizeof(udp_msg));
      ret = recvfrom(udp_sockfd, &udp_msg, sizeof(udp_msg), 0,
                     (struct sockaddr *)&cli_addr, &clilen);
      DIE(ret < 0, "recvfrom");

      /* 
         Se copiaza datele din mesajul UDP si adresa clientului UDP care a 
         trimis mesajul in mesajul TCP.
      */
      memset(&tcp_msg, 0, sizeof(tcp_message));
      memcpy(&(tcp_msg.udp_client), &cli_addr, clilen);
      memcpy(&(tcp_msg.topic), &(udp_msg.topic), sizeof(udp_message));

      // Se cauta in lista de topics topicul pe care s-a trimis mesajul.
      topic *cur_topic = get_topic_by_name(topics, tcp_msg.topic);

      /* 
         Daca nu exista in lista de topics se creeaza un nou topic si se adauga 
         in lista.
      */ 
      if (cur_topic == NULL) {
        cur_topic = create_new_topic(udp_msg.topic);

        topics = cons(cur_topic, topics);
      } else {
      	/* 
      	   Altfel se trimite mesajul catre clientii care sunt abonati la 
      	   mesajele de pe acest topic.
      	*/
        send_tcp_message(&tcp_msg, cur_topic);
      }
    } else {
      for (i = 0; i <= fdmax; ++i) {
        if (FD_ISSET(i, &tmp_fds)) {
          if (i == tcp_sockfd) {
            /* 
               A venit o cerere de conexiune pe socketul TCP (cel cu listen), 
		       pe care serverul o accepta.
		    */
            clilen = sizeof(cli_addr);

            newsockfd =
                accept(tcp_sockfd, (struct sockaddr *)&cli_addr, &clilen);
            DIE(newsockfd < 0, "accept");

            /* 
               Se primeste id-ul cu care clientul incearca sa se conecteze la 
               server.
            */
            memset(id, 0, sizeof(id));
            ret = recv(newsockfd, id, sizeof(id), 0);
            DIE(ret < 0, "recv");

            // Se cauta clientul in lista de clienti in functie de id. 
            tcp_client *client = get_client_by_id(id, clients, no_clients);

            // Este un client nou sau un client care incearca sa se reconecteze.
            if (client == NULL || client->status == CLIENT_OFF) {
              /* 
                 Se adauga noul socket intors de accept() la multimea 
                 descriptorilor de citire.
              */
              FD_SET(newsockfd, &read_fds);
              fdmax = fdmax > newsockfd ? fdmax : newsockfd;

              // Se trimite mesaj cu acceptarea conexiunii.
              strcpy(buffer, "accept");
              ret = send(newsockfd, buffer, sizeof(buffer), 0);
              DIE(ret < 0, "send");

              // Se afiseaza in server informatii despre noul client.
              printf("New client %s connected from %s:%i.\n", id,
                     inet_ntoa(cli_addr.sin_addr), cli_addr.sin_port);

              // Daca este un nou client acesta trebuie adaugat la lista de clienti.
              if (client == NULL) {
              	// Se creeaza un nou client cu datele potrivite.
              	tcp_client *new_client = create_new_client(id, newsockfd);
                memcpy(&(clients[no_clients]), new_client, sizeof(tcp_client));
                free(new_client);

                // Daca este necesar se mareste capacitatea array-ului de clienti.
                no_clients++;
                if (no_clients == clients_capacity) {
                  clients_capacity *= 2;
                  clients =
                      realloc(clients, clients_capacity * sizeof(tcp_client));
                }
              } else {
              	/* 
              	   Altfel este un client vechi care s-a reconectat si trebuie 
              	   updatate informatiile acestuia.
              	*/
                reconnect_client(client, newsockfd);
              }

            } else if (client->status == CLIENT_ON) {
              /* 
                 Se incearca conectarea cu un id care este deja utilizat si se 
            	 va refuza conexiunea.
              */
              printf("Client %s already connected.\n", id);
              
              strcpy(buffer, "reject");
              ret = send(newsockfd, buffer, sizeof(buffer), 0);
              DIE(ret < 0, "send");
            }
          } else if (i != STDIN_FILENO) {
          	// Se primeste un mesaj de abonare/dezabonare de la un client.
            memset(&sub_msg, 0, sizeof(subscribtion_message));
            ret = recv(i, &sub_msg, sizeof(subscribtion_message), 0);
            DIE(ret < 0, "recv");

            // Se cauta clientul in lista de clienti dupa socket.
            tcp_client *client = get_client_by_sockfd(i, clients, no_clients);

            // Clientul s-a deconectat si se updateaza status-ul acestuia.
            if (ret == 0) {
              printf("Client %s disconnected.\n", client->id);
              client->status = CLIENT_OFF;

              // Se scoate din multimea de citire socketul inchis.
              close(i);
              FD_CLR(i, &read_fds);
            } else {
              // Se proceseaza cererea de subscribe/unsubscribe a clientului.
              process_subscribtion_message(&sub_msg, client, &topics);
            }
          }
        }
      }
    }
  }

  /* 
     In final se elibereaza toata memoria alocata dinamic si se inchid 
     socket-urile tcp si udp.
  */

  list aux_topic = NULL, del_topic = NULL, aux_sub = NULL, sub = NULL;
  topic *cur_topic = NULL;
  tcp_client *client = NULL;

  for(aux_topic = topics; aux_topic != NULL;) {
  	cur_topic = (topic *)aux_topic->element;

  	for(sub = cur_topic->subscribers; sub != NULL;) {
  		client = ((subscriber *)sub)->client;
  		
  		while (!queue_empty(client->queued_messages)) {
    		tcp_message *aux_msg = (tcp_message *)queue_deq(client->queued_messages);
    		free(aux_msg);
  		}

  		aux_sub = sub;
  		sub = sub->next;
  		free(aux_sub);
  	}

  	del_topic = aux_topic;
  	aux_topic = aux_topic->next;
  	free(del_topic);
  }

  free(clients);

  close(tcp_sockfd);
  close(udp_sockfd);
}
