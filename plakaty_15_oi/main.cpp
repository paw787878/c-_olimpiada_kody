#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

class Klasy_rownowaznosci
{
  public:
    vector<int> ojciec;
    vector<int> ile_pod;
    Klasy_rownowaznosci(int n)
    {
        ojciec.resize(n, -1);
        ile_pod.resize(n, 1);
    }
    void polacz(int a, int b); //a oraz b od 0 do n-1
    bool czy_pol(int a, int b);
    int root(int a)
    {
        while (ojciec[a] != -1)
            a = ojciec[a];
        return a;
    }
    void wypisz();
};

void Klasy_rownowaznosci::wypisz(){
    cout<<endl<<endl;
    int ostatni=0;
    cout<<"klasa ";
    for(int i=0;i<ojciec.size();i++){
        if(!czy_pol(i,ostatni)){
            cout<<endl<<"klasa ";
            ostatni=i;
        }
        cout<<i<<" ";
    }
    cout<<endl<<endl;
}

bool Klasy_rownowaznosci::czy_pol(int a, int b)
{
    return (root(a) == root(b));
}

void Klasy_rownowaznosci::polacz(int a, int b)
{

    
    int ra = root(a);
    int rb = root(b);
    if (ra != rb)
    {
        if (ile_pod[ra] > ile_pod[rb])
        {
            ojciec[rb] = ra;
            ile_pod[ra] += ile_pod[rb];
        }
        else
        {
            ojciec[ra] = rb;
            ile_pod[rb] += ile_pod[ra];
        }
    }
}

template <class T>
void mergesort(vector<T> &tab, int a, int b,
               bool (*mniejsze)(T, T, vector<int> &wysokosci), vector<int> &wysokosci)
{
    //a oraz b to sa jak w pythonie
    if (!(b - a == 0 || b - a == 1))
    {
        int s = a + (b - a) / 2;
        mergesort(tab, a, s, mniejsze, wysokosci);
        mergesort(tab, s, b, mniejsze, wysokosci);
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
            else if (mniejsze(pier[ipier], drug[idrug], wysokosci))
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

bool mniejszy(int aa, int bb, vector<int> &wysokosci)
{
    int wys_a = wysokosci[aa];
    int wys_b = wysokosci[bb];
    if (wys_a < wys_b)
        return false;
    if (wys_a > wys_b)
        return true;

    if (aa < bb)
        return true;
    return false;
}

void wypisz_pare(pair<int, int> a)
{
    cout << endl
         << a.first << " " << a.second << endl;
}

int policz_ile(vector<int> &wysokosci)
{
    vector<int> poziome_odcinki;
    for (int i = 0; i < wysokosci.size(); i++)
    {
        poziome_odcinki.push_back(i);
    }
    mergesort<int>(poziome_odcinki, 0, poziome_odcinki.size(), mniejszy, wysokosci);
    int liczba_plakatow = 0;

    /*
    cout << endl
         << "jak wyglada plansza";
    for (int i = 0; i < wysokosci.size(); i++)
    {
        cout << "teraz i=" << i << " wys od i " << wysokosci[i] << endl;
    }
    cout << endl
         << "teraz te poziome" << endl;
    for (int i = 0; i < poziome_odcinki.size(); i++)
    {
        cout << poziome_odcinki[i] << " ";
    }
    cout<<endl;
    */
    Klasy_rownowaznosci klasy(wysokosci.size());
    int ostatni_obrabiany = 0; //nie powinienem wykorzystac tego 0
    int poprzednia_wysokosc = -1;
    for (int i = 0; i < poziome_odcinki.size() - 2; i++)
    {
        int obrabiana_pozycja = poziome_odcinki[i];
        int wys_obrabianej = wysokosci[obrabiana_pozycja];
        //laczymy lewa i prawa czesc
        if (wysokosci[obrabiana_pozycja - 1] >= wys_obrabianej)
            klasy.polacz(obrabiana_pozycja, obrabiana_pozycja - 1);
        if (wysokosci[obrabiana_pozycja + 1] >= wys_obrabianej)
            klasy.polacz(obrabiana_pozycja, obrabiana_pozycja + 1);
        //teraz sie zastanawiamy czy zwiekszyc licznik
        
        if (!(poprzednia_wysokosc == wys_obrabianej && klasy.czy_pol(ostatni_obrabiany, obrabiana_pozycja)))
        {
            liczba_plakatow++;
        }
        //teraz zapamietaj ostatnia poayzjce
        ostatni_obrabiany = obrabiana_pozycja;
        poprzednia_wysokosc = wys_obrabianej;
    }
    /*
    for (int i = 0; i < wysokosci.size(); i++)
    {
        cout << "teraz i="<<i<<" wys od i "<<wysokosci[i]<<endl;
    }
    cout<<endl<<"teraz te poziome"<<endl;
    for(int i=0;i<poziome_odcinki.size();i++){
        cout<<poziome_odcinki[i]<<" ";
    }
    */
    return liczba_plakatow;
}

int main()
{
    //ifstream we("dane.txt");
    int n;
    cin >> n;
    int dl, wyso;
    vector<int> wysokosci;
    wysokosci.push_back(-13);
    for (int i = 0; i < n; i++)
    {
        cin >> dl >> wyso;
        wysokosci.push_back(wyso);
    }
    wysokosci.push_back(-13);
    //wysokosci maja teraz sentinelow po obydwu stronach.
    cout << policz_ile(wysokosci);
    //cout << endl
         //<< "doszlo do return 0" << endl;
    return 0;
}
