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
void inicializiraj_predpretok();
void potisni(int u, int v);
int najnizji_sosed(int u);
void napolni_graf();
void sprosti_pomnilnik();

// Globalne spremenljivke.

// Množico vozlisc predstavimo z vektorjem.
vector<Vozlisce> vozlisca;

// V vrsto dajemo vozlisca, ki so v presezku. S tem lahko v
// O(1) dostopamo do naslednjega vozlisca, na katerem je potrebno
// opraviti operacijo POTISNI ali POVISAJ.
queue<int> presezki;

// V vektor shranjujemo vozlisca, ki smo jih ze dodali med
// presezke. S tem dosezemo, da se vsako vozlisce v presezkih
// pojavi kvecjemu enkrat.
vector<bool> viden;

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
    
    // Pocistimo pomnilnik, ki smo ga porabili.
    sprosti_pomnilnik();
    
    return 0;
}

//===================================================================
//
//        ALGORITEM
//
//===================================================================

// Glavna funkcija algoritma potisni-povisaj. Najprej
// inicializira predpretok, potem pa izvaja operacije potiska
// in povisanja, kakor je pac potrebno. To pocne, dokler
// obstaja vozlisce s presezkom (to ne moreta biti s in t).
int potisni_povisaj()
{
    inicializiraj_predpretok();

    // Dokler ima katero izmed vozlisc presezek toka,
    // moramo opraviti ali POTISNI ali POVISAJ. Ce opravimo
    // POVISAJ, lahko takoj potem tudi potisnemo na tisto
    // vozlisce. S tem si zmanjsamo stevilo zank.
    while (presezki.size() > 0)
    {
        int u = presezki.front();
        int v = najnizji_sosed(u);

        if (vozlisca[u].h == 1 + vozlisca[v].h)
            potisni(u, v);
        else
        {
            vozlisca[u].h = vozlisca[v].h + 1; // operacija POVISAJ
            potisni(u,v);
        }
    }

    // Vrnemo presezek v vozliscu t. Lahko bi vrnili
    // tudi -presezek v vozliscu s.
    return vozlisca.back().e;
}

// Inicializira predpretok. Visino vozlisca s nastavi na
// |V|, zasici povezave iz s, doda residualne povezave
// in ce sosed ni vozlisce t, ga doda v presezke.
void inicializiraj_predpretok()
{
    vozlisca[0].h = vozlisca.size();

    for (int v = 0; v < vozlisca.size(); v++)
    {
        if (c[0][v] > 0)
        {
            // Zasici povezavo.
            f[0][v] = c[0][v];

            // Nastavi presezek v sosedu.
            vozlisca[v].e = f[0][v];

            // Doda residualno povezavo.
            f[v][0] -= f[0][v];

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
        // Ce je visina soseda manjsa od trenutne najmanjse
        // visine in ce povezava ni zasicena, potem je to
        // kandidat za najnizjega soseda.
        if (vozlisca[v].h < min_visina && c[u][v] - f[u][v] > 0)
        {
            min_visina = vozlisca[v].h;
            sosed = v;
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

    // Ce smo z vozliscem opravili, ga odstranimo iz presezkov.
    if (vozlisca[u].e == 0)
    {
        presezki.pop();
        viden[u] = false;
    }

    // Ce konec povezave ni t ali s in ce vozlisce se ni v
    // presezkih, ga doda v presezke.
    if (v != vozlisca.size()-1 && v != 0 && !viden[v])
    {
        presezki.push(v);
        viden[v] = true;
    }
}

// Iz datoteke prebere podatke o stevilu vozlisc in
// povezavah. V vektor vozlisc doda vozlisca, ki jim
// nastavi visino in presezek na 0. Vsakemu vozliscu
// nato doda sosede in nastavi kapaciteto povezav.
void napolni_graf()
{
    int V;
    scanf("%d\n", &V);

    c = (int**) calloc(V, sizeof(int*));
    f = (int**) calloc(V, sizeof(int*));

    for (int i = 0; i < V; i++)
    {
        vozlisca.push_back(Vozlisce(0, 0));
        viden.push_back(false);
        c[i] = (int*) calloc(V, sizeof(int));
        f[i] = (int*) calloc(V, sizeof(int));
    }

    int u, v, kapaciteta;
    while (scanf("%d %d %d\n", &u, &v, &kapaciteta) != EOF)
    {
        c[u][v] += kapaciteta;
    }
}

// Sprosti pomnilnik, ki smo ga med izvajanjem uporabili.
void sprosti_pomnilnik()
{
    free(c);
    free(f);
}
