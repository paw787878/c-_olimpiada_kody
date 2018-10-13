#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

void wczytaj(vector<int> &surowe)
{
    //ifstream we("dane.txt");
    int n;
    cin >> n;
    string s;
    cin >> s;
    for (int i = 0; i < n; i++)
        if (s[i] == 'j')
            surowe.push_back(-1);
        else
            surowe.push_back(1);
}

void rob_prefixow(vector<int> &surowe, vector<int> &sumy)
{
    int suma = 0;
    sumy.push_back(0);
    for (int i = 0; i < surowe.size(); i++)
    {
        suma += surowe[i];
        sumy.push_back(suma);
    }
}

int nies = 1000000000;

void domaxuj(int &a, int b)
{
    a = max(a, b);
}

void dominuj(int &a, int b)
{
    a = min(a, b);
}

class Kolejne_wystapienia
{
  public:
    vector<int> *tab;
    int minimum;
    int maksimum;
    vector<vector<int>> wektory_wystapien;
    vector<int> wierzchni_index;
    Kolejne_wystapienia(vector<int> &tab)
    {
        this->tab = &tab;
        minimum = nies;
        maksimum = -nies;
    }
    void szukaj_minimum_i_maksimum()
    {
        for (int i = 0; i < tab->size(); i++)
        {
            dominuj(minimum, (*tab)[i]);
            domaxuj(maksimum, (*tab)[i]);
        }
    }
    int ind_z_war(int wartosc)
    {
        return wartosc - minimum;
    }

    void uzupelnij_wektory_wystapien()
    {
        int rozmiar = ind_z_war(maksimum) + 1;
        wektory_wystapien.resize(rozmiar);
        wierzchni_index.resize(rozmiar, 0);
        for (int i = 0; i < tab->size(); i++)
        {
            wektory_wystapien[ind_z_war((*tab)[i])].push_back(i);
        }
        //teraz sraznik
        for (int i = 0; i < rozmiar; i++)
            wektory_wystapien[i].push_back(nies);
    }

    int szuakaj_tej_wartosci_po_tym_indexie(int wartosc, int index)
    {
        if (wartosc < minimum || wartosc > maksimum)
            return nies;
        while (wektory_wystapien[ind_z_war(wartosc)][wierzchni_index[ind_z_war(wartosc)]] <= index)
            wierzchni_index[ind_z_war(wartosc)]++;
        return wektory_wystapien[ind_z_war(wartosc)][wierzchni_index[ind_z_war(wartosc)]];
    }
};

void odetnij_poczatek(int &startowy, int &maksymalna_znaleziona_dlugosc, vector<int> &sumy, Kolejne_wystapienia &wystapienia)
{
    //zakladam tutaj, ze startowy to taki, ze nastepny jest poprawny i wiekszy. po wywolaniu startowy jest od razu takim lub ostatnim elementem
    //albo inaczej startowy jest dowolny byle nie ostatni
    while (startowy + 1 < sumy.size() && sumy[startowy + 1] < sumy[startowy])
        startowy++;
    //cout<<endl<<"startowy po while to "<<startowy;
    if (startowy != sumy.size() - 1)
    {
        int wystapienie_o_jeden_mniej_niz_startowy = wystapienia.szuakaj_tej_wartosci_po_tym_indexie(sumy[startowy]-1, startowy);
        int proponowane_maksimum = startowy + 1;

        while (true)
        {
            while (proponowane_maksimum + 1 < sumy.size() && sumy[proponowane_maksimum + 1] > sumy[proponowane_maksimum])
                proponowane_maksimum++;
            int pom = wystapienia.szuakaj_tej_wartosci_po_tym_indexie(sumy[proponowane_maksimum], proponowane_maksimum);
            if (pom < wystapienie_o_jeden_mniej_niz_startowy)
                proponowane_maksimum = pom;
            else
                break;
        }
        domaxuj(maksymalna_znaleziona_dlugosc, proponowane_maksimum - startowy);
        //teraz trzeba przygotowac
        startowy = proponowane_maksimum;
    }
}

int szukaj_odpowiedzi(vector<int> &sumy, Kolejne_wystapienia &wystapienia)
{
    int odpowiedz = 0;
    int startowy = 0;
    while (startowy != sumy.size() - 1)
    {
        //cout<<"teraz startowy wynosi "<<startowy<<endl;
        odetnij_poczatek(startowy, odpowiedz, sumy, wystapienia);
    }
    return odpowiedz;
}

int main()
{
    vector<int> surowe;
    wczytaj(surowe);
    vector<int> sumy;
    rob_prefixow(surowe, sumy);
    Kolejne_wystapienia wystapienia(sumy);
    wystapienia.szukaj_minimum_i_maksimum();
    wystapienia.uzupelnij_wektory_wystapien();
/*
    cout << "surowe" << endl;
    for (int i = 0; i < surowe.size(); i++)
    {
        cout << surowe[i] << " ";
    }
    cout << endl
         << "sumy" << endl;
    for (int i = 0; i < sumy.size(); i++)
    {
        cout << sumy[i] << " ";
    }
*/

    cout 
         << szukaj_odpowiedzi(sumy, wystapienia);

    //cout << endl
         //<< "doszlo do return 0" << endl;
    return 0;
}
