#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <utility>

using namespace std;

class Node
{
  public:
    //liczac jego samego
    int ile_pod;
    pair<int, int> synowie;
    int ojciec;
    bool zajety;
    int nr_pokoju;
    Node()
    {
        ile_pod = 0;
        synowie = pair<int, int>(-1, -1);
        ojciec = -1;
        zajety = false;
        nr_pokoju = -1;
    }
    void wypisz();
};

void Node::wypisz()
{
     cout<< " oj " << ojciec << " zajety " << zajety << " synowie " << synowie.first << " " << synowie.second << " ile_pod " << ile_pod << " " << endl;
}

class Drzewo
{
  public:
    vector<Node> tab;
    int start;

    Drzewo(int);
    void wsadz(int, int);
    void usun(int);
    int wiekszy_lub_rowny(int);
    int poddrzewa_min(int);
    int poddrzewa_max(int);
    int mniejszy_lub_rowny(int);
    int pokoj(int);
};

int Drzewo::pokoj(int i)
{   if(i<tab.size()&&i>-1)
        return tab[i].nr_pokoju;
    return -1;
}

int Drzewo::poddrzewa_min(int root)
{
    int i = root;
    while (true)
    {

        if (tab[i].synowie.first != -1 && tab[tab[i].synowie.first].ile_pod != 0)
        {
            i = tab[i].synowie.first;
            continue;
        }
        if (tab[i].zajety)
            return i;

        if (tab[i].synowie.second != -1 && tab[tab[i].synowie.second].ile_pod != 0)
        {
            i = tab[i].synowie.second;
        }
    }
}

int Drzewo::poddrzewa_max(int root)
{
    int i = root;
    while (true)
    {
        if (tab[i].synowie.second != -1 && tab[tab[i].synowie.second].ile_pod != 0)
        {
            i = tab[i].synowie.second;
            continue;
        }
        if (tab[i].zajety)
            return i;
        if (tab[i].synowie.first != -1 && tab[tab[i].synowie.first].ile_pod != 0)
            i = tab[i].synowie.first;
    }
}

int Drzewo::wiekszy_lub_rowny(int od_ile)
{
    /* zaczynamy od naszego elementu a nastepnie jak idziemy do gory
    w prawo to sprawdzamy czy ten ma albo czy te na prawo maja cos.
    */
    if(od_ile<0 || od_ile>=tab.size())
        return -1;
    if (tab[od_ile].zajety)
        return od_ile;
    //zaczynam petle gdy nie ma nic w tym czyms i trzeba szukac na prawo
    if (tab[od_ile].synowie.second != -1 && tab[tab[od_ile].synowie.second].ile_pod != 0)
        return poddrzewa_min(tab[od_ile].synowie.second);
    int i = od_ile;
    while (tab[i].ojciec != -1)
    {

        int poprzedni = i;
        i = tab[i].ojciec;
        if (tab[i].synowie.first == poprzedni)
        {
            if (tab[i].zajety)
                return i;
            if (tab[i].synowie.second != -1 && tab[tab[i].synowie.second].ile_pod != 0)
            {
                return poddrzewa_min(tab[i].synowie.second);
            }
        }
    }
    return -1;
}

int Drzewo::mniejszy_lub_rowny(int od_ile)
{
    /* zaczynamy od naszego elementu a nastepnie jak idziemy do gory
    w prawo to sprawdzamy czy ten ma albo czy te na prawo maja cos.
    */
    if(od_ile<0 || od_ile>=tab.size())
        return -1;
    if (tab[od_ile].zajety)
        return od_ile;
    if (tab[od_ile].synowie.first != -1 && tab[tab[od_ile].synowie.first].ile_pod != 0)
        return poddrzewa_max(tab[od_ile].synowie.first);
    //zaczynam petle gdy nie ma nic w tym czyms i trzeba szukac na prawo
    int i = od_ile;
    int gdzie_szukac = -1;
    while (tab[i].ojciec != -1)
    {
        int poprzedni = i;
        i = tab[i].ojciec;
        if (tab[i].synowie.second == poprzedni)
        {
            if (tab[i].zajety)
                return i;
            if (tab[i].synowie.first != -1 && tab[tab[i].synowie.first].ile_pod != 0)
            {
                return poddrzewa_max(tab[i].synowie.first);
            }
        }
    }
    return -1;
}

void Drzewo::usun(int wartosc)
{
    if (tab[wartosc].zajety == true)
    {
        tab[wartosc].zajety = false;
        tab[wartosc].nr_pokoju = -1;
        int i = start;
        while (true)
        {
            tab[i].ile_pod--;
            if (wartosc == i)
                break;
            if (wartosc > i)
                i = tab[i].synowie.second;
            else
                i = tab[i].synowie.first;
        }
    }
}

void Drzewo::wsadz(int wartosc, int jaki_pokoj)
{
    if (tab[wartosc].zajety == false)
    {
        tab[wartosc].zajety = true;
        tab[wartosc].nr_pokoju = jaki_pokoj;
        int i = start;
        while (true)
        {
            tab[i].ile_pod++;
            if (wartosc == i)
                break;
            if (wartosc > i)
                i = tab[i].synowie.second;
            else
                i = tab[i].synowie.first;
        }
    }
}

void polaczenia(vector<Node> &tab, int ojciec, int a,
                int b, bool syn_przerosl)
{
    if (a != b)
    {
        int s = a + (b - a) / 2;
        tab[s].ojciec = ojciec;
        if (ojciec != -1)
            if (syn_przerosl)
                tab[ojciec].synowie.second = s;
            else
                tab[ojciec].synowie.first = s;
        polaczenia(tab, s, s + 1, b, true);
        polaczenia(tab, s, a, s, false);
    }
}


Drzewo::Drzewo(int n)
{
    for (int i = 0; i < n; i++)
        tab.push_back(Node());
    start = n / 2;
    polaczenia(tab, -1, 0, tab.size(), true);
}

class Pokoj
{
  public:
    vector<int> sasiedzi;
    vector<int> y;
    vector<int> x;
    Pokoj()
    {
        y.push_back(-1);
        y.push_back(-1);
        x.push_back(-1);
        x.push_back(-1);
    }
    void wypisz();
};

void Pokoj::wypisz(){
    cout<<endl<<"x "<<x[0]<<" "<<x[1];
    cout<<" y "<<y[0]<<" "<<y[1];
   cout<<endl;
    for(int i=0;i<sasiedzi.size();i++)
      cout  <<sasiedzi[i]<<" ";

}

template <class T>
void mergesort(vector<T> &tab, int a, int b,
               bool (*mniejsze)(T, T))
{
    //a oraz b to sa jak w pythonie
    if (!(b - a == 0 || b - a == 1))
    {
        int s = a + (b - a) / 2;
        mergesort(tab, a, s, mniejsze);
        mergesort(tab, s, b, mniejsze);
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
            else if (mniejsze(pier[ipier], drug[idrug]))
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

int bin_search(vector<int> &tab, int wartosc)
{
    int a = 0;
    int b = tab.size();
    while (b - a > 1)
    {
        int s = a + (b - a) / 2;
        if (tab[s] > wartosc)
            b = s;
        else
            a = s;
    }
    return a;
}

bool bok_min(pair<int, pair<int, int>> poz1, pair<int, pair<int, int>> poz2)
{
    if (poz1.first == poz2.first)
    {
        if (poz1.second.first < poz2.second.first)
            return true;
        else
            return false;
    }
    else
    {
        if (poz1.first < poz2.first)
            return true;
        else
            return false;
    }
}
bool czy_poprawne_drzewo(int n)
{
    bool dziala = true;
    for (int i = 1; i <= n; i++)
    {
        Drzewo nowe(i);

        for (int j = 0; j < i; j++)
        {
            nowe.wsadz(j, 5);

            for (int k = 0; k < i; k++)
            {
                int wynik = nowe.wiekszy_lub_rowny(k);

                if (k <= j && wynik != j)
                {

                    dziala = false;
                }
                if (k > j && wynik != -1)
                {
                    dziala = false;
                }

                wynik = nowe.mniejszy_lub_rowny(k);

                if (k >= j && wynik != j)
                {

                    dziala = false;
                }
                if (k < j && wynik != -1)
                {
                    dziala = false;
                }
            }
            nowe.usun(j);
        }
    }
    return dziala;
}

bool wnetrze_na_prawo(pair<int,pair<int,int> >pion_bok, bool pierwszy_gora){
    bool nasz_gora=(pion_bok.second.second>pion_bok.second.first);
    return (nasz_gora==pierwszy_gora);

}

void sort_pair(pair<int,int> &para){
    if(para.first>para.second){
        int pom=para.first;
        para.first=para.second;
        para.second=pom;
    }
}

void dorob_krawedzie(vector<Pokoj> &tab){
    for(int i=0;i<tab.size();i++){
        for(int j=0;j<tab[i].sasiedzi.size();j++){
            int wskazywany=tab[i].sasiedzi[j];
            tab[wskazywany].sasiedzi.push_back(i);
        }
    }
}

int znajdz(vector<Pokoj> pokoje, pair<int,int> punkt){
    for(int i=0;i<pokoje.size();i++){
        Pokoj pok=pokoje[i];
        vector<int> x=pok.x;vector<int> y=pok.y;
        if(x[0]<=punkt.first&&x[1]>=punkt.first&&x[0]!=x[1]&&
        y[0]<=punkt.second&&y[1]>=punkt.second&&y[0]!=y[1]){
            return i;
        }
    }
}

bool sensowny(Pokoj p){
    return (p.x[0]!=p.x[1]&& p.y[0]!=p.y[1]);
}

void porob_poddrzewo(vector<Pokoj> &pokoje, int root, int skad, vector<int> &ojcowie){
   // <<endl<<"root to "<<root;
    vector<int>otoczenie=pokoje[root].sasiedzi;
    for(int i=0;i<otoczenie.size();i++)
        if(otoczenie[i]!=skad&&sensowny(pokoje[otoczenie[i]])){
            int sasiad=otoczenie[i];
            ojcowie[sasiad]=root;
            porob_poddrzewo(pokoje,sasiad,root,ojcowie);
        }

    
}

vector<int> dfs(vector<Pokoj> pokoje, int koni){
    vector<int> ojcowie;
    for(int i=0;i<pokoje.size();i++){
        ojcowie.push_back(-1);
    }
    porob_poddrzewo(pokoje,koni,-1,ojcowie);
    return ojcowie;
    
}
int abs(int a){
    if(a<0)
        return -a;
    return a;
}

int odleglosc(pair<int,int>a,pair<int,int> b){
    return abs(a.first-b.first)+abs(a.second-b.second);
}

pair<int,int> znajdz_posredni(Pokoj nastepny_pokoj, pair<int,int> chwilowe_polozenie){
    //stykaja sie bokiem pionowym. 
    int x;
    if(chwilowe_polozenie.first<=nastepny_pokoj.x[0])
        x=nastepny_pokoj.x[0];
    else
        x=nastepny_pokoj.x[1];
    

    if(chwilowe_polozenie.second<=nastepny_pokoj.y[0]){
        return pair<int,int>(x,nastepny_pokoj.y[0]);
    }
    if(chwilowe_polozenie.second<=nastepny_pokoj.y[1]){
        return pair<int,int>(x,chwilowe_polozenie.second);
    }
    return pair<int,int>(x,nastepny_pokoj.y[1]);
}

void wrzuc_pokoj(vector<Pokoj> &pokoje,int x, int ymin, int ymax,int nr_starego_pokoju){
 
    Pokoj nowy;
    nowy.x[0]=x;
    nowy.y[0]=ymin;
    nowy.y[1]=ymax;

    if(nr_starego_pokoju!=-1){
        nowy.sasiedzi.push_back(nr_starego_pokoju);
    }
    pokoje.push_back(nowy);

}

void asymiluj(Pokoj &biorca, Pokoj &dawca){
    //z gory znaczy ze dawca jest u gory
    bool z_gory=dawca.y[1]>biorca.y[1];
    if(z_gory){
        biorca.y[1]=dawca.y[1];
    }else{
        biorca.y[0]=dawca.y[0];
    }
    for(int i=0;i<dawca.sasiedzi.size();i++){
        biorca.sasiedzi.push_back(dawca.sasiedzi[i]);
    }
}

int main()
{
    // zacznijmy od wczytania danych
   // ifstream we;
   // we.open("ZAM5.IN");//orginalny na nim dziala
    //we.open("dane");//trudny
    //we.open("dane2");//tylko takie co maja po prawej wnetrze, na nim dziala
    //we.open("dane3");// ma dwa takie co maja wnetrze po lewej na tym samym x, dziala

    int n;
    cin >> n;
    vector<pair<int, int>> wierz;
    for (int i = 0; i < n; i++)
    {
        int a;
        int b;
        cin >> a;
        cin >> b;
        wierz.push_back(pair<int, int>(a, b));
    }
    int a;
    int b;
    cin >> a;
    cin >> b;
    pair<int, int> wejscie(a, b);
    cin >> a;
    cin >> b;
    pair<int, int> wyjscie(a, b);
    //koniec wczytywania

    //teraz szukaj krawedzi
    //x a potem ymin ymax
    vector<pair<int, pair<int, int>>> pionowe;
    vector<pair<int, pair<int, int>>> poziome;
    for (int i = 0; i < wierz.size() - 1; i++)
    {
        if (wierz[i].first == wierz[i + 1].first)
        {
            pionowe.push_back(pair<int, pair<int, int> >(wierz[i].first, pair<int, int>(wierz[i].second,wierz[i + 1].second)));
        }
        else
            poziome.push_back(pair<int, pair<int, int> >(wierz[i].second, pair<int, int>(wierz[i].first, wierz[i + 1].first)));
    }
    int pom=wierz.size() - 1;
    if (wierz[0].first == wierz[wierz.size() - 1].first)
        {
            pionowe.push_back(pair<int, pair<int, int> >(wierz[0].first, pair<int, int>(wierz[pom].second,wierz[0].second)));
        }
        else
            poziome.push_back(pair<int, pair<int, int> >(wierz[0].second, pair<int, int>(wierz[pom].first, wierz[0].first)));

    //teraz trzeba zrobic z nich posortowanie
    mergesort(poziome, 0, poziome.size(), bok_min);
    mergesort(pionowe, 0, pionowe.size(), bok_min);

    //teraz potrzeba to stopologizowac
    //to znaczy tylko ylevele sie wprowadza
    vector<int> ylevels;
    for (int i = 0; i < poziome.size(); i++)
    {
        if (ylevels.empty() || (ylevels[ylevels.size() - 1] != poziome[i].first))
        {
            ylevels.push_back(poziome[i].first);
        }
    }
    //teraz trzeba zrobic lepsza wersje tych pionowych
    vector<pair<int, pair<int, int>>> n_pionowe;
    for (int i = 0; i < pionowe.size(); i++)
    {
        pionowe[i] = pair<int, pair<int, int> >(pionowe[i].first, pair<int, int>(
                                                                     bin_search(ylevels, pionowe[i].second.first),
                                                                     bin_search(ylevels, pionowe[i].second.second)));
    }
   
    //teraz pionowe ma znormalizowane przy pomocy poziomow ylevels pionowe boki
    //a poziome jest nie ruszane.
    //zaczynamy
    Drzewo przekroje(ylevels.size());
    bool pg=(pionowe[0].second.second>pionowe[0].second.first);
    vector<Pokoj> pokoje;
    vector<bool>wnp;
    for(int i=0;i<pionowe.size();i++){
        wnp.push_back(wnetrze_na_prawo(pionowe[i],pg));
        sort_pair( pionowe[i].second);
    }
    
    vector<vector<pair<int,int> > >boki_zamykajace;
    vector<vector<pair<int,int> > >boki_otwierajace;

    int i=0;
    vector<int> xlevels;
    
    while(i<pionowe.size()){
        vector<pair<int,int> > zamykajace_ten_x;
        vector<pair<int,int> > otwierajace_ten_x;
        int x=pionowe[i].first;
        xlevels.push_back(x);
        while(i<pionowe.size()&& pionowe[i].first==x){
            if(wnp[i])
                otwierajace_ten_x.push_back(pionowe[i].second);
            else
                zamykajace_ten_x.push_back(pionowe[i].second);
            i++;
        }
   
        boki_zamykajace.push_back(zamykajace_ten_x);
        boki_otwierajace.push_back(otwierajace_ten_x);

    }

  

    //teraz jest juz dobrze. 
 
    for(int xi=0;xi<xlevels.size();xi++){
        //najpierw trzeba wrzucac zamykajace
        vector<pair<int,int> > zamykajace_ten_x=boki_zamykajace[xi];
        vector<pair<int,int> > otwierajace_ten_x=boki_otwierajace[xi];
        int x=xlevels[xi];
        int i=0;
        while(i<zamykajace_ten_x.size()){
            //zaczynamy w nowym otwartym kiedys boku 
            pair<int,int> bok=zamykajace_ten_x[i];


            int gorny_starego_pokoju=przekroje.wiekszy_lub_rowny(bok.second);
          
            int nr_starego_pokoju=przekroje.pokoj(gorny_starego_pokoju);
          
            int dolny_starego_pokoju=pokoje[nr_starego_pokoju].y[0];
           
            //to teraz zamknijmy go
            pokoje[nr_starego_pokoju].x[1]=x;
            //teraz zamykamy to co jest na dole
            if(przekroje.pokoj(bok.first)==-1){
                // to znaczy ze cos tam na dole jest
                wrzuc_pokoj(pokoje,x,dolny_starego_pokoju,bok.first,nr_starego_pokoju);
                przekroje.wsadz(bok.first,pokoje.size()-1);
                przekroje.tab[dolny_starego_pokoju].nr_pokoju=pokoje.size()-1;
            }else{
                przekroje.usun(bok.first);
            }
            //teraz trzeba robic kombosa 
            while(i<zamykajace_ten_x.size()&&zamykajace_ten_x[i].second<=gorny_starego_pokoju){
                bok=zamykajace_ten_x[i];
                if(bok.second==gorny_starego_pokoju)
                    przekroje.usun(bok.second);
                else if(i+1<zamykajace_ten_x.size()&&zamykajace_ten_x[i+1].second<=gorny_starego_pokoju){
                    //tutaj jest pomiedzy tym a nastepnym
                    wrzuc_pokoj(pokoje,x,bok.second,zamykajace_ten_x[i+1].first,nr_starego_pokoju);
                    przekroje.wsadz(bok.second,pokoje.size()-1);
                    przekroje.wsadz(zamykajace_ten_x[i+1].first,pokoje.size()-1);
                }else{
                    //do gornej granicy po prostu
                    wrzuc_pokoj(pokoje,x,bok.second,gorny_starego_pokoju,nr_starego_pokoju);
                    przekroje.wsadz(bok.second,pokoje.size()-1);
                    przekroje.tab[gorny_starego_pokoju].nr_pokoju=pokoje.size()-1;
                }
                i++;
   

          
         
            }

        }

        //to mamy powrzucane te nsaze zamykajace. teraz wrzucamy otwierajace. tu jest trudniej
        i=0;
        while(i<otwierajace_ten_x.size()){
            //teraz po prostu dorabiamy zawsze nowy pokoj laczymy go z tym co jest z gory i z tym co jest z dolu
            pair<int,int> bok=otwierajace_ten_x[i];
            
            //teraz tworzymy sobie nowy pokoj
            int nr_nowego_pokoju=pokoje.size();
            pokoje.push_back(Pokoj());
            pokoje[nr_nowego_pokoju].x[0]=x;
            pokoje[nr_nowego_pokoju].y[0]=bok.first;
            pokoje[nr_nowego_pokoju].y[1]=bok.second;

            
            //teraz zamykamy ten na dole jesli trzeba
            if(przekroje.pokoj(bok.first)!=-1&&pokoje[przekroje.pokoj(bok.first)].x[0]!=x){
                int a=przekroje.mniejszy_lub_rowny(bok.first-1);
                int b=bok.first;
                pokoje[przekroje.pokoj(a)].x[1]=x;
                wrzuc_pokoj(pokoje,x,a,b,przekroje.pokoj(a));
                przekroje.tab[a].nr_pokoju=pokoje.size()-1;
                przekroje.tab[b].nr_pokoju=pokoje.size()-1;
            }

            //teraz wezmy zasymilujmy dolny jesli istnieje
            if(przekroje.pokoj(bok.first)!=-1){
                int nr_asymilowanego_pokoju=przekroje.pokoj(bok.first);
                pokoje[nr_asymilowanego_pokoju].x[1]=x;
                asymiluj(pokoje[nr_nowego_pokoju],pokoje[nr_asymilowanego_pokoju]);
                przekroje.usun(bok.first);
                int dolna=przekroje.mniejszy_lub_rowny(bok.first-1);
                przekroje.tab[dolna].nr_pokoju=nr_nowego_pokoju;
            }else{
                przekroje.wsadz(bok.first,nr_nowego_pokoju);
            }
            //to teraz zamykamy ten na gorze i 

            if(przekroje.pokoj(bok.second)!=-1&&pokoje[przekroje.pokoj(bok.second)].x[0]!=x){
                int a=bok.second;
                int b=przekroje.wiekszy_lub_rowny(bok.second+1);
                pokoje[przekroje.pokoj(a)].x[1]=x;
                wrzuc_pokoj(pokoje,x,a,b,przekroje.pokoj(a));
                przekroje.tab[a].nr_pokoju=pokoje.size()-1;
                przekroje.tab[b].nr_pokoju=pokoje.size()-1;
            }
            //teraz zasymiluj gore
            if(przekroje.pokoj(bok.second)!=-1){
                int nr_asymilowanego_pokoju=przekroje.pokoj(bok.second);
                pokoje[nr_asymilowanego_pokoju].x[1]=x;
                asymiluj(pokoje[nr_nowego_pokoju],pokoje[nr_asymilowanego_pokoju]);
                przekroje.usun(bok.second);
                int gorna=przekroje.wiekszy_lub_rowny(bok.second+1);
                przekroje.tab[gorna].nr_pokoju=nr_nowego_pokoju;
            }else{
                przekroje.wsadz(bok.second,nr_nowego_pokoju);
            }
            
            i++;


            
            
            

            //teraz obrobmy dol

        }



    }
     



    //jak szukasz polozenia naszego skarbu i wejsci zadabaj o to by olac pokoje o zerowym polu


    

 
   
    
    
    dorob_krawedzie(pokoje); 

   //teraz poprawiamy
   for (int i=0;i<pokoje.size();i++){
       pokoje[i].y[0]=ylevels[pokoje[i].y[0]];
       pokoje[i].y[1]=ylevels[pokoje[i].y[1]];
   }

       

    //teraz mam te pokoje takie prawdziwe. 
    int pocz=znajdz(pokoje,wejscie);
    int koni=znajdz(pokoje,wyjscie);

 
    //jak widac niektore granicza ze sobo wiele razy. 

    vector<int>ojcowie=dfs(pokoje,koni);
    

    //teraz policzmy po prostu odleglosc
    int dlugosc=0;
    int chwilowy_pokoj=pocz;
    pair<int,int> chwilowe_polozenie=wejscie;
    
    while(true){
        if(chwilowy_pokoj==koni){
            dlugosc+=odleglosc(chwilowe_polozenie,wyjscie);
            chwilowe_polozenie=wyjscie;
            break;
        }else{
            Pokoj nastepny_pokoj=pokoje[ojcowie[chwilowy_pokoj]];

            pair<int,int> posredni=znajdz_posredni(nastepny_pokoj,chwilowe_polozenie);
            dlugosc+=odleglosc(posredni,chwilowe_polozenie);
            chwilowe_polozenie=posredni;
            chwilowy_pokoj=ojcowie[chwilowy_pokoj];
        }
    }

     cout<<dlugosc;
   



    





    return 0;
}
