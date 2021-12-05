## PC - Tema 1 - Router
# Mateescu Viorel-Cristian

1. Parsarea tabelei de rutare
	Pentru intrarile din tabela de rutare am folosit structura suport din 
laboratorul 4: struct route_table_entry. Astfel, fiecare intrare din tabela
de rutare are caracteristice un prefix, un next hop, o masca si o interfata.

	Parsarea tabelei de rutare este realizata cu ajutorul functiei read_rtable.
Astfel, fisierul care contine informatiile tabelei de rutare a unui router este
citit linie cu linie, iar acestea sunt separate in cele 4 componente care 
alcatuiesc intrarea din tabela de rutare.

2. Cautarea in tabela de rutare
	Pentru a imbunatati algoritmul de cautare in tabela de rutare am ales sa
sortez tabela de rutare crescator in functie de prefix si descrescator in 
functie de masca, pentru a-l implementa ulterior ca un Binary Search. Astfel, 
complexitatea preprocesarii (a sortarii) este complexitatea algoritmului qsort: 
O(n*log(n)), iar complexitatea cautarii in tabela de rutare este Theta(log(n)) 
(complexitatea algoritmului Binary Search).

	Se cauta intrarea cea mai specifica din tabela de rutare astfel incat:
(iph−>daddr & entry.mask) == entry.prefix, iar masca este cat se poate de mare. 
Odata identificata, aceasta specifica next hop pentru pachet. In cazul in care 
nu se gaseste o ruta, se trimite un pachet de tip ICMP_DEST_UNREACH sursei. 
	
	Daca adresa MAC destinatie nu este cunoscuta local se genereaza un ARP request 
si se transmite pe interfata destinatie, apoi se salveaza pachetul in coada de 
pachete de dirijat pana cand adresa MAC va deveni cunoscuta routerului (se va primi 
un ARP reply cu aceasta adresa MAC) si va putea fi trimis. 

	Se modifica adresele sursa si destinatie MAC astfel: adresa sursa este cea a 
interfetei routerului pe care se trimite pachetul, iar cea destinatie este 
cea corespunzatoare intrarii din tabela ARP. Pachetul este trimis mai departe 
folosind functia send_packet.
	
3. Tabela ARP
	Pentru intrarile din tabela ARP am folosit structura suport din laboratorul 4:
struct arp_entry. Astfel, fiecare intrare din tabela ARP are caracteristice o 
adresa IP si una MAC.

4. Implementarea protocolului ARP
	Se verifica daca pachetul primit este de tip ARP (se verifica acest lucru 
aplicand functia parse_arp asupra m.payload. In cazul in care pachetul nu este de 
tip ARP va returna NULL, altfel va returna un pointer catre structura arp_header 
continuta in pachet.) 

	Se verifica daca este un pachet ARP Request catre un IP al routerului (adresa 
IP destinatie extrasa din headerul ARP coincide cu adresa IP a interfetei 
routerului pe care a fost primit pachetul), iar in caz afirmativ raspunde cu ARP 
Reply cu adresa MAC potrivita (adresa MAC a interfetei routerului pe care s-a 
primit pachetul). Daca pachetul nu este destinat unui IP al routerului atunci se 
arunca pachetul.

	In cazul in care pachetul este de tip ARP Reply este updatata tabela ARP prin 
adaugarea unei noi intrari in aceasta, iar daca exista pachete ce trebuie dirijate 
catre acel router se realizeaza transmiterea lor. Adresa IP a noii intrari in 
tabela ARP se gaseste in campul spa din ARP header, iar cea MAC in campul sha.

	Cererile ARP vor fi trimise ca broadcast (ff:ff:ff:ff:ff:ff). 
	Raspunsurile ARP sunt trimise catre adresa MAC a host-ului ce a facut cererea.

5. Suport pentru protocolul ICMP
	Se verifica daca pachetul primit este un pachet IP destinat routerului si 
daca acesta este de tip ICMP ECHO request. In caz afirmativ se va raspunde cu 
un pachet de tip ICMP ECHO reply.
	
	Daca se primeste un pachet cu TTL <= 1 se trimite un pachet de tip 
ICMP_TIME_EXCEEDED sursei.

	In cazul in care nu se gaseste o ruta in tabela de rutare pentru un pachet, 
se trimite un pachet de tip ICMP_DEST_UNREACH sursei.

6. Procesul de dirijare
	Procesul de dirijare a fost realizat pas cu pas conform celui specificat in 
enuntul temei.

7. Probleme intampinate
	Am observat ca daca rulez checkerul local si simultan mai am si alte aplicatii 
pornite care consuma ceva resurse (de exemplu Teams) imi pica teste random. Insa
daca rulez doar checkerul si astept fara sa fac altceva trec toate testele.
