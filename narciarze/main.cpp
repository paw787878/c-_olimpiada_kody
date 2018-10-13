#include <iostream>
#include <vector>
#include <fstream>
#include <list>
using namespace std;

//teraz moze klase ktora to bedzie robic

class Node
{
  public:
    list<int> w_dol;
    vector<pair<int, list<int>::iterator>> w_gore; //para jaka to z gory oraz jaki ten iterator;
    bool usuniety;
    Node()
    {
        usuniety = false;
    }
};

class Trasy
{
  public:
    vector<Node> polany; //zaczynamy od 1 jednak, wiec jest sentinel na dole
    Trasy();
    void wypisz();
    void usun(int usuwany_nr);
    int n;
    void spusc_trase();
};

void Trasy::spusc_trase()
{
    //zakladam, ze jest co spuszczac.
    int chwilowy = 1;
    int przedostatni = 1;
    while (chwilowy != n)
    {
        usun(chwilowy);
        //cout<<endl<<"skonczylem usuwac "<<chwilowy<<" w spusc trase"<<endl;
        przedostatni = chwilowy;
        chwilowy = polany[chwilowy].w_dol.front();
        //cout<<endl<<"chwilowy teraz wynosi "<<chwilowy<<endl;
    }
    polany[przedostatni].w_dol.erase(polany[przedostatni].w_dol.begin());
}

void Trasy::usun(int usuwany_nr)
{
    //cout<<endl<<"poczatek usun na numerze "<<usuwany_nr<<endl;
    //cout<<"a co jak tu dopisze"<<endl;
    //cout<<endl<<"bardzo dziwne"<<endl;
    if (!(polany[usuwany_nr].usuniety))
    {
        //cout<<endl<<"naprawde usuwa"<<endl;
        polany[usuwany_nr].usuniety = true;
        for (int i = 0; i < polany[usuwany_nr].w_gore.size(); i++)
        {
            list<int>::iterator a = polany[usuwany_nr].w_gore[i].second;
            int ojciec = polany[usuwany_nr].w_gore[i].first;
            polany[ojciec].w_dol.erase(a);
            if (polany[ojciec].w_dol.size() == 0)
                usun(ojciec);
        }
    }
    //cout<<"wiec to sie skonczylo"<<endl;
}

void Trasy::wypisz()
{
    for (int i = 1; i < polany.size(); i++)
    {
        //cout << endl
        //<< "polana nr " << i << " w dol do ";
        list<int>::iterator a = polany[i].w_dol.begin();
        while (a != polany[i].w_dol.end())
        {
            //cout << *(a) << " ";
            a++;
        }

        //cout << " a w gore do ";
        for (int j = 0; j < polany[i].w_gore.size(); j++)
        {
            //cout << polany[i].w_gore[j].first;
        }
    }
}

Trasy::Trasy()
{
   // cout<<"zaczyna robic konstruktor"<<endl;
    Node dummy;
    polany.push_back(dummy);
    //ifstream we("dane.txt");
    cin >> n;
    for (int i = 1; i <= n; i++)
    {
        Node wpychany;
        polany.push_back(wpychany);
    }
    for (int i = 1; i <= n - 1; i++)
    {
        int k;
        cin >> k;
        for (int j = 0; j < k; j++)
        {
            int gdzie_w_dol;
            cin >> gdzie_w_dol;
            polany[i].w_dol.push_back(gdzie_w_dol);
            list<int>::iterator wskaznik = polany[i].w_dol.end();
            
            wskaznik--;
           // cout<<"tu jeszcz bylo "<<endl;
            polany[gdzie_w_dol].w_gore.push_back(pair<int, list<int>::iterator>(i, wskaznik));
        }
    }
    
    //co jak nie jest prawda zalozenie?
    for(int i=1;i<=n-1;i++){
        if(polany[i].w_dol.size()==0)
            usun(i);
    }
   // cout<<"tu jeszcze dojechalo"<<endl;
}

int main()
{
    Trasy graf;
    //graf.wypisz();
    if (graf.n == 1)
    {
        cout << 1;
    }
    else
    {
        int wynik = 0;
        while (graf.polany[1].w_dol.size() != 0)
        {
            wynik++;
            graf.spusc_trase();
            //cout<<"spuscilem cos"<<endl;
            //graf.wypisz();
            //cout.flush();
        }
        cout << wynik;
    }
   // cout << "doszlo do return 0"<<endl;
    return 0;
}
