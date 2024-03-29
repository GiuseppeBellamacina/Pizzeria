#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>

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

// cos� mi stampa tutte le pizze dei singoli clienti (necessario l'overloading<< della classe Comanda)
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
    	Nodo* ricerca(double val);
    	double getSpesa(Nodo* n);
    	double getMedia();
    	void rimuovi(Nodo* n);
    	
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
	file.open("Comande.txt", fstream::in);
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
	file.close();
}

// (PUNTO 3) funzione di ricerca del cliente per cognome
Nodo* Lista::ricerca(const string cogn){
    Nodo* p;
    for(p=this->testa; p!=nullptr; p=p->succ)
        if(p->lista.getTesta()->c.getCognome() == cogn)
            return p;
    return nullptr;
}

// (PUNTO 3 e 4) funzione per calcolare la spesa del singolo cliente
double Lista::getSpesa(Nodo* n){
	NodoC* ls = n->lista.getTesta();
    	double sum = 0;
    	while(ls!=nullptr){
			sum += ls->c.getPrezzo()*ls->c.getQuantita();
			ls = ls->succ;
	}
	return sum;
}

// (PUNTO 3) funzione per stampare le ordinazioni di un dato cliente e calcolarne la spesa
void ordXCliente(Lista& ls){
	system("cls");
	string s;
	bool flag=true;
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
		cout << p->lista;
		sum = ls.getSpesa(p);
	}
	else{
		cout << "Il cliente non e' stato trovato, scegliere tra questi:" << endl << endl;
		cout << ls << endl;
		system("pause");
		ordXCliente(ls);
		flag=false;
	}
	if(flag)cout << "Il totale speso da " << s << " e' di " << sum << " euro." << endl;
}

// (PUNTO 4) funzione di ricerca del cliente che ha speso meno di val
Nodo* Lista::ricerca(double val){
    Nodo* p;
    for(p=this->testa; p!=nullptr; p=p->succ){
        double sum = this->getSpesa(p);
		if(sum < val) return p;
	}
    return nullptr;
}

// (PUNTO 4) funzione per rimuovere un cliente dalla lista maggiore se egli spende meno di val (la media la calcolo a parte)
void Lista::rimuovi(Nodo* n){
	Nodo* prec = this->testa;
   	if (n==nullptr)
       	return;
    else if(n->succ==nullptr){
       	while(prec->succ!=n){
       		prec = prec->succ;
		}
		prec->succ=nullptr;
	}
   	else{
       	while (n->succ!=nullptr) {
           	n->lista = n->succ->lista;
           	prec = n;
           	n = n->succ;
       	}
       	prec->succ = nullptr;
    }
}

// (PUNTO 4 BONUS) funzione per salvare su file i clienti che hanno speso meno della media
void download(Lista& ls, double val){
	fstream save("Comande_eliminate.txt", fstream::out);
	while(ls.ricerca(val)){
		Nodo* n = ls.ricerca(val);
		save << n->lista;
		ls.rimuovi(n);
	}
	save.close();
}

// (PUNTO 4) funzione per la media
double Lista::getMedia(){
	double sum = 0;
	short c = 0;
	for(Nodo* p = this->testa; p!=nullptr; p=p->succ){
		sum += this->getSpesa(p);
		c++;
	}
	return sum/c;
}

// (PUNTO 5) nodo per ListaPizze per le pizze
class NodoPizze{
	public:
		string pizza;
		int qt;
		NodoPizze* succ;
};

// (PUNTO 5) lista per le pizze
class ListaPizze{
	private:
		NodoPizze* testa;
	public:
		ListaPizze(){testa=nullptr;}
    	NodoPizze* getTesta(){return testa;}
    	void inserisci(string p, int q);
    	NodoPizze* ricerca(string pz);
    	NodoPizze* findMax();
    	void classifica(Lista& ls);
};

// (PUNTO 5) inserimento per ListaPizze
void ListaPizze::inserisci(string p, int q){
    NodoPizze* nuovo = new NodoPizze;
    nuovo->pizza = p;
    nuovo->qt = q;
    nuovo->succ = this->testa;
    this->testa = nuovo;	
}

// (PUNTO 5) ricerca nodo pizza
NodoPizze* ListaPizze::ricerca(string pz){
	NodoPizze* p;
    for(p=this->testa; p!=nullptr; p=p->succ)
        if(p->pizza == pz)
            return p;
    return nullptr;
}

// (PUNTO 5) ricerca pizza pi� ordinata
NodoPizze* ListaPizze::findMax(){
	NodoPizze* p;
	NodoPizze* m;
	int max = 0;
    for(p=this->testa; p!=nullptr; p=p->succ){
    	if(p->qt > max){
        	max = p->qt;
        	m = p;
		}
	}
	if(max) return m;
	return nullptr;
}

// (PUNTO 5) funzione per la classifica
void ListaPizze::classifica(Lista& ls){
	for(Nodo* p=ls.getTesta(); p!=nullptr; p=p->succ){
		for(NodoC* b=p->lista.getTesta(); b!=nullptr; b=b->succ){
			NodoPizze* n = this->ricerca(b->c.getPizza());
			if(n){
				n->qt += b->c.getQuantita();
			}
			else{
				this->inserisci(b->c.getPizza(),b->c.getQuantita());
			}
		}
	}
	system("cls");
	cout << "CLASSIFICA PIZZE PIU' VENDUTE:" << endl;
	int i = 1;
	while(this->findMax()){
		NodoPizze* p = this->findMax();
		cout << i++ << ") " << p->pizza << ": " << p->qt << endl;
		p->qt = 0;
	}
}

int main(){
	fstream input; // apro il file per leggere da esso
	Lista l1,l2; // creo una lista (quella maggiore)
	// PUNTO 1 e 2: carico i dati sulla lista
	upload(l1,input);
	upload(l2,input); // faccio una lista di copia cos� la classifica si fa sulla lista iniziale (con tutti i clienti)
	// PUNTO 3: cerco le ordinazioni di un cliente, le stampo e calcolo il totale
	ordXCliente(l1);
	system("pause");
	// PUNTO 4: elimino dalla lista i clienti che hanno speso poco ma salvo le loro ordinazioni su file
	double media = l1.getMedia();
	download(l1,media);
	// PUNTO 5: creo una ListaPizze e avvio la funzione
	ListaPizze ll;
	ll.classifica(l2);
	cout << "A proposito, le ordinazioni dei clienti che hanno speso meno di " << media << " sono state salvate su Comande_eliminate.txt";
	
	return 0;
}
