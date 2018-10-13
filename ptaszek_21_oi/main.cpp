#include <iostream>
#include <fstream>
#include <vector>
#include <deque>
using namespace std;

void wczytaj(vector<int> &wysokosci_drzew, vector<int> &wytrzymalosci)
{
    //ifstream we("dane.txt");
    int n;
    cin >> n;
    int pom;
    for (int i = 0; i < n; i++)
    {
        cin >> pom;
        wysokosci_drzew.push_back(pom);
    }
    int m;
    cin >> m;
    for (int i = 0; i < m; i++)
    {
        cin >> pom;
        wytrzymalosci.push_back(pom);
    }
}

class Prioryteto_kolejka
{
  public:
    deque<int> kolej; //wpychasz od konca, zdejmujesz z poczatku
    vector<int> *tab;
    int pocz, koni; //pythonowy zapis;
    bool (*wiekszy_lub_rowny_priorytet)(int, int);
    Prioryteto_kolejka(vector<int> *tablica, bool (*wiekszy_l_ro_priorytet)(int, int))
    {
        this->tab = tablica;
        this->wiekszy_lub_rowny_priorytet = wiekszy_l_ro_priorytet;
        pocz = 0;
        koni = 0;
    }
    void przesun_koni(int nowy_koniec)
    {
        while (koni < nowy_koniec)
        {
            while (!kolej.empty() && wiekszy_lub_rowny_priorytet((*tab)[koni], (*tab)[kolej.back()]))
                kolej.pop_back();
            kolej.push_back(koni);
            koni++;
        }
    }
    void przesun_pocz(int nowy_pocz)
    {
        while (pocz < nowy_pocz)
        {
            if (!kolej.empty() && kolej.front() == pocz)
                kolej.pop_front();
            pocz++;
        }
    }
    int podaj_index_o_najwyzszym_priorytecie(int a, int b)
    {
        //lepiej uzywac na niepustych;
        if (pocz > a || koni > b)
            cout << endl
                 << "cos zrabales" << endl;
        else
        {
            przesun_koni(b);
            przesun_pocz(a);
            if (!kolej.empty())
                return kolej.front();
            else
                return -1;
        }
    }
};

bool funkcja_wiekszy_lub_rowny(int a, int b)
{
    return (a >= b);
}
bool funkcja_mniejszy_lub_rowny(int a, int b)
{
    return (a <= b);
}

int minimum(int a, int b)
{
    return min(a, b);
}

int znajdz_nastepna_pewna_pozycja(vector<int> &wysokosci_drzew, Prioryteto_kolejka &kolejka_min, Prioryteto_kolejka &kolejka_max, deque<int> &stos_rozwazanych_pozycji,
                                  int wytrzy, int &ile_razy_do_gory_lub_rowno)
{
    //zakladam, pierwszy na stos_rozwazanych_pozycji to jest startowy,który na pewno ma być w optymalnej
    //start nie jest ostatnim indexem tablicy_wysokosci_drzew
    //po wykonaniu zdejmuje start z frontu
    //a nowy front to jest ta nowa pozycja
    int start = stos_rozwazanych_pozycji.front();
    int koniec = minimum(start + 1 + wytrzy, wysokosci_drzew.size());
    bool pstyczek = false;
    if (stos_rozwazanych_pozycji.size() == 1)
    {
        int index_minimalnego = kolejka_min.podaj_index_o_najwyzszym_priorytecie(start + 1, koniec);
        if (wysokosci_drzew[index_minimalnego] >= wysokosci_drzew[start])
        {
            ile_razy_do_gory_lub_rowno++;
            stos_rozwazanych_pozycji.pop_front();
            int wyrzut = kolejka_max.podaj_index_o_najwyzszym_priorytecie(start + 1, koniec);
            stos_rozwazanych_pozycji.push_back(wyrzut);
            return wyrzut;
        }
        else
        {
            pstyczek = true;
        }
    }
    else
        pstyczek = true;
    if (pstyczek)
    {
        while (true)
        {
            if (stos_rozwazanych_pozycji.back() == koniec - 1)
                break;
            int index_minimalnego = kolejka_min.podaj_index_o_najwyzszym_priorytecie(stos_rozwazanych_pozycji.back() + 1, koniec);
            if (!(wysokosci_drzew[index_minimalnego] < wysokosci_drzew[start]))
                break;
            int szukany_nowy=stos_rozwazanych_pozycji.back()+1;
            while (wysokosci_drzew[szukany_nowy] >= wysokosci_drzew[start])
                szukany_nowy++;
            //wiec teraz szukany nowy jest gotowy do wepchniecia go
            while (wysokosci_drzew[stos_rozwazanych_pozycji.back()] <= wysokosci_drzew[szukany_nowy])
                stos_rozwazanych_pozycji.pop_back();
            stos_rozwazanych_pozycji.push_back(szukany_nowy);
        }
        stos_rozwazanych_pozycji.pop_front();
    }
}

int rob_jedna_wytrzymalosc(vector<int> &wysokosci_drzew,
                           int wytrzy)
{
    Prioryteto_kolejka kolejka_min(&wysokosci_drzew, funkcja_mniejszy_lub_rowny);
    Prioryteto_kolejka kolejka_max(&wysokosci_drzew, funkcja_wiekszy_lub_rowny);
    deque<int> stos_rozwazanych_pozycji;
    stos_rozwazanych_pozycji.push_back(0);
    int ile_razy_do_gory_lub_rowno = 0;
    while (stos_rozwazanych_pozycji.front() != wysokosci_drzew.size() - 1)
    { 
        /*
        cout << stos_rozwazanych_pozycji.front()<<endl;
        cout<<"rozmiar"<<stos_rozwazanych_pozycji.size()<<endl<<endl;
        */
        znajdz_nastepna_pewna_pozycja(wysokosci_drzew, kolejka_min, kolejka_max, stos_rozwazanych_pozycji, wytrzy, ile_razy_do_gory_lub_rowno);
    }
    return ile_razy_do_gory_lub_rowno;
}

int main()
{
    vector<int> wysokosci;
    vector<int> wytrzymalosci;
    wczytaj(wysokosci, wytrzymalosci);
    for (int i = 0; i < wytrzymalosci.size(); i++){
        cout << rob_jedna_wytrzymalosc(wysokosci, wytrzymalosci[i]);
        if(i!= wytrzymalosci.size()-1)
            cout<<endl;

    }
    //cout << endl
         //<< "doszlo do return 0" << endl;
    return 0;
}
