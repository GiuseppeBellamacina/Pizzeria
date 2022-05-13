#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>
#define DIM 59

using namespace std;

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

ostream& operator<< (ostream& os, Comanda& a){
	return a.print(os);
}

class NodoC{
	public:
    	Comanda c;
    	NodoC* succ;
};

class ListaC{
	private:
    	NodoC* testa;
	public:
    	ListaC(){testa=nullptr;}
    	NodoC* getTesta(){return testa;}
    	void inserisci(const Comanda com);
    	NodoC* ricerca(const Comanda com);
    	void rimuovi(Comanda com);
    	
    	friend
    	ostream& operator<<(ostream& out, const ListaC& ls);
};

ostream& operator<<(ostream& out, const ListaC& ls){
    NodoC* iter = ls.testa;
    while(iter!=nullptr){
        out << iter->c << endl;
        iter = iter->succ;
    }
    return out;
}

class Nodo{
	public:
		string cliente;
    	ListaC ls;
    	Nodo* succ;
};

class Lista{
	private:
    	Nodo* testa;
	public:
    	Lista(){testa=nullptr;}
    	Nodo* getTesta(){return testa;}
    	void inserisci(const ListaC lista);
    	Nodo* ricerca(const ListaC lista);
    	void rimuovi(ListaC lista);
    	
    	friend
    	ostream& operator<<(ostream& out, const Lista& ls);
};

ostream& operator<<(ostream& out, const Lista& ls){
    Nodo* iter = ls.testa;
    while(iter!=nullptr){
        out << iter->cliente << endl;
        iter = iter->succ;
    }
    return out;
}

void ListaC::inserisci(const Comanda com){
    NodoC* nuovo = new NodoC;
    nuovo->c = com;
    nuovo->succ = this->testa;
    this->testa = nuovo;
}

void Lista::inserisci(ListaC lista){
    Nodo* nuovo = new Nodo;
    nuovo->ls = lista;
    nuovo->succ = this->testa;
    this->testa = nuovo;
}

void charge(ListaC& ls, fstream& file){
	string s;
	while(file){
		file >> s;
		Comanda com(s);
		ls.inserisci(com);
	}
}

int main(){
	fstream input("Comande.txt", fstream::in);
	ListaC ls;
	charge(ls,input);
	cout << ls;
}
