#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

int nies = 1000000000;

void wczytywanie(vector<int> &klocki, int &k, int &n)
{
    //ifstream we("dane.txt");
    cin >> n >> k;
    string pom;
    cin >> pom;
    for (int i = 0; i < n; i++)
    {
        if (pom[i] == 'c')
            klocki.push_back(-k);
        else
            klocki.push_back(1);
    }
}

class Eventy
{
  public:
    vector<int> pierwszy_interesujacy_index;
    vector<vector<int>> eventy;
    int przesuniecie;
    int rozmiar;
    int podaj_nastepne_na_tej_h(int h, int index_mimimalny)
    {
        while (eventy[h - przesuniecie][pierwszy_interesujacy_index[h - przesuniecie]] < index_mimimalny)
            pierwszy_interesujacy_index[h - przesuniecie]++;
        return eventy[h - przesuniecie][pierwszy_interesujacy_index[h - przesuniecie]];
    }
};

void wypisz_wektor(vector<int> &wektor, string wiadomosc)
{
    cout << endl
         << endl
         << wiadomosc << endl;
    for (int i = 0; i < wektor.size(); i++)
    {
        cout << i << " " << wektor[i] << endl;
    }
    cout << endl
         << endl;
}

void wypisz_eventy(int przesuniecie, vector<vector<int>> &eventy, string wiadomosc)
{
    cout << endl
         << endl
         << wiadomosc << endl
         << endl;
    for (int i = 0; i < eventy.size(); i++)
    {
        cout << "h= " << i + przesuniecie << endl;
        for (int j = 0; j < eventy[i].size(); j++)
        {
            cout << eventy[i][j] << " ";
        }
        cout << endl;
    }
}

void rob_sumy_prefixow(vector<int> &klocki, vector<int> &sumy)
{
    int suma = 0;
    sumy.resize(klocki.size(), -1);
    for (int i = 0; i < klocki.size(); i++)
    {
        suma += klocki[i];
        sumy[i] = suma;
    }
}

void dominuj(int &a, int b)
{
    a = min(a, b);
}

void domaksuj(int &a, int b)
{
    a = max(a, b);
}

void znajdz_min_max(int &minimum, int &maksimum, vector<int> &sumy)
{
    minimum = nies;
    maksimum = -nies;
    for (int i = 0; i < sumy.size(); i++)
    {
        dominuj(minimum, sumy[i]);
        domaksuj(maksimum, sumy[i]);
    }
    dominuj(minimum, 0);
    domaksuj(maksimum, 0);
}

void rob_eventy_wzrostu(vector<int> &sumy, int rozmiar, int przesuniecie, Eventy &eventy_wzrostu)
{
    //event w indexie i dla wysokosci h oznacza, ze przed dodaniem bylo h po dodaniu bylo h+1
    eventy_wzrostu.eventy.resize(rozmiar);
    eventy_wzrostu.pierwszy_interesujacy_index.resize(rozmiar, 0);
    eventy_wzrostu.przesuniecie = przesuniecie;
    eventy_wzrostu.rozmiar = rozmiar;
    int poprzedni = 0;
    for (int i = 0; i < sumy.size(); i++)
    {
        if (sumy[i] - poprzedni == 1)
            eventy_wzrostu.eventy[poprzedni - przesuniecie].push_back(i);
        poprzedni = sumy[i];
    }
}

void rob_eventy_spadku(int k, vector<int> &sumy, int rozmiar, int przesuniecie, Eventy &eventy_spadku)
{
    //tak jak wzrostu, tylko myslac o -k jako o wielu -1
    int poprzedni = 0;
    eventy_spadku.eventy.resize(rozmiar);
    eventy_spadku.pierwszy_interesujacy_index.resize(rozmiar, 0);
    eventy_spadku.rozmiar = rozmiar;
    eventy_spadku.przesuniecie = przesuniecie;
    for (int i = 0; i < sumy.size(); i++)
    {
        if (sumy[i] - poprzedni == -k)
        {
            for (int j = 0; j < k; j++)
            {
                eventy_spadku.eventy[poprzedni - przesuniecie - j].push_back(i);
            }
        }
        poprzedni = sumy[i];
    }
}

void zbierz_iles_bialych_szukajac_od_indexu(vector<int> &gdzie_zbierac, int ile, int startowy_index, Eventy &eventy_wzrostu, vector<int> &sumy)
{
    //pierwszy ktory moze byc wziety jest startowy index
    int poprzednia_wysokosc;
    if (startowy_index == 0)
        poprzednia_wysokosc = 0;
    else
        poprzednia_wysokosc = sumy[startowy_index - 1];
    int chwilowy_startowy_index = startowy_index;
    //cout<<endl<<"chwilowy startowy index "<<chwilowy_startowy_index<< endl;
    for (int i = 0; i < ile; i++)
    {
        //cout << endl
            // << "poprzednia wysokosc= " << poprzednia_wysokosc << " chwilowy startowy index " << chwilowy_startowy_index << endl;
        int nastepny = eventy_wzrostu.podaj_nastepne_na_tej_h(poprzednia_wysokosc, chwilowy_startowy_index);
        gdzie_zbierac.push_back(nastepny);
        //cout << endl
            // << "nastepny= " << nastepny << endl;
        poprzednia_wysokosc++;
        chwilowy_startowy_index = nastepny + 1;
    }
}

void znajdz_ruch_tu_sie_zaczynajacy(vector<int> &gdzie_zbierac, int k, int pierwszy_brany_index, Eventy &eventy_wzrostu, Eventy &eventy_spadku, vector<int> &sumy, vector<int> &klocki)
{
    gdzie_zbierac.push_back(pierwszy_brany_index);
    if (klocki[pierwszy_brany_index] == 1)
    {
        //czyli pierwszy to bialy.
        int chwilowa_wysokosc=sumy[pierwszy_brany_index];//bo mysmy go wzieli

        int czarny_index = eventy_spadku.podaj_nastepne_na_tej_h(chwilowa_wysokosc, pierwszy_brany_index + 1);
        //cout<<endl<<"czarny index"<<czarny_index<<endl;
        int ile_przed_bialych_zebrac = sumy[czarny_index - 1] - chwilowa_wysokosc;
        //cout<<endl<<"ile_przed bialych zebrac"<<ile_przed_bialych_zebrac<<endl;
        zbierz_iles_bialych_szukajac_od_indexu(gdzie_zbierac,ile_przed_bialych_zebrac,pierwszy_brany_index+1,eventy_wzrostu,sumy);
        //cout<<endl<<"jeszcze zyje po pierwszym zbieraniu"<<endl;
        gdzie_zbierac.push_back(czarny_index);
        zbierz_iles_bialych_szukajac_od_indexu(gdzie_zbierac,k-ile_przed_bialych_zebrac-1,czarny_index+1,eventy_wzrostu,sumy);
    }
    else
    {
        //czyli pierwszy jest -k. latwo
        zbierz_iles_bialych_szukajac_od_indexu(gdzie_zbierac, k, pierwszy_brany_index + 1, eventy_wzrostu, sumy);
    }
}

void znajdz_wszystkie_ruchy(vector<vector<int> > & ruchy, int k, Eventy &eventy_wzrostu, Eventy &eventy_spadku, vector<int> &sumy, vector<int> &klocki)
{
    vector<bool> czy_tkniete;
    czy_tkniete.resize(klocki.size(),false);
    for(int i=0;i<klocki.size();i++)
        if(czy_tkniete[i]==false){
            //cout<<endl<<"teraz moj startowy to "<<i<<endl;
            vector<int> chwilowe_ruchy;
            znajdz_ruch_tu_sie_zaczynajacy(chwilowe_ruchy,k,i,eventy_wzrostu,eventy_spadku,sumy,klocki);
            for(int j=0;j<chwilowe_ruchy.size();j++)
                czy_tkniete[chwilowe_ruchy[j]]=true;
            //wypisz_wektor(chwilowe_ruchy,"chwilowe ruchy");
            ruchy.push_back(chwilowe_ruchy);
        }
}



int main()
{
    int n, k;
    vector<int> klocki;
    wczytywanie(klocki, k, n);
    vector<int> sumy_prefixow;
    rob_sumy_prefixow(klocki, sumy_prefixow);
    int minimum, maksimum;
    znajdz_min_max(minimum, maksimum, sumy_prefixow);
    int przesuniecie = minimum;
    int rozmiar = maksimum - minimum + 1;

    Eventy eventy_wzrostu;
    rob_eventy_wzrostu(sumy_prefixow, rozmiar, przesuniecie, eventy_wzrostu);

    Eventy eventy_spadku;
    rob_eventy_spadku(k, sumy_prefixow, rozmiar, przesuniecie, eventy_spadku);

    //wypisz_wektor(klocki, "klocki");
    //wypisz_wektor(sumy_prefixow, "sumy_prefixow");

    //wypisz_eventy(przesuniecie, eventy_wzrostu.eventy, "eventy wzrostu");
    //wypisz_eventy(przesuniecie, eventy_spadku.eventy, "eventy spadkul");

    vector<vector<int> > wszystkie_ruchy;
    znajdz_wszystkie_ruchy(wszystkie_ruchy,k,eventy_wzrostu,eventy_spadku,sumy_prefixow,klocki);

    for(int i=0;i<wszystkie_ruchy.size();i++){
        for(int j=0;j<wszystkie_ruchy[i].size();j++){
            cout<<wszystkie_ruchy[i][j]+1;
            if(j!=wszystkie_ruchy[i].size()-1)
                cout<<" ";
        }
        if(i!=wszystkie_ruchy.size()-1)
            cout<<endl;
    }

    //cout << endl
         //<< "doszlo do return 0" << endl;
    return 0;
}
