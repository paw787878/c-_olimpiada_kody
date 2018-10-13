#include <iostream>
#include <vector>
#include <fstream>
#include <set>
using namespace std;

class Zeznanie
{
  public:
    int czas;
    int nr_informatyka;
    int ilu_innych;
    Zeznanie()
    {
        czas = 0;
        nr_informatyka = 0;
        ilu_innych = 0;
    }
};

void znajdz_kolejne(vector<vector<int>> &numery_wystapien_informatykow, vector<Zeznanie> &zeznania, int &ile_informatykow, int &ile_zeznan)
{
    numery_wystapien_informatykow.resize(ile_informatykow);
    for (int i = 0; i < ile_zeznan; i++)
    {
        numery_wystapien_informatykow[zeznania[i].nr_informatyka].push_back(i);
    }
    int nies = 1000000000;
    for (int i = 0; i < ile_informatykow; i++)
    {
        numery_wystapien_informatykow[i].push_back(nies);
    }
}

bool czy_pirwsze_k_jest_prawdziwych(int k, vector<vector<int>> &numery_wystapien_informatykow, vector<Zeznanie> &zeznania, int &ile_informatykow, int &ile_zeznan)
{
    if (k > ile_zeznan)
        return false;
    //czyli jak w pythonie. latwo
    int liczba_dotknietych = 1;
    int ile_skonczonych = 0;
    int ile_w_trakcie = 0;
    int ile_niezidentyfikowanych = 1;
    int ostatni_czas = -13;
    int ostatnia_liczba_innych_zalogowanych = 0;
    vector<int> indeks_ostatniego_wystapienia_informatyka;
    indeks_ostatniego_wystapienia_informatyka.resize(ile_informatykow, 0);
    vector<int> pom_czas;
    pom_czas.resize(ile_informatykow, -1);
    //jak bedzie fail to zrobie return
    int i = 0;
    while (i < k)
    {
        Zeznanie chwilowe = zeznania[i];
        if (chwilowe.ilu_innych != ostatnia_liczba_innych_zalogowanych)
        {

            //musimy ich dodawac lub odejmowac
            int o_ile = chwilowe.ilu_innych - ostatnia_liczba_innych_zalogowanych;
            if (o_ile > 0)
            {
                ile_niezidentyfikowanych += o_ile;
                liczba_dotknietych += o_ile;
            }
            else
            {
                o_ile = -o_ile;
                if (ile_skonczonych + ile_niezidentyfikowanych < o_ile)
                    return false;
                if (o_ile <= ile_skonczonych)
                    ile_skonczonych -= o_ile;
                else
                {
                    ile_niezidentyfikowanych -= (o_ile - ile_skonczonych);
                    ile_skonczonych = 0;
                }
            }
        }
        int startowy_index = i;
        int rozwazany_czas = chwilowe.czas;
        int rozwazana_liczba_zalogowanych = chwilowe.ilu_innych;
        //set<int> informtaycy_w_tym_czasie;
        while (i < k && zeznania[i].czas == rozwazany_czas)
        {
            chwilowe = zeznania[i];
            //informtaycy_w_tym_czasie.insert(chwilowe.nr_informatyka);
            if (chwilowe.ilu_innych != rozwazana_liczba_zalogowanych)
                return false;
            indeks_ostatniego_wystapienia_informatyka[chwilowe.nr_informatyka]++;
            //teraz on wskzuje na nastepne wystapienie
            if (indeks_ostatniego_wystapienia_informatyka[chwilowe.nr_informatyka] == 1)
            {
                //znaczy to, ze to jest pierwszy raz.
                if (ile_niezidentyfikowanych > 0)
                {
                    ile_niezidentyfikowanych--;
                    ile_w_trakcie++;
                }
                else if (ile_skonczonych > 0)
                {
                    liczba_dotknietych++;
                    ile_skonczonych--;
                    ile_w_trakcie++;
                }
                else
                {
                    //nikogo nie moge wyprosic, wiec
                    return false;
                }
            }

            if (liczba_dotknietych > ile_informatykow)
                return false;

            i++;
        }
        //teraz mamy juz za soba eventy rozszerzania/zwezania
        //to robimy jak jest piewrszy. teraz co robimy jak jest ostatni?

        for (int j = startowy_index; j < i; j++)
        {
            chwilowe = zeznania[j];
            if (pom_czas[chwilowe.nr_informatyka] != rozwazany_czas)
            {
                if (numery_wystapien_informatykow[chwilowe.nr_informatyka][indeks_ostatniego_wystapienia_informatyka[chwilowe.nr_informatyka]] >= k)
                {
                    ile_w_trakcie--;
                    ile_skonczonych++;
                }
                pom_czas[chwilowe.nr_informatyka] = rozwazany_czas;
            }
            //zauwazmy, ze dla srodkowego nic sie nie dzieje
        }
        chwilowe = zeznania[i - 1];
        ostatni_czas = chwilowe.czas;
        ostatnia_liczba_innych_zalogowanych = chwilowe.ilu_innych;
    }

    return true;
}

void sortowanie_pozycyjne(vector<Zeznanie> &zeznania, int ile_zeznan)
{
    vector<vector<Zeznanie>> kubelki;
    kubelki.resize(ile_zeznan);
    for (int i = 0; i < zeznania.size(); i++)
    {
        kubelki[zeznania[i].czas].push_back(zeznania[i]);
    }
    int licznik = 0;
    for (int i = 0; i < kubelki.size(); i++)
    {
        for (int j = 0; j < kubelki[i].size(); j++)
        {
            zeznania[licznik] = kubelki[i][j];
            licznik++;
        }
    }
}

void wczytywanie(vector<int> &odpowiedzi)
{
    vector<Zeznanie> zeznania;
    int ile_informatykow;
    int ile_zeznan;
    //ifstream we("moj_0.txt");
    int z;
    cin >> z;
    for (int j = 0; j < z; j++)
    {
        cin >> ile_informatykow >> ile_zeznan;
        zeznania.resize(ile_zeznan);
        for (int i = 0; i < ile_zeznan; i++)
        {
            cin >> zeznania[i].czas >> zeznania[i].nr_informatyka >> zeznania[i].ilu_innych;
            zeznania[i].nr_informatyka--;
            zeznania[i].czas--;
        }
        //to preprocesing mamy za soba;
        int a = 0;
        int b = ile_zeznan + 1;
        while (b - a != 1)
        {
            int sredni = a + (b - a) / 2;
            vector<vector<int>> numery_wystapien_informatykow;
            vector<Zeznanie> podzbior;
            for (int i = 0; i < sredni; i++)
                podzbior.push_back(zeznania[i]);
            sortowanie_pozycyjne(podzbior, ile_zeznan);

            znajdz_kolejne(numery_wystapien_informatykow, podzbior, ile_informatykow, sredni);
            if (czy_pirwsze_k_jest_prawdziwych(sredni, numery_wystapien_informatykow, podzbior, ile_informatykow, sredni))
                a = sredni;
            else
                b = sredni;
        }
        odpowiedzi.push_back(a);
    }
}

int main()
{
    vector<int> odpowiedzi;
    wczytywanie(odpowiedzi);
    for (int i = 0; i < odpowiedzi.size(); i++)
    {
        cout << odpowiedzi[i];
        if (i != odpowiedzi.size() - 1)
            cout << endl;
    }
    //cout << endl
    //<< "doszlo do return 0" << endl;
    return 0;
}