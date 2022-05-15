#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>
#define DIM 59

using namespace std;

// qui creo la classe per le comande
class Comanda{
	private:
		string cognome;
		string pizza;
		int quantita;
		double prezzo;
	public:
		Comanda(){};
		Comanda(string s){
			int i = 0;
			while(s[i] != ','){
				cognome += s[i];
				i++;
			}
			i++;
			while(s[i] != ','){
				pizza += s[i];
				i++;
			}
			i++;
			stringstream ss1;
			while(s[i] != ','){
				ss1 << s[i];
				i++;
			}
			ss1 >> quantita;
			i++;
			stringstream ss2;
			while(s[i] != '.'){
				ss2 << s[i];
				i++;
			}
			ss2 >> prezzo;
			i++;
			double aux;
			stringstream ss3;
			while(i<s.length()){
				ss3 << s[i];
				i++;
			}
			ss3 >> aux;
			prezzo += aux/10;
		}
		
		string const getCognome(){
			return cognome;
		}
		
		string const getPizza(){
			return pizza;
		}
		
		int const getQuantita(){
			return quantita;
		}
		
		double const getPrezzo(){
			return prezzo;
		}
		
		ostream& print(ostream& os){
			return os << cognome << "," << pizza << "," << quantita << "," << prezzo;
		}
};

// creo anche un modo per stamparle
ostream& operator<< (ostream& os, Comanda& a){
	return a.print(os);
}

// nodo della lista minore (lista pizze per cliente)
class NodoC{
	public:
    	Comanda c;
    	NodoC* succ;
};

// lista pizze per cliente
class ListaC{
	private:
    	NodoC* testa;
	public:
    	ListaC(){testa=nullptr;}
    	NodoC* getTesta(){return testa;}
    	void inserisci(const Comanda com);
    	
    	friend
    	ostream& operator<<(ostream& out, const ListaC& ls);
};

// così mi stampa tutte le pizze dei singoli clienti (necessario l'overloading<< della classe Comanda)
ostream& operator<<(ostream& out, const ListaC& ls){
    NodoC* iter = ls.testa;
    while(iter!=nullptr){
        out << iter->c << endl;
        iter = iter->succ;
    }
    return out;
}

// nodo della lista maggiore (lista dei clienti)
class Nodo{
	public:
    	ListaC lista;
    	Nodo* succ;
};

// lista di liste (contiene i clienti e da loro si dirama la lista delle pizze)
class Lista{
	private:
    	Nodo* testa;
	public:
    	Lista(){testa=nullptr;}
    	Nodo* getTesta(){return testa;}
    	void inserisci(ListaC ls);
    	Nodo* ricerca(const string cogn);
    	void rimuovi(double val);
    	
    	friend
    	ostream& operator<<(ostream& out, const Lista& ls);
};

// se stampo la lista delle liste lui mi stampa la lista dei cognomi dei clienti
ostream& operator<<(ostream& out, const Lista& ls){
    Nodo* iter = ls.testa;
    while(iter!=nullptr){
        out << iter->lista.getTesta()->c.getCognome() << endl;
        iter = iter->succ;
    }
    return out;
}

// funzione per aggiungere le singole comande ad una lista minore
void ListaC::inserisci(const Comanda com){
    NodoC* nuovo = new NodoC;
    nuovo->c = com;
    nuovo->succ = this->testa;
    this->testa = nuovo;
}

// funzione per aggiungere un nuovo cliente alla lista maggiore
void Lista::inserisci(ListaC ls){
    Nodo* nuovo = new Nodo;
    nuovo->lista = ls;
    nuovo->succ = this->testa;
    this->testa = nuovo;
}

// (PUNTO 1 e 2) funzione per carire i dati dal file sulla lista di liste (in modo organizzato)
void upload(Lista& ls, fstream& file){
	string s, c = "";
	while(file){
		file >> s;
		Comanda com(s);
		if(com.getCognome()!=c){
			ListaC nuovo;
			ls.inserisci(nuovo);
			c = com.getCognome();
		}
		ls.getTesta()->lista.inserisci(com);
		if(file.eof()) break;
	}
}

// (PUNTO 3) funzione di ricerca del cliente per cognome
Nodo* Lista::ricerca(const string cogn){
    Nodo* p;
    for(p=this->testa; p!=nullptr; p=p->succ)
        if(p->lista.getTesta()->c.getCognome() == cogn)
            return p;
    return nullptr;
}

// (PUNTO 3) funzione per stampare le ordinazioni di un dato cliente e calcolarne la spesa
void ordXCliente(Lista& ls){
	system("cls");
	string s;
	double sum = 0;
	cout << "Inserisci nome cliente:\t";
	cin >> s;
	while (cin.fail()){
		cerr << "Cosa caspita hai scritto?" << endl;
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "Inserisci nome cliente:\t";
		cin >> s;
	}
	if(ls.ricerca(s)){
		Nodo* p = ls.ricerca(s);
		NodoC* b = p->lista.getTesta();
		cout << p->lista;
		while(b!=nullptr){
			sum += b->c.getPrezzo()*b->c.getQuantita();
			b = b->succ;
		}
	}
	else{
		cout << "Il cliente non e' stato trovato, scegliere tra questi:" << endl << endl;
		cout << ls;
		system("pause");
		ordXCliente(ls);
	}
	cout << "Il totale speso da " << s << " e' di " << sum << " euro." << endl;
}

// (PUNTO 4) funzione per rimuovere un cliente dalla lista maggiore se egli spende meno di val (la media la calcolo a parte)
void Lista::rimuovi(double val){ // NON FUNZIONA CORRETTAMENTE
    Nodo* prec;					 // VEDI DI SISTEMARE STO SCHIFO
    Nodo* curr = this->testa;
    while(curr!=nullptr){
		NodoC* ls = curr->lista.getTesta();
    	double sum = 0;
    	while(ls!=nullptr){
			sum += ls->c.getPrezzo()*ls->c.getQuantita();
			cout << ls->c.getCognome() << endl;
			cout << sum << endl;
			ls = ls->succ;
		}
    	if(sum<val){
        	prec = this->testa;
        	this->testa = prec->succ;
    	    delete prec;
	    }
	    curr = curr->succ;
	}	
}

int main(){
	fstream input("Comande.txt", fstream::in);
	Lista l;
	upload(l,input);
	ordXCliente(l);
	//l.rimuovi(25);
}
