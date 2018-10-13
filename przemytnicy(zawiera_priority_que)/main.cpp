#include <iostream>
#include <vector>
#include <fstream>
#include <limits>
using namespace std;

template <class T,class S>
class Node
{
  public:
    T war;
    S sat;
    int i;
    Node(T war,S sat, int i)
    {
        this->sat=sat;
        this->i = i;
        this->war = war;
    }
};

template <class T,class S>
class Heap
{
  public:
    vector<Node<T,S> *> tab;
    bool (*mniejszy)(T, T);
    Heap(bool (*mniejszy)(T, T), T dummy_war,S dummy_sat)
    {
        this->mniejszy = mniejszy;
        tab.push_back(new Node<T,S>(dummy_war,dummy_sat, 0));
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
    pair<T,S> pop_min();
    void zamien(int a, int b)
    {
        Node<T,S> *pom = tab[a];
        tab[a] = tab[b];
        tab[b] = pom;
        tab[a]->i = a;
        tab[b]->i = b;
    }
    Node<T,S>* wsadz(T elem, S sat);
    void minimalize(Node<T,S> *co, T ile_teraz_wynosi);
    void heap_up(int start);
    void wypisz();
};

template<class T,class S>
void Heap<T,S>::wypisz(){
    cout<<endl<<"wypisuje heap"<<endl;

    for(int i=1;i<tab.size();i++){
        cout<<"index "<<i<<"("<<tab[i]->i<<") wart "<<tab[i]->war<<" sat +1 "<<tab[i]->sat+1<<endl;
    }
}

template <class T,class S>
void Heap<T,S>::minimalize(Node<T,S> *co, T ile_teraz_wynosi)
{
    int start=co->i;
    co->war=ile_teraz_wynosi;
    heap_up(start);
}

template <class T,class S>
pair<T,S> Heap<T,S>::pop_min()
{
    pair<T,S> wyrzut;
    wyrzut.first=tab[1]->war;
    wyrzut.second=tab[1]->sat;
    zamien(1, tab.size() - 1);
    delete tab[tab.size() - 1];
    tab.resize(tab.size() - 1);
    //teraz trzeba spuscic ziomeczka w dol;
    int chwilowy = 1;
    while (true)
    {
        vector<T *> synowie;
        for (int k = 0; k < 2; k++)
            if (2 * chwilowy + k < tab.size())
                synowie.push_back(&(tab[2 * chwilowy + k]->war));
        if (synowie.size() == 0)
            break;
        int index_min_syna = 0;
        for (int i = 0; i < synowie.size(); i++)
            if (mniejszy(*synowie[i], *synowie[index_min_syna]))
                index_min_syna = i;
        //teraz mamy juz tego mniejszego.
        if (mniejszy(*synowie[index_min_syna], tab[chwilowy]->war))
        {
            zamien(chwilowy, 2 * chwilowy + index_min_syna);
            chwilowy = 2 * chwilowy + index_min_syna;
        }
        else
            break;
    }

    return wyrzut;
}

template <class T,class S>
void Heap<T,S>::heap_up(int start)
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

template <class T,class S>
Node<T,S>* Heap<T,S>::wsadz(T elem,S sat)
{
    Node<T,S>* wyrzut=new Node<T,S>(elem,sat, tab.size());
    tab.push_back(wyrzut);
    //teraz trzebawsadzic
    int chwilowy = tab.size() - 1;
    heap_up(chwilowy);
    return wyrzut;
}

bool mniejszy_long_long_int(long long int a,long long int b)
{
    return a < b;
}

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


int main()
{
    //ifstream we("dane.txt");
    int n;
    cin>>n;
    vector<int> polowy_cen;
    for(int i=0;i<n;i++){
        int pom;
        cin>>pom;
        polowy_cen.push_back(pom/2);
    }
    int m;
    cin>>m;
    vector<vector<pair<int,int> > > graf;//gdzie za ile
    for(int i=0;i<n;i++){
        vector<pair<int,int> > pom;
        graf.push_back(pom);
    }
   
    for(int i=0;i<m;i++){
        int a,b,ile;
        cin>>a>>b>>ile;
        graf[a-1].push_back(pair<int,int>(b-1,ile));
    }

    
    vector<long long int> odleglosci_wte;
    djakira(graf,odleglosci_wte);
    vector<long long int>odleglosci_wefte;
    vector<vector<pair<int,int> > > trans_graf;
    for(int i=0;i<graf.size();i++){
        vector<pair<int,int> > pom;
        trans_graf.push_back(pom);
    }
    for(int i=0;i<graf.size();i++){
        for(int j=0;j<graf[i].size();j++){
            int kto=i;
            int z_kim=graf[i][j].first;
            int za_ile=graf[i][j].second;
            trans_graf[z_kim].push_back(pair<int,int>(kto,za_ile));
        }
    }
    djakira(trans_graf,odleglosci_wefte);

    /*
    cout<<endl<<"odleglosci"<<endl;
    for(int i=0;i<odleglosci_wefte.size();i++){
        cout<<i+1<<" "<<odleglosci_wefte[i]<<endl;
    }
    */

    long long int najmniejsza=numeric_limits<long long int>::max();
    for(int i=0;i<odleglosci_wefte.size();i++){
        long long int prop=odleglosci_wefte[i]+odleglosci_wte[i]+polowy_cen[i];
        if(prop<najmniejsza)
            najmniejsza=prop;
    }
    cout<<najmniejsza;

    /*
    cout << endl
         << "doszlo do return 0";
    */
    return 0;
}
