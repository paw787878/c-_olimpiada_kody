#include <iostream>
#include <vector>
#include <fstream>
#include <queue>
using namespace std;

void wczytaj(vector<vector<int>> &wysokosci, vector<vector<pair<int, int>>> &punkty_bajtocji, int &m, int &n)
{
    //ifstream we("dane.txt");
    //int m,n;
    cin >> m >> n;
    punkty_bajtocji.resize(1001);
    for (int i = 0; i < m; i++)
    {
        vector<int> linia_wys;
        for (int j = 0; j < n; j++)
        {
            int pom;
            cin >> pom;

            if (pom > 0)
            {
                punkty_bajtocji[pom].push_back(pair<int, int>(i, j));
            }
            else
                pom = -pom;

            linia_wys.push_back(pom);
        }
        wysokosci.push_back(linia_wys);
    }
}

bool czy_siedzi(int m, int n, pair<int, int> punkt)
{
    return punkt.first < m && punkt.first >= 0 && punkt.second < n && punkt.second >= 0;
}

void rob_sasiadow(vector<pair<int, int>> &sasiedzi, int m, int n, pair<int, int> &obrabiany_punkt)
{
    vector<pair<int, int>> przesuniecia;
    przesuniecia.push_back(pair<int, int>(0, 1));
    przesuniecia.push_back(pair<int, int>(0, -1));
    przesuniecia.push_back(pair<int, int>(1, 0));
    przesuniecia.push_back(pair<int, int>(-1, 0));
    for (int i = 0; i < 4; i++)
    {
        pair<int, int> nowy = pair<int, int>(obrabiany_punkt.first + przesuniecia[i].first, obrabiany_punkt.second + przesuniecia[i].second);
        if (czy_siedzi(m, n, nowy))
            sasiedzi.push_back(nowy);
    }
}

int main()
{
    vector<vector<int>> wysokosci;
    vector<vector<pair<int, int>>> punkty_bajtocji;
    int m, n;
    wczytaj(wysokosci, punkty_bajtocji, m, n);
    //robie tablice nr wylewu
    vector<vector<int>> nr_wylewu_tab;
    nr_wylewu_tab.resize(m);
    for (int i = 0; i < m; i++)
    {
        nr_wylewu_tab[i].resize(n, -1);
    }

    //zaczynam te bfs
    queue<pair<int, int>> punkty_do_porobienia;
    int chwilowy_nr_wylewu = 0;
    int liczba_pomp = 0;
    for (int wys = 0; wys <= 1000; wys++)
    {
        for (int i_wys = 0; i_wys < punkty_bajtocji[wys].size(); i_wys++)
        {
            pair<int, int> poczatkowy_punkt = punkty_bajtocji[wys][i_wys];
            if (nr_wylewu_tab[poczatkowy_punkt.first][poczatkowy_punkt.second] == -1)
            {
                bool czy_trafilem_do_starego_rozlewiska = false;
                punkty_do_porobienia.push(poczatkowy_punkt);
                nr_wylewu_tab[poczatkowy_punkt.first][poczatkowy_punkt.second] = chwilowy_nr_wylewu;
                while (!punkty_do_porobienia.empty())
                {
                    pair<int, int> obrabiany_punkt = punkty_do_porobienia.front();
                    punkty_do_porobienia.pop();
                    vector<pair<int, int>> sasiedzi;
                    rob_sasiadow(sasiedzi, m, n, obrabiany_punkt);
                    for (int i = 0; i < sasiedzi.size(); i++)
                    {
                        pair<int, int> sasiad = sasiedzi[i];
                        if (nr_wylewu_tab[sasiad.first][sasiad.second] == -1)
                        {
                            if (wysokosci[sasiad.first][sasiad.second] <= wys)
                            {
                                punkty_do_porobienia.push(sasiad);
                                nr_wylewu_tab[sasiad.first][sasiad.second] = chwilowy_nr_wylewu;
                            }
                        }
                        else {
                            //teraz nr_wylewu to nie -1
                            if(nr_wylewu_tab[sasiad.first][sasiad.second] != chwilowy_nr_wylewu)
                                czy_trafilem_do_starego_rozlewiska=true;
                        }
                    }
                }

                chwilowy_nr_wylewu++;
                if (!czy_trafilem_do_starego_rozlewiska)
                    liczba_pomp++;
            }
        }
    }

    cout<<liczba_pomp;

    return 0;
}
