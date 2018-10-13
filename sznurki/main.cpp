#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

void wczytaj(vector<vector<int>> &graf, int &n)
{
    bool przelacznik = false;
    if (przelacznik)
    {
        ifstream we("dane.txt");
        we >> n;
        for (int i = 0; i < n; i++)
        {
            vector<int> pom;
            graf.push_back(pom);
        }
        for (int i = 0; i < n - 1; i++)
        {
            int a, b;
            we >> a >> b;
            graf[a - 1].push_back(b - 1);
            graf[b - 1].push_back(a - 1);
        }
    }
    else
    {
        //ifstream we("dane.txt");
        cin >> n;
        for (int i = 0; i < n; i++)
        {
            vector<int> pom;
            graf.push_back(pom);
        }
        for (int i = 0; i < n - 1; i++)
        {
            int a, b;
            cin >> a >> b;
            graf[a - 1].push_back(b - 1);
            graf[b - 1].push_back(a - 1);
        }
    }
}

void wypisz_graf(vector<vector<int>> &graf)
{
    cout << endl
         << "graf to " << endl;
    for (int i = 0; i < graf.size(); i++)
    {
        cout << i + 1 << ": ";
        for (int j = 0; j < graf[i].size(); j++)
        {
            cout << graf[i][j] + 1 << " ";
        }
        cout << endl;
    }
}

int ile_kawalkow_sznurka(vector<vector<int>> &graf, int &root)
{
    int wynik = 0;
    root = -1;
    for (int i = 0; i < graf.size(); i++)
    {
        if (graf[i].size() % 2 == 1)
            wynik++;
        if (graf[i].size() == 1)
            root = i;
    }
    return wynik / 2;
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

bool mniejszy_int(int a, int b)
{
    return a < b;
}

class Dziurawy
{
  public:
    int dlugosc;    //dlugosc tego odcinka
    int odpadajacy; //odpadajaacy inex. albo 0<= odpadajacy<dlugosc albo odpadajacy=-1
    Dziurawy()
    {
        dlugosc = 0;
        odpadajacy = -1;
    }
    int min_nie_w();
};

int Dziurawy::min_nie_w()
{
    if (odpadajacy != -1)
        return odpadajacy;
    else
        return dlugosc;
}

Dziurawy sum_dziu(Dziurawy da, Dziurawy db)
{
    Dziurawy wynik;
    wynik.dlugosc = max(da.dlugosc, db.dlugosc);
    int a = da.odpadajacy;
    int b = db.odpadajacy;
    if (da.odpadajacy == db.odpadajacy)
        wynik.odpadajacy = da.odpadajacy;
    else
    {
        if (da.odpadajacy < db.dlugosc)
            a = -1;
        if (db.odpadajacy < da.dlugosc)
            b = -1;
        int ile_m1 = 0;
        if (a == -1)
            ile_m1++;
        if (b == -1)
            ile_m1++;

        if (ile_m1 == 2)
            wynik.odpadajacy = -1;
        else if (ile_m1 == 1)
        {
            wynik.odpadajacy = max(a, b);
        }
        else
        {
            //obydwa sa nie rowne -1. tak byc nie powinno bylo byc.
            cout << endl
                 << "tak nie powino byc w dodawaniu dziurawych" << endl;
            //tutaj robie wiec cokolwiek.
            wynik.odpadajacy = -1;
        }
    }
    return wynik;
}

Dziurawy jakie_odrzucac(vector<int> &iwws, int dawca_perspektywy, int ogr_sznurka)
{
    //dawca perspektywy to index ktory ma byc zadowolony.
    int roz = iwws.size();
    Dziurawy wynik;
    if (dawca_perspektywy > 0 && iwws[dawca_perspektywy] + iwws[roz - dawca_perspektywy] > ogr_sznurka)
        //to znaczy, ze
        wynik.dlugosc = dawca_perspektywy;
    if (dawca_perspektywy != roz / 2 && iwws[dawca_perspektywy] + iwws[roz - dawca_perspektywy - 1] > ogr_sznurka)
        wynik.dlugosc = roz - 1 - dawca_perspektywy;
    if (iwws[dawca_perspektywy] + iwws[roz - dawca_perspektywy - 2] > ogr_sznurka)
        wynik.dlugosc = roz;
    if (dawca_perspektywy < wynik.dlugosc)
        wynik.odpadajacy = dawca_perspektywy;
    return wynik;
}

int nietrywialny(vector<int> &iwws, int ogr_sznurka)
{
    //iwws zawiera ile wejdzie w danych synow. posortowane jest juz.
    //dlugosc sznurka mniejsza lub rowna ogr_sznurka. jak wynik jest >ogr_sznurka to wychodzi -1.
    //liczba elementow jest nieparzysta. trzeba wybrac ktory z synow przedluzyc.
    Dziurawy odrzucone; //przechowuje te indexy tablicy iwws ktore nie moga byc wybrane na przedluzenie tego roota naszego.
    int roz = iwws.size();
    for (int i = 0; i < roz / 2 + 1; i++)
        odrzucone = sum_dziu(odrzucone, jakie_odrzucac(iwws, i, ogr_sznurka));
    int index = odrzucone.min_nie_w();
    if (index >= roz)
        return -1;
    int chyba_wynik = iwws[odrzucone.min_nie_w()];
    if (chyba_wynik > ogr_sznurka)
        return -1;
    else
        return chyba_wynik;
}

bool zlozone_ok(vector<int> &iwws, int ogr_sznurka)
{
    //zakladam, ze posortowane oraz nieparzyscie wiele
    int roz = iwws.size();
    for (int i = 0; i < roz / 2; i++)
        if (iwws[i] + iwws[roz - 1 - i] > ogr_sznurka)
            return false;
    return true;
}

int min_wej_szn(vector<vector<int>> &graf, int ja, int ojciec, int ogr_sznurka)
{
    //dlugosc sznurka moze byc maksymalnie ogr_sznurka, -1 oznacza, ze sie nie da tego porobic
    //to znaczy, ze na przyklad wynik jest wiekszy niz ogr_sznurka;
    //bo wynik to jest liczony od korzenia w dol ( to znaczy ta dlugosc sznurka)
    //na korzeniu wywoluemy z ojcem -1
    vector<int> ile_wejdzie_w_syna;
    for (int i = 0; i < graf[ja].size(); i++)
        if (graf[ja][i] != ojciec)
        {
            int pom = min_wej_szn(graf, graf[ja][i], ja, ogr_sznurka);
            if (pom == -1)
                return -1;
            ile_wejdzie_w_syna.push_back(pom + 1);
        }
    mergesort<int>(ile_wejdzie_w_syna, 0, ile_wejdzie_w_syna.size(), &mniejszy_int);
    if (ile_wejdzie_w_syna.size() % 2 == 1)
    {
        return nietrywialny(ile_wejdzie_w_syna, ogr_sznurka);
    }
    else if (zlozone_ok(ile_wejdzie_w_syna, ogr_sznurka))
        return 0;
    else
    {
        //gready choice to end the longest
        if (ile_wejdzie_w_syna[ile_wejdzie_w_syna.size() - 1] > ogr_sznurka)
            return -1;
        ile_wejdzie_w_syna.resize(ile_wejdzie_w_syna.size() - 1);
        return nietrywialny(ile_wejdzie_w_syna, ogr_sznurka);
    }
}

bool czy_sie_da(vector<vector<int>> &graf, int root, int ogr_sznurka)
{
    if (root == -1)
    {
        return ogr_sznurka >= 0;
    }
    //wiec root ma syna jednego
    int syn = graf[root][0];
    int o_synie = min_wej_szn(graf, syn, root, ogr_sznurka);
    if (o_synie == -1)
        return false;
    return o_synie + 1 <= ogr_sznurka;
}

int main()
{
    vector<vector<int>> graf;
    int n;
    wczytaj(graf, n);
    
    int root = -1;
    int liczba_kawalkow_sznurka = ile_kawalkow_sznurka(graf, root);
   // wypisz_graf(graf);

    int a = 0; //jesli jest false,false,true,true i tak dalej to a to jest najdalej co ma -1
    int b = n; //b to jest najnizej co ma nie -1.
    while (b - a != 1)
    {
        int srodek = a + (b - a) / 2;
        //cout<<endl<<"robie dla "<<srodek<<endl;
        //cout<<"a="<<a<<" b="<<b<<endl;
        bool dla_srodka = czy_sie_da(graf, root, srodek);
        // cout<<endl<<"skonczylem dla "<<srodek<<endl;
        if (dla_srodka)
        {
            b = srodek;
        }
        else
        {
            a = srodek;
        }
    }
    cout << liczba_kawalkow_sznurka << " " << b;

   // cout << endl
     //    << "doszlo do return 0" << endl;
    return 0;
}
