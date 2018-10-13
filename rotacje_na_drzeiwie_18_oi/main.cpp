#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
#include <algorithm>
using namespace std;

class Node
{
  public:
    Node *lewy;
    Node *prawy;
    int liczebnosc_poddrzewa;
    int wartosc;
    int nr_autostrady;
    Node()
    {
        liczebnosc_poddrzewa = -1;
        lewy = 0;
        prawy = 0;
        wartosc = -1;
        nr_autostrady = -1;
    }
};

class Stosowy
{
  public:
    Node *rozwazany;
    bool synowie_zrobieni;
    Stosowy(Node *rozwazany)
    {
        this->rozwazany = rozwazany;
        synowie_zrobieni = false;
    }
};

class Przedzialowy_node
{
  public:
    int na_lewo_mniejsze_od;
    int liczebnosc_poddrzewa;
    Przedzialowy_node()
    {
        na_lewo_mniejsze_od = -1;
        liczebnosc_poddrzewa = 0;
    }
};

class Drzewo_przedzialowe
{
  public:
    vector<int> zbior;
    vector<Przedzialowy_node> tab;
    int potega_2;
    int size;
    //vector<int> potegi_2;
    void dodaj(int a)
    {
        zbior.push_back(a);
    }
    bool czy_lisc(int index)
    {
        if (index < potega_2)
            return false;
        return true;
    }
    void kompiluj()
    {
        sort(zbior.begin(), zbior.end());
        size = zbior.size();
        potega_2 = 1;
        //potegi_2.push_back(potega_2);
        while (potega_2 < zbior.size())
        {
            potega_2 *= 2;
            //potegi_2.push_back(potega_2);
        }
        tab.resize(potega_2 + size);
        for (int i = 0; i < zbior.size(); i++)
            tab[potega_2 + i].na_lewo_mniejsze_od = zbior[i];
        int nieskonczonosc = zbior[zbior.size() - 1] + 1;
        /*
        for (int i = zbior.size(); i < potega_2; i++)
        {
            tab[potega_2 + i].na_lewo_mniejsze_od = nieskonczonosc;
            nieskonczonosc++;
        }
        */
        zbior.resize(0);

        int ile_razy_trzeba_pomnozyc_zeby_dojsc_do_podstawy = potega_2;
        int a = 1;
        int b = 2;
        while (ile_razy_trzeba_pomnozyc_zeby_dojsc_do_podstawy != 1)
        {
            for (int i = a; i < b; i++)
            {
                if (i * ile_razy_trzeba_pomnozyc_zeby_dojsc_do_podstawy + (ile_razy_trzeba_pomnozyc_zeby_dojsc_do_podstawy / 2) < tab.size())
                    tab[i].na_lewo_mniejsze_od = tab[i * ile_razy_trzeba_pomnozyc_zeby_dojsc_do_podstawy + (ile_razy_trzeba_pomnozyc_zeby_dojsc_do_podstawy / 2)].na_lewo_mniejsze_od;
                else{
                    tab[i].na_lewo_mniejsze_od=nieskonczonosc;
                }
            }
            ile_razy_trzeba_pomnozyc_zeby_dojsc_do_podstawy /= 2;
            a *= 2;
            b = (b - 1) * 2 + 1 + 1;
        }
    }
    void wrzuc_do_skompilowanego(int a, int &ile_nad, int &ile_pod)
    {
        //zakladam, ze tego wczesniej nie bylo i ze to jest dobra wartosc
        int chwilowa = 1;
        tab[chwilowa].liczebnosc_poddrzewa++;
        ile_nad = 0;
        ile_pod = 0;
        while (!czy_lisc(chwilowa))
        {
            if (a < tab[chwilowa].na_lewo_mniejsze_od)
            {
                ile_nad += tab[2 * chwilowa + 1].liczebnosc_poddrzewa;
                chwilowa = 2 * chwilowa;
            }
            else
            {
                ile_pod += tab[2 * chwilowa].liczebnosc_poddrzewa;
                chwilowa = 2 * chwilowa + 1;
            }
            tab[chwilowa].liczebnosc_poddrzewa++;
        }
    }
    void zrob_wektor(vector<int> &wektor)
    {
        for (int i = potega_2; i < potega_2 + size; i++)
        {
            wektor.push_back(tab[i].na_lewo_mniejsze_od);
        }
    }
    long long int asymiluj_do_mnie_prawy(Drzewo_przedzialowe &prawy)
    {
        //w kolejnosci rosnacej i zawsze wiekszych szukamy
        long long int ile_inwersji = 0;
        int ile_nad, ile_pod;
        vector<int> od_prawego;
        prawy.zrob_wektor(od_prawego);
        for (int i = 0; i < od_prawego.size(); i++)
        {
            this->wrzuc_do_skompilowanego(od_prawego[i], ile_nad, ile_pod);
            ile_inwersji += (long long int)ile_nad;
        }
        prawy.tab.resize(0);
        return ile_inwersji;
    }
    long long int asymiluj_do_mnie_lewy(Drzewo_przedzialowe &lewy)
    {
        //w kolejnosci rosnacej i zawsze wiekszych szukamy
        long long int ile_inwersji = 0;
        int ile_nad, ile_pod;
        vector<int> od_lewego;
        lewy.zrob_wektor(od_lewego);
        for (int i = od_lewego.size() - 1; i >= 0; i--)
        {
            this->wrzuc_do_skompilowanego(od_lewego[i], ile_nad, ile_pod);
            ile_inwersji += (long long int)ile_pod;
        }
        lewy.tab.resize(0);
        return ile_inwersji;
    }
};

class Drzewo_binarne
{
  public:
    Node *root;
    vector<Node *> kolejnosc_dfs_postorder;
    long long int ile_min_inwersji(vector<Drzewo_przedzialowe> &autostrady)
    {
        //zakladam, ze autostrady juz sa skompilowane.
        long long int odpowiedz = 0;
        int ile_nad, ile_pod;
        for (int i = 0; i < kolejnosc_dfs_postorder.size(); i++)
        {
            Node *rozwazany = kolejnosc_dfs_postorder[i];
            if (rozwazany->wartosc != -1)
            {
                //to znaczy, ze tylko do autostrady cos wrzucamy
                autostrady[rozwazany->nr_autostrady].wrzuc_do_skompilowanego(rozwazany->wartosc, ile_nad, ile_pod);
            }
            else
            {
                //ma jakies dzieci.
                int nr_lewej_autostrady = -1;
                int nr_prawej_autostrady = -1;
                if (rozwazany->prawy != 0)
                    nr_prawej_autostrady = rozwazany->prawy->nr_autostrady;
                if (rozwazany->lewy != 0)
                    nr_lewej_autostrady = rozwazany->lewy->nr_autostrady;
                if (rozwazany->nr_autostrady == nr_lewej_autostrady)
                {
                    if (nr_prawej_autostrady != -1)
                    {
                        //to znaczy, ze do lewego dolaczymy prawy
                        //dla kazdego z prawych szukam mniejszych, wiec w malejacej
                        long long int inwersji_tutaj;
                        inwersji_tutaj = autostrady[rozwazany->nr_autostrady].asymiluj_do_mnie_prawy(autostrady[nr_prawej_autostrady]);
                        long long int wszystkich_par = (long long int)rozwazany->lewy->liczebnosc_poddrzewa * (long long int)rozwazany->prawy->liczebnosc_poddrzewa;
                        odpowiedz += min(inwersji_tutaj, wszystkich_par - inwersji_tutaj);
                    }
                }
                else
                {
                    //to znaczy, ze do prawej dolaczymy lewa
                    if (nr_lewej_autostrady != -1)
                    {
                        long long int inwersji_tutaj;
                        inwersji_tutaj = autostrady[rozwazany->nr_autostrady].asymiluj_do_mnie_lewy(autostrady[nr_lewej_autostrady]);
                        long long int wszystkich_par = (long long int)rozwazany->lewy->liczebnosc_poddrzewa * (long long int)rozwazany->prawy->liczebnosc_poddrzewa;
                        odpowiedz += min(inwersji_tutaj, wszystkich_par - inwersji_tutaj);
                    }
                }
            }
        }
        return odpowiedz;
    }
    void wypisz(Node *start)
    {
        if (start != 0)
            if (start->wartosc != -1)
                cout << start->wartosc;
            else
            {
                cout << "(";
                wypisz(start->lewy);
                cout << "|";
                wypisz(start->prawy);
                cout << ")";
            }
    }
    void wypisz()
    {
        wypisz(root);
    }
    void wypisz_ile_pod(Node *start)
    {
        if (start != 0)
        {
            cout << "(";
            cout << start->liczebnosc_poddrzewa;
            cout << ": ";
            wypisz_ile_pod(start->lewy);
            cout << "|";
            wypisz_ile_pod(start->prawy);
            cout << ")";
        }
    }
    void wypisz_ile_pod()
    {
        wypisz_ile_pod(root);
    }
    void rob_kolejnosc()
    {
        stack<Stosowy> stos;
        stos.push(Stosowy(root));
        while (!stos.empty())
        {
            Stosowy zdjety = stos.top();
            if (!zdjety.synowie_zrobieni)
            {
                //trzeba ich powrzucac
                stos.top().synowie_zrobieni = true;
                if (zdjety.rozwazany->prawy != 0)
                    stos.push(Stosowy(zdjety.rozwazany->prawy));
                if (zdjety.rozwazany->lewy != 0)
                    stos.push(Stosowy(zdjety.rozwazany->lewy));
            }
            else
            {
                //teraz synowie sa powrzucani
                stos.pop();
                kolejnosc_dfs_postorder.push_back(zdjety.rozwazany);
            }
        }
    }
    void dorob_liczebnosci()
    {
        for (int i = 0; i < kolejnosc_dfs_postorder.size(); i++)
        {
            Node *rozwazany = kolejnosc_dfs_postorder[i];
            if (rozwazany->wartosc != -1)
                rozwazany->liczebnosc_poddrzewa = 1;
            else
            {
                //to znaczy, ze jest cos pod nim;
                rozwazany->liczebnosc_poddrzewa = 0;
                if (rozwazany->lewy != 0)
                    rozwazany->liczebnosc_poddrzewa += rozwazany->lewy->liczebnosc_poddrzewa;
                if (rozwazany->prawy != 0)
                    rozwazany->liczebnosc_poddrzewa += rozwazany->prawy->liczebnosc_poddrzewa;
            }
        }
    }
    void pouzupelniaj_zbiory(vector<Drzewo_przedzialowe> &autostrady)
    {
        for (int i = 0; i < kolejnosc_dfs_postorder.size(); i++)
        {
            Node *rozwazany = kolejnosc_dfs_postorder[i];
            if (rozwazany->wartosc != -1)
            {
                // to jest lisc
                autostrady.resize(autostrady.size() + 1);
                rozwazany->nr_autostrady = autostrady.size() - 1;
                autostrady[autostrady.size() - 1].dodaj(rozwazany->wartosc);
            }
            else
            {
                //ma jakies dzieci
                int liczebnosc_lewego = 0;
                int liczebnosc_prawego = 0;
                if (rozwazany->lewy != 0)
                    liczebnosc_lewego = rozwazany->lewy->liczebnosc_poddrzewa;
                if (rozwazany->prawy != 0)
                    liczebnosc_prawego = rozwazany->prawy->liczebnosc_poddrzewa;
                Node *z_niego_kradnij = 0;
                if (liczebnosc_lewego > liczebnosc_prawego)
                {
                    rozwazany->nr_autostrady = rozwazany->lewy->nr_autostrady;
                    z_niego_kradnij = rozwazany->prawy;
                }
                else
                {
                    rozwazany->nr_autostrady = rozwazany->prawy->nr_autostrady;
                    z_niego_kradnij = rozwazany->lewy;
                }
                if (z_niego_kradnij != 0)
                {
                    //no to kradniemy
                    int nr_kradnietej_autostrady = z_niego_kradnij->nr_autostrady;
                    for (int j = 0; j < autostrady[nr_kradnietej_autostrady].zbior.size(); j++)
                    {
                        autostrady[rozwazany->nr_autostrady].dodaj(autostrady[nr_kradnietej_autostrady].zbior[j]);
                    }
                }
            }
        }
    }
};

void wczytywanie(Drzewo_binarne &drzewo)
{
    //ifstream we("dane.txt");
    int n;
    cin >> n;
    drzewo.root = new Node;
    stack<Node *> stos;
    stos.push(drzewo.root);
    int wczytana;
    while (!stos.empty())
    {
        Node *zdjety = stos.top();
        stos.pop();
        cin >> wczytana;
        if (wczytana == 0)
        {
            zdjety->lewy = new Node;
            zdjety->prawy = new Node;
            stos.push(zdjety->prawy);
            stos.push(zdjety->lewy);
        }
        else
        {
            zdjety->wartosc = wczytana;
        }
    }
}

int main()
{
    Drzewo_binarne drzewo;
    wczytywanie(drzewo);
    drzewo.rob_kolejnosc();
    drzewo.dorob_liczebnosci();
    vector<Drzewo_przedzialowe> autostrady;
    drzewo.pouzupelniaj_zbiory(autostrady);
    for (int i = 0; i < autostrady.size(); i++)
    {
        autostrady[i].kompiluj();
    }

    cout << drzewo.ile_min_inwersji(autostrady);

    //cout << endl
    //<< "doszlo do return 0" << endl;
    return 0;
}
