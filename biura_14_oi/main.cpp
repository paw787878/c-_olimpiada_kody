#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <time.h>
using namespace std;

int nastepna_potega_2(int a)
{
    int potega = 1;
    while (potega < a)
        potega *= 2;
    return potega;
}

void rob_potegi_2(vector<int> &potegi, int roz)
{
    int pot = 1;
    while (pot <= 2 * roz)
    {
        potegi.push_back(pot);
        pot *= 2;
    }
}

void rob_glebokosci(vector<int> &glebokosci, int roz)
{
    int log = -1;
    int potega = 1;
    for (int i = 0; i < 2 * roz; i++)
    {
        if (i >= potega)
        {
            potega *= 2;
            log++;
        }
        glebokosci.push_back(log);
    }
}

void wypisz_wektor(vector<int> &wek, string wiadomosc)
{
    cout << endl
         << wiadomosc << endl;
    for (int i = 0; i < wek.size(); i++)
    {
        cout << i << " " << wek[i] << endl;
    }
    cout << endl;
}

void wypisz_pare(pair<int, int> p, string wiadmosc)
{
    cout << endl
         << wiadmosc << endl;
    cout << "pierwszy " << p.first << " drugi " << p.second;
    cout << endl;
}

class Drzewo_odwiedzenia
{
  public:
    vector<int> tab; //0 znaczy, ze nic z tego odcinka nie bylo brane, 1 to znaczy ze zaczeli, 2 to znaczy, ze zaczeli i skonczyli.
    vector<int> potegi_2;
    vector<int> glebokosci; //daje 0 korzeniowi 1 synom jego i tak dalej.
    pair<int, int> jaki_to_przedzial(int node);
    void zaznacz(int node, pair<int, int> przedzial, queue<int> &kolejka, bool &skonczylismy_ten_przedzial);
    int maksymalna_glebokosc;
    int roz;
    Drzewo_odwiedzenia(int wielkosc)
    {
        roz = nastepna_potega_2(wielkosc);
        tab.resize(2 * roz, 0);
        rob_potegi_2(potegi_2, roz);
        rob_glebokosci(glebokosci, roz);
        maksymalna_glebokosc = glebokosci[glebokosci.size() - 1];
    }
};

void Drzewo_odwiedzenia::zaznacz(int node, pair<int, int> przedzial, queue<int> &kolejka, bool &skonczylismy_ten_przedzial)
{

    //sprawdzmy czy przedzial to nie jest przypadkiem cale to
    //cout << "robie teraz zaznaczanie dla nodu " << node;
    //cout << "dla przedzialu " << przedzial.first << " " << przedzial.second << endl;
    if (tab[node] == 2)
    {
        skonczylismy_ten_przedzial = true;
    }
    else
    {
        skonczylismy_ten_przedzial = false;

        pair<int, int> ten = jaki_to_przedzial(node);
        if (ten.first == przedzial.first && ten.second == przedzial.second)
        {
            //sa rozne mozliwosci
            if (tab[node] == 0)
            {
                //to znaczy, ze go po prostu dodajemy normalnie.
                kolejka.push(node);
                tab[node] = 2;
            }
            else if (tab[node] == 2)
            {
                //to znaczy, ze jest juz obrobiony i skonczony. nic nie trzeba tutaj robic z tym.
            }
            else
            {
                //to znaczy, ze wynosi 1. jest to najtrudniejszy przypdaek.
                tab[node] = 2;
                int syn1, syn2;
                syn1 = 2 * node;
                syn2 = 2 * node + 1;
                bool whatever;
                zaznacz(syn1, jaki_to_przedzial(syn1), kolejka, whatever);
                zaznacz(syn2, jaki_to_przedzial(syn2), kolejka, whatever);

                //w koncu dokopiesz sie do takiego co ma 2 lub 0, bo lisc nie moze byc inny
            }
            skonczylismy_ten_przedzial = true;
        }
        else
        {
            //schodzimy gdzies nizej na pewno. albo tam dodamy cos nowego, albo tam jest cos dodane.
            //tak czy owak jak skonczymy to bedzie mialo 1 ten node,chyba, ze skonczymy obydwa nasze te
            tab[node] = 1;
            //teraz tak.
            pair<int, int> lewego = jaki_to_przedzial(2 * node);
            pair<int, int> prawego = jaki_to_przedzial(2 * node + 1);
            vector<bool> czy_dzieci_skonczylismy;
            if (przedzial.first <= lewego.second)
            {
                pair<int, int> nowy_przedzial(przedzial.first, min(lewego.second, przedzial.second));
                bool wkladany = false;
                zaznacz(2 * node, nowy_przedzial, kolejka, wkladany);
                czy_dzieci_skonczylismy.push_back(wkladany);
            }
            else
            {
                //lewego nie robimy, ale dalej mozemy znac jego czy skonczylismy
                czy_dzieci_skonczylismy.push_back(tab[2 * node] == 2);
            }
            if (przedzial.second >= prawego.first)
            {
                pair<int, int> nowy_przedzial(max(przedzial.first, prawego.first), przedzial.second);
                bool wkladany = false;
                zaznacz(2 * node + 1, nowy_przedzial, kolejka, wkladany);
                czy_dzieci_skonczylismy.push_back(wkladany);
            }
            else
            {
                czy_dzieci_skonczylismy.push_back(tab[2 * node + 1] == 2);
            }
            skonczylismy_ten_przedzial = czy_dzieci_skonczylismy[0] && czy_dzieci_skonczylismy[1];
            if (skonczylismy_ten_przedzial)
            {
                tab[node] = 2;
            }
        }
    }
}

pair<int, int> Drzewo_odwiedzenia::jaki_to_przedzial(int node)
{
    int wysokosc = maksymalna_glebokosc - glebokosci[node];
    int poczatek = potegi_2[wysokosc] * node;
    int koniec = poczatek + potegi_2[wysokosc] - 1;
    return pair<int, int>(poczatek - roz, koniec - roz);
}

template <class T>
void mergesort(vector<T> &tab, int a, int b,
               bool (*mniejsze)(T, T))
{
    //a oraz b to sa jak w pythonie
    if (!(b - a == 0 || b - a == 1))
    {
        int s = a + (b - a) / 2;
        mergesort(tab, a, s, mniejsze);
        mergesort(tab, s, b, mniejsze);
        vector<T> pier;
        vector<T> drug;
        for (int i = a; i < s; i++)
            pier.push_back(tab[i]);
        for (int i = s; i < b; i++)
            drug.push_back(tab[i]);

        int ipier = 0;
        int idrug = 0;
        for (int i = a; i < b; i++)
        {
            if (ipier == pier.size())
            {
                tab[i] = drug[idrug];
                idrug++;
            }
            else if (idrug == drug.size())
            {
                tab[i] = pier[ipier];
                ipier++;
            }
            else if (mniejsze(pier[ipier], drug[idrug]))
            {
                tab[i] = pier[ipier];
                ipier++;
            }
            else
            {
                tab[i] = drug[idrug];
                idrug++;
            }
        }
    }
}

bool mniejszy(int a, int b)
{
    return a < b;
}

void wczytywanie(vector<vector<int>> &graf)
{
    ifstream we("in/biu8.in");
    int n, m;
    we >> n >> m;
    graf.resize(n);
    for (int i = 0; i < m; i++)
    {
        int a, b;
        we >> a >> b;
        a--;
        b--;
        graf[a].push_back(b);
        graf[b].push_back(a);
    }
    for (int i = 0; i < n; i++)
    {
        mergesort<int>(graf[i], 0, graf[i].size(), &mniejszy);
    }
}

void rob_odcinki_graniczacych_wierzcholkow(vector<int> &graniczace, int n, vector<pair<int, int>> &pary_graniczacych)
{
    //n to liczba wszystkich wierzcholkow
    //juz posortowane sa te graniczace
    int odkad_nastepny = 0;
    for (int i = 0; i < graniczace.size(); i++)
    {
        int koniec = graniczace[i] - 1;
        if (koniec >= odkad_nastepny)
        {
            pary_graniczacych.push_back(pair<int, int>(odkad_nastepny, koniec));
        }
        odkad_nastepny = graniczace[i] + 1;
    }
    //teraz jeszcze ten ostatni mozna sprobowac
    int koniec = n - 1;
    if (koniec >= odkad_nastepny)
    {
        pary_graniczacych.push_back(pair<int, int>(odkad_nastepny, koniec));
    }
}

void produkuj_odpowiedz(vector<vector<int>> &graf, vector<int> &liczebnosci_budynkow)
{
    int n = graf.size();
    Drzewo_odwiedzenia dotkniete(n);
    //wypisz_wektor(dotkniete.tab,"tab dotknietych na poczatku");
    queue<int> kolejka;
    vector<bool> skonczone;
    skonczone.resize(n, false);
    for (int i = 0; i < n; i++)
        if (skonczone[i] == false)
        {
            //kolejka.push(i + dotkniete.roz);
            bool cokolwiek;
            //cout<<"tu bylem"<<endl;
            //wypisz_pare(pair<int,int>(i+dotkniete.roz,i+dotkniete.roz),"kurde");
            dotkniete.zaznacz(1, pair<int, int>(i, i), kolejka, cokolwiek);
            //cout<<"a tu nie"<<endl;
            int ile_tego_koloru = 0;
            while (!kolejka.empty())
            {
                //sciagam cos z kolejki
                int sciagany = kolejka.front();
                //cout<<"sciagnalem "<<sciagany<<endl;
                kolejka.pop();
                pair<int, int> przedzial_sciaganego = dotkniete.jaki_to_przedzial(sciagany);
                for (int j = przedzial_sciaganego.first; j <= przedzial_sciaganego.second; j++)
                {
                    ile_tego_koloru++;
                    skonczone[j] = true;
                    vector<pair<int, int>> przedzialy_dotykanych;
                    rob_odcinki_graniczacych_wierzcholkow(graf[j], n, przedzialy_dotykanych);
                    bool whatever;
                    for (int k = 0; k < przedzialy_dotykanych.size(); k++)
                    {
                        dotkniete.zaznacz(1, przedzialy_dotykanych[k], kolejka, whatever);
                    }
                }
            }
            liczebnosci_budynkow.push_back(ile_tego_koloru);
        }
}

class Naiwna
{
  public:
    vector<bool> tab;
    Naiwna(int n)
    {
        tab.resize(n, false);
    }
    int zaznacz(pair<int, int> przedzial)
    {
        int wyrzut = 0;
        for (int i = przedzial.first; i <= przedzial.second; i++)
        {
            if (tab[i] == false)
            {
                tab[i] = true;
                wyrzut++;
            }
        }
        return wyrzut;
    }
};

int main()
{
    
    vector<vector<int>> graf;
    wczytywanie(graf);
    vector<int> odpowiedzi;
    produkuj_odpowiedz(graf, odpowiedzi);
    mergesort<int>(odpowiedzi, 0, odpowiedzi.size(), &mniejszy);
    //wypisz_wektor(odpowiedzi, "odpowiedzi");
    cout << odpowiedzi.size() << endl;
    for (int i = 0; i < odpowiedzi.size(); i++)
    {
        cout << odpowiedzi[i] << " ";
    }
    
   /*
    srand(time(NULL));
    int roz = 16;
    Naiwna naiwna(roz);
    Drzewo_odwiedzenia drzewo(roz);
    
    for (int i = 0; i < 100; i++)
    {
        int a=rand()%roz;
        int b=rand()%roz;
        if(b>=a){
            cout<<"wkladam pare "<<a<<" "<<b<<" otrzymujac zgodnosc ";
            int od_naiwnej=naiwna.zaznacz(pair<int,int>(a,b));
            queue<int> kolejka;
            bool whatever;
            drzewo.zaznacz(1,pair<int,int>(a,b),kolejka,whatever);
            int od_drzewa=0;
            while(! kolejka.empty()){
                pair<int,int> pom=drzewo.jaki_to_przedzial(kolejka.front());
                od_drzewa+=pom.second-pom.first+1;
                kolejka.pop();
            }
            cout<<(od_drzewa==od_naiwnej)<<endl;
        }
    }
    */
   /*
    wypisz_wektor(drzewo.tab, " przed ");
    queue<int> kolejka;
    bool cokolwiek;
    drzewo.zaznacz(1, pair<int, int>(9, 15), kolejka, cokolwiek);
    wypisz_wektor(drzewo.tab, " po 9,15 ");
    cout << "te nody spadly do kolejki ";
    while (!kolejka.empty())
    {
        cout << kolejka.front() << " ";
        kolejka.pop();
    }
    drzewo.zaznacz(1, pair<int, int>(11, 12), kolejka, cokolwiek);
    wypisz_wektor(drzewo.tab, " po 11,12 ");
    cout << "te nody spadly do kolejki ";
    while (!kolejka.empty())
    {
        cout << kolejka.front() << " ";
        kolejka.pop();
    }
    */

    /*
    for (int i = 0; i < graf.size(); i++)
    {
        cout << "wypisuje dla i=" << i << endl
             << endl;
        wypisz_wektor(graf[i], "wierzchole");
    }

    vector<pair<int,int> > wektor_par;
    rob_odcinki_graniczacych_wierzcholkow(graf[0],graf.size(),wektor_par);
    for(int i=0;i<wektor_par.size();i++){
        wypisz_pare( wektor_par[i],"cokolwiek");
    }
    */
    /*
    proba.zaznacz(1,pair<int,int>(3,4),kolejka,whatever);
    //proba.zaznacz(1,pair<int,int>(4,7),kolejka,whatever);

    
    while(!kolejka.empty()){
        cout<<kolejka.front()<<" ";
        kolejka.pop();
    }
    //proba.zaznacz(1,pair<int,int>(1,1),kolejka,whatever);

    //proba.zaznacz(1,pair<int,int>(0,7),kolejka,whatever);


    wypisz_wektor(proba.tab, "tablica");
    //wypisz_wektor(proba.potegi_2, "potegi 2");
    //wypisz_wektor(proba.glebokosci, " glebokosci");
    //wypisz_pare(proba.jaki_to_przedzial(9), "przedzial ");
    */
    //cout << endl
      //   << "doszlo do return 0" << endl;
    return 0;
}
