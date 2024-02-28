## Tema 3 Client Web. REST API.
# Mateescu Viorel-Cristian

Cateva detalii de la care am pornit implementarea:
	- Pentru a parsa raspunsurile primite de la server am folosit biblioteca parson 
	pentru C (recomandata in enunt). Astfel mi-a fost mult mai usor sa caut informatia 
	care ma intereseaza in raspunsurile primite de la server pentru cererile mele.

	- Am incercat sa fac o afisare cat mai user-friendly: daca cererea a esuat 
	afisez un mesaj de eroare in functie de eroarea intoarsa de catre server, 
	altfel afisez informatia utila pentru user din raspunsul serverului intr-un 
	format inteligibil pentru orice.

	- Pentru a mentine conexiunea dintre client si server am ales sa deschid si 
	sa inchid conexiunea la fiecare iteratie asa cum s-a sugerat pe forum.

Am ales sa pun urmatoarele conditii:
	
	- Un client deja logat nu poate crea un alt cont pana cand nu si da logout 
	de pe contul curent.

	- Daca id-ul unei carti/numarul de pagini al acesteia nu sunt in format numeric 
	se va afisa un mesaj de eroare.

	- Daca user-ul doreste sa efectueze operatii asupra bibliotecii, dar nu are 
	acces la ea se va afisa un mesaj de eroare.

	- Daca user-ul deja are acces la biblioteca si il solicita iarasi se va afisa 
	un mesaj de instiintare a utilizatorului in legatura cu acest lucru.

	- Un client deja logat nu se poate loga de pe un alt cont pana cand nu si 
	da logout de pe cel curent. 

Functia "register_user" este folosita pentru a inregistra un nou user pe server.
Se preiau datele viitorului user de la tastatura folosind un prompt pentru username 
si password. Se creaza obiectul json care contine datele de autentificare si se 
trimite o cerere POST catre server cu acesta. Din raspunsul primit de la server 
se constata daca inregistrarea a esuat (prea multe cereri de inregistare trimise 
de acelasi client recent sau username-ul este deja luat) sau a reusit si se 
afiseaza un mesaj sugestiv.

Functia "login_user" este folosita pentru a loga un user pe server. Se preiau 
datele de autentificare ale user-ului de la tastatura folosind un prompt pentru 
username si password. Se creaza obiectul json care contine datele de autentificare 
si se trimite o cerere POST catre server cu acesta. Din raspunsul primit de la server 
se constata daca logarea a esuat (datele de sutentificare nu sunt corecte) sau a 
reusit si se afiseaza un mesaj sugestiv. In cazul in care logarea a reusit se extrage 
cookie-ul de sesiune al user-ului si se salveaza acesta.

Functia "enter_library" este folosita pentru a solicita acces la biblioteca de catre 
user. Se trimite o cerere GET care contine cookie-ul de sesiune al user-ului. 
Serverul intoarce un token JWT, care demonstreaza accesul la biblioteca pentru 
user-ul respectiv. Acesta se extrage din raspunsul serverului si se salveaza.

Functia "get_books" este folosita pentru a solicita serverului lista de carti 
disponibile in biblioteca. Se trimite o cerere GET care contine cookie-ul de 
sesiune si token-ul JWT al user-ului. Din raspunsul primit de la server se 
constata daca exista sau nu carti in biblioteca. In cazul in care nu exista carti 
in biblioteca se afiseaza un mesaj de informare, altfel se parseaza array-ul de 
carti din raspunsul server-ului si se afiseaza intr-un format inteligibil toate 
cartile disponibile.

Functia "get_books" este folosita pentru a solicita serverului informatii despre 
o anumita carte in functie de ID-ul acesteia. Se preia id-ul cartii de la tastatura 
folosind un prompt pentru acesta. Se verifica daca id-ul este numeric, iar in cazul 
in care nu este se va afisa un mesaj de eroare. Se trimite o cerere GET care contine 
cookie-ul de sesiune si token-ul JWT al user-ului, cat si id-ul cartii dorite 
("/api/v1/tema/library/books/:bookId"). Din raspunsul primit de la server se 
constata daca exista sau nu o carte cu id-ul respectiv in biblioteca. In cazul in 
care nu exista se va afisa un mesaj de eroare, altfel se parseaza informatiile 
despre carte din raspunsul serverului si se afiseaza intr-un format inteligibil 
aceste date.

Functia "add_book" este folosita pentru a adauga o carte in biblioteca. Se preiau 
titlul, autorul, genul, editura si numarul de pagini al cartii de la tastatura 
folosind un prompt pentru acestea. Se verifica daca numarul de pagini este numeric, 
iar in cazul in care nu este se va afisa un mesaj de eroare. Se creaza obiectul json 
care contine datele cartii si se trimite o cerere POST catre server cu acesta. 
Din raspunsul primit de la server se constata daca adaugarea cartii a esuat (prea 
multe cereri de adaugare de carti trimise de acelasi client recent) sau a reusit 
si se afiseaza un mesaj sugestiv.

Functia "delete_book" este folosita pentru a sterge o carte din biblioteca. Se 
preia id-ul cartii de la tastatura folosind un prompt pentru acesta. Se verifica 
daca id-ul este numeric, iar in cazul in care nu este se va afisa un mesaj de 
eroare. Se trimite o cerere DELETE care contine cookie-ul de sesiune si token-ul 
JWT al user-ului, cat si id-ul cartii dorite ("/api/v1/tema/library/books/:bookId").
Din raspunsul primit de la server se constata daca a existat sau nu o carte cu id-ul 
respectiv in biblioteca. In cazul in care nu a existat se va afisa un mesaj de 
eroare, altfel cartea a existat si a fost stearsa din biblioteca in urma cererii de
DELETE.

In "main" se porneste si opreste conexiunea cu server-ul, se citeste cate o comanda, 
se interpreteaza acesta si se executa in cazul in care este o comanda valida. In 
cazul unei comenzi invalide se va afisa un mesaj de eroare.
