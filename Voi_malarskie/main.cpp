#include <iostream>
#include <vector>
#include <assert.h>
#include <sstream>
#include <fstream>
using namespace std;

class Duza_liczba
{
  public:
    int n;
    vector<int> tab;
    int pods;
    int ile_cyfr_na_pojedynczy;
    Duza_liczba();
    Duza_liczba operator+(Duza_liczba &b);
    Duza_liczba operator-(Duza_liczba &b);
    bool operator>(Duza_liczba &b);
    void wczytaj(string napis);
    void wypisz();
};

class Info_dla_mozliwych_tlow
{
  public:
    Duza_liczba dane[2][2][2][2]; //to sie samo zainicjalizuje do samych 0
    //Duza_liczba potenga;
    void wpisz(bool gorny_lewy, bool gorny_prawy, bool dolny_lewy, bool dolny_prawy, Duza_liczba wartosc); //bool rowny 1 rekurencyjna struktura
    Duza_liczba odczytaj(bool gorny_lewy, bool gorny_prawy, bool dolny_lewy, bool dolny_prawy);
};

void Info_dla_mozliwych_tlow::wpisz(bool gorny_lewy, bool gorny_prawy, bool dolny_lewy, bool dolny_prawy, Duza_liczba wartosc)
{
    dane[gorny_lewy][gorny_prawy][dolny_lewy][dolny_prawy] = wartosc;
}

Duza_liczba Info_dla_mozliwych_tlow::odczytaj(bool gorny_lewy, bool gorny_prawy, bool dolny_lewy, bool dolny_prawy)
{
    return dane[gorny_lewy][gorny_prawy][dolny_lewy][dolny_prawy];
}

void Duza_liczba::wypisz()
{
    bool jechanie_zer = true;
    //cout<<"n dla mnie wynosi "<<n;
    for (int i = n - 1; i >= 0; i--)
        if ((!(tab[i] == 0 && jechanie_zer)) || i == 0)
        {
            //cout<<"wypisuje numer "<<i<<" w funkcji wypisz"<<endl;
            if(jechanie_zer)
                cout << tab[i];
            else{
                stringstream ss;
                ss<<tab[i];
                string pom;
                ss>>pom;
                for(int j=0;j<(ile_cyfr_na_pojedynczy-pom.size());j++)
                    cout<<0;
                cout<<pom;
            }
            jechanie_zer = false;
        }
}

void Duza_liczba::wczytaj(string napis)
{
    int napis_size = napis.size();
    int ile_cyfr_nadpisac = (napis_size / ile_cyfr_na_pojedynczy) + 1;
    for (int i = 0; i < ile_cyfr_nadpisac; i++)
    {
        int ile_odciac;
        if (i == ile_cyfr_nadpisac - 1)
            ile_odciac = napis_size - i * ile_cyfr_na_pojedynczy;
        else
            ile_odciac = ile_cyfr_na_pojedynczy;

        string odcinany = napis.substr(max(0, napis_size - (i + 1) * ile_cyfr_na_pojedynczy), ile_odciac);
        stringstream ss;
        ss << odcinany;
        ss >> tab[i];
    }
    for (int i = ile_cyfr_na_pojedynczy; i < n; i++)
        tab[i] == 0;
}

bool Duza_liczba::operator>(Duza_liczba &b)
{
    for (int i = n - 1; i >= 0; i--)
    {
        if (tab[i] > b.tab[i])
            return true;
        else if (tab[i] < b.tab[i])
            return false;
    }
    return false;
}

Duza_liczba Duza_liczba::operator-(Duza_liczba &b)
{
    assert((*this) > b);
    Duza_liczba wyrzut;
    Duza_liczba a;
    for (int i = 0; i < n; i++)
        a.tab[i] = tab[i];

    for (int i = 0; i < n; i++)
    {
        if (a.tab[i] < b.tab[i])
        {
            //teraz trzeba nazbieraz z tych wyzszych
            //cout<<"teraz zaczynam rozdystrybuudowac "<<endl;
            int j = i + 1;
            for (; j < n; j++)
                if (!(a.tab[j] == 0))
                    break;
            //teraz j to jest pierwsza nie zerowym
            //cout<<"j "<<j<<endl;
            for (int k = j; k >= i + 1; k--)
            {
                a.tab[k]--;
                a.tab[k - 1] += pods;
            }
        }
        wyrzut.tab[i] = a.tab[i] - b.tab[i];
    }
    return wyrzut;
}

Duza_liczba Duza_liczba::operator+(Duza_liczba &b)
{
    //cout<<"wywoluje doawanie"<<endl;
    Duza_liczba wyrzut;
    int pom;
    int z_poprzedniego = 0;
    for (int i = 0; i < n; i++)
    {
        //cout<<"i teraz wynosi "<<i<<endl;
        pom = tab[i] + b.tab[i] + z_poprzedniego;
        //cout<<"pom"<<pom<<endl;
        wyrzut.tab[i] = (pom % pods);
        //cout<<"dopisalem "<<(pom%pods)<<endl;
        z_poprzedniego = (pom / pods);
        //cout<<"z popzedniego wynosi teraz "<<z_poprzedniego<<endl;
    }
    return wyrzut;
}

Duza_liczba::Duza_liczba()
{
    //cout<<"wywoluje domyslny konstrutkor duzej liczby "<<endl;
    n = 20;
    ile_cyfr_na_pojedynczy = 8;
    pods = 1;
    for (int i = 0; i < ile_cyfr_na_pojedynczy; i++)
        pods *= 10;
    for (int i = 0; i < n; i++)
        tab.push_back(0);
}

//zaczynam wlasciwa funkcje

Info_dla_mozliwych_tlow liczba_zoltych(int n, Duza_liczba x, Duza_liczba y, vector<Duza_liczba> &potegi)
{
    //to jest odpowiedzia na pytanie, gdy mamy kwadrat-tlo zlozony z 4 mniejszych o bokach 2**n (kazdy moze byc rekurencyjny
    //lub pusty) i na srodku umieszczony kwadrat 2**n na 2**n (pelny) to ile wyjdzie.
    //dodatkowo tam jest tez w wyniku potenga rowna 2**n
    // 0<= x <= 2**n i tak samo y.
    /*
    cout << endl
         << "wywoluje info-dla_mozliwych_tlow n=" << n << " x=";
    x.wypisz();
    cout << " y=";
    y.wypisz();
    cout << endl;
    */
    Info_dla_mozliwych_tlow wyrzut;
    if (n == 0)
    {

        for (int gl = 0; gl < 2; gl++)
            for (int gr = 0; gr < 2; gr++)
                for (int dl = 0; dl < 2; dl++)
                    for (int dr = 0; dr < 2; dr++)
                    {
                        bool pom_bool_tab[2][2];
                        pom_bool_tab[0][0] = dl;
                        pom_bool_tab[1][0] = dr;
                        pom_bool_tab[0][1] = gl;
                        pom_bool_tab[1][1] = gr;
                        if (pom_bool_tab[x.tab[0]][y.tab[0]])
                        {
                            Duza_liczba a;
                            a.wczytaj("1");
                            wyrzut.wpisz(gl, gr, dl, dr, a);
                        }
                        else
                        {
                            Duza_liczba a;
                            wyrzut.wpisz(gl, gr, dl, dr, a);
                        }
                    }
    }
    else
    {
        //sprowadzamy do poprzedniego przypadku
        Duza_liczba jedna_czwarta = potegi[n - 1];
        Duza_liczba nowy_x = x;
        Duza_liczba nowy_y = y;
        int o_ile_x = 0, o_ile_y = 0;
        while (nowy_x > jedna_czwarta)
        {
            o_ile_x++;
            nowy_x = nowy_x - jedna_czwarta;
        }
        while (nowy_y > jedna_czwarta)
        {
            o_ile_y++;
            nowy_y = nowy_y - jedna_czwarta;
        }
        Info_dla_mozliwych_tlow rekurencyjna_informacja = liczba_zoltych(n - 1, nowy_x, nowy_y, potegi);
        //teraz potrzeba nasza pomocnicza tablice napisac
        bool plansza[4][4];
        vector<pair<int, int>> lewe_dolne;
        lewe_dolne.push_back(pair<int, int>(0 + o_ile_x, 0 + o_ile_y));
        lewe_dolne.push_back(pair<int, int>(1 + o_ile_x, 0 + o_ile_y));
        lewe_dolne.push_back(pair<int, int>(1 + o_ile_x, 1 + o_ile_y));

        //teraz trzeba je wszystkie przerobic.

        for (int gl = 0; gl < 2; gl++)
            for (int gr = 0; gr < 2; gr++)
                for (int dl = 0; dl < 2; dl++)
                    for (int dr = 0; dr < 2; dr++)
                    {
                        for (int i = 0; i < 4; i++)
                            for (int j = 0; j < 4; j++)
                                plansza[i][j] = false;

                        //teraz trzeba zrobic to nasze plansza
                        vector<pair<int, int>> do_dorobienia;
                        if (dl)
                            do_dorobienia.push_back(pair<int, int>(0, 0));
                        if (dr)
                            do_dorobienia.push_back(pair<int, int>(2, 0));
                        if (gl)
                            do_dorobienia.push_back(pair<int, int>(0, 2));
                        if (gr)
                            do_dorobienia.push_back(pair<int, int>(2, 2));
                        for (int i = 0; i < do_dorobienia.size(); i++)
                        {
                            plansza[do_dorobienia[i].first][do_dorobienia[i].second] = true;
                            plansza[do_dorobienia[i].first + 1][do_dorobienia[i].second] = true;
                            plansza[do_dorobienia[i].first + 1][do_dorobienia[i].second + 1] = true;
                        }
                        /*
                        cout<<endl<<"teraz czy plansza jest dobrze zrobiona"<<endl;
                        cout<<gl<<" "<<gr<<endl<<dl<<" "<<dr<<endl<<endl;
                        for(int i=3;i>=0;i--){
                            for(int j=0;j<4;j++)
                                cout<<plansza[j][i];
                            cout<<endl;
                        }
                        */
                       /*
                        cout << endl
                             << "plansza typu" << endl;
                        cout << gl << " " << gr << endl
                             << dl << " " << dr << endl
                             << endl;
                        
                        for (int i = 3; i >= 0; i--)
                        {
                            for (int j = 0; j < 4; j++)
                                cout << plansza[j][i];
                            cout << endl;
                        }
                        cout<<endl;
                        */
                        //teraz odczytujemy sobie
                        Duza_liczba suma;
                        for (int i = 0; i < 3; i++)
                        {
                            pair<int, int> lewy_dolny_punkt = lewe_dolne[i];
                            //cout << endl
                                 //<< "wsporzedne lewego dolnego malego kwadraciku " << lewy_dolny_punkt.first << " " << lewy_dolny_punkt.second << endl;
                            bool gorny_lewy = plansza[lewy_dolny_punkt.first][lewy_dolny_punkt.second + 1];
                            bool gorny_prawy = plansza[lewy_dolny_punkt.first + 1][lewy_dolny_punkt.second + 1];
                            bool dolny_lewy = plansza[lewy_dolny_punkt.first][lewy_dolny_punkt.second];
                            bool dolny_prawy = plansza[lewy_dolny_punkt.first + 1][lewy_dolny_punkt.second];
                            /*
                            cout << endl
                                 << "mala plansza jest taka" << endl;
                            cout << gorny_lewy << " " << gorny_prawy << endl
                                 << dolny_lewy << " " << dolny_prawy << endl
                                 << endl;
                            */
                            Duza_liczba kurde = (rekurencyjna_informacja.odczytaj(gorny_lewy, gorny_prawy, dolny_lewy, dolny_prawy));

                            suma = suma + kurde;
                        }
                        wyrzut.wpisz(gl, gr, dl, dr, suma);
                    }
    }
    return wyrzut;
}

int main()
{


    
    
    vector<Duza_liczba> potegi;
    Duza_liczba a;
    a.wczytaj("1");

    //a.wypisz();

    for (int i = 0; i < 101; i++)
    {
        potegi.push_back(a);
        a = a + a;
    }

   int n;
   cin>>n;

    Duza_liczba x, y;
    string pom;
    cin>>pom;
    x.wczytaj(pom);
    cin>>pom;
    y.wczytaj(pom);
    liczba_zoltych(n, x, y, potegi).odczytaj(false, false, true, false).wypisz();




   
    return 0;
}
