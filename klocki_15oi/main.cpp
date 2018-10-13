#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

void wypisz_wektor(vector<int> &wektor)
{
    cout << endl;
    for (int i = 0; i < wektor.size(); i++)
        cout << wektor[i] << " ";
    cout << endl;
}

class Heap_priority
{
  public:
    vector<int> tab; // zawera pozycje w tej wielkiej tablicy naszej
    vector<int> *klocki;
    vector<int> *pamiec_o_wskaznikach;
    bool (*mniejszy)(int, int);
    Heap_priority(vector<int> *klocki, vector<int> *pamiec_o_wskaznikach, bool (*mniejszy)(int, int))
    {
        this->klocki = klocki;
        this->pamiec_o_wskaznikach = pamiec_o_wskaznikach;
        this->mniejszy = mniejszy;
        tab.push_back(-13);
    }
    bool wieksze_priority(int a, int b)
    {
        if (a < 1)
            return a;
        if (b < 1)
            return b;
        if (a >= tab.size())
            return b;
        if (b >= tab.size())
            return a;
        return (*mniejszy)((*klocki)[tab[a]], (*klocki)[tab[b]]);
    }
    void heap_up(int start);
    void zamien(int a, int b);
    void heap_down(int start);
    void zmien_wartosc_na_indexie(int index, int nowa_wartosc);
    void wepchnij(int nowa_wartosc);
    int usun();
    int usun(int ktory);
    int front()
    {
        return tab[1];
    }
    int size()
    {
        return tab.size() - 1;
    }
};

int Heap_priority::usun(int ktory)
{
    int wyrzut = tab[ktory];
    int jaka_wartosc_ostatniego = tab[tab.size() - 1];
    tab.resize(tab.size() - 1);
    if (ktory < tab.size())
        zmien_wartosc_na_indexie(ktory, jaka_wartosc_ostatniego);
    return wyrzut;
}

int Heap_priority::usun()
{
    return usun(1);
}

void Heap_priority::wepchnij(int nowa_wartosc)
{
    tab.resize(tab.size() + 1);
    zmien_wartosc_na_indexie(tab.size() - 1, nowa_wartosc);
}

void Heap_priority::zmien_wartosc_na_indexie(int index, int nowa_wartosc)
{
    tab[index] = nowa_wartosc;
    (*pamiec_o_wskaznikach)[nowa_wartosc] = index;
    heap_up(index);
    heap_down(index);
}

void Heap_priority::heap_down(int start)
{
    while (true)
    {
        //najpierw szukasz mniejszego z synow
        int syn1 = 2 * start;
        int syn2 = 2 * start + 1;
        if (syn1 >= tab.size())
            break;
        int mniejszy_syn;
        if (wieksze_priority(syn1, syn2))
            mniejszy_syn = syn1;
        else
            mniejszy_syn = syn2;
        //teraz z nim porownujemy
        if (wieksze_priority(mniejszy_syn, start))
        {
            zamien(mniejszy_syn, start);
            start = mniejszy_syn;
        }
        else
        {
            break;
        }
    }
}

void Heap_priority::zamien(int a, int b)
{
    int pom = tab[a];
    tab[a] = tab[b];
    tab[b] = pom;
    (*pamiec_o_wskaznikach)[tab[a]] = a;
    (*pamiec_o_wskaznikach)[tab[b]] = b;
}

void Heap_priority::heap_up(int start)
{
    while (true)
    {
        if (start == 1)
            break;
        if (wieksze_priority(start, start / 2))
        {
            zamien(start, start / 2);
            start = start / 2;
        }
        else
            break;
    }
}

bool mniejszy_int(int a, int b)
{
    return a < b;
}

bool wiekszy_int(int a, int b)
{
    return a > b;
}

class Okno
{
  public:
    int k;
    vector<int> *klocki;
    vector<int> pamiec_o_wskaznikach; //teraz ma rozmiar taki co klocki koniec babci srania
    Heap_priority male;
    Heap_priority duze; //najmniejszy element tego to jest ta nasza wysokosc, czyli mediana.
    int wysokosc;       //chwilowa wysokosc. tego czegos
    int index_lewego_ziomka;
    long long int ile_ruchow;
    int najlepszy_index_lewego_ziomka;
    int wysokosc_dla_najlepszego;
    long long int najlepsza_ilosc_ruchow;
    Okno(int k, vector<int> *klocki) : male(klocki, &pamiec_o_wskaznikach, &wiekszy_int), duze(klocki, &pamiec_o_wskaznikach, &mniejszy_int)
    {
        this->k = k;
        this->klocki = klocki;
        pamiec_o_wskaznikach.resize((*klocki).size(), 0);
        for (int i = 0; i < k; i++)
            duze.wepchnij(i);
        //wypisz_wektor(duze.tab);
        //teraz trzeba troche posprzatac.przenosimy sobie rzeczy
        for (int i = 0; i < k / 2; i++)
        {
            int wychodzaca = duze.usun();
            //wypisz_wektor(duze.tab);
            male.wepchnij(wychodzaca);
        }
        wysokosc = (*klocki)[duze.front()];
        ile_ruchow = 0;
        for (int i = 0; i < k; i++)
            ile_ruchow += abs(wysokosc - (*klocki)[i]);
        index_lewego_ziomka = 0;
        najlepszy_index_lewego_ziomka = 0;
        wysokosc_dla_najlepszego = wysokosc;
        najlepsza_ilosc_ruchow = ile_ruchow;
        //wypisz_informacje();
    }
    void wykonaj_przesuniecie();
    void wypisz_informacje();
};

void Okno::wypisz_informacje()
{
    cout << endl
         << endl;
    cout << endl
         << "wypiuje okna informacje" << endl
         << "index lewego ziomka " << index_lewego_ziomka;
    cout << endl
         << "ile_ruchow" << ile_ruchow;
    cout << endl
         << "wysokosc " << wysokosc;
    cout << endl
         << "te sa w malych" << endl;
    for (int i = 1; i < male.tab.size(); i++)
    {
        cout << (*klocki)[male.tab[i]] << " ";
    }
    cout << "te sa w duzych" << endl;
    for (int i = 1; i < duze.tab.size(); i++)
    {
        cout << (*klocki)[duze.tab[i]] << " ";
    }
    cout << endl
         << endl;
}

void Okno::wykonaj_przesuniecie()
{
    bool usuwany_jest_w_malych;
    int usuwany_wskaznik = pamiec_o_wskaznikach[index_lewego_ziomka];
    if (male.tab.size() > usuwany_wskaznik && male.tab[usuwany_wskaznik] == index_lewego_ziomka)
        usuwany_jest_w_malych = true;
    else
        usuwany_jest_w_malych = false;
    //teraz zastanowmy sie gdzie chce sie dostac ten nowy
    bool nowy_chce_do_malego = ((*klocki)[index_lewego_ziomka + k] < wysokosc);
    //teraz wkadaj oraz usuwaj
    //cout<<endl<<"usuwany jest w malych"<<usuwany_jest_w_malych;
    //cout<<endl<<"nowy chce do malego "<<nowy_chce_do_malego<<endl;
    if (usuwany_jest_w_malych)
        male.usun(usuwany_wskaznik);
    else
    {
        duze.usun(usuwany_wskaznik);

    }

    if (nowy_chce_do_malego)
        male.wepchnij(index_lewego_ziomka + k);
    else
    {
        duze.wepchnij(index_lewego_ziomka + k);
    }
    ile_ruchow -= abs(wysokosc - (*klocki)[index_lewego_ziomka]);
    ile_ruchow += abs(wysokosc - (*klocki)[index_lewego_ziomka + k]);
    //teraz ile ruchow jest ok tylko trzeba poprzemieszczac
    if (male.size() > k / 2)
    {
        //jest za duzo w malych. na poczatek zmniejszasz wysokosc
        int o_ile_zmniejszysz_wysokosc = wysokosc - (*klocki)[male.front()];
        ile_ruchow -= (male.size() - duze.size()) * o_ile_zmniejszysz_wysokosc;
        wysokosc -= o_ile_zmniejszysz_wysokosc;
        duze.wepchnij(male.usun());
    }
    else if (male.size() < k / 2)
    {
        //cout<<"akuku"<<endl;
        //trzeba zabrac z duzych i dac do malych;
        int o_ile_zwiekszyc_wysokosc = (*klocki)[duze.front()] - wysokosc;
        //cout<<endl<<o_ile_zwiekszyc_wysokosc<<endl;
        ile_ruchow -= (duze.size() - male.size()) * o_ile_zwiekszyc_wysokosc;
        wysokosc += o_ile_zwiekszyc_wysokosc;
        male.wepchnij(duze.usun());
        //ale teraz jeszcze pewnie trzeba podniesc wartosc
        o_ile_zwiekszyc_wysokosc = (*klocki)[duze.front()] - wysokosc;
        //cout<<endl<<o_ile_zwiekszyc_wysokosc<<endl;
        ile_ruchow -= (duze.size() - male.size()) * o_ile_zwiekszyc_wysokosc;
        wysokosc = (*klocki)[duze.front()];
    }
    else
    {
        //to znaczy, ze jest jesli chodzi o rozmiar dobrze, ale dalej trzeba poprawaic wysokosc byc moze
        int o_ile_zwiekszyc_wysokosc = (*klocki)[duze.front()] - wysokosc;
        ile_ruchow -= (duze.size() - male.size()) * o_ile_zwiekszyc_wysokosc;
        wysokosc += o_ile_zwiekszyc_wysokosc;
    }
    index_lewego_ziomka++;

    if (ile_ruchow < najlepsza_ilosc_ruchow)
    {
        najlepsza_ilosc_ruchow = ile_ruchow;
        najlepszy_index_lewego_ziomka = index_lewego_ziomka;
        wysokosc_dla_najlepszego = wysokosc;
    }

    //wypisz_informacje();

    /*

    ile_ruchow -= abs(mediana - (*klocki)[index_lewego_ziomka]);
    ile_ruchow += abs(mediana - (*klocki)[index_lewego_ziomka + k]);
    if (nowy_chce_do_malego == usuwany_jest_w_malych)
    {
        //prostszy przypadek. rownowaga zachowana

        if (nowy_chce_do_malego)
            male.zmien_wartosc_na_indexie(usuwany_wskaznik, index_lewego_ziomka + k);
        else
            duze.zmien_wartosc_na_indexie(usuwany_wskaznik, index_lewego_ziomka + k);
    }
    else
    {
        //teraz jest gorzej.
        int stara_mediana = mediana;

        if (nowy_chce_do_malego)
        {
            //cout << "nowy chce do malego trudny przypadek" << endl;
            //bug nie wiesz jeszcze, ze to male front odpadnie z malego
            int wskaznik_usuwanego_z_malego;
            //cout<<"klocki male front to "<<(*klocki)[male.front()];
            if ((*klocki)[male.front()] > (*klocki)[index_lewego_ziomka + k])
            {
                //cout << "gorny";
                wskaznik_usuwanego_z_malego = male.front();
                male.zmien_wartosc_na_indexie(1, index_lewego_ziomka + k);
            }
            else
            {
                wskaznik_usuwanego_z_malego = index_lewego_ziomka + k;
            }
            duze.zmien_wartosc_na_indexie(usuwany_wskaznik, wskaznik_usuwanego_z_malego);
            mediana = duze.front();
            if (k % 2 == 1)
                ile_ruchow -= (stara_mediana - mediana);
            else
                ile_ruchow -= 2 * (stara_mediana - mediana);
        }
        else
        {
            int wskaznik_usuwanego_z_duzego;
            if ((*klocki)[duze.front()] < (*klocki)[index_lewego_ziomka + k])
            {
                wskaznik_usuwanego_z_duzego = duze.front();
                duze.zmien_wartosc_na_indexie(1, index_lewego_ziomka + k);
            }
            else
                wskaznik_usuwanego_z_duzego = index_lewego_ziomka + k;
            male.zmien_wartosc_na_indexie(usuwany_wskaznik, wskaznik_usuwanego_z_duzego);
            mediana = duze.front();
            if (k % 2 == 1)
                ile_ruchow -= (mediana - stara_mediana);
            else
            {
                //tutaj nic sie nie dzieje.
            }
        }

        //teraz by potrzeba nam bylo updatowac warttosc tego wszystkiego
    }
    index_lewego_ziomka++;
    if (ile_ruchow < najlepsza_ilosc_ruchow)
    {
        najlepsza_ilosc_ruchow = ile_ruchow;
        najlepszy_index_lewego_ziomka = index_lewego_ziomka;
        wysokosc_dla_najlepszego = mediana;
    }
    //cout<<"mediana po przesuniecieu wynosi "<<mediana<<endl;
    */
}

int main()
{
    //ifstream we("in/klo7a.in");
    int n, k;
    cin >> n >> k;
    vector<int> klocki;
    for (int i = 0; i < n; i++)
    {
        int pom;
        cin >> pom;
        klocki.push_back(pom);
    }
    if (k == 1)
    {
        cout << 0 << endl;
        for (int i = 0; i < klocki.size(); i++)
            cout << klocki[i] << endl;
    }
    else
    {
        Okno optimizer(k, &klocki);
        while (optimizer.index_lewego_ziomka + k < n)
        {
            optimizer.wykonaj_przesuniecie();
        }
        
        cout << optimizer.najlepsza_ilosc_ruchow << endl;
        
        for (int i = 0; i < optimizer.najlepszy_index_lewego_ziomka; i++)
        {
            cout << klocki[i] << endl;
        }
        for (int i = optimizer.najlepszy_index_lewego_ziomka; i < optimizer.najlepszy_index_lewego_ziomka + k; i++)
            cout << optimizer.wysokosc_dla_najlepszego << endl;
        for (int i = optimizer.najlepszy_index_lewego_ziomka + k; i < n; i++)
        {
            cout << klocki[i] << endl;
        }
        
        
        //cout<<endl<<optimizer.najlepszy_index_lewego_ziomka;
    }

    return 0;
}
