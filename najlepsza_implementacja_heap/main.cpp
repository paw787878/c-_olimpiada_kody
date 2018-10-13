#include <iostream>
#include <vector>
#include <fstream>
#include <limits>
#include <queue>
#include <assert.h>
using namespace std;

template <class T, class S>
class Node
{
  public:
    T war;
    S sat;
    int i;
    Node(T war, S sat, int i)
    {
        this->sat = sat;
        this->i = i;
        this->war = war;
    }
};

template <class T, class S>
class Heap
{
  public:
    vector<Node<T, S> *> tab;
    int w_srodku;
    int ile_w_srodku()
    {
        return w_srodku;
    }
    bool (*mniejszy)(T, T);
    Heap(bool (*mniejszy)(T, T), T dummy_war, S dummy_sat)
    {
        this->mniejszy = mniejszy;
        tab.push_back(new Node<T, S>(dummy_war, dummy_sat, 0));
        w_srodku = 0;
        //bo zerowy element ni ejest istotny
    }
    bool empty()
    {
        return tab.size() == 0 || tab.size() == 1;
    }

    T min()
    {
        return tab[1]->war;
    }
    pair<T, S> pop_min();
    void zamien(int a, int b)
    {
        Node<T, S> *pom = tab[a];
        tab[a] = tab[b];
        tab[b] = pom;
        tab[a]->i = a;
        tab[b]->i = b;
    }
    Node<T, S> *wsadz(T elem, S sat);
    void minimalize(Node<T, S> *co, T ile_teraz_wynosi);
    void heap_up(int start);
    void wypisz();
};

template <class T, class S>
void Heap<T, S>::wypisz()
{
    cout << endl
         << "wypisuje heap prawdziwy" << endl;

    for (int i = 1; i < tab.size(); i++)
    {
        cout << "index " << i << "(" << tab[i]->i << ") wart " << tab[i]->war << " sat+1 " << tab[i]->sat + 1 << endl;
    }
}

template <class T, class S>
void Heap<T, S>::minimalize(Node<T, S> *co, T ile_teraz_wynosi)
{
    int start = co->i;
    co->war = ile_teraz_wynosi;
    heap_up(start);
}

template <class T, class S>
pair<T, S> Heap<T, S>::pop_min()
{
    w_srodku--;
    pair<T, S> wyrzut;
    wyrzut.first = tab[1]->war;
    wyrzut.second = tab[1]->sat;
    zamien(1, tab.size() - 1);
    delete (tab[tab.size() - 1]);
    tab.resize(tab.size() - 1);
    //teraz trzeba spuscic ziomeczka w dol;
    int chwilowy = 1;
    while (true)
    {
        //vector<T *> synowie;
        vector<int> jaki_k;
        for (int k = 0; k < 2; k++)
            if (2 * chwilowy + k < tab.size())
            {
                //synowie.push_back(&(tab[2 * chwilowy + k]->war));
                jaki_k.push_back(k);
            }
        if (jaki_k.size() == 0)
            break;
        int index_min_k = 0;
        for (int i = 0; i < jaki_k.size(); i++)
            if (mniejszy(tab[2*chwilowy+jaki_k[i]]->war, tab[2*chwilowy+jaki_k[index_min_k]]->war))
                index_min_k = i;
        //teraz mamy juz tego mniejszego.
        if (mniejszy(tab[2*chwilowy+jaki_k[index_min_k]]->war, tab[chwilowy]->war))
        {
            zamien(chwilowy, 2 * chwilowy + jaki_k[index_min_k]);
            chwilowy = 2 * chwilowy + jaki_k[index_min_k];
        }
        else
            break;
    }

    return wyrzut;
}

template <class T, class S>
void Heap<T, S>::heap_up(int start)
{
    int chwilowy = start;
    while (chwilowy != 1)
    {
        int ten = chwilowy;
        int ojc = chwilowy / 2;
        if (mniejszy(tab[ten]->war, tab[ojc]->war))
        {
            zamien(ten, ojc);
            chwilowy = ojc;
        }
        else
        {
            break;
        }
    }
}

template <class T, class S>
Node<T, S> *Heap<T, S>::wsadz(T elem, S sat)
{
    w_srodku++;
    Node<T, S> *wyrzut = new Node<T, S>(elem, sat, tab.size());
    tab.push_back(wyrzut);
    //teraz trzebawsadzic
    int chwilowy = tab.size() - 1;
    heap_up(chwilowy);
    return wyrzut;
}

bool mniejszy_long_long_int(long long int a, long long int b)
{
    return a < b;
}
/*
void djakira(vector<vector<pair<int,int> > >& graf,vector<long long int>& odleglosci){
    long long int nies=numeric_limits<long long int>::max();
    Heap<long long int,int> heap_odleglosci(&mniejszy_long_long_int,-1,-1);
    vector<Node<long long int,int>*> wskazniki; 
    vector<bool> czy_usuniety;
    for(int i=0;i<graf.size();i++){
        odleglosci.push_back(nies);
        wskazniki.push_back(heap_odleglosci.wsadz(nies,i));
        czy_usuniety.push_back(false);
    }
    heap_odleglosci.minimalize(wskazniki[0],0);
    //cout<<"startowy heap"<<endl;
    //heap_odleglosci.wypisz();
    while(! heap_odleglosci.empty()){
        //cout<<endl;
        pair<long long int,int> obrabiany=heap_odleglosci.pop_min();
        int jaki_wierz=obrabiany.second;
        czy_usuniety[jaki_wierz]=true;
        //cout<<"po popowaniu "<<jaki_wierz+1<<endl;
        //heap_odleglosci.wypisz();
        long long int jaka_wartosc=obrabiany.first;
        odleglosci[jaki_wierz]=jaka_wartosc;
        for(int i=0;i<graf[jaki_wierz].size();i++){
            int id_syna=graf[jaki_wierz][i].first;
            long long int waga_krawedzi=graf[jaki_wierz][i].second;
            if(!czy_usuniety[id_syna])
                heap_odleglosci.minimalize(wskazniki[id_syna],min(wskazniki[id_syna]->war,jaka_wartosc+waga_krawedzi));
            //cout<<"po minowaniu "<<id_syna+1<<endl;
            //heap_odleglosci.wypisz();
        }

    }

}
*/
/*
class Heap_naive
{
  public:
    vector<int> odleglosci; //gdy jakiegos samochodu nie ma to ma odleglosc -1;
    Heap_naive(int ile_samochodow)
    {
        for (int i = 0; i < ile_samochodow; i++)
        {
            odleglosci.push_back(-1);
        }
    }
    void wsadz(int nr_samoch, int polozenie);
    int szukaj_max();
    int pop_max();
    void wypisz();
};

void Heap_naive::wypisz()
{
    cout << endl
         << "wypisuje naiwne" << endl;
    for (int i = 0; i < odleglosci.size(); i++)
        if (odleglosci[i] != -1)
            cout << "samoch (+1) " << i + 1 << " odleglosc " << odleglosci[i] << endl;
}

int Heap_naive::pop_max()
{
    int maximum_index = szukaj_max();
    //int wyrzut=odleglosci[maximum_index];
    odleglosci[maximum_index] = -1;
    return maximum_index;
}

void Heap_naive::wsadz(int nr_samoch, int polozenie)
{
    //assert( odleglosci[nr_samoch]==-1);
    odleglosci[nr_samoch] = polozenie;
}

int Heap_naive::szukaj_max()
{
    int maximum = 0;
    for (int i = 0; i < odleglosci.size(); i++)
    {
        if (odleglosci[i] > odleglosci[maximum])
            maximum = i;
    }
    return maximum;
}
*/
bool wiekszy_int(int a, int b)
{
    return a > b;
}

int main()
{
    ifstream we("sam/in/sam5.in");

    int licz_samoch, poj_pod, dl_ciagu;
    we >> licz_samoch >> poj_pod >> dl_ciagu;
    vector<int> zyczenia;
    for (int i = 0; i < dl_ciagu; i++)
    {
        int pom;
        we >> pom;
        zyczenia.push_back(pom);
    }

    vector<queue<int>> nastepny_samochod;
    vector<Node<int, int> *> wskazniki;

    for (int i = 0; i < licz_samoch; i++)
    {
        wskazniki.push_back(0);
        queue<int> pom;
        nastepny_samochod.push_back(pom);
    }
    for (int i = 0; i < dl_ciagu; i++)
    {
        int samoch = zyczenia[i] - 1;
        nastepny_samochod[samoch].push(i);
    }
    //teraz wstawmy wreszcie sentinelow
    for (int i = 0; i < licz_samoch; i++)
    {
        nastepny_samochod[i].push(1000000);
        nastepny_samochod[i].pop();
    }
    //i usuwamy te pirwsze wystapienia.teraz jest tam tyle elementow co bylo samochodow tego typu
    Heap<int, int> kopiec_podlogi(&wiekszy_int, -1, -1);
    //Heap_naive kopiec_naive(licz_samoch);

    int wynik = 0;
    //na poczatek trzeba wstawic te poczatkowe.
    for (int i = 0; i < dl_ciagu; i++)
        if (kopiec_podlogi.ile_w_srodku() != poj_pod)
        {
            int samoch = zyczenia[i] - 1;
            int jak_daleko = nastepny_samochod[samoch].front();
            nastepny_samochod[samoch].pop();
            if (wskazniki[samoch] == 0)
            {
                wskazniki[samoch] = kopiec_podlogi.wsadz(jak_daleko, samoch);
               // kopiec_naive.wsadz(samoch, jak_daleko);
                wynik++;
            }
            else
            {
                kopiec_podlogi.minimalize(wskazniki[samoch],jak_daleko);
                //kopiec_naive.wsadz(samoch, jak_daleko);
            }
        }
        else

        //kopiec_podlogi.wypisz();

        //teraz zakladajac, ze jeszcze sa zyczenia to trzeba je wszystkie obrobic.
        {
            int wkladany_samochod = zyczenia[i] - 1;
            if(wskazniki[wkladany_samochod]==0)
            //if (kopiec_naive.odleglosci[wkladany_samochod] == -1)
            {
                //trzeba cos wypiepszyc.
                pair<int, int> para = kopiec_podlogi.pop_min();
                int odkladany_samochod = para.second;


                //assert(odkladany_samochod == kopiec_naive.pop_max());
                //cout<<odkladany_samochod+1<<endl;
                wskazniki[odkladany_samochod] = 0;
                int jak_daleko_nowy = nastepny_samochod[wkladany_samochod].front();
                nastepny_samochod[wkladany_samochod].pop();
                wskazniki[wkladany_samochod] = kopiec_podlogi.wsadz(jak_daleko_nowy, wkladany_samochod);
               // kopiec_naive.wsadz(wkladany_samochod, jak_daleko_nowy);
                wynik++;
            }
            else
            {
                //tylko updatujemy
                Node<int, int> *wskaz = wskazniki[wkladany_samochod];
                int ile_teraz = nastepny_samochod[wkladany_samochod].front();
                nastepny_samochod[wkladany_samochod].pop();
                kopiec_podlogi.minimalize(wskaz, ile_teraz);

              //  kopiec_naive.wsadz(wkladany_samochod, ile_teraz);
            }
            //kopiec_podlogi.wypisz();
        }

    cout << wynik;

    //cout << endl
      //   << "doszlo do return 0";

    return 0;
}
