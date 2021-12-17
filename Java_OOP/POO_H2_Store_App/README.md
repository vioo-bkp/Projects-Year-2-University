## Programare Orientata pe Obiecte
### Tema 2 - Store Application

# amazon_co-ecommerce_sample.csv.

- Am folosit Apache Commons CSV pentru a citi/scrie in/din fisierul CSV si Apache Commons Pair pentru a crea o pereche
de tip < Double, Currency> atunci cand citesc pretul unui produs din fisier.

- Am implementat metodele loadStore si saveStore folosindu-ma de serializarea Store-ului si a componentelor acestuia.

- Am ales sa implementez ca extra Design Pattern Facade asupra Store-ului pentru a crea o interfata mai prietenoasa
pentru utilizator (proprietarul magazinului). Astfel, ii ofer acestuia un panou de control de la care el poate efectua
comenzile/operatiile dorite asupra magazinului sau.

# Clase:
- CSVFileHeader -> enum care contine numele coloanelor ce sunt citite/scrise in fisierul CSV.

- Currency -> clasa care implementeaza comportamentul unei monezi. Aceasta este caracterizata prin nume, simbol si
paritate (curs) fata de EURO. Pentru a afisa monezile in ordine alfabetica atunci cand se apeleaza comanda
listcurrencies am implementat Comparator< Currency > CurrencyNameComparator, am suprascris metoda toString a
acesteia pentru a compara doua monezi dupa simbolurile acestora si am declarat ca implementeaza interfata
Serializable pentru a o putea printa/citi intr-un/dintr-un fisier binar.

- CurrencyNotFoundException -> exceptie aruncata atunci cand se incearca utilizarea unei monezi care nu exista in
lista de monezi.

- Discount -> clasa care implementeaza comportamentul unei reduceri. Aceasta este caracterizata printr-un nume,
un tip de discount (fix sau procentual), o valoare si ultima data cand a fost aplicat unui produs.

- DiscountNotFoundException -> exceptie aruncata atunci cand se incearca aplicarea unui discount care nu exista in
lista de discounturi.

- DiscountType -> enum care contine tipurile de discount.

- DuplicateManufacturerException -> exceptie aruncata atunci cand se incearca adaugarea in lista de producatori a
magazinului a unui producator care deja exista in aceasta.

- DuplicateProductException -> exceptie aruncata atunci cand se incearca adaugarea in lista de produse a magazinului
    a unui produs care deja exista in aceasta.

- Manufacturer -> clasa care implementeaza comporatamentul unui producator. Aceasta este caracterizata printr-un
nume si un numar de produse pe care le produce. Pentru a afisa producatorii in ordine alfabetica atunci cand se
apeleaza comanda listmanufacturers am implementat Comparator< Manufacturer > ManufacturerNameComparator, am
suprascris metoda toString a acesteia pentru a compara doi producatori dupa numele acestora si am declarat ca
implementeaza interfata Serializable pentru a o putea printa/citi intr-un/dintr-un fisier binar.

- NegativePriceException -> exceptie aruncata atunci cand un discount produce un pret negativ unui produs.

- PriceHelper -> clasa helper folosita pentru a separa pretul unui produs de moneda in care se afla acesta. De
asemenea este folosita pentru a converti un pret dintr-o moneda in alta.

- Product -> clasa care implementeaza comporatamentul unui produs. Aceasta este caracterizata printr-un ID, nume,
producator, cantitate, discount si pret. Am implementat aici Design Pattern-ul Builder asa cum s-a cerut in cerinta,
folosindu-ma de clasa statica interna Builder. Am suprascris metoda toString a acesteia pentru a compara doua
produse dupa ID-ul acestora si am declarat ca implementeaza interfata Serializable pentru a o putea printa/citi
intr-un/dintr-un fisier binar.

-> StockHelper -> clasa Helper folosita pentru a extrage stocul unui produs din magazin dintr-un string citit din
fisierul CSV.

-> Store -> clasa care implementeaza comportamentul unui magazin. Aceasta este caracterizata printr-un nume, lista
de produse si lista de producatori. Am implementat aici Design Pattern-ul Singleton asa cum s-a cerut in cerinta,
folosindu-ma de campul storeInstance si metoda getInstance. Am declarat ca implementeaza interfata Serializable
pentru a o putea printa/citi intr-un/dintr-un fisier binar (metodele saveStore si loadStore).

-> StoreServiceFacade -> clasa care implementeaza un panou de control pentru proprietarul magazinului. Deoarece
un magazin presupune o serie de comenzi ce pot fi aplicate asupra acestuia, magazinul propriu-zis, lista de
discounturi si cea de monezi am ales sa implementez un fel de panou de control pentru proprietarul store-ului astfel
incat el sa nu fie nevoit sa cunoasca decat operatiile ce pot fi aplicate asupra magazinului, de restul ocupandu-se
programul in spatele "fatadei". De aici se realizeaza toate operatiile asupra magazinului.

-> StoreOwner -> clasa proprietarului magazinului. Datorita implementarii Design Pattern-ului Facade, proprietarul
magazinului trebuie doar sa deschida panoul de control (new StoreServiceFacade()) si sa introduca comenzile dorite.
