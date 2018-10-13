#include <iostream>
#include <fstream>
#include <vector>
#include <set>
using namespace std;

int char_do_int(char a)
{
    return int(a) - 97;
}

int otaczajaca_potega_2(int a)
{
    int wyrzut = 1;
    while (wyrzut < a)
        wyrzut *= 2;
    return wyrzut;
}

void domaxuj(int & a,int b){
    a=max(a,b);
}

class Drzewo_max
{
  public:
    vector<int> tab; //wiadomo, 1 to root
    int rozmiar;     //potega 2
    Drzewo_max(vector<int> &liczby)
    {
        rozmiar=otaczajaca_potega_2(liczby.size());
        tab.resize(2*rozmiar,0);
        for(int i=rozmiar;i<2*rozmiar;i++){
            tab[i]=liczby[i-rozmiar];
        }
        liczby.resize(0);
        for(int i=rozmiar-1;i>=1;i--){
            tab[i]=max(tab[2*i],tab[2*i+1]);
        }
    }
    int max_na_pythonowym_obszarze(int a,int b){
        //na wstepie obslugujemy pusty przpyadek
        a+=rozmiar;
        b+=rozmiar;
        if(a==b)
            return 0;
        b-=1;
        //teraz przypadek jest normalny
        int wyrzut=0;
        if(a==b)
            return tab[a];
        //niezmiennik jest taki, ze pod a oraz pod b jest porobione
        domaxuj(wyrzut,tab[a]);
        domaxuj(wyrzut,tab[b]);
        while(a/2 != b/2){
            if(a%2==0)
                domaxuj(wyrzut,tab[a+1]);
            if(b%2==1)
                domaxuj(wyrzut,tab[b-1]);
            b/=2;
            a/=2;
        }
        return wyrzut;
    }
};

class Event_domaxowania
{
  public:
    int nazwa_slowa;
    int odkad;
    int dokad;
    Event_domaxowania(int nazwa_slowa, int odkad, int dokad)
    {
        this->nazwa_slowa = nazwa_slowa;
        this->odkad = odkad;
        this->dokad = dokad;
    }
    void wypisz()
    {
        cout << "event" << endl;
        cout << "nazwa_slowa " << nazwa_slowa << endl;
        cout << "odkad " << odkad << endl;
        cout << "dokad " << dokad << endl;
        cout << "koniec evrent" << endl;
    }
};

class Linia_kolejowa
{
  public:
    int nazwa_slowa;
    int od_kiedy;
    int max_dotychczas;
    vector<Event_domaxowania> eventy_domaxowania;
    Linia_kolejowa(int nazwa_slowa)
    {
        od_kiedy = 0;
        max_dotychczas = 1;
        this->nazwa_slowa = nazwa_slowa;
    }
    void wypisz()
    {
        cout << "wypisywaie jednej lini kolejnowej" << endl;
        cout << "nazwa_slowa " << nazwa_slowa << endl;
        cout << "od_kiedy " << od_kiedy << endl;
        for (int i = 0; i < eventy_domaxowania.size(); i++)
        {
            eventy_domaxowania[i].wypisz();
            cout << endl;
        }
    }
};

class Info_o_nazwach
{
  public:
    vector<vector<int>> maxima;
    Info_o_nazwach(int ile_nazw)
    {
        maxima.resize(ile_nazw);
        for (int i = 0; i < ile_nazw; i++)
            maxima[i].push_back(0);
    }

    void dolicz_ile_jest_na_poczatku(vector<Linia_kolejowa> &linie_kolejowe)
    {
        for (int i = 0; i < linie_kolejowe.size(); i++)
        {
            maxima[linie_kolejowe[i].nazwa_slowa][0]++;
        }
    }
    void wypisz()
    {
        cout << endl
             << endl
             << "wypisuje info_o_nazwach" << endl;
        for (int i = 0; i < maxima.size(); i++)
        {
            cout << endl
                 << "nazwa " << i << endl;
            for (int j = 0; j < maxima[i].size(); j++)
            {
                cout << maxima[i][j] << " ";
            }
            cout << endl;
        }
    }
};

class Paropodobna
{
  public:
    pair<int, int> para;
    int *wskaznik;
    Paropodobna(int x, int y, int *wskaznik)
    {
        para = pair<int, int>(x, y);
        this->wskaznik = wskaznik;
    }
};

class Pociagi
{
  public:
    vector<int *> tab_pociagow;
    int dlugosc_pociagow;
    void inicjoj_wskazniki(int n, int l)
    {
        dlugosc_pociagow = otaczajaca_potega_2(l);
        for (int i = 0; i < n; i++)
        {
            tab_pociagow.push_back(new int[dlugosc_pociagow]);
            for (int j = l; j < dlugosc_pociagow; j++)
                tab_pociagow[i][j] = 0;
        }
    }
    void eksportuj_do_wektora_na_koniec(vector<Linia_kolejowa> &wyrzut)
    {
        for (int i = 0; i < tab_pociagow.size(); i++)
        {
            wyrzut.push_back(Linia_kolejowa(tab_pociagow[i][0]));
            delete[] tab_pociagow[i];
            tab_pociagow[i] = 0;
        }
    }
    void wypisz()
    {
        for (int i = 0; i < tab_pociagow.size(); i++)
        {
            cout << endl;
            for (int j = 0; j < dlugosc_pociagow; j++)
            {
                cout << tab_pociagow[i][j] << " ";
            }
        }
    }

    void rob_kopie_wektorowa(vector<vector<int>> &wyrzut)
    {
        wyrzut.resize(tab_pociagow.size());
        for (int i = 0; i < tab_pociagow.size(); i++)
        {
            for (int j = 0; j < dlugosc_pociagow; j++)
            {
                wyrzut[i].push_back(tab_pociagow[i][j]);
            }
        }
    }
};

class Wydarzenia_proste
{
  public:
    int *nowa_wartosc;
    int *ktory_pociag;
    int *ktory_wagon;
    int liczba_wydarzen;
    void initialize(int m)
    {
        nowa_wartosc = new int[2 * m];
        ktory_pociag = new int[2 * m];
        ktory_wagon = new int[2 * m];
        liczba_wydarzen = 2 * m;
    }
    void deinitialize()
    {
        delete[] nowa_wartosc;
        delete[] ktory_pociag;
        delete[] ktory_wagon;
    }
    void wypisz()
    {
        for (int i = 0; i < liczba_wydarzen; i++)
        {
            if (i % 2 == 0)
                cout << endl
                     << "kolejne wydarzenia" << endl;
            cout << endl
                 << "pociag" << ktory_pociag[i] + 1 << endl;
            cout << "wagon" << ktory_wagon[i] + 1 << endl;
            cout << "nowa wartosc" << nowa_wartosc[i] << endl;
        }
    }
};

void zamien(int &a, int &b)
{
    int pom = a;
    a = b;
    b = pom;
}

void wczytywanie(Pociagi &pociagi, Wydarzenia_proste &wydarzenia)
{
    //ifstream we("dane.txt");
    int n, m, l;
    cin >> n >> l >> m;
    pociagi.inicjoj_wskazniki(n, l);
    vector<vector<int>> kopia;
    wydarzenia.initialize(m);
    //n to licza pociagow,
    //l to dlugosci
    //m to ile wymian
    for (int i = 0; i < n; i++)
    {
        string pom;
        cin >> pom;
        for (int j = 0; j < pom.size(); j++)
        {
            pociagi.tab_pociagow[i][j] = char_do_int(pom[j]);
        }
    }
    pociagi.rob_kopie_wektorowa(kopia);
    int w1, w2, p1, p2;
    for (int i = 0; i < m; i++)
    {
        cin >> p1 >> w1 >> p2 >> w2;
        //to jest nr wagonu i nr pociagu tylko taki zrabany
        p1--;
        p2--;
        w1--;
        w2--;
        zamien(kopia[p1][w1], kopia[p2][w2]);
        wydarzenia.ktory_pociag[2 * i] = p1;
        wydarzenia.ktory_wagon[2 * i] = w1;
        wydarzenia.nowa_wartosc[2 * i] = kopia[p1][w1];

        wydarzenia.ktory_pociag[2 * i + 1] = p2;
        wydarzenia.ktory_wagon[2 * i + 1] = w2;
        wydarzenia.nowa_wartosc[2 * i + 1] = kopia[p2][w2];
    }
}

void sortoj_paropodobne(vector<Paropodobna> &tab, vector<int> &chwilowa_kolejnosc, int historyczna_liczba_wartosci, int ktora_wsporzedna)
{
    vector<vector<int>> kubelki;
    kubelki.resize(historyczna_liczba_wartosci);
    int pom;
    for (int i = 0; i < tab.size(); i++)
    {
        if (ktora_wsporzedna == 0)
            pom = tab[chwilowa_kolejnosc[i]].para.first;
        else
            pom = tab[chwilowa_kolejnosc[i]].para.second;
        kubelki[pom].push_back(chwilowa_kolejnosc[i]);
    }
    int podrozojocy_wskaznik = 0;
    for (int i = 0; i < kubelki.size(); i++)
    {
        for (int j = 0; j < kubelki[i].size(); j++)
        {
            chwilowa_kolejnosc[podrozojocy_wskaznik] = kubelki[i][j];
            podrozojocy_wskaznik++;
        }
    }
}

void ponazywaj_intami(vector<Paropodobna> &tab, vector<int> &chwilowa_kolejnosc, int &nowa_liczba_wartosci)
{
    pair<int, int> poprzednia_wartosc = pair<int, int>(-1, -1);
    int nazywajacy_numer = -1;
    for (int i = 0; i < chwilowa_kolejnosc.size(); i++)
    {
        int index_tab = chwilowa_kolejnosc[i];
        if (poprzednia_wartosc != tab[index_tab].para)
        {
            nazywajacy_numer++;
            poprzednia_wartosc = tab[index_tab].para;
        }
        *(tab[index_tab].wskaznik) = nazywajacy_numer;
    }
    nowa_liczba_wartosci = nazywajacy_numer + 1;
}

void rob_kolejna_warstwe(Pociagi &pociagi, Wydarzenia_proste &wydarzenia, int historyczna_liczba_wartosci, int &nowa_liczba_wartosci)
{
    vector<Paropodobna> rzeczy_do_sortowania;
    //najpierw trzeba pozbierac z pociagow;
    for (int i = 0; i < pociagi.tab_pociagow.size(); i++)
    {
        for (int j = 0; 2 * j < pociagi.dlugosc_pociagow; j++)
        {
            rzeczy_do_sortowania.push_back(Paropodobna(pociagi.tab_pociagow[i][2 * j], pociagi.tab_pociagow[i][2 * j + 1], pociagi.tab_pociagow[i] + j));
        }
    }
    pociagi.dlugosc_pociagow /= 2;
    //teraz trzeba to samo zrobic z wydarzeniami. tutaj bedzie sie tez odbywac symulacja
    for (int i = 0; i < wydarzenia.liczba_wydarzen; i++)
    {
        //najpierw zamien a potem dodaj
        pociagi.tab_pociagow[wydarzenia.ktory_pociag[i]][wydarzenia.ktory_wagon[i]] = wydarzenia.nowa_wartosc[i];
        //teraz nalezy dodac
        int wx, wy, kt_wag;
        kt_wag = wydarzenia.ktory_wagon[i];

        if (kt_wag % 2 == 0)
        {
            wx = kt_wag;
            wy = kt_wag + 1;
        }
        else
        {
            wx = kt_wag - 1;
            wy = kt_wag;
        }
        rzeczy_do_sortowania.push_back(Paropodobna(pociagi.tab_pociagow[wydarzenia.ktory_pociag[i]][wx], pociagi.tab_pociagow[wydarzenia.ktory_pociag[i]][wy], wydarzenia.nowa_wartosc + i));
        //teraz trzeba zmienic wydarzenia
        wydarzenia.ktory_wagon[i] /= 2;
    }
    //teraz potrzeba to po prostu rzeczy_do_sortowania posortowac pozycyjnie
    vector<int> chwilowa_kolejnosc;
    for (int i = 0; i < rzeczy_do_sortowania.size(); i++)
        chwilowa_kolejnosc.push_back(i);
    sortoj_paropodobne(rzeczy_do_sortowania, chwilowa_kolejnosc, historyczna_liczba_wartosci, 1);
    sortoj_paropodobne(rzeczy_do_sortowania, chwilowa_kolejnosc, historyczna_liczba_wartosci, 0);
    //teraz chwilowa_kolejnosc zawiera dobra kolejnosc indexow
    ponazywaj_intami(rzeczy_do_sortowania, chwilowa_kolejnosc, nowa_liczba_wartosci);
}

void zmien_koniec_wektora_o(vector<int> &tab, int zmiana_wartosci)
{
    tab[tab.size() - 1] += zmiana_wartosci;
}

void rob_nte_wydarzenie(int i, Wydarzenia_proste &wydarzenia, vector<Linia_kolejowa> &linie_kolejowe, Info_o_nazwach &info_o_nazwach)
{
    vector<int> rozwazane_z_2_wydarzen;
    if (wydarzenia.ktory_pociag[2 * i] != wydarzenia.ktory_pociag[2 * i + 1])
    {
        rozwazane_z_2_wydarzen.push_back(0);
        rozwazane_z_2_wydarzen.push_back(1);
    }
    else
    {
        rozwazane_z_2_wydarzen.push_back(1);
    }
    set<int> dotkniete_nazwy;
    //vector<int> wszystkie_nazwy;
    for (int a = 0; a < rozwazane_z_2_wydarzen.size(); a++)
    {
        dotkniete_nazwy.insert(wydarzenia.nowa_wartosc[2 * i + rozwazane_z_2_wydarzen[a]]);
        //teraz jeszcze sa dotkniete te, z ktorych wychodzimy
        dotkniete_nazwy.insert(linie_kolejowe[wydarzenia.ktory_pociag[2 * i + rozwazane_z_2_wydarzen[a]]].nazwa_slowa);
    }
    //teraz tym dotknietym dorabiamy nowe
    for (set<int>::iterator iter = dotkniete_nazwy.begin(); iter != dotkniete_nazwy.end(); ++iter)
        info_o_nazwach.maxima[*iter].push_back(info_o_nazwach.maxima[*iter][info_o_nazwach.maxima[*iter].size() - 1]);
    //teraz wepchnalem taka sama wartosc co ostatnio
    //teraz po prostu trzeba to zrobic
    //na poczatek zmienmy ostatnie te wartosci ilosci takich samych
    for (int a = 0; a < rozwazane_z_2_wydarzen.size(); a++)
    {
        zmien_koniec_wektora_o(info_o_nazwach.maxima[wydarzenia.nowa_wartosc[2 * i + rozwazane_z_2_wydarzen[a]]], 1);
        zmien_koniec_wektora_o(info_o_nazwach.maxima[linie_kolejowe[wydarzenia.ktory_pociag[2 * i + rozwazane_z_2_wydarzen[a]]].nazwa_slowa], -1);
    }
    //teraz infa_o_nazwach jest juz zadowolona. zadowolmy linie_kolejowe

    for (int a = 0; a < rozwazane_z_2_wydarzen.size(); a++)
    {
        int nr_wydarzenia = 2 * i + rozwazane_z_2_wydarzen[a];
        int rozwazany_pociag = wydarzenia.ktory_pociag[nr_wydarzenia];
        int jego_stare_slowo = linie_kolejowe[rozwazany_pociag].nazwa_slowa;
        int jego_nowe_slowo = wydarzenia.nowa_wartosc[nr_wydarzenia];
        int jego_odkad = linie_kolejowe[rozwazany_pociag].od_kiedy;
        int jego_dokad = info_o_nazwach.maxima[jego_stare_slowo].size() - 1;
        linie_kolejowe[rozwazany_pociag].eventy_domaxowania.push_back(Event_domaxowania(jego_stare_slowo, jego_odkad, jego_dokad));
        linie_kolejowe[rozwazany_pociag].od_kiedy = info_o_nazwach.maxima[jego_nowe_slowo].size() - 1;
        linie_kolejowe[rozwazany_pociag].nazwa_slowa = jego_nowe_slowo;
    }
    //to by bylo chyba na tyle
}

void pom_wypisz(vector<vector<int>> &kopia, vector<vector<int>> &zapis_skondensowany)
{
    for (int i = 0; i < kopia.size(); i++)
    {
        zapis_skondensowany[i].push_back(kopia[i][0]);
    }
}

void wypisz_symulacje(Pociagi &pociagi, Wydarzenia_proste &wydarzenia)
{
    //cout<<endl<<"poczatek wypisu przebiegu"<<endl;
    //cout << "startowy " << endl;
    cout << endl
         << endl;
    vector<vector<int>> kopia_pociagow;
    vector<vector<int>> zapis_skondensowany;
    zapis_skondensowany.resize(pociagi.tab_pociagow.size());
    pociagi.rob_kopie_wektorowa(kopia_pociagow);
    //pociagi.wypisz();
    pom_wypisz(kopia_pociagow, zapis_skondensowany);

    for (int i = 0; 2 * i < wydarzenia.liczba_wydarzen; i++)
    {
        kopia_pociagow[wydarzenia.ktory_pociag[2 * i]][wydarzenia.ktory_wagon[2 * i]] = wydarzenia.nowa_wartosc[2 * i];
        kopia_pociagow[wydarzenia.ktory_pociag[2 * i + 1]][wydarzenia.ktory_wagon[2 * i + 1]] = wydarzenia.nowa_wartosc[2 * i + 1];
        pom_wypisz(kopia_pociagow, zapis_skondensowany);
    }

    cout << "wypisywanie przebiegu, wiersz to nr kolei zas kolumna to stan" << endl;
    for (int i = 0; i < zapis_skondensowany.size(); i++)
    {
        for (int j = 0; j < zapis_skondensowany[i].size(); j++)
        {
            if (zapis_skondensowany[i][j] < 10)
                cout << " ";
            cout << zapis_skondensowany[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl
         << "koniec wypisywanie przebiegu" << endl
         << endl;
}

int main()
{

    Pociagi pociagi;
    Wydarzenia_proste wydarzenia;
    wczytywanie(pociagi, wydarzenia);

    int historyczna_liczba_wartosci = 30;
    int nowa_liczba_wartosci = -13;
    while (pociagi.dlugosc_pociagow != 1)
    {
        rob_kolejna_warstwe(pociagi, wydarzenia, historyczna_liczba_wartosci, nowa_liczba_wartosci);
        historyczna_liczba_wartosci = nowa_liczba_wartosci;
    }

    //wyglada na to, ze na razie dziala.

    /*
    wydarzenia.wypisz();
    wypisz_symulacje(pociagi, wydarzenia);
    */

    //fajnie nowe klasy
    vector<Linia_kolejowa> linie_kolejowe;
    pociagi.eksportuj_do_wektora_na_koniec(linie_kolejowe);
    Info_o_nazwach info_o_nazwach(historyczna_liczba_wartosci);
    info_o_nazwach.dolicz_ile_jest_na_poczatku(linie_kolejowe);
    for (int i = 0; 2 * i < wydarzenia.liczba_wydarzen; i++)
    {
        rob_nte_wydarzenie(i, wydarzenia, linie_kolejowe, info_o_nazwach);
    }
    //trzeba teraz dodac te nieskonczone jeszcze eventy
    for (int i = 0; i < linie_kolejowe.size(); i++)
    {
        int odkiedy = linie_kolejowe[i].od_kiedy;
        int do_kiedy = info_o_nazwach.maxima[linie_kolejowe[i].nazwa_slowa].size();
        int slowo = linie_kolejowe[i].nazwa_slowa;
        linie_kolejowe[i].eventy_domaxowania.push_back(Event_domaxowania(slowo, odkiedy, do_kiedy));
    }
    //teraz wazne sa tylko te info_o_wartosci.maxi oraz te linie_kolejowe.eventy
    /*
    for (int i = 0; i < linie_kolejowe.size(); i++)
    {
        cout << endl
             << "pociag " << i + 1 << endl
             << endl;
        linie_kolejowe[i].wypisz();
        cout << endl;
    }
    info_o_nazwach.wypisz();
    */
    vector<Drzewo_max> drzewa;
    for(int i=0;i<info_o_nazwach.maxima.size();i++){
        Drzewo_max nowe (info_o_nazwach.maxima[i]);
        drzewa.push_back(nowe);
    }
    //teraz te drzewa trzeba wykorzystac
    for(int i=0;i<linie_kolejowe.size();i++){
        for(int j=0;j<linie_kolejowe[i].eventy_domaxowania.size();j++){
            Event_domaxowania event=linie_kolejowe[i].eventy_domaxowania[j];
            domaxuj(linie_kolejowe[i].max_dotychczas,drzewa[event.nazwa_slowa].max_na_pythonowym_obszarze(event.odkad,event.dokad));
        }
    }
    for(int i=0;i<linie_kolejowe.size();i++){
        cout<<linie_kolejowe[i].max_dotychczas;
        if(i!=linie_kolejowe.size())
            cout<<endl;
    }
    


    return 0;
}
