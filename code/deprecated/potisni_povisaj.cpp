// Implementacija algoritma potisni-povisaj v C++.
// Za vec informacij o algoritmu obiscite
// https://en.wikipedia.org/wiki/Push-povisaj_maximum_flow_algorithm
// povezava na github...

//===================================================================
//
//        KNJIZNICE IN DEFINICIJE
//
//===================================================================

#include<iostream>
#include<vector>
#include<climits>
#include<cstdio>

using namespace std;

// Vozlisce in povezavo predstavimo s strukturo.
struct Vozlisce
{
    int h, e;
    
    Vozlisce(int h, int e)
    {
        this->h = h;
        this->e = e;
    }
};

struct Povezava
{
    int u, v;
    int c;
    int f;
    
    Povezava(int u, int v, int c, int f)
    {
        this->u = u;
        this->v = v;
        this->c = c;
        this->f = f;
    }
};

// Prototipi funkcij.
int potisni_povisaj();
void inicializiraj_predtok();
int potisni(int u);
void povisaj(int u);
int vozlisce_s_presezkom();
void posodobi_obratno_povezavo(int i, int delta);
void napolni_graf();

// Globalne spremenljivke.
int V; // Stevilo vozlisc. Sicer velja V = vozlisca.size().
vector<Vozlisce> vozlisca;
vector<Povezava> povezave;

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
    
    while (vozlisce_s_presezkom() != -1)
    {
        int u = vozlisce_s_presezkom();
        
        // Ce ne mores potisniti, potem povecaj visino vozlisca.
        if (!potisni(u))
            povisaj(u);
    }
    
    // Vrni presezek v zadnjem vozliscu, to je ravno t.
    return vozlisca[V-1].e;
}

// Inicializira predtok. To pomeni, da nastavi visino vozlisca s
// in potisne tok iz s v vsa sosednja vozlisca. Pri tem zasici
// povezave.
void inicializiraj_predtok()
{
    // Visina vozlisca s je enaka stevilu vozlisc.
    vozlisca[0].h = V;
    
    for (int i = 0; i < povezave.size(); i++)
    {
        // Ce se povezava zacne v vozliscu s...
        if (povezave[i].u == 0)
        {
            // Zasici povezavo.
            povezave[i].f = povezave[i].c;
            
            // Sosednje vozlisce dobi presezek toka.
            vozlisca[povezave[i].v].e = povezave[i].f;
            
            // Posodobimo obratno povezavo. Ce je ni,
            // se doda nova v residualnem grafu.
            posodobi_obratno_povezavo(i, povezave[i].f);
        }
    }
}

// Operacija potisni. Najprej preverimo, ali se povezava zacne
// v vozliscu u, ce je na povezavi se kaj residualne kapacitete,
// torej ali je povezava se nezasicena. Ce je vozlisce u na
// vecji visini kot sosed, potem izvrsi potisk.
int potisni(int u)
{
    for (int i = 0; i < povezave.size(); i++)
    {
        // Prepricaj se, da pogoji drzijo.
        if (povezave[i].u == u && povezave[i].f < povezave[i].c &&
                vozlisca[u].h > vozlisca[povezave[i].v].h)
        {
            // Potisnemo lahko manjse izmed presezka vozlisca u
            // ter residualne kapacitete povezave.
            int delta =
                min(vozlisca[u].e, povezave[i].c - povezave[i].f);
            
            // Presezek u je pomanjsan za toliko,
            // kolikor smo potisnili.
            vozlisca[u].e -= delta;
        
            // V sosedu se za ravno toliko poveca presezek.
            vozlisca[povezave[i].v].e += delta;
        
            // Tok cez povezavo se poveca.
            povezave[i].f += delta;
        
            // Posodobimo tok po obratni povezavi. Ce povezave
            // ni, jo dodamo v residualnem grafu.
            posodobi_obratno_povezavo(i, delta);
        
            // Potisk uspesen.
            return 1;
        }
    }
    
    // Nismo uspeli najti povezavi, po kateri bi lahko izvrsili
    // potisk. To pomeni, da bo vozliscu potrebno povecati visino.
    return 0;
}

// Operacija povisaj. Nastavi visino vozlisca u na minimum visin
// sosednjih vozlisc + 1.
void povisaj(int u)
{
    // Zacetna minimalna visina naj bo nekaj velikega.
    int min_visina_sosedov = INT_MAX;
    
    for (int i = 0; i < povezave.size(); i++)
    {
        // Prepricati se moramo, da se povezava zacne v u
        // in da ni nasicena. Ce velja se, da je visina
        // drugega konca povezave (torej soseda od u) manjsa
        // kot trenutna najmanjsa, posodobimo trenutno najmanjso.
        if (povezave[i].u == u && povezave[i].f < povezave[i].c &&
                vozlisca[povezave[i].v].h < min_visina_sosedov)
            min_visina_sosedov = vozlisca[povezave[i].v].h;
    }
    
    // Spremenimo visino vozlisca u na 1 + minimalna visina sosedov.
    vozlisca[u].h = min_visina_sosedov + 1;
}

//===================================================================
//
//        POMOZNE FUNKCIJE
//
//===================================================================

// Poisce vozlisce s presezkom in vrne njegov indeks.
// Ce vozlisca ne najde, vrne sentinel.
int vozlisce_s_presezkom()
{
    for (int u = 1; u < V-1; u++)
    {
        if (vozlisca[u].e > 0)
            return u;
    }
    
    return -1;
}

// Posodobi tok na obratni povezavi (povezavi, ki ima
// konca v istih vozliscih, kakor tista, po kateri smo
// potisnili tok, in kaze v drugo smer). Ce te povezave
// ni, jo dodaj v residualnem grafu.
void posodobi_obratno_povezavo(int i, int delta)
{
    for (int j = 0; j < povezave.size(); j++)
    {
        // Prepricamo se, da gre za obratno povezavo.
        if (povezave[i].u == povezave[j].v &&
                povezave[i].v == povezave[j].u)
        {
            // Odstej tok, ki je bil potisnjen in koncaj.
            povezave[j].f -= delta;
            return;
        }
    }
    
    // Ni bilo obratne povezave. To pomeni, da jo moramo
    // dodati v residualnem grafu.
    povezave.push_back(
        Povezava(povezave[i].v, povezave[i].u, 0, -delta));
}


// S standardnega vhoda prebere podatke o grafu
// in napolni vektorja vozlisc in povezav.
void napolni_graf()
{
    scanf("%d\n", &V);
    for (int i = 0; i < V; i++)
        vozlisca.push_back(Vozlisce(0, 0));
    
    int u, v, c;
    while (scanf("%d %d %d\n", &u, &v, &c) != EOF)
        povezave.push_back(Povezava(u, v, c, 0));
}

