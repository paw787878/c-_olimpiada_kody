#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

class Slownik_podslow_bazowych
{
  public:
    vector<vector<int>> tab; //pierwszy to potega 2, drugi to polozenie lewego konca danego slowa
    vector<int> wykladnik_mniejszej_lub_rownej_potegi_2;//jak w nazwie
    vector<int> potega_2;
    Slownik_podslow_bazowych(vector<int> &slowo);
    void dorob_warstwe(int chwilowa_potega);
    pair<int,int> identyfikator_slowa(int pocz,int konc);
};

pair<int,int>Slownik_podslow_bazowych::identyfikator_slowa(int pocz,int konc){
    //jak w pythonie
    int dlugosc=konc-pocz;
    int wyk=wykladnik_mniejszej_lub_rownej_potegi_2[dlugosc];
    int ta_pot=potega_2[wyk];
    int pierwsze_bazowe=tab[wyk][pocz];
    int drugie_bazowe=tab[wyk][konc-ta_pot];
    return pair<int,int>(pierwsze_bazowe,drugie_bazowe);
}


void kubelkowe_sortowanie(int (*int_z_pary)(pair<int, int>), vector<pair<pair<int, int>, int>> &pary, int n)
{
    //tworzymy tablice kubelkow
    vector<vector<int>> kubelki;
    kubelki.resize(n);
    for (int i = 0; i < pary.size(); i++)
    {
        kubelki[int_z_pary(pary[i].first)].push_back(i);
    }
    //teraz po prostu robimy od nowa nasz wektor
    vector<pair<pair<int, int>, int>> pom;
    pom = pary;
    pary.resize(0);

    for (int i = 0; i < kubelki.size(); i++)
        for (int j = 0; j < kubelki[i].size(); j++)
        {
            pary.push_back(pom[kubelki[i][j]]);
        }
}

void kubelkowe_sortowanie_2(int (*int_z_pary)(pair<int, int>), vector<pair<int,int> > &pary, int n)
{
    //tworzymy tablice kubelkow
    vector<vector<int>> kubelki;
    kubelki.resize(n);
    for (int i = 0; i < pary.size(); i++)
    {
        kubelki[int_z_pary(pary[i])].push_back(i);
    }
    //teraz po prostu robimy od nowa nasz wektor
    vector<pair<int, int> > pom;
    pom = pary;
    pary.resize(0);

    for (int i = 0; i < kubelki.size(); i++)
        for (int j = 0; j < kubelki[i].size(); j++)
        {
            pary.push_back(pom[kubelki[i][j]]);
        }
}


int pierwszy(pair<int, int> a)
{
    return a.first;
}

int drugi(pair<int, int> a)
{
    return a.second;
}

void nadaj_nazwy(vector<pair<pair<int, int>, int>> &pary, vector<int> &nowa_warstwa)
{
    //trzeba po kolei jakoś je jechać.
    nowa_warstwa.resize(pary.size());
    int wartosc_chwilowego = -1;
    for (int i = 0; i < pary.size(); i++)
    {
        if (i > 0 && pary[i].first == pary[i - 1].first)
        {
            //nic nie rob
        }
        else
        {
            wartosc_chwilowego++;
        }
        nowa_warstwa[pary[i].second] = wartosc_chwilowego;
    }
}

void Slownik_podslow_bazowych::dorob_warstwe(int chwilowa_potega)
{
    vector<pair<pair<int, int>, int>> pary;
    int ostatni = tab.size() - 1;
    for (int i = 0; i + chwilowa_potega < tab[ostatni].size(); i++)
    {
        pary.push_back(pair<pair<int, int>, int>(pair<int, int>(tab[ostatni][i], tab[ostatni][i + chwilowa_potega]), i));
    }
    //teraz trzeba te pary sortowac jakos
    kubelkowe_sortowanie(&drugi, pary, tab[ostatni].size());
    kubelkowe_sortowanie(&pierwszy, pary, tab[ostatni].size());
    //teraz z tego robimy nowa warstwe
    vector<int> nowa_warstwa;
    nadaj_nazwy(pary, nowa_warstwa);
    tab.push_back(nowa_warstwa);
}

void dorob_potegi_2(vector<int> & wyk_mn_lub_row_pot_2,int n){
    int potega_chwilowa=1;
    int co_wpisac=0;
    for(int i=0;i<n;i++){
        if(2*potega_chwilowa<=i){
            potega_chwilowa*=2;
            co_wpisac++;
        }
        wyk_mn_lub_row_pot_2.push_back(co_wpisac);
    }
}

Slownik_podslow_bazowych::Slownik_podslow_bazowych(vector<int> &slowo)
{
    //na poczatek rob pierwsza warstwe
    tab.push_back(slowo);
    //teraz trzeba bedzie dorabiac kolejne warstwy chyba
    int potega = 1; //to co teraz mamy zrobione
    potega_2.push_back(potega);
    while (2 * potega <= slowo.size())
    {
        dorob_warstwe(potega);
        potega *= 2;
        potega_2.push_back(potega);
    }
    dorob_potegi_2(wykladnik_mniejszej_lub_rownej_potegi_2,slowo.size());
    
}

void wypisz(vector<vector<int>> &tab)
{
    cout << endl;
    for (int i = 0; i < tab.size(); i++)
    {
        cout << endl;
        for (int j = 0; j < tab[i].size(); j++)
        {
            int co = tab[i][j];
            if (co < 10)
                cout << " ";
            cout << co << " ";
        }
    }
}

void wypisz(pair<int,int> a){
    cout<<endl<<a.first<<" "<<a.second<<endl;
}

pair<int,int> podaj_encoding(Slownik_podslow_bazowych& encoding_slow,int pocz,int konc){
    int pocz_rozmiar=encoding_slow.tab[0].size()/2;
    pair<int,int> encoding_1=encoding_slow.identyfikator_slowa(pocz,konc);
    pair<int,int> encoding_2=encoding_slow.identyfikator_slowa(2*pocz_rozmiar-konc,2*pocz_rozmiar-pocz);
    if(encoding_1.first<encoding_2.first)
        return encoding_1;
    if(encoding_1.first>encoding_2.first)
        return encoding_2;
    if(encoding_1.second<encoding_2.second)
        return encoding_1;
    return encoding_2;
}

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

bool mniejsza_para(pair<int,int>a, pair<int,int>b){
    if(a.first<b.first)
        return true;
    if(a.first>b.first)
        return false;
    if(a.second<b.second)
        return true;
    return false;
}

int licz_odpowiedz(vector<int> & rekordowe_k, vector<int>& slowo){
    int pocz_rozmiar=slowo.size();
    for(int i=pocz_rozmiar-1;i>=0;i--)
        slowo.push_back(slowo[i]);
    Slownik_podslow_bazowych encoding_slow (slowo);

    int rekordowe_ile_roznych=-1;
    
    

    for(int k=1;k<=pocz_rozmiar;k++){
        vector<pair<int,int> > pary;
        for(int pocz=0;pocz+k<=pocz_rozmiar;pocz+=k){
            pary.push_back(podaj_encoding(encoding_slow,pocz,pocz+k));
        }
        mergesort<pair<int,int> >(pary,0,pary.size(),&mniejsza_para);
        //teraz zliczamy ile jest roznych;
        int ile_roznych=0;
        for(int i=0;i<pary.size();i++){
            if(i>0 && pary[i-1]==pary[i]){
                //nic nie rob
            }else{
                ile_roznych++;
            }
        }
        //tu dalej piszesz
        if(ile_roznych>rekordowe_ile_roznych){
            rekordowe_k.resize(0);
            rekordowe_ile_roznych=ile_roznych;
        }
        if(ile_roznych==rekordowe_ile_roznych)
            rekordowe_k.push_back(k);
    }
    return rekordowe_ile_roznych;
}

int main()
{
    

    ifstream we("in/kor12c.in");
    int n;
    we>> n;
    vector<int> slowo;
    for(int i=0;i<n;i++){
        int pom;
        we>>pom;
        slowo.push_back(pom);
    }


    vector<int> rekordowe_k;
    int ile_roznych=licz_odpowiedz(rekordowe_k,slowo);
    cout<<ile_roznych<<" "<<rekordowe_k.size()<<endl;
    for(int i=0;i<rekordowe_k.size();i++)
        cout<<rekordowe_k[i]<<" ";


        cout << endl
         << "doszlo do return 0" << endl;
    return 0;
}
