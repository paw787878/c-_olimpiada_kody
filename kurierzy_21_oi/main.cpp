#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <set>
using namespace std;

class Zapytanie
{
  public:
    int poczatek;
    int koniec;

    Zapytanie()
    {
    }
};

float load_factor=100.;//default 1.

void wczytywanie(vector<Zapytanie> &zapytania, vector<int> &zlecenia_firmom, bool czy_zmieniona, int &m)
{
    //ifstream we("dane.txt");
    int n;
    cin >> n >> m;
    if (!czy_zmieniona)
        zapytania.resize(m);
    zlecenia_firmom.resize(n);
    for (int i = 0; i < n; i++)
    {
        cin >> zlecenia_firmom[i];
        zlecenia_firmom[i]--;
    }
    if (!czy_zmieniona)
    {
        for (int i = 0; i < m; i++)
        {
            cin >> zapytania[i].poczatek >> zapytania[i].koniec;
            zapytania[i].poczatek--;
            zapytania[i].koniec--;
        }
    }
}

class Node
{
  public:
    int dominujaca_firma;
    unordered_map<int, int> liczebnosc_firmy;
    int dlugosc_przedzialu;
    Node()
    {
        dlugosc_przedzialu = 1;
        dominujaca_firma = -1;
        liczebnosc_firmy.insert(pair<int, int>(-1, 1));
        liczebnosc_firmy.max_load_factor(load_factor);
    }
    void inicjoj(int firma)
    {
        dominujaca_firma = firma;
        liczebnosc_firmy.insert(pair<int, int>(firma, 1));
        liczebnosc_firmy[-1]--;
    }
    void wrzuc_do_mnie(Node &lewy)
    {
        for (unordered_map<int, int>::iterator iteratorek = lewy.liczebnosc_firmy.begin();
             iteratorek != lewy.liczebnosc_firmy.end(); ++iteratorek)
        {
            int kto = iteratorek->first;
            int ile = iteratorek->second;
            unordered_map<int, int>::iterator od_calosci;
            od_calosci = liczebnosc_firmy.find(kto);
            if (od_calosci == liczebnosc_firmy.end())
                liczebnosc_firmy.insert(pair<int, int>(kto, ile));
            else
                liczebnosc_firmy[kto] += ile;
        }
    }
    int ile_tej_firmy(int firma)
    {
        unordered_map<int, int>::iterator znaleizony;
        znaleizony = liczebnosc_firmy.find(firma);
        if (znaleizony == liczebnosc_firmy.end())
            return 0;
        return znaleizony->second;
    }
    void policz_na_podstawie_synow(Node &lewy, Node &prawy)
    {
        dlugosc_przedzialu = 2 * lewy.dlugosc_przedzialu;
        liczebnosc_firmy.clear();
        wrzuc_do_mnie(lewy);
        wrzuc_do_mnie(prawy);
        set<int> podejrzani;
        if (lewy.dominujaca_firma != -1)
            podejrzani.insert(lewy.dominujaca_firma);
        if (prawy.dominujaca_firma != -1)
            podejrzani.insert(prawy.dominujaca_firma);
        int rekordowa_liczebnosc = 0;
        for (set<int>::iterator i = podejrzani.begin(); i != podejrzani.end(); ++i)
        {
            int jego_liczebnosc = 0;
            jego_liczebnosc += lewy.ile_tej_firmy(*i);
            jego_liczebnosc += prawy.ile_tej_firmy(*i);
            if (jego_liczebnosc > rekordowa_liczebnosc)
            {
                dominujaca_firma = *i;
                rekordowa_liczebnosc = jego_liczebnosc;
            }
        }
        if (!(rekordowa_liczebnosc > dlugosc_przedzialu / 2))
            dominujaca_firma = -1;
    }

    void wypisz(string tekst)
    {
        cout << endl
             << tekst << endl;
        cout << "dominujacy firma to " << dominujaca_firma + 1 << endl;
        cout << "dlugosc przedzialu to " << dlugosc_przedzialu << endl;
        cout << "teraz liczebnosci firm" << endl
             << endl;
        for (unordered_map<int, int>::iterator i = liczebnosc_firmy.begin(); i != liczebnosc_firmy.end(); ++i)
        {
            cout << "firma " << i->first + 1 << " wystepuje " << i->second << endl;
        }
        cout << endl
             << "koniec " << tekst;
        cout << endl;
    }
};

class Drzewo
{
  public:
    vector<Node> tab;
    int rozmiar_pot_2;
    Drzewo(vector<int> &zlecenia_firmom)
    {
        rozmiar_pot_2 = 1;
        while (rozmiar_pot_2 < zlecenia_firmom.size())
            rozmiar_pot_2 *= 2;
        tab.resize(2 * rozmiar_pot_2);
        for (int i = 0; i < zlecenia_firmom.size(); i++)
            tab[rozmiar_pot_2 + i].inicjoj(zlecenia_firmom[i]);
        zlecenia_firmom.resize(0);
        for (int i = rozmiar_pot_2 - 1; i > 0; i--)
        {
            tab[i].policz_na_podstawie_synow(tab[2 * i], tab[2 * i + 1]);
        }
    }
    void rozkladaj(vector<int> &podzial, int a, int b)
    {
        a += rozmiar_pot_2;
        b += rozmiar_pot_2;
        podzial.push_back(a);
        if (a != b)
            podzial.push_back(b);
        while (a / 2 != b / 2)
        {
            if (a % 2 == 0)
                podzial.push_back(a + 1);
            if (b % 2 == 1)
                podzial.push_back(b - 1);
            a /= 2;
            b /= 2;
        }
    }
    int podaj_dominujacego(int a, int b)
    {
        //cout<<endl<<endl;
        //cout<<"robie dla a="<<a<<"b="<<b<<endl;
        vector<int> podzial;
        rozkladaj(podzial, a, b);
        //teraz trzeba pozbierac mozliwe typy
        set<int> propozycje;
        for (int i = 0; i < podzial.size(); i++)
        {
            int rozwazana_firma = tab[podzial[i]].dominujaca_firma;
            if (rozwazana_firma != -1)
                propozycje.insert(rozwazana_firma);
        }
        int dominujaca_firma = -1;
        int rekordowa_liczebnosc = 0;
        for (set<int>::iterator i = propozycje.begin(); i != propozycje.end(); ++i)
        {
            int rozwazana_liczebnosc = 0;
            int rozwazana_firma = *i;
            for (int j = 0; j < podzial.size(); j++)
            {
                rozwazana_liczebnosc += tab[podzial[j]].ile_tej_firmy(rozwazana_firma);
            }
            if (rozwazana_liczebnosc > rekordowa_liczebnosc)
            {
                dominujaca_firma = rozwazana_firma;
                rekordowa_liczebnosc = rozwazana_liczebnosc;
            }
        }
        //cout<<endl<<"wyszlo mi najliczniejsza_firma "<<dominujaca_firma+1;
        //cout<<" wystepujaca "<<rekordowa_liczebnosc<<"razy "<<endl;
        if (dominujaca_firma == -1)
            return -1;
        if (!(rekordowa_liczebnosc > (b - a + 1) / 2))
            return -1;
        return dominujaca_firma;
    }
    void wypisz()
    {
        cout << endl
             << "wypisuje drzewo " << endl;
        for (int i = 1; i < tab.size(); i++)
        {
            cout << "node " << i << endl;
            tab[i].wypisz("node kurde");
        }
    }
};

int main()
{
    vector<int> zlecenia_firmom;
    vector<Zapytanie> zapytania;
    int m;
    wczytywanie(zapytania, zlecenia_firmom, true, m);
    Drzewo drzewo(zlecenia_firmom);

    /*
    vector<int> podzial;
    drzewo.rozkladaj(podzial,0,2);
    for(int i=0;i<podzial.size();i++)
        cout<<podzial[i]<<" ";

    
    cout<<endl<<endl;
    cout<<zlecenia_firmom.size();
    cout<<endl<<endl;
    for(int i=0;i<zlecenia_firmom.size();i++){
        cout<<zlecenia_firmom[i]+1<<" ";
    }
    cout<<endl<<endl;
    //drzewo.wypisz();
    cout<<endl<<endl;
    */
    int a, b;
    for (int i = 0; i < m; i++)
    {
        cin >> a >> b;
        a--;
        b--;
        cout << drzewo.podaj_dominujacego(a, b) + 1;
        if (i != zapytania.size() - 1)
            cout << endl;
    }
    //cout << endl
    //<< "doszlo do return 0" << endl;
    return 0;
}
