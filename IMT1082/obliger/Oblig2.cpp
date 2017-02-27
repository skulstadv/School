// Skulstad, Vegard Nygaard Student nr: 745790  16HBITSECA
#ifdef _MSC_VER	
#define _CRT_SECURE_NO_WARNINGS
#endif 

#include  <iostream>    
#include  <fstream>     
#include  <cstring>     
#include  <cctype>      
#include  <cstdlib>     

using namespace std;

const int MAXANS = 100;
const int MAXBARN = 20;
const int MAXTXT = 80;
const int DATOLEN = 7;

enum Kjonn { jente, gutt };

void skrivMeny();
char les();
int finnesAllerede(int nr);
int les(char* t, int min, int max);
void les(char* t, char* s, bool dato);
void lesDato(char* t, char* s);
void lesFraFil();
void skrivTilFil();
void nyAnsatt();
void partnerEndring();
void nyttBarn();
void dataOmAnsatt();
void alleEttAar();
void fjernAnsatt();


class Person {
protected:
	char * fornavn;
	char fodselsdato[DATOLEN];		// Paa formen yymmdd

public:
	Person() {
		fornavn = new char[MAXTXT];
		les("Skriv inn fornavn: ", fornavn, false);
		les("Skriv inn dato [AAMMDD]: ", fodselsdato, true);
	}

	Person(ifstream & inn) {
		fornavn = new char[MAXTXT];
		inn >> fornavn >> fodselsdato;
	}

	~Person() {
		delete fornavn;
	}

	void skrivTilFil(ofstream & ut) {
		ut << fornavn << " " << fodselsdato << " ";
	}

	void display() {
		cout << fornavn << endl << fodselsdato << endl;
	}

	// Returnerer aar i fodselsdato (index 0 og 1) castet til int
	int getDato() {
		int ret = (fodselsdato[0] - '0') * 10;
		ret += fodselsdato[1] - '0';
		return ret;
	}

};


class Barn : public Person {
private:
	Kjonn kjonn;

public:
	Barn() : Person() {
		if (les("Er barnet gutt eller jente? Jente = 0, Gutt =  1", 0, 1))
			kjonn = gutt;
		else
			kjonn = jente;
	}

	Barn(ifstream & inn) : Person(inn) {
		bool erGutt;
		inn >> erGutt;
		erGutt ? kjonn = gutt : kjonn = jente;
	}

	void skrivTilFil(ofstream & ut) {
		Person::skrivTilFil(ut);
		ut << (int)kjonn << " ";
	}

	void display() {
		if (kjonn)
			cout << "Gutt" << endl;
		else
			cout << "Jente" << endl;
		Person::display();
	}
};


class Voksen : public Person {
protected:
	char * etternavn;

public:
	Voksen() : Person() {
		etternavn = new char[MAXTXT];
		les("Skriv etternavn: ", etternavn, false);
	}

	Voksen(ifstream & inn) : Person(inn) {
		etternavn = new char[MAXTXT];
		inn >> etternavn;
	}

	~Voksen() {
		delete etternavn;
	}

	void skrivTilFil(ofstream & ut) {
		Person::skrivTilFil(ut);
		ut << etternavn << " ";
	}

	void display() {
		cout << etternavn << ", ";
		Person::display();
	}
};


class Partner : public Voksen {
private:
	int telefon1;
	int telefon2;

public:
	Partner() : Voksen() {
		telefon1 = les("Skriv inn telefon jobb 8 siffer: ", 11111111, 99999999);
		telefon2 = les("Skriv inn telefon mobil 8 siffer: ", 11111111, 99999999);
	}

	Partner(ifstream & inn) : Voksen(inn) {
		inn >> telefon1 >> telefon2;
	}

	void skrivTilFil(ofstream & ut) {
		Voksen::skrivTilFil(ut);
		ut << telefon1 << " " << telefon2 << " ";
	}

	void display() {
		Voksen::display();
		cout << telefon1 << endl << telefon2 << endl;
	}
};


class Ansatt : public Voksen {
private:
	int nr;
	int antBarn;
	char* adresse;
	Partner* partner;
	Barn* barn[MAXBARN + 1];

public:
	Ansatt(int n) : Voksen() {
		adresse = new char[MAXTXT];
		this->nr = n;
		antBarn = 0;
		// NULL for aa ha muligheten til aa sjekke om partner er initialisert
		partner = NULL;
		les("Skriv din adresse: ", adresse, false);
	}

	Ansatt(int n, ifstream & inn) : Voksen(inn) {
		adresse = new char[MAXTXT];
		inn >> nr >> antBarn;
		inn.ignore();
		inn.getline(adresse, MAXTXT);
		bool harPartner;
		inn >> harPartner;
		harPartner ? partner = new Partner(inn) : 0;
		for (int i = 1; i <= antBarn; i++)
			barn[i] = new Barn(inn);
	}

	~Ansatt() {
		delete adresse;
		delete partner;
		// Sletter alle barna
		for (int i = 1; i <= antBarn; i++) {
			delete barn[i];
		}
	}

	void skrivTilFil(ofstream & ut) {
		Voksen::skrivTilFil(ut);
		ut << nr << " " << antBarn << endl << adresse << endl;
		if (partner != nullptr) {
			ut << 1 << " ";
			partner->skrivTilFil(ut);
		}
		else
			ut << 0 << " ";
		for (int i = 1; i <= antBarn; i++)
			barn[i]->skrivTilFil(ut);
	}

	void display() {
		Voksen::display();
		cout << adresse << endl;
		if (partner != nullptr) {
			partner->display();
		}
		// Skriver alle barna til skjermen
		for (int i = 1; i <= antBarn; i++) {
			barn[i]->display();
		}
		cout << endl << endl;
	}

	// Bytter partner til ny partner og returnerer adressen til gammel
	Partner * setPartner(Partner * partner) {
		// Tar vare pa gammel partner
		Partner * ret = this->partner;
		this->partner = partner;
		return ret;
	}

	Partner * getPartner() {
		return partner;
	}

	int getNumber() {
		return nr;
	}

	void nyttBarn() {
		if (antBarn < MAXBARN) {
			barn[++antBarn] = new Barn();
			return;
		}
		cout << "Barn er full" << endl;
	}

	// Skriver data om alle barn som er fodt aar n
	void harBarnDato(int n) {
		for (int i = 1; i <= antBarn; i++) {
			barn[i]->getDato() == n ? barn[i]->display() : 0;
		}
	}
};


Ansatt* ansatte[MAXANS + 1];
int sisteBrukt = 0;	// Antall ansatte i indeks array

int main() {
	char kommando;
	skrivMeny();
	lesFraFil();
	kommando = les();
	while (kommando != 'Q') {
		switch (kommando) {
		case 'N': nyAnsatt();       break;   //  Legg inn (om mulig) ny ansatt.
		case 'P': partnerEndring(); break;   //  Endre data om partner.
		case 'B': nyttBarn();       break;   //  Legg inn (om mulig) nytt barn.
		case 'D': dataOmAnsatt();   break;   //  Skriv alle data om en ansatt.
		case 'A': alleEttAar();     break;   //  Ansnr med barn f›dt et gitt †r.
		case 'F': fjernAnsatt();    break;   //  Fjern/slett en ansatt.
		default:  skrivMeny();      break;   //  Meny 
		}
		kommando = les();	//  Leser brukerens ›nske/valg.
		skrivTilFil();
	}
	return 0;
}


void skrivMeny() {
	cout << "\n\nFLGENDE KOMMANDOER ER TILGJENGELIGE:";
	cout << "\n\tN - Ny ansatt";
	cout << "\n\tP - Partner-endring";
	cout << "\n\tB - nytt Barn";
	cout << "\n\tD - alle Data om en ansatt";
	cout << "\n\tA - Alle barn f›dt ett gitt †r";
	cout << "\n\tF - Fjern en ansatt";
	cout << "\n\tQ - Quit / avslutt";
}


char les() {
	char ch;
	cout << "\n\nKommando:  ";
	cin >> ch;  cin.ignore();
	return (toupper(ch));
}


int les(char* t, int min, int max) {
	int tall;
	do {
		cout << '\t' << t << " (" << min << '-' << max << "):  ";
		cin >> tall;  cin.ignore();
	} while (tall < min || tall > max);
	return tall;
}


/* Printer output streng og leser henholdsvis inn DATOLEN eller MAXTXT tegn i s
 * avhengig av om det er en dato eller ikke
 */
void les(char* s, char* t, bool dato) {
	char* temp;
	int len = 7;
	if (dato)
		temp = new char[7];
	else {
		temp = new char[MAXTXT];
		len = MAXTXT;
	}
	cout << s << endl;
	cin.getline(temp, len);
	strcpy_s(t, len, temp);
}


void lesDato(char* s, char* t) {
	char temp[5];
	cout << s << endl;
	cin.getline(temp, 5);
	strcpy_s(t, 5, temp);
}


//  Leter etter en ansatt med et gitt nummer og returnerer indeks i ansatt
int finnesAllerede(int nr) {
	for (int i = 1; i <= sisteBrukt; i++) {
		if (ansatte[i]->getNumber() == nr)
			return i;
	}
	return 0;
}


void skrivTilFil() {
	ofstream ut = ofstream("output.txt");
	ut << sisteBrukt << " ";
	// Gaar gjennom alle ansatte og kaller skrivTilFil
	for (int i = 1; i <= sisteBrukt; i++) {
		ut << ansatte[i]->getNumber() << " ";
		ansatte[i]->skrivTilFil(ut);
	}
	ut.close();
}


void lesFraFil() {
	ifstream inn = ifstream("output.txt");
	// Sjekker at filen ikke er tom
	if (inn.peek() == EOF)
		return;
	// Forste tegn i filen er antallet ansatte som skal i ansatte[] 
	int antallAnsatte;
	inn >> antallAnsatte;
	sisteBrukt = 0;
	int n = 0;
	// Oppretter alle ansatte 
	for (int i = 0; i < antallAnsatte; i++) {
		inn >> n;
		ansatte[++sisteBrukt] = new Ansatt(n, inn);
	}
	inn.close();
}


void nyAnsatt() {
	int nr = les("Skriv inn ansattnummer: ", 1, 100);
	// Sjekker at ansattnr ikke er i bruk og at vi har plass til en til ansatt
	!finnesAllerede(nr) && sisteBrukt < MAXANS ? 
		ansatte[++sisteBrukt] = new Ansatt(nr) : 0;
}


void partnerEndring() {
	int nr = les("Skriv ansatt nr du vil endre partner pa: ", 1, 100);
	if (finnesAllerede(nr)) {
		switch (les("Velg 0 for a endre eller opprette, 1 for a slette: ", 0, 1)) {
		case 0: delete ansatte[finnesAllerede(nr)]->setPartner(new Partner()); break;
		case 1: delete ansatte[finnesAllerede(nr)]->setPartner(NULL); break;
		default: break;
		}
	}
	else
		cout << "Fant ikke ansatt med det nummeret.";
}


void nyttBarn() {
	int n = les("Skriv ansatt nr du vil legge til barn pa: ", 1, 100);
	// Hvis ansatt med id finnes sa legges barn til
	if (finnesAllerede(n))
		ansatte[finnesAllerede(n)]->nyttBarn();
	else
		cout << "Fant ikke ansatt med det nummeret.";
}


// Skriver data om ansatt hvis den eksisterer
void dataOmAnsatt() {
	int id = les("Skriv AnsattNR: ", 1, 100);
	finnesAllerede(id) ? ansatte[finnesAllerede(id)]->display():0;
}


// Skriver ansatte med barn fodt i aar som leses inn
void alleEttAar() {
	char aar[5];
	lesDato("Skriv inn aar: ", aar);
	int toSiffer = (aar[2] - '0') * 10;
	toSiffer += aar[3] - '0';
	for (int i = 1; i <= sisteBrukt; i++) {
		ansatte[i]->harBarnDato(toSiffer);
	}
}


void fjernAnsatt() {
	int n = les("Skriv inn nr du vil slette: ", 1, 100);
	if (finnesAllerede(n)) {
		Ansatt * temp = ansatte[finnesAllerede(n)];
		ansatte[finnesAllerede(n)] = ansatte[sisteBrukt--];
		delete temp;
		cout << "Ansatt fjernet";
	}
	else
		cout << "Fant ikke ansatt med nr " << n << endl;
}
