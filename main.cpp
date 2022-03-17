#include<iostream>
#include<string>
#include<list>
#include<vector>
#include<queue>
#include<stack>
#include <algorithm>

using namespace std;

#ifndef _Cvor_h_
#define _Cvor_h_

class  Cvor
{
public:
	struct Grana
	{
		int index_dest;
		unsigned int w;
		bool izgradjen;

		Grana(int i, unsigned int w) : index_dest(i), w(w) {}
		Grana(int i, unsigned int w, bool iz) : index_dest(i), w(w), izgradjen(iz) {}
		Grana() {}
	};

	struct Lista
	{
		Grana g;
		Lista* sled;

		Lista(Grana g1) : g(g1), sled(nullptr) {}
	};

	Lista* susedi = nullptr, * kraj = nullptr;
	int index, brSuseda = 0;

	Cvor() = default;
	Cvor(int i) : index(i) {}

	Cvor& operator = (const Cvor&) = default;


	Cvor(const Cvor& c)
	{
		index = c.index; brSuseda = c.brSuseda;
		Lista* novi = nullptr, * tek = c.susedi;

		while (tek)
		{
			novi = new Lista(tek->g);
			if (!susedi) susedi = novi;
			else kraj->sled = novi;
			kraj = novi;
			tek = tek->sled;
		}
	}

	Cvor(Cvor&& c)
	{
		index = c.index; brSuseda = c.brSuseda;
		susedi = c.susedi;
		kraj = c.kraj;

		c.susedi = nullptr;
		c.kraj = nullptr;
	}

	~Cvor()
	{
		UkloniSveSusede();
	}

	bool DodajSuseda(int i, int w)
	{
		Lista* novi = new Lista(Grana(i, w));
		if (!susedi) susedi = novi;
		else kraj->sled = novi;
		kraj = novi;
		brSuseda++;

		return true;
	}// O(1)

	bool DodajSuseda_Izgradjen(int i, int w, bool iz)
	{
		Lista* novi = new Lista(Grana(i, w, iz));
		if (!susedi) susedi = novi;
		else kraj->sled = novi;
		kraj = novi;
		brSuseda++;

		return true;
	}// O(1)

	bool UkloniSuseda(int index)
	{
		Lista* tek = susedi, * pret = nullptr;
		bool brisi = false;

		if (tek)
		{

			while (tek)
			{
				if (tek->g.index_dest == index)
				{
					if (!pret) susedi = tek->sled;
					else
					{
						pret->sled = tek->sled;
						if (!tek->sled)
							kraj = pret;
					}
					brisi = true;
					break;
				}
				pret = tek;
				tek = tek->sled;
			}
			if (brisi) { delete(tek); brSuseda--; return true; }
			else return false;
		}


		return false;
	}

	bool UkloniSveSusede()
	{
		Lista* tek = susedi, * stari;

		while (tek)
		{
			stari = tek;
			tek = tek->sled;
			delete stari;
		}
		brSuseda = 0;

		return true;
	}

	vector<int> SviSusedi() const
	{
		Lista* tek = susedi;
		vector<int> niz;
		niz.reserve(brSuseda);

		while (tek)
		{
			niz.push_back(tek->g.index_dest);
			tek = tek->sled;
		}

		return niz;
	}

	void Ispis() const
	{
		Lista* tek = susedi, * stari;
		cout << "Cvor " << index << ":\t";
		while (tek)
		{
			cout << tek->g.index_dest << " --> ";
			tek = tek->sled;
		}
		cout << endl;
	}

	Grana Min(bool* b)
	{
		Lista* tek = susedi, * stari;
		Grana min = { 0, 0 };
		bool prvi = true;

		while (tek)
		{
			if (b[tek->g.index_dest - 1])
			{
				tek = tek->sled;
				continue;
			}

			if (prvi)
			{
				min = tek->g;
				prvi = false;
			}
			else if (tek->g.w < min.w)
			{
				min = tek->g;
			}
			tek = tek->sled;
		}

		return min;
	}

	int* VektorTezina(int n)
	{
		Lista* tek = susedi;
		int* niz = new int[n];

		for (int i = 0; i < n; i++)
		{
			niz[i] = -1;  // <==> infinity 
		}

		while (tek)
		{
			niz[tek->g.index_dest - 1] = tek->g.w;
			tek = tek->sled;
		}

		return niz;
	}

	bool Izgradjen_Put(int index)
	{
		Lista* tek = susedi;

		while (tek)
		{
			if (tek->g.index_dest == index)
			{
				return tek->g.izgradjen;
			}
			tek = tek->sled;
		}

		return 0;
	}
};

#endif // _Cvor_h_

#ifndef _Graf_h_
#define _Graf_h_

class  Graf
{
	int e = 0, brV;


public:
	int n = 0;
	vector<Cvor> cvorovi; //public
	Graf() = default;

	Graf(int V_number) : n(V_number), brV(V_number)
	{
		//cvorovi.reserve(n);
		for (int i = 0; i < n; i++)
		{
			cvorovi.push_back(Cvor(i + 1));
			//cout << cvorovi[i].index << endl; // skloni ovo
		}
	}// O(n)

	bool DodajCvor()
	{
		n++; brV++;
		cvorovi.push_back(Cvor(n));
		cout << "Cvor pod rednim brojem (indeksom) " << n << " je uspesno dodat." << endl;
		return true;
	}// O(1)

	bool UkloniCvor(int index)
	{
		if (index <= 0 || index > n) return false;
		if (!cvorovi[index - 1].index) return false;

		for (int i = 0; i < n; i++)
			if (i + 1 != index) cvorovi[i].UkloniSuseda(index);
		//n--;
		brV--;
		cvorovi[index - 1].UkloniSveSusede();
		cvorovi[index - 1] = Cvor(0);

		return true;
	}

	bool DodajGranu(int index_izvor, int index_dest, int tezina = 0)
	{
		if (index_izvor <= 0 || index_izvor > n || index_dest <= 0 || index_dest > n) return false;
		if (tezina < 0) tezina = 0;
		if (!cvorovi[index_izvor - 1].index || !cvorovi[index_dest - 1].index) return false;
		if (!cvorovi[index_izvor - 1].DodajSuseda(index_dest, tezina) || !cvorovi[index_dest - 1].DodajSuseda(index_izvor, tezina)) return false;
		e++;

		return true;
	}

	bool DodajGranu_Izgradjen(int index_izvor, int index_dest, bool izgradjen, int tezina = 0)
	{
		if (index_izvor <= 0 || index_izvor > n || index_dest <= 0 || index_dest > n) return false;
		if (tezina < 0) tezina = 0;
		if (!cvorovi[index_izvor - 1].index || !cvorovi[index_dest - 1].index) return false;
		if (!cvorovi[index_izvor - 1].DodajSuseda_Izgradjen(index_dest, tezina, izgradjen) || !cvorovi[index_dest - 1].DodajSuseda_Izgradjen(index_izvor, tezina, izgradjen)) return false;
		e++;

		return true;
	}

	bool UkloniGranu(int index_izvor, int index_dest)
	{
		if (index_izvor <= 0 || index_izvor > n || index_dest <= 0 || index_dest > n) { cout << "Uklanjanje grane nije bilo uspesno, uneti cvorovi ne postoje." << endl; return false; }
		if (!cvorovi[index_izvor - 1].index || !cvorovi[index_dest - 1].index) { cout << "Uklanjanje grane nije bilo uspesno, uneti cvorovi ne postoje." << endl; return false; }
		if (!cvorovi[index_izvor - 1].UkloniSuseda(index_dest) || !cvorovi[index_dest - 1].UkloniSuseda(index_izvor)) { cout << "Uklanjanje grane nije bilo uspesno, trazena grana ne postoji." << endl; return false; }

		return true;
	}

	vector<int> BFS(int index) const
	{
		vector<int> bfs, susedi;
		bool* posecen = new bool[n];
		int v = index - 1;
		queue<int> red;

		if (index <= 0 || index > n || !cvorovi[index - 1].index)
		{
			cout << "Greska! Uneti cvor ne postoji." << endl;
		}
		else
		{
			bfs.reserve(n);
			for (int i = 0; i < n; i++)
				posecen[i] = false;

			posecen[v] = true;
			red.push(v);
			bfs.push_back(v + 1);
			while (!red.empty())
			{
				v = red.front();
				red.pop();
				susedi = cvorovi[v].SviSusedi();
				for (int i = 0; i < susedi.size(); i++)
				{
					if (!posecen[susedi[i] - 1])
					{
						posecen[susedi[i] - 1] = true;
						red.push(susedi[i] - 1);
						bfs.push_back(susedi[i]);
					}

				}

			}
		}

		return bfs;
	}

	vector<int> DFS(int index) const
	{
		vector<int> dfs, susedi;
		bool* posecen = new bool[n];
		int v = index;
		stack<int> stek;

		if (index <= 0 || index > n || !cvorovi[index - 1].index)
		{
			cout << "Greska! Uneti cvor ne postoji." << endl;
		}
		else
		{
			dfs.reserve(n);
			for (int i = 0; i < n; i++)
				posecen[i] = false;

			stek.push(v);
			while (!stek.empty())
			{
				v = stek.top();
				stek.pop();
				if (!posecen[v - 1])
				{
					posecen[v - 1] = true;
					dfs.push_back(v);
					susedi = cvorovi[v - 1].SviSusedi();
					for (int i = 0; i < susedi.size(); i++)
					{
						if (!posecen[susedi[i] - 1])
							stek.push(susedi[i]);
					}
				}
			}
		}

		return dfs;
	}

	void MemorijskiIspis() const
	{
		vector<int> ispis;

		for (int i = 0; i < n; i++)
		{
			if (cvorovi[i].index)
			{
				cvorovi[i].Ispis();
			}
		}
	}

	void BrisanjeGrafa()
	{
		n = e = 0;

		for (int i = 0; i < n; i++)
			cvorovi[i].UkloniSveSusede();

		cvorovi.clear();
	}

	~Graf()
	{
		BrisanjeGrafa();
	}


	friend int Prim(Graf* G, Graf& MST);
	friend void Dijkstra(Graf* G, int** w, int*& d, int*& t, int v);
	friend int** MatricaTezina(Graf* G);
};


#endif // _Graf_h_

#ifndef _Put_h_
#define _Put_h_

class Put
{
public:
	vector<int> put; int broj_neizgradjenih = 0;

	Put(vector<int> p, int br)
	{
		put = p; broj_neizgradjenih = br;
	}

	void Stampaj()
	{
		for (int i = 0; i < put.size(); i++)
		{
			if (i != put.size() - 1) cout << put[i] << " - ";
			else cout << put[i];
		}
		cout << endl;
	}
};

#endif // !_Put_h_


void KreirajGraf(Graf* G, int& n)
{
	if (!n)
	{
		do
		{
			cout << "Unesite broj cvorova: ";
			cin >> n;
			if (n <= 0)
				cout << "Broj cvorova mora biti veci od nule!" << endl;
		} while (n <= 0);
		*G = Graf(n);
		cout << "Graf uspesno kreiran!\n";
	}
	else
	{
		cout << "Graf je vec kreiran, prvo ga morate izbrisati iz memorije.\n";
	}
}

void DodajCvor(Graf* G, int n)
{
	if (n > 0)
		G->DodajCvor();
	else
		cout << "Graf nije kreiran. Prvo kreirajte graf, da biste vrsili operacije nad njim." << endl;
}

void UkloniCvor(Graf* G, int n)
{
	if (n <= 0)
	{
		cout << "Graf nije kreiran. Prvo kreirajte graf, da biste vrsili operacije nad njim." << endl;
		return;
	}

	int temp;
	cout << "Unesite redni broj cvora koji zelite da uklonite: "; cin >> temp;
	if (G->UkloniCvor(temp))
		cout << "Uspesno uklonjen cvor!" << endl;
	else
		cout << "Uklanjanje cvora nije bilo uspesno, uneti cvor ne postoji." << endl;
}

void PoveziLokacije(Graf* G, int n, Graf& Metro)
{
	int cena, kraj = 0;
	bool prvi = 1;
	if (n <= 0)
	{
		cout << "Graf nije kreiran. Prvo kreirajte graf, da biste vrsili operacije nad njim." << endl;
		return;
	}

	int temp, temp1, temp2;
	cout << "Prvo morate uneti izmedju kojih lokacija je moguca izgradnja metroa, kao i cenu da se izgradi: \n";
	while (true)
	{
		if (prvi) prvi = 0;
		else
		{
			cout << "Da li zelite da nastavite sa unosom?\nUnesite (0) za NE / (1) za DA >>> "; cin >> kraj;
			if (!kraj) break;
		}

		do
		{
			cout << "Unesite redne brojeve lokacija koje zelite da povezete: "; cin >> temp >> temp1;
			cout << "\nUnesite cenu puta: "; cin >> temp2;
			if (temp <= 0 || temp1 < 0)
				cout << "Broj (indeks) lokacije mora biti veci od nule!" << endl;
		} while (temp < 0 || temp1 < 0);
		if (G->DodajGranu(temp, temp1, temp2))
			cout << "Uspesno dodata grana!" << endl;
		else
			cout << "Dodavanje grane nije bilo uspesno, uneti cvorovi ne postoje." << endl;
	}

	if ((cena = Prim(G, Metro)) == -1)
	{
		cout << "\n\nDoslo je do greske prilikom racunanja cene metroa, proverite da li je graf povezan." << endl;
		return;
	}
	else
	{
		cout << "\n\nMinimalna cena metroa je: " << cena << endl;
	}
}

void PoveziLokacije_Vatrogasci(Graf* G, int n)
{
	int cena, kraj = 0;
	bool prvi = 1;
	if (n <= 0)
	{
		cout << "Graf nije kreiran. Prvo kreirajte graf, da biste vrsili operacije nad njim." << endl;
		return;
	}

	int temp, temp1, temp2;
	cout << "Unesite lokacije izmedju kojih vec postoji izgradjen put: \n\n";
	while (true)
	{
		if (prvi) prvi = 0;
		else
		{
			cout << "Da li zelite da nastavite sa unosom?\nUnesite (0) za NE / (1) za DA >>> "; cin >> kraj;
			if (!kraj) break;
		}

		do
		{
			cout << "Unesite redne brojeve lokacija koje zelite da povezete: "; cin >> temp >> temp1;
			cout << "\nUnesite duzinu puta: "; cin >> temp2;
			if (temp <= 0 || temp1 < 0)
				cout << "Broj (indeks) lokacije mora biti veci od nule!" << endl;
		} while (temp < 0 || temp1 < 0);
		if (G->DodajGranu_Izgradjen(temp, temp1, 0, temp2))
			cout << "Uspesno dodata grana!" << endl;
		else
			cout << "Dodavanje grane nije bilo uspesno, uneti cvorovi ne postoje." << endl;
	}

	prvi = 1;
	cout << "Unesite lokacije izmedju kojih je moguce izgraditi put: \n\n";
	while (true)
	{
		if (prvi) prvi = 0;
		else
		{
			cout << "Da li zelite da nastavite sa unosom?\nUnesite (0) za NE / (1) za DA >>> "; cin >> kraj;
			if (!kraj) break;
		}

		do
		{
			cout << "Unesite redne brojeve lokacija koje zelite da povezete: "; cin >> temp >> temp1;
			cout << "\nUnesite duzinu puta: "; cin >> temp2;
			if (temp <= 0 || temp1 < 0)
				cout << "Broj (indeks) lokacije mora biti veci od nule!" << endl;
		} while (temp < 0 || temp1 < 0);
		if (G->DodajGranu_Izgradjen(temp, temp1, 1, temp2))
			cout << "Uspesno dodata grana!" << endl;
		else
			cout << "Dodavanje grane nije bilo uspesno, uneti cvorovi ne postoje." << endl;
	}
}

void UkloniGranu(Graf* G, int n)
{
	if (n <= 0)
	{
		cout << "Graf nije kreiran. Prvo kreirajte graf, da biste vrsili operacije nad njim." << endl;
		return;
	}

	int temp, temp1;
	do
	{
		cout << "Unesite redne brojeve cvorova izmedju kojih zelite da uklonite granu: "; cin >> temp >> temp1;
		if (temp <= 0 || temp1 < 0)
			cout << "Broj cvorova mora biti veci od nule!" << endl;
	} while (temp < 0 || temp1 < 0);


	if (G->UkloniGranu(temp, temp1))
		cout << "Uspesno uklonjena grana!" << endl;
}

void IspisReprezentacije(Graf* G, int n)
{
	if (n <= 0)
	{
		cout << "Graf nije kreiran. Prvo kreirajte graf, da biste vrsili operacije nad njim." << endl;
		return;
	}

	vector<int> rez;
	int temp, temp1;

	cout << "Ukoliko zelite:\nStampanje po sirini\t\t\t Unesite --> 1 \nStampanje po dubini\t\t\t Unesite --> 2 \nStampanje memorijske reprezentacije\t Unesite --> 3\n\nUnos --> ";
	do
	{
		cin >> temp;
		if (temp != 1 && temp != 2 && temp != 3)
			cout << "\nUneta nedefinisana vrednost!\n";
	} while (temp != 1 && temp != 2 && temp != 3);

	if (temp == 1)
	{
		cout << "\nUnesite pocetni cvor: ";
		cin >> temp1;
		rez = G->BFS(temp1);
		for (int i = 0; i < rez.size(); i++)
			cout << rez[i] << " ";
		cout << endl;
	}
	else if (temp == 2)
	{
		cout << "\nUnesite pocetni cvor: ";
		cin >> temp1;
		rez = G->DFS(temp1);
		for (int i = 0; i < rez.size(); i++)
			cout << rez[i] << " ";
		cout << endl;
	}
	else
	{
		G->MemorijskiIspis();
	}


}

void BrisanjeGrafa(Graf* G, int& n)
{
	if (n <= 0)
	{
		cout << "Graf nije kreiran. Prvo kreirajte graf, da biste vrsili operacije nad njim." << endl;
		return;
	}

	G->BrisanjeGrafa();
	cout << "Graf uspesno izbrisan iz memorije!" << endl;
	n = 0;
}

int Prim(Graf* G, Graf& MST)
{
	int v = -1, mini = 0, u = 1, cena = 0;
	bool* obradjen = new bool[G->n];
	Cvor::Grana min, minC;
	bool prvi;

	MST = Graf(G->brV);
	for (int i = 0, br = 0; i < G->n; i++)
	{
		if (G->cvorovi[i].index) MST.cvorovi[br++] = Cvor(G->cvorovi[i].index);
	}

	//odredjivanje pocetnog neizbrisanog cvora --> Usvojena pretpostavka da graf mora biti povezan
	for (int i = 0; i < G->n; i++)
		if (G->cvorovi[i].index)
		{
			v = i;
			break;
		}

	if (v == -1)
		return v;

	for (int i = 0; i < G->n; i++)
		obradjen[i] = false;

	obradjen[v] = 1;
	while (u != G->brV)
	{
		prvi = 1;
		for (int i = 0; i < G->n; i++)
		{
			if (obradjen[i] && (minC = G->cvorovi[i].Min(obradjen)).index_dest)
			{
				if (prvi)
				{
					min = minC;
					mini = i + 1;
					prvi = 0;
				}
				else if (minC.w < min.w)
				{
					min = minC;
					mini = i + 1;
				}
			}

		}
		obradjen[min.index_dest - 1] = 1;
		cena += min.w;
		u++;

		/*if(mini)*/ MST.DodajGranu(mini, min.index_dest, min.w);
	}

	return cena;
}

int** MatricaTezina(Graf* G)
{
	int** w = new int* [G->n];
	for (int i = 0; i < G->n; i++)
	{
		w[i] = new int[G->n];
		w[i] = G->cvorovi[i].VektorTezina(G->n);
		w[i][i] = 0;
	}

	return w;
}

void Dijkstra(Graf* G, int** w, int*& d, int*& t, int v, vector<Put>*& putevi)
{
	int min, min_index, prev, prev2, br;
	bool* obradjen = new bool(G->n);
	bool prvi;

	for (int i = 0; i < G->n; i++)
		obradjen[i] = false;

	obradjen[v - 1] = 1;
	d = new int[G->n]; t = new int[G->n];

	//formiranje d[] i t[]
	for (int i = 0; i < G->n; i++)
	{
		if (i != (v - 1))
		{
			d[i] = w[v - 1][i];
			if (w[v - 1][i] != -1) t[i] = v - 1;
			else t[i] = 0;
		}
	}

	for (int i = 0; i < G->n - 1; i++)
	{
		//racunanje min elementa u d[]
		prvi = 1;
		for (int j = 0; j < G->n; j++)
			if (!obradjen[j])
			{
				if (prvi)
				{
					min = d[j];
					min_index = j;
					prvi = 0;
				}
				else if (d[j] < min)
				{
					min = d[j];
					min_index = j;
				}
			}
		if (min == -1) break;
		obradjen[min_index] = 1;
		for (int j = 0; j < G->n; j++)
		{
			if (w[min_index][j] > 0 && d[min_index] + w[min_index][j] == d[j]) //w[min_index][j] > 0 <==> w[min_index][j] != -1 && w[min_index][j] != 0
			{
				stack<int> s;
				vector<int> put;
				prev = j; br = 0;
				s.push(prev + 1);
				while (prev != v - 1)
				{
					prev2 = prev;
					if (prev == j)
						prev = min_index;
					else
						prev = t[prev];
					s.push(prev + 1);
					br += G->cvorovi[prev].Izgradjen_Put(prev2);
				}

				while (!s.empty())
				{
					put.push_back(s.top());
					s.pop();
				}

				putevi[j].push_back(Put(put, br));
			}

			if (!obradjen[j] && w[min_index][j] != -1)
			{
				if (d[j] == -1)
				{
					d[j] = d[min_index] + w[min_index][j];
					t[j] = min_index;
				}
				else if (d[min_index] + w[min_index][j] < d[j])
				{
					d[j] = d[min_index] + w[min_index][j];
					t[j] = min_index;
				}
			}
		}
	}
}
bool poredjenje(Put i, Put j) { return (i.broj_neizgradjenih < j.broj_neizgradjenih); }
void NajmanjaRuta(Graf* G, int n)
{
	int* d, temp, * t;
	vector<Put>* putevi = new vector<Put>[G->n];
	stack<int> s;
	if (n <= 0)
	{
		cout << "Graf nije kreiran. Prvo kreirajte graf, da biste vrsili operacije nad njim." << endl;
		return;
	}
	cout << "Unesite indeks cvora V: >>> "; cin >> temp;
	PoveziLokacije_Vatrogasci(G, n);
	Dijkstra(G, MatricaTezina(G), d, t, temp, putevi);

	int prev, prev2, br;
	vector<int> put;
	//putevi.reserve(G->n - 1);

	for (int i = 0; i < G->n; i++)
	{
		if (i != temp - 1)
		{
			prev = i; br = 0;
			s.push(prev + 1);
			while (prev != temp - 1)
			{
				prev2 = prev;
				prev = t[prev];
				s.push(prev + 1);
				br += G->cvorovi[prev].Izgradjen_Put(prev2);
			}
			put.clear();
			while (!s.empty())
			{
				put.push_back(s.top());
				s.pop();
			}

			putevi[i].push_back(Put(put, br));
			sort(putevi[i].begin(), putevi[i].end(), poredjenje);
		}
	}

	for (int i = 0; i < G->n; i++)
	{
		if (i != temp - 1)
		{
			for (int j = 0; j < putevi[i].size(); j++)
			{
				putevi[i][j].Stampaj();
			}
		}
	}
}


int main()
{
	int kmd, n = 0;
	Graf G, Metro;
	bool kraj = false;

	while (true)
	{
		cout << "\n\nUnesite: " << endl;
		cout << "\t --> 1 za:\t Unos broja kljucnih lokacija" << endl;
		cout << "\t --> 2 za: \t Racunanje minimalne cene metroa" << endl;
		cout << "\t --> 3 za:\t Pronalazenje najkrace rute za vatrogasna kola" << endl;
		cout << "\t --> 4 za:\t Brisanje lokacija" << endl;
		cout << "\t --> 5 za:\t Izlazak iz programa" << endl << endl << endl;

		cout << "Komanda: ";  cin >> kmd; cout << endl << endl;

		switch (kmd)
		{
		case 1:
			KreirajGraf(&G, n);
			break;
		case 2:
			PoveziLokacije(&G, n, Metro);
			break;
		case 3:
			NajmanjaRuta(&G, n);
			break;
		case 4:
			BrisanjeGrafa(&G, n);
			break;
		case 5:
			kraj = true;
			break;

		default:
			cout << "Nepredvidjenja komanda!" << endl;
			break;
		}

		if (kraj) break;
	}
}