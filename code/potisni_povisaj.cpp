// Implementacija algoritma potisni-povisaj v C++,
// ki deluje v O(V^2*E).
// Za vec informacij o algoritmu obiscite
// https://en.wikipedia.org/wiki/Push-povisaj_maximum_flow_algorithm
// povezava na github...

//===================================================================
//
//        KNJIZNICE IN DEFINICIJE
//
//===================================================================

#include<iostream>
#include<queue>
#include<cstdio>
#include<cstdlib>
#include<climits>

using namespace std;

// Vozlisce predstavimo s strukturo. Za vsako vozlisce si zapomnimo
// njegovo visino in presezek toka v njem.
struct Vozlisce
{
	int h, e;
	
	Vozlisce(int h, int e)
	{
		this->h = h;
		this->e = e;
	}
};

// Prototipi funkcij.
int potisni_povisaj();
void inicializiraj_predtok();
void potisni(int u, int v);
void povisaj(int u, int h);
int najnizji_sosed(int u);
void napolni_graf();

// Globalne spremenljivke.

// Graf predstavimo z vektorjem vozlisc. Povezav si ne rabimo
// zapomniti, saj ima vsako vozlisce podatke o svojih sosedih.
vector<Vozlisce> vozlisca;

// V vrsto dajemo vozlisca, ki so v presezku. S tem lahko v
// O(1) dostopamo do naslednjega vozlisca, na katerem je potrebno
// opraviti operacijo POTISNI ali POVISAJ.
queue<int> presezki;

// V vektor shranjujemo vozlisca, ki smo jih ze dodali med
// presezke. S tem dosezemo, da se vsako vozlisce v presezkih
// pojavi kvecjemu enkrat.
vector<bool>viden;

// Matrika povezav grafa. Ce je p[i][j] true, to pomeni, da
// povezava obstaja. S tem se izognemo temu, da bi se v sosedih
// vozlisc zaceli sosedi ponavljati. Prej smo namrec za vsako
// vozlisce imeli vektor sosedov in s tem ponazorili povezave.
// V obeh primerih s tem zmanjsamo casovno zahtevnost iz O(E)
// na O(V).
bool** p;

int** c; // Matrika kapacitet povezav.
int** f; // Matrika toka.

//===================================================================
//
//        MAIN
//
//===================================================================

// Main funkcija, uporabljena za testiranje programa.
int main()
{
    // S standardnega vhoda preberi podatke o vozliscih
    // in povezavah in napolni vektorja vozlisca in povezave.
    napolni_graf();
    
    // Izvedi algoritem POTISNI-POVISAJ in izpisi rezultat.
    cout << "Maksimalni pretok je " << potisni_povisaj() << endl;
}

//===================================================================
//
//        ALGORITEM
//
//===================================================================

// Glavna funkcija algoritma potisni-povisaj. Najprej
// inicializira predtok, potem pa izvaja operacije potiska
// in povisanja, kakor je pac potrebno. To pocne, dokler
// obstaja vozlisce s presezkom (to ne moreta biti s in t).
int potisni_povisaj()
{
	inicializiraj_predtok();
	
	// Dokler ima katero izmed vozlisc presezek toka,
	// moramo opraviti ali POTISNI ali POVISAJ. Ce opravimo
	// POVISAJ, lahko takoj potem tudi potisnemo na tisto
	// vozlisce. S tem si zmanjsamo stevilo zank.
	while (presezki.size() > 0)
	{
		int u = presezki.front();
		int v = najnizji_sosed(u);
		
		if (vozlisca[v].h < vozlisca[u].h)
			potisni(u, v);
		else
		{
			povisaj(u, vozlisca[v].h);
			potisni(u,v);
		}
	}
	
	// Da preverimo, ali se oddani tok ujema s prejetim, da ga ni
	// slucajno ostalo kaj vmes. (Za nekaksno lahko preverjanje.)
	cout << "e(s) = " << vozlisca[0].e << endl;
	
	// Vrnemo presezek v vozliscu t. Lahko bi vrnili
	// tudi -presezek v vozliscu s.
	return vozlisca.back().e;
}


// Inicializira predtok. Visino vozlisca s nastavi na
// |V|, zasici povezave iz s, doda residualne povezave
// in ce sosed ni vozlisce t, ga doda v presezke.
void inicializiraj_predtok()
{
	vozlisca[0].h = vozlisca.size();
	
	for (int v = 0; v < vozlisca.size(); v++)
	{
		if (p[0][v])
		{
			// Zasici povezavo.
			f[0][v] = c[0][v];
		
			// Nastavi presezek v sosedu.
			vozlisca[v].e = f[0][v];
		
			// Doda residualno povezavo.
			f[v][0] -= f[0][v];
			p[v][0] = true;
		
			// V vozliscu s posodobi oddani tok.
			vozlisca[0].e -= f[0][v];
		
			// Ce vozlisce ni t, ga doda v presezke.
			if (v != vozlisca.size()-1)
			{
				presezki.push(v);
				viden[v] = true;
			}
		}
	}
}


// Vrne indeks najnizjega soseda.
int najnizji_sosed(int u)
{
	int sosed;
	int min_visina = INT_MAX;
		
	for (int v = 0; v < vozlisca.size(); v++)
	{
		if (p[u][v])
		{
			// Ce je visina soseda manjsa od trenutne najmanjse
			// visine in ce povezava ni zasicena, potem je to
			// kandidat za najnizjega soseda.
			if (vozlisca[v].h < min_visina && c[u][v] - f[u][v] > 0)
			{
				min_visina = vozlisca[v].h;
				sosed = v;
			}
		}
	}
	
	return sosed;
}

// Operacija POTISNI. Potisne lahko minimum med presezkom
// v vozliscu in residualno kapaciteto povezave.
void potisni(int u, int v)
{
	int delta = min(vozlisca[u].e, c[u][v] - f[u][v]);
    
    // Posodobi presezek v krajiscih povezave.
    vozlisca[u].e -= delta;
    vozlisca[v].e += delta;
    
    // Posodobi tok prek povezave in residualne povezave.
    f[u][v] += delta;
    f[v][u] -= delta;
    
    // Doda obratno povezavo.
    p[v][u] = true;
    
    // Ce smo z vozliscem opravili, ga odstranimo iz presezkov.
    if (vozlisca[u].e == 0)
    {
		presezki.pop();
		viden[u] = false;
	}
    
    // Ce konec povezave ni t, ga doda v presezke.
    if (v != vozlisca.size()-1 && v != 0 && !viden[v])
    {
    	presezki.push(v);
    	viden[v] = true;
    }
}

// Operacija POVISAJ. Vozliscu nastavi visino na prej
// izracunano minimalno visino "dobrih" sosedov h + 1.
void povisaj(int u, int h)
{
	vozlisca[u].h = h + 1;
}


// Iz datoteke prebere podatke o stevilu vozlisc in
// povezavah. V vektor vozlisc doda vozlisca, ki jim
// nastavi visino in presezek na 0. Vsakemu vozliscu
// nato doda sosede in nastavi kapaciteto povezav.
void napolni_graf()
{
	int V;
	scanf("%d\n", &V);
	
	c = (int**) malloc(V*sizeof(int*));
	f = (int**) malloc(V*sizeof(int*));
	p = (bool**) malloc(V*sizeof(int*));
	
    for (int i = 0; i < V; i++)
    {
        vozlisca.push_back(Vozlisce(0, 0));
        viden.push_back(false);
    	c[i] = (int*) malloc(V*sizeof(int));
    	f[i] = (int*) malloc(V*sizeof(int)); 
    	p[i] = (bool*) malloc(V*sizeof(bool));   
    }
        
    
    int u, v, kapaciteta;
    while (scanf("%d %d %d\n", &u, &v, &kapaciteta) != EOF)
    {
    	c[u][v] += kapaciteta;
    	p[u][v] = 1;
    }
}

