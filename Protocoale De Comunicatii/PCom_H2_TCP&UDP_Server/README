## Protocoale de Comunicatii
# Tema 2 - Aplicatie client-server TCP & UDP
## Mateescu Viorel-Cristian

## Implementarea protocolului TCP:

Mesajele emise de clientii UDP catre server sunt compuse din:
	- topic -> numele topicului
	- data_type -> tipul de date continut in payload
	- payload -> informatia propriu-zisa continuta in mesaj

Mesajele TCP pot fi de mai multe tipuri:
- string -> trimis de client catre server atunci cand doreste sa se conecteze 
			la acesta (cand trimite id-ul de conectare).
	
- subscribtion_message -> trimis de client catre server atunci cand doreste 
			sa se aboneze/dezaboneze la un topic.
			-> este compus din: 
			- comanda -> subscribe/unsubscribe
			- topic -> numele topicului
			- sf -> daca clientul doreste sa se aboneze la topic cu optiunea store&forward sau nu
	
- tcp_message -> trimis de catre server catre clienti atunci cand primeste 
			un mesaj de la un client UDP. Este compus din aceleasi 
			informatii precum mesajul UDP, dar la acesta se adauga si 
			datele clientului UDP care a emis mesajul.
			-> este compus din:
			- udp_client -> contine datele clientului UDP care a emis mesajul initial
			- topic -> numele topicului
			- data_type -> tipul de date continut in payload
			- payload -> informatia propriu-zisa continuta de mesaj

Implementarea aplicatiei client-server TCP si UDP pentru gestionarea mesajelor:

## "helpers.h":
	- Macro-ul de verificare a erorilor DIE preluat din laborator.
	
	- Cateva valori pentru lungimile unor array-uri, numarul maxim de clienti 
	ce pot fi conectati simultan la server si valori pentru tipurile de date ce
	pot fi transmise intre clientii UDP -> server -> clientii TCP.

	- Structurile de date pentru clienti, abonatii la diverse topice, topice, 
	mesajele de la clientii UDP, mesajele pentru clientii TCP, mesaje de abonare 
	ale clientilor TCP la anumite topice.

## "server.c":
	- Functiile pentru cautarea unui client in lista de clienti TCP a serverului 
	in functie de id ("get_client_by_id"), respectiv dupa socket 
	("get_client_by_sockfd").

	- Functia "create_new_topic" pentru crearea unui nou topic. Se aloca memorie 
	pentru acesta, se completeaza numele acestuia si se initializeaza lista de 
	subscriberi la acest topic cu NULL.

	- Functia "get_topic_by_name" pentru cautarea unui topic dupa nume in lista 
	de topice existente.

	- Functia "process_subscribtion_message" pentru procesarea unui mesaj de 
	subscribe/unsubscribe la un topic primit de la un client TCP. 
		-> Subscribe: Se aloca memorie pentru o structura de tip subscriber la 
		topicul respectiv pentru client si se completeaza campurile acesteia. 
		Daca nu exista deja un topic cu numele celui la care doreste sa se 
		aboneze clientul se creeaza unul nou si se adauga in liste de topice. 
		Se adauga subscriberul creat la lista de subscriberi a topicului 
		respectiv.

		-> Unsubscribe: Se verifica daca exista un topic cu numele celui de la 
		care se doreste dezabonarea clientului. In caz afirmativ se cauta 
		subscriberul corespunzator clientului respectiv in lista de subscriberi 
		a topicului si se elimina din aceasta.

	- Functia "send_tcp_message" pentru procesarea unui mesaj tcp si trimiterea 
	acestuia la clientii TCP abonati la topicul respectiv. Acesta functie se 
	apeleaza dupa ce datele mesajului primit de la clientul UDP au fost mutate 
	intr-un mesaj pentru clientii de tip TCP. Pentru fiecare subscriber al 
	topicului pe care s-a creat mesajul se verifica daca clientul este online si
	i se trimitea mesajul in caz afirmativ, altfel se verifica daca s-a abonat cu 
	optiunea store and forward la topicul respectiv si in caz afirmativ i se adauga
	mesajul in coada de mesaje in asteptare (pe care le va primi atunci cand se va 
	fi iarasi online).

	- Functia "create_new_client" pentru crearea unui nou client TCP. Se aloca 
	memorie pentru noul client si se completeaza campurile structurii cu datele 
	clientului.

	- Functia "reconnect_client" pentru reconectarea unui client la server. Se 
	updateaza starea acestuia si daca are mesaje in asteptare i se trimit in acest 
	moment.

	Server-ul functioneaza in urmatorul mod:
		- Sunt porniti socketii UDP si TCP, prin care se realizeaza comunicarea 
		cu clientii UDP, respectiv prin care se conecteaza clientii TCP la 
		server (socket pasiv, cel cu listen).
		
		- Se adauga file descriptorii (socketii pe care se asculta conexiuni) in 
		multimea read_fds (udp_sockfd, tcp_sockfd si STDIN_FILENO).

		- Se verifica de unde au fost primite date folosind multiplexare I/O:
			-> De la STDIN se poate primi doar comanda "exit" care determina 
			inchiderea server-ului si a tuturor clientilor conectati la acesta.

			-> Pe socket-ul UDP se primesc date de la clientii UDP folosind
			structura udp_message. Datele din mesajul UDP sunt copiate intr-un 
			mesaj TCP la care se adauga datele despre clientul UDP care a trimis
			mesajul UDP. Daca nu exista un topic in lista de topice a server-ului
			pentru cel pentru care a fostb emis mesajul se creeaza unul si se 
			adauga in aceasta, altfel se trimis subscriber-ilor acestui topic 
			mesajul TCP.

			-> Pe socket-ul pasiv TCP (cel cu listen) se primesc cereri de 
			conexiune de la clientii TCP. Server-ul accepta in prima faza 
			conexiunea si primeste id-ul cu care clientul TCP doreste sa se 
			conecteze pe server. Daca exista deja un client TCP conectat cu 
			id-ul respectiv server-ul va refuza conexiunea. Altfel se creeaza 
			un client nou si se adauga in lista de clienti (daca nu exista 
			niciun client cu id-ul respectiv online/offline) sau se updateaza 
			socket-ul si id-ul in cazul in care client-ul respectiv este unul
			mai vechi, dar care s-a reconectat la server.

			-> Prin socketii clientilor TCP server-ul este informat daca clientii
			s-au deconectat de la server sau daca acestia au emis cereri de 
			abonare/dezabonare la vreun topic.

## "subscriber.c":
	- Functia "process_tcp_message" pentru procesarea datelor primite intr-un 
	mesaj de tip TCP de la server si afisarea acestora. In functie de tipul
	datelor trimise se construieste si se afiseaza informatia utila a mesajului.

	- Functia "process_client_request" pentru procesarea unei cereri de 
	subscribe/unsubscribe a clientului la un topic. 
	  Daca mesajul nu respecta standardele:
    	-> subscribe nume_topic sf
     	-> unsubscribe nume_topic
    nu se va trimite cerearea catre server.
      Se extrag mai intai comanda (subscribe/unsubscribe) si numele topicului 
      la care se doreste abonarea/dezabonarea:
      	-> Daca este un mesaj de subscribe se extrage campul store and forward 
      	si se verifica daca acesta este '1' sau '0'. Daca are o valoare corecta 
      	(una dintre cele doua) se trimite mesajul se subscribe la server, altfel 
      	se ignora.

      	-> Daca este un mesaj de unsubscribe se trimite cererea de dezabonare la 
      	server.

    - Client-ul functioneaza in urmatorul mod:
    	- Se porneste un socket TCP prin care clientul va comunica cu server-ul.

    	- Se adauga file descriptorii (socketii pe care se asculta conexiuni) in 
		multimea read_fds (sockfd si STDIN_FILENO).

		- Clientul va trimite server-ului id-ul cu care doreste conectarea, iar 
    	acesta ii poate refuza conectarea in functie de disponibilitatea id-ului 
    	respectiv.

    	- Se verifica de unde au fost primite date folosind multiplexare I/O: 
    		-> De la STDIN se poate primi comanda "exit" care determina inchiderea 
    		clientului sau o cerere de subscribe/unsubscribe la un anumit topic.

    		-> Pe socket-ul TCP se primesc mesajele emise de clientii UDP catre 
    		server si convertite de acesta la mesaje TCP care sunt apoi prelucrate 
    		si afisate de client.

Precizari:
	Am folosit structurile de lista si coada oferite la Tema 1.

Cazuri speciale:
	- Daca se primeste o cerere de dezabonare la un topic la care clientul 
	respectiv nu este abonat se verifica mai intai acest lucru si daca se 
	confirma se ignora cererea.

	- Daca un client emite o cerere de abonare la un topic la care este deja
	abonat se updateaza datele de abonare ale acestuia pentru topicul respectiv
	(poate doreste sa-si schimbe optiunea pentro store and forward).

	- Daca un mesaj de abonare/dezabonare la un topic nu respecta structura 
	precizata in enunt se va ignora cererea respectiva.

	- Daca in server se primeste de la tastatura o alta comanda decat "exit" 
	aceasta va fi ignoarata.

	- Daca in client se primeste de la tastatura o alta comanda decat "exit" 
	sau "subscribe"/"unsubscribe" acesta va fi ignorata.

Probleme intampinate:
	- Cand am modificat array-ul de clienti cu o lista de clienti am luat timeout 
	si nu am inteles de ce asa ca m-am intors la array.

	- Cand testam si nu mi se inchideau corect clientii/server-ul procesele continuau 
	sa ruleze in fundal si nu stiam de ce nu mai pot rula sau de ce primesc output 
	ciudat.

	- La un moment dat cred ca a dat crash server-ul si a ramas un client UDP pornit 
	si desi am rezolvat problema server-ului primeam output ciudat in clienti si nu 
	intelegeam de ce. S-a rezolvat doar prin restart la masina virtuala.
