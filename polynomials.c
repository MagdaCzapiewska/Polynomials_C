/*
	Zadanie zaliczeniowe 1: Wielomiany
	
	Kompilacja poleceniem:
	gcc @opcje wielomiany_ver4.c -o wielomiany_ver4

	Program umozliwia wykonywanie dodawania i mnozenia wielomianow.
	Pierwszym argumentem dzialania jest wielomian przechowywany w akumulatorze (poczatkowo zerowy, a w kolejnych  turach wynik poprzedniego dzialania).
	Drugim argumentem dzialania jest wielomian podany przez uzytkownika po podaniu dzialania, ktore ma byc wykonane ('+' lub '*').
	Program konczy dzialanie po podaniu przed uzytkownika '.'.
	
	autor: Magdalena Czapiewska <mc427863@students.mimuw.edu.pl>
	data: 21 listopada 2021

*/

#include <stdio.h>
#include <stdbool.h>
#define JEDENASCIE 11
#define DZIESIEC 10

typedef struct pol{ //struktura, w ktorej przechowuje dane dotyczace polecenia
		int dzialanie; // + dodaj do akumulatora | * pomnoz akumulator przez | . zakoncz dzialanie programu
		int wielomian[JEDENASCIE]; //tablica, w ktorej bede przechowywac wspolczynniki wielomianu (indeks komorki w tablicy to wykladnik potegi zmiennej, przy ktorej stoi wspolczynnik)
		int stopien; //stopien wielomianu
		int aktualny_wspolczynnik;
		int aktualna_operacja;
		int aktualny_indeks;
		int ostatni_znak; //zmienne aktualny_wspolczynnik, aktualna_operacja, aktualny_indeks i ostatni_znak sa zmiennymi pomocniczymi podczas wczytywania polecenia
} pol;
	
typedef struct aku{ //struktura, w ktorej przechowuje dane dotyczace akumulatora
		int wielomian[JEDENASCIE]; //tablica, w ktorej bede przechowywac wspolczynniki wielomianu (indeks komorki w tablicy to wykladnik potegi zmiennej, przy ktorej stoi wspolczynnik)
		int stopien; //stopien wielomianu
} aku;


int wiekszy(int a, int b){ //funkcja, ktora zwraca wieksza z dwoch wartosci
	
	if(a > b){
		return a;
	}
	else{
		return b;
	}
	
}

int wart_bez(int a){ //funkcja, ktora zwraca wartosc bezwzgledna z liczby
	
	if(a >= 0){
		return a;
	}
	else{
		return (-1) * a;
	}
	
}

int sprawdz_stopien(int a[], int gdzie){ //funkcja, ktora sprawdza stopien wielomianu
	
	for(int i = gdzie; i >= 0; i--){
		if(a[i] != 0){
			return i;
		}
	}
	
	return -1;
}

void wyzeruj_tablice(int a[]){ //funkcja, ktora umieszcza w tablicy wspolczynniki wielomianu zerowego
	
	for(int i = 0; i < JEDENASCIE; i++){
		a[i] = 0;
	}
	
}

aku skopiuj_tablice(aku kopiowana, aku wypelniana){ //funkcja, ktora tablice "wypelniana" wypelnia zawartoscia tablicy "kopiowana"
	
	for(int i = 0; i < JEDENASCIE; i++){
		wypelniana.wielomian[i] = kopiowana.wielomian[i];
	}
	wypelniana.stopien = kopiowana.stopien;
	return wypelniana;
	
}

aku dodaj(aku a, pol b){ //funkcja, ktora dodaje wielomiany, ktorych wspolczynniki sa zapisane w tablicach a.wielomian i b.wielomian i umieszcza wynik w tablicy a.wielomian
	
	int w = wiekszy(a.stopien, b.stopien);
	for(int i = 0; i <= w; i++){
		a.wielomian[i] += b.wielomian[i];
	}
	a.stopien=sprawdz_stopien(a.wielomian, w);
	return a;
}

aku pomnoz(aku a, pol b){ //funkcja, ktora mnozy wielmiany, ktorych wspolczynniki sa zapisane w tablicach a.wielomian i b.wielomian i umieszcza wynik w tablicy a.wielomian
							// w ta funkcje wchodze tylko, gdy zaden z wielomianow a.wielomian, b.wielomian nie jest wielomianem zerowym
	aku pomocnicza;
	pomocnicza = skopiuj_tablice(a, pomocnicza);
	wyzeruj_tablice(a.wielomian);
	a.stopien = -1; //to w zasadzie nie ma znaczenia, ale nadaje wielomianowi a.wielomian stopien -1 dla formalnosci, bo go wyzerowalam
	
	for(int i = 0; i <= b.stopien; i++){
		for(int j = 0; j <= pomocnicza.stopien; j++){
			a.wielomian[i+j] += (b.wielomian[i] * pomocnicza.wielomian[j]);
		}
	}
	a.stopien = b.stopien + pomocnicza.stopien;
	return a;
		
}

void wypisz_znak(int n){
	if(n > 0){
		printf(" + ");
	}
	else{
		printf(" - ");
	}
}

void wypisz_pierwszy_jednomian(aku a){ //funkcja, ktora wypisuje pierwszy jednomian wchodzacy w sklad wielomianu (chodzi tu o brak obecnosci znaku '+' przy dodatnim wspolczynniku i brak spacji miedzy ewentualnym znakiem '-' i wartoscia bezwzgledna liczby)
	
	if((a.wielomian[a.stopien] != 1) && (a.wielomian[a.stopien] != (-1))){
		printf("%d", a.wielomian[a.stopien]);
	}
	else if(a.wielomian[a.stopien] == (-1)){
		printf("-");
	}
	printf("x"); 
	if(a.stopien > 1){
		printf("^%d", a.stopien);
	}
	
}

void wypisz_srodek_wielomianu(aku a){ //funkcja, ktora wypisuje jednomiany wchodzace w sklad wielomianu o wykladniku potegi zmiennej od st_a-1 do 2. Jesli st_a<=2, petla sie nie wykona ani razu
	
	for(int i = a.stopien-1; i >= 2; i--){ 
		if(a.wielomian[i] != 0){
			wypisz_znak(a.wielomian[i]);
			if((a.wielomian[i] != 1) && (a.wielomian[i] != (-1))){
				printf("%d", wart_bez(a.wielomian[i]));
			}
			printf("x^%d", i); //dla jednomianow stopnia >=2 wypisuje wykladnik potegi przy x. Jednomian stopnia 1 wypisuje ponizej, zeby nie musiec tutaj zawsze sprawdzac czy i>1
		}
	}
	
}

void wypisz_jednomian_o_stopniu_1(int a[]){ //funkcja, ktora wypisuje jednomian stopnia 1
	
	if(a[1] != 0){
		wypisz_znak(a[1]);
		if((a[1] != 1) && (a[1] != (-1))){
			printf("%d", wart_bez(a[1]));
		}
		printf("x"); //bez wykladnika potegi przy x - dlatego wyroznilam ten jednomian, tworzac dla niego osobna funkcje
	}
	
}

void wypisz_wyraz_wolny(int a[]){
	
	wypisz_znak(a[0]);
	printf("%d", wart_bez(a[0]));
	
}

void wypisz_wielomian(aku a){ //funkcja, ktora wypisuje wielomian stopnia a.stopien, ktorego wspolczynniki sa zapisne w tablicy a.wielomian
	
	if(a.stopien == -1){
		printf("0\n");
	}
	else if(a.stopien == 0){
			printf("%d\n", a.wielomian[0]);
		}
		else{
			wypisz_pierwszy_jednomian(a);
			wypisz_srodek_wielomianu(a);
			if(a.stopien > 1){ //Jesli st_a>1, to pierwszy jednomian byl stopnia wiekszego od 1, czyli jednomianu stopnia 1 jeszcze nie wypisalam
				wypisz_jednomian_o_stopniu_1(a.wielomian);
			}
			if(a.wielomian[0] != 0){
				wypisz_wyraz_wolny(a.wielomian);
			}
			printf("\n");
		}
	
	
}

bool spacja(int c){
	
	if(c == ' '){
		return true;
	}
	else{
		return false;
	}
	
}

bool enter(int c){
	
	if(c == '\n'){
		return true;
	}
	else{
		return false;
	}
	
}

bool operacja(int c){
	
	if(c == '+' || c == '-'){
		return true;
	}
	else{
		return false;
	}
	
}

bool cyfra(int c){
	
	if(c >= '0' && c <= '9'){
		return true;
	}
	else{
		return false;
	}
	
}

bool iks(int c){
	
	if(c == 'x'){
		return true;
	}
	else{
		return false;
	}
	
}

bool daszek(int c){
	
	if(c == '^'){
		return true;
	}
	else{
		return false;
	}
	
}

int pomin_spacje(){
	
	int c = getchar();
	while(spacja(c)){
		c = getchar();
	}
	return c;
	
}

pol wypelnij(pol b){ //funkcja, ktora zapisuje wspolczynnik jednomianu w odpowiednie miejsce tablicy reprezentujacej wielomian
	
	if(b.aktualna_operacja == '+'){
		b.wielomian[b.aktualny_indeks] = b.aktualny_wspolczynnik;
	}
	else if(b.aktualna_operacja == '-'){
		b.wielomian[b.aktualny_indeks] = ((-1) * b.aktualny_wspolczynnik);
	}
	if(b.stopien == -1){
		b.stopien = b.aktualny_indeks;
	}
	return b;
	
}

pol szukaj_wykladnika_faza_koncowa(pol b){ //funkcja, ktora szuka wykladnika jednomianu, jesli wiemy juz, ze nie jest to 0 ani 1
	b.aktualny_indeks = 0;
	b.ostatni_znak = pomin_spacje();
	while(cyfra(b.ostatni_znak)){
		b.aktualny_indeks *= DZIESIEC;
		b.aktualny_indeks += (b.ostatni_znak - '0');
		b.ostatni_znak = getchar();
	}
	if(spacja(b.ostatni_znak)){
		b.ostatni_znak = pomin_spacje();
	}
	return b;
	
}



pol szukaj_wykladnika_faza_poczatkowa(pol b){ //funkcja, ktora szuka pierwszego znaku po x, ktory nie jest spacja

	
	b.ostatni_znak = pomin_spacje();
	return b;
	
}

pol szukaj_wspolczynnika(pol b){ //funkcja, ktora szuka wspolczynnika jednomianu

	if(spacja(b.ostatni_znak)){
		b.ostatni_znak = pomin_spacje();
	}
	
	if(iks(b.ostatni_znak)){
		b.aktualny_wspolczynnik = 1;
	}
	else{
		b.aktualny_wspolczynnik = 0;
		if(cyfra(b.ostatni_znak)){ //czyli jest to jednomian najbardziej z lewej strony o dodatnim wspolczynniku
			b.aktualny_wspolczynnik = (b.ostatni_znak - '0');
		}
		b.ostatni_znak = pomin_spacje();
		while(cyfra(b.ostatni_znak)){
			b.aktualny_wspolczynnik *= DZIESIEC;
			b.aktualny_wspolczynnik += (b.ostatni_znak - '0');
			b.ostatni_znak = getchar();
		}
		if(spacja(b.ostatni_znak)){
			b.ostatni_znak = pomin_spacje();
		}
	}
	return b;
	
}

pol wczytaj_pierwszy_znak(pol b){ //funkcja, ktora wczytuje pierwszy znak pierwszego jednomianu
                                        
	b.ostatni_znak = pomin_spacje();
	if(b.ostatni_znak == '0'){
		return b;
	}
	if(cyfra(b.ostatni_znak)){
		b.aktualna_operacja = '+';
	}
	else if(b.ostatni_znak == '-'){
		b.aktualna_operacja = '-';
	}
	else if(iks(b.ostatni_znak)){
		b.aktualna_operacja = '+';
		b.aktualny_wspolczynnik = 1;
	}
	return b;
	
	
}

pol wczytaj_reszte_polecenia(pol b){ //funkcja, ktora wczytuje reszte polecenia, kiedy pierwszym znakiem polecenia nie byla kropka, oznaczajaca koniec dzialania programu
	
	b.stopien = -1;
	wyzeruj_tablice(b.wielomian);
	
	b = wczytaj_pierwszy_znak(b);
	if(b.ostatni_znak == '0'){
		getchar(); //tzn, ze wczytuje znak konca linii
		return b;
	}
	
	while(enter(b.ostatni_znak) == false){
		b = szukaj_wspolczynnika(b);
		if(enter(b.ostatni_znak)){
			b.aktualny_indeks = 0;
			b = wypelnij(b);
		}
		else if(iks(b.ostatni_znak)){
			if(b.aktualny_wspolczynnik == 0){
				b.aktualny_wspolczynnik = 1;
			}
			b = szukaj_wykladnika_faza_poczatkowa(b);
			if(enter(b.ostatni_znak)){
				b.aktualny_indeks = 1;
				b = wypelnij(b);
			}
			else if(daszek(b.ostatni_znak)){
				b = szukaj_wykladnika_faza_koncowa(b);
				b = wypelnij(b);
				if(operacja(b.ostatni_znak)){
					b.aktualna_operacja = b.ostatni_znak;
				}
			}
			else if(operacja(b.ostatni_znak)){
				b.aktualny_indeks = 1;
				b = wypelnij(b);
				b.aktualna_operacja = b.ostatni_znak;
			}
		}
	}
	return b;
	
}

aku obsluz_polecenie(aku a, pol b){
	
	if(b.dzialanie == '+'){
		a = dodaj(a, b);
	}
	else if(b.dzialanie == '*'){
		if(b.stopien == (-1)){ //Jesli wielomian b.wielomian jest wielomianem zerowym, to pomnozony przez niego wielomian a.wielomian zostanie wyzerowany
			wyzeruj_tablice(a.wielomian);
			a.stopien = -1;
		}
		else if(a.stopien != (-1)){ //Jesli wielomian a.wielomian jest wielomianem zerowym, to pomnozenie go przez cokolwiek i tak nic nie zmieni
			a = pomnoz(a, b);
		}
	}
	wypisz_wielomian(a);
	return a;
	
}

void wypisz_tablice(int a[]){
	for(int i=0; i<JEDENASCIE; i++){
		printf("%d ", a[i]);
	}
	printf("\n");
}

int main(){
	
	aku akumulator;
	pol polecenie;
	
	akumulator.stopien = -1;
	wyzeruj_tablice(akumulator.wielomian);
	
	polecenie.aktualny_indeks = 0;

	while((polecenie.dzialanie = getchar()) != '.'){
		polecenie = wczytaj_reszte_polecenia(polecenie);
		akumulator = obsluz_polecenie(akumulator, polecenie);
	}
	
	return 0;
	
}
