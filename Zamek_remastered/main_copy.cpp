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
    cout << " oj " << ojciec << " zajety " << zajety << " synowie " << synowie.first << " " << synowie.second << " ile_pod " << ile_pod << " " << endl;
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
        cout<<sasiedzi[i]<<" ";

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
        if(x[0]<=punkt.first&&x[1]>=punkt.first&&
        y[0]<=punkt.second&&y[1]>=punkt.second){
            return i;
        }
    }
}

void porob_poddrzewo(vector<Pokoj> &pokoje, int root, int skad, vector<int> &ojcowie){
    vector<int>otoczenie=pokoje[root].sasiedzi;
    for(int i=0;i<otoczenie.size();i++)
        if(otoczenie[i]!=skad){
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

int main()
{
    // zacznijmy od wczytania danych
    ifstream we;
    we.open("ZAM1.IN");//orginalny na nim dziala
    //we.open("dane");//trudny
    //we.open("dane2");//tylko takie co maja po prawej wnetrze, na nim dziala
    //we.open("dane3");// ma dwa takie co maja wnetrze po lewej na tym samym x, dziala

    int n;
    we >> n;
    vector<pair<int, int>> wierz;
    for (int i = 0; i < n; i++)
    {
        int a;
        int b;
        we >> a;
        we >> b;
        wierz.push_back(pair<int, int>(a, b));
    }
    int a;
    int b;
    we >> a;
    we >> b;
    pair<int, int> wejscie(a, b);
    we >> a;
    we >> b;
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
    
    //teraz sprawdzamy czy dobrze wczytujemy
    /*
    cout<<"ylevels";
    for(int i=0;i<ylevels.size();i++)
        cout<<endl<<i<<" "<<ylevels[i];
    //to jest ok
    cout<<endl<<"krawedzie pionowe";
    for(int i=0;i<pionowe.size();i++){
        cout<<endl<<"x "<<pionowe[i].first<<" y "<<pionowe[i].second.first<<" "<<pionowe[i].second.second<<
        " wnetrze po prawo "<<wnetrze_na_prawo(pionowe[i],pg);
    }
    //wiec jest okay it seems;

    cout<<endl<<"teraz beda wyswietlane pojawiajace sie pokoje";

    //na razie tylko prawy pokoj wskazuje na lewy
    */

   //wyglada na to ze sie wykrzczylo w tym zamku1 na ostatniej krawedzi.
     
    for(int i=0;i<pionowe.size();i++){
    //    cout<<i<<endl;  cout.flush();
        pair<int,pair<int,int> > p=pionowe[i];

 

        pair<int,int> bok=p.second;
        int x=p.first;


        if(wnp[i]){
            
   
            bool jest_dolny=(przekroje.pokoj(bok.first)!=-1);
            bool jest_gorny=(przekroje.pokoj(bok.second)!=-1);
            int dolnagranica;
            int gornagranica;
            
            if(jest_dolny){
                dolnagranica=przekroje.mniejszy_lub_rowny(bok.first-1);
            }else{
                dolnagranica=bok.first;
            }
            if(jest_gorny){
                gornagranica=przekroje.wiekszy_lub_rowny(bok.second+1);
            }else{
                gornagranica=bok.second;
            }
            
            //teraz zdecydujmy jak ma sie nazywac nasz otwierany pokoj
            int pokoj_numer;
            bool kontynuacja_pokoju=(jest_dolny && pokoje[przekroje.pokoj(bok.first)].x[0]==x);
            if(kontynuacja_pokoju){
                pokoj_numer=przekroje.pokoj(bok.first);
                dolnagranica=pokoje[pokoj_numer].y[0];
            }else{
                pokoj_numer=pokoje.size();
                pokoje.push_back(Pokoj());
            }
            
            pokoje[pokoj_numer].x[0]=x;
            pokoje[pokoj_numer].y[0]=dolnagranica;
            pokoje[pokoj_numer].y[1]=gornagranica;
            //teraz wkladanie do przekrojow
            
            if(jest_gorny){
                int zamykany_pokoj=przekroje.pokoj(bok.second);
                pokoje[zamykany_pokoj].x[1]=x;

                pokoje[pokoj_numer].sasiedzi.push_back(zamykany_pokoj);

                przekroje.usun(bok.second);
                przekroje.tab[gornagranica].nr_pokoju=pokoj_numer;
            }else{
                przekroje.wsadz(bok.second,pokoj_numer);
            }
            
            if(jest_dolny){
                if(!kontynuacja_pokoju){
                    int zamykany_pokoj=przekroje.pokoj(bok.first);
                    pokoje[zamykany_pokoj].x[1]=x;

                    pokoje[pokoj_numer].sasiedzi.push_back(zamykany_pokoj);
                }
                

                przekroje.usun(bok.first);
                przekroje.tab[dolnagranica].nr_pokoju=pokoj_numer;
            }else{
                przekroje.wsadz(bok.first,pokoj_numer);
            }
            
        }
        
        else{
            //wnetrze jest na lewo najgorzej

            //Jesli jest na dole cos to to odetnij
            bool odetnij_dol=przekroje.pokoj(bok.first)==-1;
            int dolnagranica=przekroje.mniejszy_lub_rowny(bok.first);
            int zamykany_pokoj=przekroje.pokoj(przekroje.wiekszy_lub_rowny(bok.second));
            //teraz gorna granica
            bool czy_jest_cos_u_gory_dziwnego=false;
            if(i<pionowe.size()-1&& 
            pionowe[i+1].first==pionowe[i].first&&
            przekroje.wiekszy_lub_rowny(bok.second)>=pionowe[i+1].second.second)
                czy_jest_cos_u_gory_dziwnego=true;
            bool odetnij_gore=przekroje.pokoj(bok.second)==-1;
            int gornagranica;
            if(czy_jest_cos_u_gory_dziwnego)
                gornagranica=pionowe[i+1].second.first;
            else
                gornagranica=przekroje.wiekszy_lub_rowny(bok.second);
            bool kontynuacja=((!odetnij_dol)&&pokoje[przekroje.pokoj(bok.first)].x[0]==x);
            
            if(odetnij_dol){
                          
                int pokoj_numer=pokoje.size();
                pokoje.push_back(Pokoj());
                pokoje[pokoj_numer].x[0]=x;
                pokoje[pokoj_numer].y[0]=dolnagranica;
                pokoje[pokoj_numer].y[1]=bok.first;

                pokoje[pokoj_numer].sasiedzi.push_back(zamykany_pokoj);
                //ponizsza linia jest wadliwa. segmentation error. 
                //wynika to z tego, ze czasem nie ma prawidlowych 
                przekroje.tab[dolnagranica].nr_pokoju=pokoj_numer;

                przekroje.wsadz(bok.first,pokoj_numer);


                
            }
            
            
            else{
                //to bedzie prowdadzi dobuguw
                //tu sa dwie mozliwosci. albo jest to kontynuacja albo po prostu tutaj zaczynamy.
                //sprawdzmy czy jest kontynuacja
                
                if(kontynuacja){
                    //w sumie nic nie nalezy robic z ta dolna granica
                    
                }else{
                    przekroje.usun(bok.first);
                   
                }

            }
            //problem jest chyba ponizej
            
            if(odetnij_gore){

                int pokoj_numer=pokoje.size();
                pokoje.push_back(Pokoj());
                pokoje[pokoj_numer].x[0]=x;
                pokoje[pokoj_numer].y[0]=bok.second;
                pokoje[pokoj_numer].y[1]=gornagranica;
                pokoje[pokoj_numer].sasiedzi.push_back(zamykany_pokoj);
                przekroje.wsadz(bok.second,pokoj_numer);
                if(!czy_jest_cos_u_gory_dziwnego){
                    przekroje.tab[gornagranica].nr_pokoju=pokoj_numer;
                }else{
                    //czyzby tutaj. przeciez gornagranica to jest pierwszy 
                    //znaleziony do gory wiec on juz jest. nie ma co 
                    //wsadac.
                    //stara to 
                    //przekroje.wsadz(gornagranica,pokoj_numer);
                    //nowa to 
                    przekroje.tab[gornagranica].nr_pokoju=pokoj_numer;
                }
            }
            //chyba nawet jest ponizej tego
            
            else{
                //pozbywamy sie wiec tego jednego poziomu

                //tez teraz dopisalem
                przekroje.usun(bok.second);
                
                //przekroje.usun(gornagranica);
            }
            //wyglada na to, ze strukturka jest teraz oczyszczona. ewentualnie zostaje ten u gory jeden ale tak to jest czysto.
            pokoje[zamykany_pokoj].x[1]=x;
            
            
        }

        
        
        
    }
    int pomsum=0;
    for(int i=0;i<ylevels.size();i++){
        if(przekroje.tab[i].nr_pokoju!=-1){
            pomsum++;
        }
    }
    //cout<<"jest tyle zlych wierzcholkow w przekorjach "<<pomsum<<endl;
  //zamek 1 jeszcze skonczyl tutaj to robic
  //cout<<"tutaj jeszcze robi. skonczyl robic boxy"<<endl;cout.flush();
//cout<<endl<<"przed dorobieniem krawedzi"<<endl;
/*
    for(int i=0;i<pokoje.size();i++){
        cout<<i<<" graniczy ";
        Pokoj pok=pokoje[i];
        for(int j=0;j<pok.sasiedzi.size();j++){
            cout<<pok.sasiedzi[j]<<" ";
        }
        cout<<endl;
    }
    */
    //wyglada na to, ze czasem w tej tej jest zdublowane dodawanie krawedzi. 
    
    dorob_krawedzie(pokoje); 

   //teraz poprawiamy
   for (int i=0;i<pokoje.size();i++){
       pokoje[i].y[0]=ylevels[pokoje[i].y[0]];
       pokoje[i].y[1]=ylevels[pokoje[i].y[1]];
   }

       

    //teraz mam te pokoje takie prawdziwe. 
    int pocz=znajdz(pokoje,wejscie);
    int koni=znajdz(pokoje,wyjscie);
  //  cout<<"przed dfs"<<endl;cout.flush();
  //  cout<<"ale po dorobieniu krawedzi";

    //jak widac niektore granicza ze sobo wiele razy. 
  //  cout.flush();
    vector<int>ojcowie=dfs(pokoje,koni);
   // cout<<"po dfs"<<endl;cout.flush();

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
   // cout.flush();



    





    return 0;
}
