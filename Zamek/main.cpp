#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <utility>

using namespace std;

class Node{
    public:

    //liczac jego samego
    int ile_pod;
    pair<int,int> synowie;
    int ojciec;
    bool zajety;
    Node(){
    ile_pod=0;
    synowie=pair<int,int>(-1,-1);
    ojciec=-1;zajety=false;}


};

class Drzewo{
public:
    vector<Node> tab;
    int start;

    Drzewo(int);
    void wsadz(int);
    void usun(int);
    int wiekszy_lub_rowny(int);
    int poddrzewa_min(int);
    int poddrzewa_max(int);
    int mniejszy_lub_rowny(int);
};

int Drzewo::poddrzewa_min(int root){
    int i=root;
    while(true){
        if(tab[i].synowie.first!=-1&&tab[tab[i].synowie.first].ile_pod!=0)
            {i=tab[i].synowie.first;continue;}
        if(tab[i].zajety)
            return i;
        if(tab[i].synowie.second!=-1&&tab[tab[i].synowie.second!=-1].ile_pod!=0)
            i=tab[i].synowie.second;

    }

}

int Drzewo::poddrzewa_max(int root){
    int i=root;
    while(true){
        if(tab[i].synowie.second!=-1&&tab[tab[i].synowie.second].ile_pod!=0)
            {i=tab[i].synowie.second;continue;}
        if(tab[i].zajety)
            return i;
        if(tab[i].synowie.first!=-1&&tab[tab[i].synowie.first!=-1].ile_pod!=0)
            i=tab[i].synowie.first;

    }

}

int Drzewo::wiekszy_lub_rowny(int od_ile){
    /* zaczynamy od naszego elementu a nastepnie jak idziemy do gory
    w prawo to sprawdzamy czy ten ma albo czy te na prawo maja cos.
    */
    if (tab[od_ile].zajety)
        return od_ile;
    //zaczynam petle gdy nie ma nic w tym czyms i trzeba szukac na prawo
    if(tab[od_ile].synowie.second!=-1&&tab[tab[od_ile].synowie.second].ile_pod!=0)
        return poddrzewa_min(tab[od_ile].synowie.second);
    int i=od_ile;
    while(tab[i].ojciec!=-1){

        int poprzedni=i;
        i=tab[i].ojciec;
        if(tab[i].synowie.first==poprzedni){
            if(tab[i].zajety)
                return i;
            if(tab[i].synowie.second!=-1&&tab[tab[i].synowie.second].ile_pod!=0){
                return poddrzewa_min(tab[i].synowie.second);
            }


        }


    }
    return -1;

}

int Drzewo::mniejszy_lub_rowny(int od_ile){
    /* zaczynamy od naszego elementu a nastepnie jak idziemy do gory
    w prawo to sprawdzamy czy ten ma albo czy te na prawo maja cos.
    */
    if (tab[od_ile].zajety)
        return od_ile;
    if(tab[od_ile].synowie.first!=-1&&tab[tab[od_ile].synowie.first].ile_pod!=0)
        return poddrzewa_max(tab[od_ile].synowie.first);
    //zaczynam petle gdy nie ma nic w tym czyms i trzeba szukac na prawo
    int i=od_ile;
    int gdzie_szukac=-1;
    while(tab[i].ojciec!=-1){
        int poprzedni=i;
        i=tab[i].ojciec;
        if(tab[i].synowie.second==poprzedni){
            if(tab[i].zajety)
                return i;
            if(tab[i].synowie.first!=-1&&tab[tab[i].synowie.first].ile_pod!=0){
                return poddrzewa_max(tab[i].synowie.first);
            }


        }


    }
    return -1;

}

void Drzewo::usun(int wartosc){
    if(tab[wartosc].zajety==true){
        tab[wartosc].zajety=false;
        int i=start;
        while (true){
        tab[i].ile_pod--;
        if(wartosc==i)
            break;
        if(wartosc>i)
            i=tab[i].synowie.second;
        else
            i=tab[i].synowie.first;
        }
    }
}

void Drzewo::wsadz(int wartosc){
    if(tab[wartosc].zajety==false){
        tab[wartosc].zajety=true;
        int i=start;
        while (true){
        tab[i].ile_pod++;
        if(wartosc==i)
            break;
        if(wartosc>i)
            i=tab[i].synowie.second;
        else
            i=tab[i].synowie.first;
        }
    }
}

void polaczenia(vector<Node> &tab,int ojciec,int a,
int b,bool syn_przerosl){
    if(a!=b){
        int s=a+(b-a)/2;
        tab[s].ojciec=ojciec;
        if(ojciec!=-1)
        if (syn_przerosl)
            tab[ojciec].synowie.second=s;
        else
            tab[ojciec].synowie.first=s;
        polaczenia(tab,s,s+1,b,true);
        polaczenia(tab,s,a,s,false);
    }
}


Drzewo::Drzewo(int n){
for(int i=0;i<n;i++)
    tab.push_back(Node());
start=n/2;
polaczenia(tab,-1,0,tab.size(),true);
}


class Pokoj{
    public:
    vector<int> lewi_sasiedzi;
    pair<int,int> y;
    pair<int,int> x;
};

template <class T>
void mergesort(vector<T> &tab,int a,int b,
bool (*mniejsze)(T,T)){
    //a oraz b to sa jak w pythonie
    if(!(b-a==0 || b-a==1)){
        int s=a+ (b-a)/2;
        mergesort(tab,a,s,mniejsze);
        mergesort(tab,s,b,mniejsze);
        vector<T> pier;
        vector<T> drug;
        for (int i=a;i<s;i++)
            pier.push_back(tab[i]);
        for (int i=s;i<b;i++)
            drug.push_back(tab[i]);

        int ipier=0;int idrug=0;
        for(int i=a;i<b;i++){
            if (ipier==pier.size()){
                tab[i]=drug[idrug];idrug++;}
            else if(idrug==drug.size()){
                tab[i]=pier[ipier];ipier++;}
            else if(mniejsze(pier[ipier],drug[idrug])){
                tab[i]=pier[ipier];ipier++;
                }
            else{
                tab[i]=drug[idrug];idrug++;
                }
        }

    }
}

int bin_search(vector<int> &tab, int wartosc){
    int a=0;
    int b=tab.size();
    while(b-a>1){
        int s=a+(b-a)/2;
        if(tab[s]>wartosc)
            b=s;
        else
            a=s;
    }
    return a;

}

bool bok_min( pair<int,pair<int,int> > poz1,pair<int,pair<int,int> > poz2){
    if (poz1.first==poz2.first){
        if (poz1.second.first<poz2.second.first)
            return true;
        else
            return false;

    }else{
        if(poz1.first<poz2.first)
            return true;
        else
            return false;
    }
}


int main()
{
    // zacznijmy od wczytania danych
    ifstream we;
    we.open("dane");
    int n;
    we>>n;
    vector<pair<int,int> > wierz;
    for(int i=0;i<n;i++){
        int a;
        int b;
        we>>a;we>>b;
        wierz.push_back(pair<int,int>(a,b));
    }
    int a;int b;we>>a;we>>b;
    pair<int,int> wejscie (a,b);
    we>>a;we>>b;
    pair<int,int> wyjscie (a,b);
    //koniec wczytywania


    //teraz szukaj krawedzi
    //x a potem ymin ymax
    vector<pair<int,pair<int,int> > > pionowe;
    vector<pair<int,pair<int,int> > > poziome;
    for(int i=0;i<wierz.size()-1;i++){
        if (wierz[i].first==wierz[i+1].first){
            pionowe.push_back(pair<int,pair<int,int> > (wierz[i].first,pair<int,int>(
            min(wierz[i].second,wierz[i+1].second),
            max(wierz[i].second,wierz[i+1].second))));
            }
            else
            poziome.push_back(pair<int,pair<int,int> > (wierz[i].second,pair<int,int>(
            min(wierz[i].first,wierz[i+1].first),
            max(wierz[i].first,wierz[i+1].first))));
    }
        if (wierz[0].first==wierz[wierz.size()-1].first){
            pionowe.push_back(pair<int,pair<int,int> > (wierz[0].first,pair<int,int>(
            min(wierz[0].second,wierz[wierz.size()-1].second),
            max(wierz[0].second,wierz[wierz.size()-1].second))));
            }
            else
            poziome.push_back(pair<int,pair<int,int> > (wierz[0].second,pair<int,int>(
            min(wierz[0].first,wierz[wierz.size()-1].first),
            max(wierz[0].first,wierz[wierz.size()-1].first))));

    //teraz trzeba zrobic z nich posortowanie
    mergesort(poziome,0,poziome.size(),bok_min);
    mergesort(pionowe,0,pionowe.size(),bok_min);

    //teraz potrzeba to stopologizowac
    //to znaczy tylko ylevele sie wprowadza
    vector<int> ylevels;
    for (int i=0;i<poziome.size();i++){
        if(ylevels.empty()||(ylevels[ylevels.size()-1]!=poziome[i].first)){
            ylevels.push_back(poziome[i].first);
        }
    }
    //teraz trzeba zrobic lepsza wersje tych pionowych
    vector<pair<int,pair<int,int> > > n_pionowe;
    for(int i=0;i<pionowe.size();i++){
        pionowe[i]=pair<int,pair<int,int> >(pionowe[i].first,pair<int,int>(
        bin_search(ylevels,pionowe[i].second.first),
        bin_search(ylevels,pionowe[i].second.second)));
    }

    //teraz pionowe ma znormalizowane przy pomocy poziomow ylevels pionowe boki
    //a poziome jest nie ruszane.
    //bo teraz potrzebna bedzie tylko tablica n_pionowe
    //pokoj_levelu zawiera infomracje jak sie nazywa ten pokoj co sie tutaj zaczyna.
    //kazdy pokoj zawiera w sobie informacje o tym z ktorym graniczy od lewej
    //oraz o levelu na ktorym sie zaczyna oraz konczy oraz x na ktorym sie zaczyna i konczy.
    vector<int> pokoj_levelu;
    for(int i=0;i<ylevels.size();i++){
        pokoj_levelu.push_back(-1);
    }
    vector<Pokoj> pokoje;

    cout<<"kurwa";

    Drzewo nowe (101);
    nowe.wsadz(2);
    nowe.wsadz(5);
    nowe.wsadz(30);
    nowe.wsadz(44);
    nowe.usun(44);
    nowe.wsadz(40);



    int k=45;

    cout<<endl<<nowe.wiekszy_lub_rowny(k)<<endl<<nowe.mniejszy_lub_rowny(k)<<endl;
    cout<<endl;
//cout<<flush;
cout.flush();
    //cout<<endl<<nowe.mniejszy_lub_rowny(k);




    return 0;
}
