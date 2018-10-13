#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

class Info_punktu
{
  public:
    int nr;
    int nr_rosnacego;
    int nr_malejacego;
    Info_punktu(int nr, int nr_rosnacego, int nr_malejacego)
    {
        this->nr = nr;
        this->nr_rosnacego = nr_rosnacego;
        this->nr_malejacego = nr_malejacego;
    }
};

void domaxuj(int &a, int b)
{
    if (a < b)
        a = b;
}

void dominuj(int &a, int b)
{
    if (a > b)
        a = b;
}

int h(pair<int, int> &para)
{
    return para.second;
}

int ros(pair<int, int> &para)
{
    return para.second - para.first;
}

int mal(pair<int, int> &para)
{
    return para.second + para.first;
}

void znajdz_ograniczenia(vector<pair<int, int>> &kolejne_punkty, int &h_do_odejmowania, int &rosnace_do_odejmowania, int &malejace_do_odejmowania,
                         vector<int> &wektor_rosnacych, vector<int> &wektor_malejacych, vector<vector<Info_punktu>> &kubelki)
{
    //teraz trzeba znalezc maksy
    int nies = 1001000;
    int h_min = nies;
    int h_max = -nies;
    int ros_min = 2 * nies;
    int ros_max = -2 * nies;
    int mal_min = 2 * nies;
    int mal_max = -2 * nies;

    //zawsze niech te wielkosci rosna dla rosnacego y na przyklad
    for (int i = 0; i < kolejne_punkty.size(); i++)
    {
        int h_tego = h(kolejne_punkty[i]);
        int ros_tego = ros(kolejne_punkty[i]);
        int mal_tego = mal(kolejne_punkty[i]);

        dominuj(h_min, h_tego);
        domaxuj(h_max, h_tego);
        dominuj(ros_min, ros_tego);
        domaxuj(ros_max, ros_tego);
        dominuj(mal_min, mal_tego);
        domaxuj(mal_max, mal_tego);
    }
    //fajnie to teraz robimy tak
    h_do_odejmowania = h_min;
    rosnace_do_odejmowania = ros_min;
    malejace_do_odejmowania = mal_min;
    //teraz trzeba pozmieniac rozmiary naszych tych
    wektor_rosnacych.resize(ros_max - ros_min + 1, -1);
    wektor_malejacych.resize(mal_max - mal_min + 1, -1);
    kubelki.resize(h_max - h_min + 1);
}

void wepchnij_punkt(vector<vector<Info_punktu>> &kubelki, pair<int, int> &wsporzedne, int nr, bool czy_ros, bool czy_mal, int ros_min, int mal_min, int h_min)
{
    int nr_rosnacego, nr_malejacego;
    if (czy_ros)
        nr_rosnacego = ros(wsporzedne) - ros_min;
    else
        nr_rosnacego = -1;
    if (czy_mal)
        nr_malejacego = mal(wsporzedne) - mal_min;
    else
        nr_malejacego = -1;
    Info_punktu nowy(nr, nr_rosnacego, nr_malejacego);
    kubelki[h(wsporzedne) - h_min].push_back(nowy);
}

void polozenie_wzgledne(pair<int, int> &ja, pair<int, int> &on, int &w_pionie, int &w_poziomie)
{
    // w pionie moze wyniesc na zakonczenie -1,0 lub 1 i to zanczy, ze on jest pod, na tej samej lub wyzej
    if (on.first > ja.first)
        w_poziomie = 1;
    else if (on.first < ja.first)
        w_poziomie = -1;
    else
        w_poziomie = 0;

    if (on.second > ja.second)
        w_pionie = 1;
    else if (on.second < ja.second)
        w_pionie = -1;
    else
        w_pionie = 0;
}

void ustal_skos(pair<int, int> &ja, pair<int, int> &som1, pair<int, int> &som2, bool &czy_ros, bool &czy_mal)
{
    int w_pionie_1, w_pionie_2, w_poziomie_1, w_poziomie_2;
    polozenie_wzgledne(ja, som1, w_pionie_1, w_poziomie_1);
    polozenie_wzgledne(ja, som2, w_pionie_2, w_poziomie_2);
    int pion_suma = w_pionie_1 + w_pionie_2;
    int poziom_suma = w_poziomie_1 + w_poziomie_2;
    int iloczyn = pion_suma * poziom_suma;
    if (iloczyn == 1)
    {
        czy_ros = true;
        czy_mal = false;
    }
    else if (iloczyn == -1)
    {
        czy_ros = false;
        czy_mal = true;
    }
    else
    {
        cout << endl
             << endl
             << "tak byc nie powinno ustal skos" << endl
             << endl;
    }
}

void punkty_wewnetrzne_obwodu(pair<int, int> &pocz, pair<int, int> &konc, vector<pair<int, int>> &wewnetrzne)
{
    if (pocz.first == konc.first)
    {
        for (int y = min(pocz.second, konc.second) + 1; y < max(pocz.second, konc.second); y++)
        {
            wewnetrzne.push_back(pair<int, int>(pocz.first, y));
        }
    }
    else
    {
        for (int x = min(pocz.first, konc.first) + 1; x < max(pocz.first, konc.first); x++)
            wewnetrzne.push_back(pair<int, int>(x, pocz.second));
    }
}

void dodaj_krawedz(vector<vector<int>> &graf, int a, int b)
{
    graf[a].push_back(b);
    graf[b].push_back(a);
}

void koncowka_robienia_grafu(vector<vector<Info_punktu>> &kubelki, vector<vector<int>> &graf, vector<int> &wek_ros, vector<int> &wek_mal)
{
    //tutaj juz jest pouzupelniane kubeli tylko potrzeba porobic z tego graf;
    for (int i = 0; i < kubelki.size(); i++)
    {
        for (int j = 0; j < kubelki[i].size(); j++)
        {
            Info_punktu obrabiany = kubelki[i][j];
            if (obrabiany.nr_rosnacego != -1)
            {
                //to znaczy, ze cos robimy z tym
                int nr_ros = obrabiany.nr_rosnacego;
                if (wek_ros[nr_ros] == -1)
                {
                    wek_ros[nr_ros] = obrabiany.nr;
                }
                else
                {
                    dodaj_krawedz(graf, obrabiany.nr, wek_ros[nr_ros]);
                    wek_ros[nr_ros] = -1;
                }
            }
            if (obrabiany.nr_malejacego != -1)
            {
                //to znaczy, ze cos robimy z tym
                int nr_mal = obrabiany.nr_malejacego;
                if (wek_mal[nr_mal] == -1)
                {
                    wek_mal[nr_mal] = obrabiany.nr;
                }
                else
                {
                    dodaj_krawedz(graf, obrabiany.nr, wek_mal[nr_mal]);
                    wek_mal[nr_mal] = -1;
                }
            }
        }
    }
}

void rob_graf(vector<pair<int, int>> &kolejne_punkty, vector<vector<int>> &graf)
{
    //na poczatek robimy potrzebne zmienne
    int h_min;
    int ros_min;
    int mal_min;
    vector<int> wek_ros;
    vector<int> wek_mal;
    vector<vector<Info_punktu>> kubelki;
    znajdz_ograniczenia(kolejne_punkty, h_min, ros_min, mal_min, wek_ros, wek_mal, kubelki);
    //fajnie. to teraz mamy takie cos. na poczatek robimy rogi
    int obecnie_wsadzany_wierzcholek = 0;
    for (int i = 0; i < kolejne_punkty.size(); i++)
    {
        int index_som_1, index_som_2;
        if (i > 0)
            index_som_1 = i - 1;
        else
            index_som_1 = kolejne_punkty.size() - 1;

        if (i < kolejne_punkty.size() - 1)
            index_som_2 = i + 1;
        else
            index_som_2 = 0;

        bool czy_ros, czy_mal;
        ustal_skos(kolejne_punkty[i], kolejne_punkty[index_som_1], kolejne_punkty[index_som_2], czy_ros, czy_mal);
        //to teraz mozna nareszcie to wsadzic
        wepchnij_punkt(kubelki, kolejne_punkty[i], obecnie_wsadzany_wierzcholek, czy_ros, czy_mal, ros_min, mal_min, h_min);
        obecnie_wsadzany_wierzcholek++;
    }
    //fajnie to teraz trzeba wrzucic elementy obwodu. wiec trzeba miec funkcje ktora daje nam wnetrze tego
    for (int i = 0; i < kolejne_punkty.size(); i++)
    {
        pair<int, int> pocz = kolejne_punkty[i];
        pair<int, int> konc;
        if (i < kolejne_punkty.size() - 1)
            konc = kolejne_punkty[i + 1];
        else
            konc = kolejne_punkty[0];
        vector<pair<int, int>> wewnetrzne;
        punkty_wewnetrzne_obwodu(pocz, konc, wewnetrzne);
        for (int j = 0; j < wewnetrzne.size(); j++)
        {
            wepchnij_punkt(kubelki, wewnetrzne[j], obecnie_wsadzany_wierzcholek, true, true, ros_min, mal_min, h_min);
            obecnie_wsadzany_wierzcholek++;
        }
    }
    //fajnie to teraz trzeba zresizowac nasz graf chyba
    graf.resize(obecnie_wsadzany_wierzcholek);

    koncowka_robienia_grafu(kubelki, graf, wek_ros, wek_mal);
}

void wypisz_pary(vector<vector<int>> &graf, int n)
{
    vector<bool> odwiedzone;
    odwiedzone.resize(n, false);
    cout << n/2;

    for (int i = 0; i < n; i++)
        if (odwiedzone[i] == false)
        {
            //zaczynamy szukac w grafie jego drugiego konca
            int chwilowy = graf[i][0];
            int poprzedni = i;
            while (graf[chwilowy].size() == 2)
            {
                for (int j = 0; j < 2; j++)
                {
                    if (graf[chwilowy][j] != poprzedni)
                    {
                        poprzedni = chwilowy;
                        chwilowy = graf[chwilowy][j];
                        break;
                    }
                }
            }
            odwiedzone[i]=true;
            odwiedzone[chwilowy]=true;
            cout<<endl<<i+1<<" "<<chwilowy+1;
        }
}

void wczytaj(vector<pair<int, int>> &kolejne_punkty)
{
    //ifstream we("dane.txt");
    int n;
    cin >> n;
    int a, b;
    for (int i = 0; i < n; i++)
    {
        cin >> a >> b;
        kolejne_punkty.push_back(pair<int, int>(a, b));
    }
}

int main()
{
    vector<pair<int, int>> kolejne_punkty;
    wczytaj(kolejne_punkty);
    vector<vector<int> > graf;
    rob_graf(kolejne_punkty,graf);
    wypisz_pary(graf,kolejne_punkty.size());


    //cout << endl
         //<< "doszlo do return 0" << endl;
    return 0;
}
