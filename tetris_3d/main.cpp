#include <iostream>
#include <vector>
#include <fstream>
#include <time.h>
//#include <queue>
using namespace std;

int nastepna_potega(int a)
{
    int wyrzut = 1;
    while (wyrzut < a)
        wyrzut *= 2;
    return wyrzut;
}

void rob_1d_podzial_i_przodkow(int roz, int pocz, int koni, vector<int> &podzial, vector<int> &przodkowie)
{
    //roz to jest najmniejsza potega 2 co jest nie mniejsza niz te nasze rzeczy. pocz,koni sa od 0 do roz-1
    //pocz oraz koni nlaeza do przedzialu
    //na poczatek trzeba to jakos skleic
    int a = roz + pocz;
    int b = roz + koni;

    if (a != b)
    {
        podzial.push_back(a);
        podzial.push_back(b);

        while (a / 2 != b / 2)
        {
            //tutaj juz a oraz b sa obrobione
            //ich ojcowie sa rozne
            if (a % 2 == 0)
                podzial.push_back(a + 1);
            if (b % 2 == 1)
                podzial.push_back(b - 1);
            a = a / 2;
            b = b / 2;
            przodkowie.push_back(a);
            przodkowie.push_back(b);
        }
        //teraz mamy juz ziomeczkow co maja wspolnego ojca. ziomeczki sa obrobione
        a = a / 2;
        b = b / 2;
        przodkowie.push_back(a);
    }
    else
    {
        podzial.push_back(a);
    }
    //teraz mamy tak, ze jest tylko a i jest obrobiony juz.
    while (a / 2 != 0)
    {
        a = a / 2;
        przodkowie.push_back(a);
    }
}

class Node_1d
{
  public:
    int max_w_poddrzewie;
    int domaxuj_spod;
    Node_1d()
    {
        max_w_poddrzewie = 0;
        domaxuj_spod = 0;
    }
};

class Struktura_1d
{
  public:
    int roz;
    vector<Node_1d> tab_nodow;
    void domaxuj_odcinek(vector<int>& podstawa,vector<int>& przodkowie, int wartosc); //od 0 do roz-1 konce tez sie licza do odcinka
    int max_na_odcinku(vector<int>& podstawa,vector<int>& przodkowie);
    Struktura_1d(int n)
    {
        roz = nastepna_potega(n);
        tab_nodow.resize(2 * roz);
    }
};

void domaxowywanie(int &a, int b)
{
    if(b>a)
        a=b;
}

void Struktura_1d::domaxuj_odcinek(vector<int>& podstawa,vector<int>& przodkowie, int wartosc)
{
    for (int i = 0; i < podstawa.size(); i++)
    {
        domaxowywanie(tab_nodow[podstawa[i]].domaxuj_spod, wartosc);
        domaxowywanie(tab_nodow[podstawa[i]].max_w_poddrzewie, wartosc);
    }
    for (int i = 0; i < przodkowie.size(); i++)
    {
        int ktory = przodkowie[i];
        domaxowywanie(tab_nodow[ktory].max_w_poddrzewie, wartosc);
    }
}

int Struktura_1d::max_na_odcinku(vector<int>& podstawa,vector<int>& przodkowie)
{
    int maximum = 0;
    for (int i = 0; i < podstawa.size(); i++)
        if (tab_nodow[podstawa[i]].max_w_poddrzewie > maximum)
            maximum = tab_nodow[podstawa[i]].max_w_poddrzewie;
    for (int i = 0; i < przodkowie.size(); i++)
        if (tab_nodow[przodkowie[i]].domaxuj_spod > maximum)
            maximum = tab_nodow[przodkowie[i]].domaxuj_spod;
    return maximum;
}

class Node_2d
{
  public:
    Struktura_1d max_spod;
    Struktura_1d domaxowanie_spodu;
    Node_2d(int roz): max_spod(roz), domaxowanie_spodu(roz)
    {
    }
};

class Struktura_2d
{
  public:
    int roz_x;
    int roz_y;
    vector<Node_2d> tab_nodow;
    //int znajdz_max_prostokata(int ax,int bx,int ay,int by);
    //void domaxuj_prostokata(int ax,int bx,int ay,int by,int wartosc);
    void upusc_prostokata(int ax,int bx,int ay,int by,int wysokosc);
    int wysokosc();

    Struktura_2d(int bok_x, int bok_y)
    {
        roz_x = nastepna_potega(bok_x);
        roz_y=nastepna_potega(bok_y);
        for (int i = 0; i < 2 * roz_x; i++)
        {
            tab_nodow.push_back(Node_2d(bok_y));
        }
    }
};

int Struktura_2d::wysokosc(){
    return tab_nodow[1].max_spod.tab_nodow[1].max_w_poddrzewie;
}

void Struktura_2d::upusc_prostokata(int ax,int bx,int ay,int by,int wysokosc){
    vector<int> podstawa_x,podstawa_y,przodkowie_x,przodkowie_y;
    rob_1d_podzial_i_przodkow(roz_x,ax,bx,podstawa_x,przodkowie_x);
    rob_1d_podzial_i_przodkow(roz_y,ay,by,podstawa_y,przodkowie_y);

    int maximum=0;
    for (int i = 0; i < podstawa_x.size(); i++){
        //cout<<"teraz domaxowywuje element "<<podstawa[i]<<"na odcinku "<<ay<<" "<<by<<endl;
        domaxowywanie(maximum,tab_nodow[podstawa_x[i]].max_spod.max_na_odcinku(podstawa_y,przodkowie_y));
        //cout<<"max wynosi "<<maximum<<endl;
    }
    //cout<<"druga petla znajdz_max"<<endl;
    for (int i = 0; i < przodkowie_x.size(); i++){
        //cout<<"max wynosi "<<maximum<<endl;
        domaxowywanie(maximum,tab_nodow[przodkowie_x[i]].domaxowanie_spodu.max_na_odcinku(podstawa_y,przodkowie_y));
    }
    //cout<<"zwracane maximum to "<<maximum<<endl;

    int wartosc=maximum+wysokosc;

    for(int i=0;i<podstawa_x.size();i++){
        int ktory=podstawa_x[i];
        tab_nodow[ktory].max_spod.domaxuj_odcinek(podstawa_y,przodkowie_y,wartosc);
        tab_nodow[ktory].domaxowanie_spodu.domaxuj_odcinek(podstawa_y,przodkowie_y,wartosc);
    }
    for(int i=0;i<przodkowie_x.size();i++){
        int ktory=przodkowie_x[i];
        tab_nodow[ktory].max_spod.domaxuj_odcinek(podstawa_y,przodkowie_y,wartosc);
    }

}
/*
void Struktura_2d::domaxuj_prostokata(int ax,int bx,int ay,int by,int wartosc){
    vector<int> podstawa;
    vector<int> przodkowie;
    rob_1d_podzial_i_przodkow(roz_x,ax,bx,podstawa,przodkowie);
    for(int i=0;i<podstawa.size();i++){
        int ktory=podstawa[i];
        tab_nodow[ktory].max_spod.domaxuj_odcinek(ay,by,wartosc);
        tab_nodow[ktory].domaxowanie_spodu.domaxuj_odcinek(ay,by,wartosc);
    }
    for(int i=0;i<przodkowie.size();i++){
        int ktory=przodkowie[i];
        tab_nodow[ktory].max_spod.domaxuj_odcinek(ay,by,wartosc);
    }
}

int Struktura_2d::znajdz_max_prostokata(int ax,int bx,int ay,int by){
    //cout<<"zaczynam znajdz max prostokataa"<<endl;
    vector<int> podstawa;
    vector<int> przodkowie;
    rob_1d_podzial_i_przodkow(roz_x,ax,bx,podstawa,przodkowie);
    //cout<<"podstawa to "<<endl;
    //cout<<endl;
    //cout<<"przodkowie to "<<endl;

    int maximum=0;
    for (int i = 0; i < podstawa.size(); i++){
        //cout<<"teraz domaxowywuje element "<<podstawa[i]<<"na odcinku "<<ay<<" "<<by<<endl;
        domaxowywanie(maximum,tab_nodow[podstawa[i]].max_spod.max_na_odcinku());
        //cout<<"max wynosi "<<maximum<<endl;
    }
    //cout<<"druga petla znajdz_max"<<endl;
    for (int i = 0; i < przodkowie.size(); i++){
        //cout<<"max wynosi "<<maximum<<endl;
        domaxowywanie(maximum,tab_nodow[przodkowie[i]].domaxowanie_spodu.max_na_odcinku(ay,by));
    }
    //cout<<"zwracane maximum to "<<maximum<<endl;
    return maximum;
}
*/
int main()
{
    //ifstream we("dane.txt");
    int D, S, N;
    cin >> D >> S >> N;
    Struktura_2d plan(D, S);
   // cout<<plan.znajdz_max_prostokata(0,0,0,0)<<endl;
    
    int d, s, w, x, y;
    for (int i = 0; i < N; i++)
    {
        cin >> d >> s >> w >> x >> y;
        plan.upusc_prostokata(x, x + d - 1, y, y + s - 1, w);
    }
    cout<<plan.wysokosc();
    
    //cout << endl
         //<< "doszlo do return 0" << endl;
    return 0;
}
