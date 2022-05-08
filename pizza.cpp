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

void charge(Comanda* vec[], int n, fstream& file){
	string s;
	for(int i=0; i<n; i++){
		file >> s;
		vec[i] = new Comanda(s);
	}
}

void stampa(Comanda* vec[], int n){
	for(int i=0; i<n; i++){
		cout << *vec[i] << endl;
	}
}

void ordXCliente(Comanda* vec[], int n){
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
	for(int i=0; i<n; i++){
		if(vec[i]->getCognome() == s){
			cout << *vec[i] << endl;
			sum += vec[i]->getPrezzo() * vec[i]->getQuantita();
		}
	}
	if(sum == 0){
		cout << "Cliente non trovato, riprova" << endl;
		ordXCliente(vec,n);
	}
	else cout << "Il totale speso da " << s << " e' di " << sum << " euro." << endl;
}

int main(){
	fstream input("Comande.txt", fstream::in);
	Comanda* vett[DIM];
	
	charge(vett,DIM,input);
	ordXCliente(vett,DIM);
}
