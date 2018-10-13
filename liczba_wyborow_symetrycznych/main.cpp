#include <iostream>
#include <fstream>
#include <vector>
#include <assert.h>
#include <sstream>
using namespace std;

class Wybor
{
  public:
    //int nr_wyboru;
    bool wyb_pierwszy;
    Wybor(bool wp)
    {
        //nr_wyboru=nw;
        wyb_pierwszy = wp;
    }
};
class Para
{
  public:
    int lewe_slowo_poczatek;
    int lewe_slowo_koniec;
    int prawe_slowo_poczatek;
    int prawe_slowo_koniec;
    Para(int lp, int lk, int pp, int pk)
    {
        lewe_slowo_koniec = lk;
        lewe_slowo_poczatek = lp;
        prawe_slowo_koniec = pk;
        prawe_slowo_poczatek = pp;
    }
    void wypisz()
    {
        cout << endl
             << "lp " << lewe_slowo_poczatek << " lk " << lewe_slowo_koniec << " pp " << prawe_slowo_poczatek << " pk " << prawe_slowo_koniec << endl;
    }
};

void wczytywanie(string nazwa_pliku, vector<char> &skonkatenowane_slowa,
                 vector<Wybor> &jakie_to_wybory, int &n, vector<int> &nr_pary, vector<Para> &wektor_par)
{
    ifstream we(nazwa_pliku);
    string pierwsza_linia;
    // getline(we,pierwsza_linia);
    //stringstream ss;
    //ss<<pierwsza_linia;
    //ss>>n;
    we >> n;
    vector<string> pierwsze;
    vector<string> drugie;
    string pom_string;

    for (int i = 0; i < n; i++)
    {
        //getline(we,pom_string);
        we >> pom_string;
        //cout<<endl<<"wczytany string do pierwszych to "<<pom_string<<" "<<pom_string.size();
        pierwsze.push_back(pom_string);
    }
    for (int i = 0; i < n; i++)
    {
        //getline(we,pom_string);
        we >> pom_string;
        //cout<<endl<<"wczytany string do drugich to "<<pom_string<<" "<<pom_string.size();
        drugie.push_back(pom_string);
    }
    for (int i = 0; i < n; i++)
    {
        int terazniejszy_index = skonkatenowane_slowa.size();
        Para wkladana(terazniejszy_index, terazniejszy_index + pierwsze[i].size() - 1,
                      terazniejszy_index + pierwsze[i].size(), terazniejszy_index + pierwsze[i].size() + drugie[i].size() - 1);
        wektor_par.push_back(wkladana);
        int dl_slowa;
        string obrabiane_slowo;

        dl_slowa = pierwsze[i].size();
        obrabiane_slowo = pierwsze[i];
        for (int j = 0; j < dl_slowa; j++)
        {
            skonkatenowane_slowa.push_back(obrabiane_slowo[j]);
            Wybor wyb_pom(true);
            jakie_to_wybory.push_back(wyb_pom);
            nr_pary.push_back(i);
        }

        dl_slowa = drugie[i].size();
        obrabiane_slowo = drugie[i];
        for (int j = 0; j < dl_slowa; j++)
        {
            skonkatenowane_slowa.push_back(obrabiane_slowo[j]);
            Wybor wyb_pom(false);
            jakie_to_wybory.push_back(wyb_pom);
            nr_pary.push_back(i);
        }
    }
}

void wczytywanie_z_standardowego( vector<char> &skonkatenowane_slowa,
                 vector<Wybor> &jakie_to_wybory, int &n, vector<int> &nr_pary, vector<Para> &wektor_par)
{
    //ifstream we(nazwa_pliku);
    string pierwsza_linia;
    // getline(we,pierwsza_linia);
    //stringstream ss;
    //ss<<pierwsza_linia;
    //ss>>n;
    cin >> n;
    vector<string> pierwsze;
    vector<string> drugie;
    string pom_string;

    for (int i = 0; i < n; i++)
    {
        //getline(we,pom_string);
        cin >> pom_string;
        //cout<<endl<<"wczytany string do pierwszych to "<<pom_string<<" "<<pom_string.size();
        pierwsze.push_back(pom_string);
    }
    for (int i = 0; i < n; i++)
    {
        //getline(we,pom_string);
        cin >> pom_string;
        //cout<<endl<<"wczytany string do drugich to "<<pom_string<<" "<<pom_string.size();
        drugie.push_back(pom_string);
    }
    for (int i = 0; i < n; i++)
    {
        int terazniejszy_index = skonkatenowane_slowa.size();
        Para wkladana(terazniejszy_index, terazniejszy_index + pierwsze[i].size() - 1,
                      terazniejszy_index + pierwsze[i].size(), terazniejszy_index + pierwsze[i].size() + drugie[i].size() - 1);
        wektor_par.push_back(wkladana);
        int dl_slowa;
        string obrabiane_slowo;

        dl_slowa = pierwsze[i].size();
        obrabiane_slowo = pierwsze[i];
        for (int j = 0; j < dl_slowa; j++)
        {
            skonkatenowane_slowa.push_back(obrabiane_slowo[j]);
            Wybor wyb_pom(true);
            jakie_to_wybory.push_back(wyb_pom);
            nr_pary.push_back(i);
        }

        dl_slowa = drugie[i].size();
        obrabiane_slowo = drugie[i];
        for (int j = 0; j < dl_slowa; j++)
        {
            skonkatenowane_slowa.push_back(obrabiane_slowo[j]);
            Wybor wyb_pom(false);
            jakie_to_wybory.push_back(wyb_pom);
            nr_pary.push_back(i);
        }
    }
}

vector<pair<int, int>> krawedzie(pair<int, int> rozwazany, vector<char> &skonkatenowane_slowa,
                                 vector<Wybor> &jakie_to_wybory, vector<int> &nr_pokojow, vector<Para> &wektor_pokojow)
{
    //od first wlacznie do second wlacznie jest teren jeszcze sie nie rowna nie jest wlczony do polindroma
    //specjalny -1,-1 to krawedzi do celu to jest do sytuacji gdy wszystko zostalo spalindromowane
    //specjalny -2,-2 to jest poczatkowy
    assert(rozwazany.first <= rozwazany.second);
    vector<pair<int, int>> wyrzut;
    if (rozwazany.first == -2 && rozwazany.second == -2)
    {
        Para pierwsza = wektor_pokojow[0];
        Para ostatnia = wektor_pokojow[wektor_pokojow.size() - 1];
        if (wektor_pokojow.size() >= 2)
        {
            wyrzut.push_back(pair<int, int>(pierwsza.lewe_slowo_poczatek, ostatnia.prawe_slowo_koniec));
            wyrzut.push_back(pair<int, int>(pierwsza.lewe_slowo_poczatek, ostatnia.lewe_slowo_koniec));
            wyrzut.push_back(pair<int, int>(pierwsza.prawe_slowo_poczatek, ostatnia.prawe_slowo_koniec));
            wyrzut.push_back(pair<int, int>(pierwsza.prawe_slowo_poczatek, ostatnia.lewe_slowo_koniec));
        }
        else
        {
            wyrzut.push_back(pair<int, int>(pierwsza.lewe_slowo_poczatek, ostatnia.lewe_slowo_koniec));
            wyrzut.push_back(pair<int, int>(pierwsza.prawe_slowo_poczatek, ostatnia.prawe_slowo_koniec));
        }
        return wyrzut;
    }
    assert(0 <= rozwazany.first && rozwazany.first < skonkatenowane_slowa.size() - 1 &&
           0 < rozwazany.second && rozwazany.second < skonkatenowane_slowa.size() && skonkatenowane_slowa.size() == jakie_to_wybory.size());
    //bo trzeba dbac o to by nie bylo raka.

    if (skonkatenowane_slowa[rozwazany.first] != skonkatenowane_slowa[rozwazany.second])
    {
        //to znaczy, ze jest zle, nie dojdziemy z tego nigdzie
        return wyrzut;
    }
    if (rozwazany.first == rozwazany.second)
    {
        wyrzut.push_back(pair<int, int>(-1, -1)); //po prostu do konca wskazuje
        return wyrzut;
    }
    //teraz sa taie same wiec na pewno jakos skurczymy, chyba ze pracujemy na zlych wyborach.
    /*
    if(rozwazany.second-rozwazany.first==1){
        bool b1=jakie_to_wybory[rozwazany.first].wyb_pierwszy;
        bool b2=jakie_to_wybory[rozwazany.second].wyb_pierwszy;
        if(b1==b2||(nr_pokojow[rozwazany.first]!=nr_pokojow[rozwazany.second]))
            wyrzut.push_back(pair<int,int>(-1,-1));//po prostu do konca wskazuje
        return wyrzut;
    }
    */
    
    

    //ponizej nie wyjdziemy poza talice bo assert jest taki.
    bool z_lewej_takie_same = (jakie_to_wybory[rozwazany.first].wyb_pierwszy == jakie_to_wybory[rozwazany.first + 1].wyb_pierwszy);
    bool z_prawej_takie_same = (jakie_to_wybory[rozwazany.second].wyb_pierwszy == jakie_to_wybory[rozwazany.second - 1].wyb_pierwszy);
    bool first_bool = jakie_to_wybory[rozwazany.first].wyb_pierwszy;
    bool second_bool = jakie_to_wybory[rozwazany.second].wyb_pierwszy;
    //teraz musze rozpoznac kiedy dojdzie w nastepnym kroku do anihilacji.
    bool dojdzie_do_anihilacji = false;
    if(rozwazany.second-rozwazany.first==1&&first_bool==second_bool)
        dojdzie_do_anihilacji=true;
    //ale jeszcze moga byc tak na odleglosc.
    if((!z_lewej_takie_same)&&(!z_prawej_takie_same)&&nr_pokojow[rozwazany.second]-
    nr_pokojow[rozwazany.first]==1)
        dojdzie_do_anihilacji=true;
    if(dojdzie_do_anihilacji){
        wyrzut.push_back(pair<int, int>(-1, -1)); //po prostu do konca wskazuje
        return wyrzut;
    }
    //teraz pozbylismy sie tych przypadkow gdzie cos tam sie anihiluje. 
    vector<int> mozliwe_first;
    vector<int> mozliwe_second;
    /*
    cout<<"z lewej taki esame"<<z_lewej_takie_same;
    cout<<endl<<"z prawej takie same"<<z_prawej_takie_same;
    cout<<endl<<"first_bool"<<first_bool;
    cout<<endl<<"second_bool"<<second_bool;
    */

    if (z_lewej_takie_same)
    {
        mozliwe_first.push_back(rozwazany.first + 1);
    }
    else
    {
        int start_nowej_pary;
        if (first_bool)
            start_nowej_pary = wektor_pokojow[nr_pokojow[rozwazany.first]].prawe_slowo_koniec + 1;
        else
            start_nowej_pary = rozwazany.first + 1;
        //teraz start nowej pary to jest dobry
        if (start_nowej_pary == skonkatenowane_slowa.size())
        {
            //nic dla nas nie ma nowego
        }
        else
        {
            Para nowa = wektor_pokojow[nr_pokojow[start_nowej_pary]];
            mozliwe_first.push_back(nowa.lewe_slowo_poczatek);
            mozliwe_first.push_back(nowa.prawe_slowo_poczatek);
        }
    }
    //teraz mozliwe_first ma w sobie jakie sa mozliwe noew w przyszylym ruchu
    if (z_prawej_takie_same)
    {
        mozliwe_second.push_back(rozwazany.second - 1);
    }
    else
    {
        int koniec_poprzedniej_pary;
        if (!second_bool)
            koniec_poprzedniej_pary = wektor_pokojow[nr_pokojow[rozwazany.second]].lewe_slowo_poczatek - 1;
        else
            koniec_poprzedniej_pary = rozwazany.second - 1;
        //teraz start nowej pary to jest dobry
        if (koniec_poprzedniej_pary == -1)
        {
            //nic dla nas nie ma nowego
        }
        else
        {
            Para nowa = wektor_pokojow[nr_pokojow[koniec_poprzedniej_pary]];
            mozliwe_second.push_back(nowa.lewe_slowo_koniec);
            mozliwe_second.push_back(nowa.prawe_slowo_koniec);
        }
    }

    //teraz nastepuje taka sytuacja, ze tworzymy nowe te
    for (int i = 0; i < mozliwe_first.size(); i++)
        for (int j = 0; j < mozliwe_second.size(); j++)
        {
            pair<int, int> potencjalna_para(mozliwe_first[i], mozliwe_second[j]);
            bool czy_dobre = true;
            if (nr_pokojow[potencjalna_para.first] == nr_pokojow[potencjalna_para.second] &&
                jakie_to_wybory[potencjalna_para.first].wyb_pierwszy != jakie_to_wybory[potencjalna_para.second].wyb_pierwszy)
                czy_dobre = false;
            //to jest dobre, bo wyeliminowalismy wczesniej przypadke gdy obydwa 
            //przeskakuja naraz
            if (czy_dobre)
                wyrzut.push_back(pair<int, int>(mozliwe_first[i], mozliwe_second[j]));
        }

    return wyrzut;
}

void wypisz_wektor_par_int(vector<pair<int, int>> &wypisywany)
{
    cout << endl;
    for (int i = 0; i < wypisywany.size(); i++)
    {
        cout << wypisywany[i].first << " " << wypisywany[i].second << ", ";
    }
    cout << endl;
}

int na_ile_sposobow(pair<int, int> &rozwazany, vector<vector<int>> &odpowiedzi,
                    vector<char> &skonkatenowane_slowa, vector<Wybor> &jakie_to_wybory, vector<int> &nr_pary, vector<Para> &wektor_par)
{
    //cout << endl
        // << "zaczynam rozwazanie " << rozwazany.first << " " << rozwazany.second;
    if (rozwazany.first == -1 && rozwazany.second == -1)
    {
        // cout<<endl<<"przypadek bazowy";
        return 1;
    }
    if (rozwazany.first != -2 && rozwazany.second != -2 && odpowiedzi[rozwazany.first][rozwazany.second] != -1)
        return odpowiedzi[rozwazany.first][rozwazany.second];
    //wiec to nie jest przypadek podstawowy. sasiedzi
    vector<pair<int, int>> sasiedzi = krawedzie(rozwazany, skonkatenowane_slowa, jakie_to_wybory, nr_pary, wektor_par);
    int odpowiedz = 0;
    for (int i = 0; i < sasiedzi.size(); i++)
    {
        pair<int, int> sasiad = sasiedzi[i];
        odpowiedz += na_ile_sposobow(sasiad, odpowiedzi,
                                     skonkatenowane_slowa, jakie_to_wybory, nr_pary, wektor_par);
    }
    if (rozwazany.first != -2 && rozwazany.second != -2)
        odpowiedzi[rozwazany.first][rozwazany.second] = odpowiedz;

   // cout << endl
        //<< "koncze rozwazanie " << rozwazany.first << " " << rozwazany.second;

    return odpowiedz;
}

int main()
{

    int n;
    //jak przechowywac te slowa?
    vector<char> skonkatenowane_slowa;
    vector<Wybor> jakie_to_wybory;
    vector<int> nr_pary;
    vector<Para> wektor_par;
    string plik = "LICTESTY/LIC11.IN";
    //string plik="dane.txt";


    //teraz mam taka sytuacje, ze trzeba wczytywac z standardowego wejscia
    //wczytywanie(plik, skonkatenowane_slowa, jakie_to_wybory, n, nr_pary, wektor_par);
    wczytywanie_z_standardowego( skonkatenowane_slowa, jakie_to_wybory, n, nr_pary, wektor_par);
    /* 
    cout << endl
         << "literek jest " << skonkatenowane_slowa.size();
    cout << endl;
    for (int i = 0; i < skonkatenowane_slowa.size(); i++)
        cout << " " << skonkatenowane_slowa[i];
    cout << endl;
    for (int i = 0; i < skonkatenowane_slowa.size(); i++)
        if (i < 10)
            cout << " " << i;
        else
            cout << i;
    cout << endl;
    for (int i = 0; i < jakie_to_wybory.size(); i++)
        cout << " " << jakie_to_wybory[i].wyb_pierwszy;
    cout << endl;
    for (int i = 0; i < jakie_to_wybory.size(); i++)
    {
        int do_wypisu = nr_pary[i];
        if (do_wypisu < 10)
            cout << " " << do_wypisu;
        else
            cout << do_wypisu;
    }
    */

    int N = skonkatenowane_slowa.size();
    //teraz nasza baza zapamietanych odpowiedzi
    vector<vector<int>> odpowiedzi;
    for (int i = 0; i < N; i++)
    {
        vector<int> linia;
        for (int j = 0; j < N; j++)
            linia.push_back(-1);
        odpowiedzi.push_back(linia);
    }

    /*
    for(int i=0;i<wektor_par.size();i++){
        cout<<endl<<"pokoj nr "<<i;
        wektor_par[i].wypisz();
    }
    */

    //teraz odpowiedzi ma w sobie -1 tam gdzie trzeba.
    //teraz przyda sie funkcja ktora nam daje krawedzie
/*
    cout << endl;
    pair<int, int> sprawdzany = pair<int, int>(13, 16);
    cout << "krawedzie naszego " << sprawdzany.first << " " << sprawdzany.second;
    vector<pair<int, int>> znalezione_krawedzie = krawedzie(sprawdzany, skonkatenowane_slowa, jakie_to_wybory, nr_pary, wektor_par);
    wypisz_wektor_par_int(znalezione_krawedzie);

    cout << endl
         << "koniec zabawy z wuszukiwaniem krawedzi";
*/
    pair<int, int> poczatkowy(-2, -2);
    int wynik = na_ile_sposobow(poczatkowy, odpowiedzi,
                                skonkatenowane_slowa, jakie_to_wybory, nr_pary, wektor_par);
    cout << wynik;

    /*
    cout<<endl<<"jakie sa niezerowe w odpowiedzi?";
    for(int i=0;i<N;i++)
        for(int j=0;j<N;j++)
            if(odpowiedzi[i][j]!=-1){
                cout<<endl<<"niezerowy "<<i<<" "<<j<<"wynosi"<<odpowiedzi[i][j];
            }
    */

    //cout<<endl<<"doszlo do return 0";
    //cout.flush();
    return 0;
}
