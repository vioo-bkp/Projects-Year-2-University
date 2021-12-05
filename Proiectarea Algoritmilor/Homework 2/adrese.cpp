#include <bits/stdc++.h>
using namespace std;

/*
   Structura pentru a retine numele, adresele de mail si numarul de adrese al
   fiecarui client.
*/
struct client {
  char name[50];
  int n;
  set<string> addresses;
};

/*
   Functie pentru a compara doi clienti, mai intai dupa numarul de adrese de
   mail, apoi dupa nume.
*/
bool compareClients(client *c1, client *c2) {
  // Daca au acelasi numar de adrese de mail.
  if (c1->n == c2->n) {
    // Compara dupa nume.
    if (strcmp(c1->name, c2->name) < 0) {
      return true;
    } else {
      return false;
    }
  }
  // Compara dupa numarul de adrese de mail.
  if (c1->n - c2->n < 0) {
    return true;
  }

  return false;
}

int main() {
  int N, no_addr, no_clients, not_added;
  FILE *in = fopen("adrese.in", "rt");

  // Se citeste numarul de clienti.
  fscanf(in, "%d\n", &N);

  struct client *clients[N];

  char *address;

  no_clients = 0;
  struct client *new_client;

  // Se citeste fiecare client.
  for (int i = 0; i < N; ++i) {
    new_client = new client;

    // Se citeste numele clientului si numarul sau de adrese.
    fscanf(in, "%s %d ", new_client->name, &no_addr);

    // Se citeste fiecare adresa si se adauga in lista de adrese a clientului.
    for (int j = 0; j < no_addr; ++j) {
      address = (char *)calloc(50, sizeof(char));
      fscanf(in, "%s\n", address);

      std::string str(address);

      new_client->addresses.insert(str);
    }

    // Se seteaza numarul de adrese al clientului.
    new_client->n = no_addr;

    // Retine daca clientul curent a fost adaugat in lista de clienti.
    not_added = 1;

    /*
       Se vor compara adresele de mail ale tuturor clientilor deja existenti
       cu cele ale noului client.
    */
    for (int j = 0; j < no_clients && not_added; ++j) {
      for (auto addr1 : clients[j]->addresses) {
        for (auto addr2 : new_client->addresses) {
          /*
             Daca 2 clienti au o adresa identica atunci este vorba de acelasi
             client.
          */
          if (addr1.compare(addr2) == 0) {
            /*
               Adaugam adresele noului client clientului vechi si updatam
               numarul de adrese ale clientului respectiv. (in cazul in
               care noul client are un nume mai scurt dpvd lexicografic
               decat cel vechi updatam si numele clientului vechi cu cel
               al noului client).

               Se updateaza not_added pentru a nu adauga noul client in
               lista.
            */
            if (strcmp(clients[j]->name, new_client->name) <= 0) {
              clients[j]->addresses.insert(new_client->addresses.begin(),
                                           new_client->addresses.end());
              clients[j]->n = clients[j]->addresses.size();
              not_added = 0;
            } else {
              clients[j]->addresses.insert(new_client->addresses.begin(),
                                           new_client->addresses.end());
              memcpy(clients[j]->name, new_client->name,
                     sizeof(new_client->name));
              clients[j]->n = clients[j]->addresses.size();
              not_added = 0;
            }
          }
          if (!not_added) {
            break;
          }
        }
        if (!not_added) {
          break;
        }
      }
    }

    // Daca noul client nu exista deja in lista se adauga.
    if (not_added == 1) {
      clients[no_clients] = new_client;
      no_clients++;
    }
  }

  fclose(in);

  // Se sorteaza lista de clienti cu ajutorul fuctiei de mai sus.
  sort(clients, clients + no_clients, compareClients);

  FILE *out = fopen("adrese.out", "wt");

  // Se afiseaza numarul real de clienti.
  fprintf(out, "%d\n", no_clients);

  char aux_addr[50];

  /*
     Pentru fiecare client se vor afisa numele, numarul sau de adrese si
     adresele sale.
  */
  for (int i = 0; i < no_clients; ++i) {
    fprintf(out, "%s %d\n", clients[i]->name, clients[i]->n);

    for (auto cur_client : clients[i]->addresses) {
      strcpy(aux_addr, cur_client.c_str());
      fprintf(out, "%s\n", aux_addr);
    }
  }

  fclose(out);

  return 0;
}
