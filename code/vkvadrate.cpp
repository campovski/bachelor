#include<iostream>
#include<queue>
#include<cstdio>
#include<cstdlib>
#include<climits>

using namespace std;

struct Vozlisce
{
	int h, e;
	vector<int> sosedi;
	
	Vozlisce(int h, int e)
	{
		this->h = h;
		this->e = e;
	}
};

int potisni_povisaj();
void inicializiraj_predtok();
void potisni(int u, int v);
void povisaj(int u, int h);
int najnizji_sosed(int u);
void napolni_graf();

vector<Vozlisce> vozlisca;
queue<int> presezki; // stack indexov vozlisc v presezku
vector<bool>viden;
int** c;
int** f;

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
    
    // Izvedi algoritem potisni-povisaj in izpisi rezultat.
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
	
	while (presezki.size() > 0)
	{
		//for (int v = 0; v < presezki.size(); v++)
			//printf("%d ", presezki[v]);
		for (int i = 0; i < vozlisca.size(); i++)
			printf("%d ", vozlisca[i].e);
		int u = presezki.front();
		printf("\nu = %d\n", u);
		
		int v = najnizji_sosed(u);
		printf("v = %d\n", v);
		if (vozlisca[v].h < vozlisca[u].h)
			potisni(u, v);
		else povisaj(u, vozlisca[v].h);
	}
	
	return vozlisca.back().e;
}

void inicializiraj_predtok()
{
	vozlisca[0].h = vozlisca.size();
	
	for (int v : vozlisca[0].sosedi)
	{
		printf("%d ", v);
		printf("\n");
		f[0][v] = c[0][v];
		vozlisca[v].e = f[0][v];
		f[v][0] -= f[0][v];
		vozlisca[0].e -= f[0][v];
		vozlisca[v].sosedi.push_back(0);
		
		if (v != vozlisca.size()-1)
		{
		    presezki.push(v);
		    viden[v] = true;
		}
	}
}

int najnizji_sosed(int u)
{
	int sosed;
	int min_visina = INT_MAX;
		
	for (int v : vozlisca[u].sosedi)
	{
		if (vozlisca[v].h < min_visina && c[u][v] - f[u][v] > 0)
		{
			min_visina = vozlisca[v].h;
			sosed = v;
		}
	}
	
	return sosed;
}

void potisni(int u, int v)
{
	printf("potisni(%d, %d)\n", u, v);
	int delta = min(vozlisca[u].e, c[u][v] - f[u][v]);
    
    vozlisca[u].e -= delta;
    vozlisca[v].e += delta;
    f[u][v] += delta;
    f[v][u] -= delta;
    
    vozlisca[v].sosedi.push_back(u);
    
    presezki.pop();
    viden[u] = false;
    if (vozlisca[u].e > 0)
    {
    	presezki.push(u);
    	viden[u] = true;
   	}
    
    if (v != vozlisca.size()-1 && v != 0 && !viden[v])
    {
    	presezki.push(v);
    	viden[v] = true;
    }
}

void povisaj(int u, int h)
{
	printf("povisaj(%d) na %d\n", u, h+1);
	vozlisca[u].h = h + 1;
}

void napolni_graf()
{
	int V;
	scanf("%d\n", &V);
	
	c = (int**) malloc(V*sizeof(int*));
	f = (int**) malloc(V*sizeof(int*));
	
    for (int i = 0; i < V; i++)
    {
        vozlisca.push_back(Vozlisce(0, 0));
        viden.push_back(false);
    	c[i] = (int*) malloc(V*sizeof(int));
    	f[i] = (int*) malloc(V*sizeof(int));    
    }
        
    
    int u, v, kapaciteta;
    while (scanf("%d %d %d\n", &u, &v, &kapaciteta) != EOF)
    {
    	c[u][v] += kapaciteta;
    	vozlisca[u].sosedi.push_back(v);
    }
}






























