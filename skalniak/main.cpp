#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

void zamien(int &x, int &y)
{
    int pom = x;
    x = y;
    y = pom;
}

void domaxuj(int &a, int b)
{
    a = max(a, b);
}

void dominuj(int &a, int b)
{
    a = min(a, b);
}

void wczytywanie(vector<pair<int, int>> &punkty, vector<bool> &czy_przestawilem,
                 vector<int> &ciezary, int &x_min, int &x_max, int &y_min, int &y_max)
{
    //ifstream we("dane.txt");
    int n;
    cin >> n;
    x_min = 1000 * 1000 * 1000;
    y_min = 1000 * 1000 * 1000;
    x_max = 0;
    y_max = 0;
    int x, y, pom;
    bool przestawienie;
    for (int i = 0; i < n; i++)
    {
        przestawienie = false;
        cin >> x >> y >> pom;
        //ma byc x<= y
        if (x > y)
        {
            przestawienie = true;
            zamien(x, y);
        }

        dominuj(x_min, x);
        dominuj(y_min, y);

        domaxuj(x_max, x);
        domaxuj(y_max, y);

        punkty.push_back(pair<int, int>(x, y));
        czy_przestawilem.push_back(przestawienie);
        ciezary.push_back(pom);
    }
}

class Info_o_klasie
{
  public:
    int ile_w_niej_jest;
    int koszt_gdy_klasa_jest_u_gory;
    int koszt_gdy_klasa_jest_na_dole;
    Info_o_klasie()
    {
        ile_w_niej_jest = 0;
        koszt_gdy_klasa_jest_na_dole = 0;
        koszt_gdy_klasa_jest_u_gory = 0;
    }
};

void podziel_i_zeskatystykuj(vector<pair<int, int>> &punkty, vector<bool> &czy_przestawilem,
                             vector<int> &ciezary, int &x_min, int &x_max, int &y_min, int &y_max, vector<bool> &w_x_przes, vector<bool> &w_y_przes, vector<vector<Info_o_klasie>> &infa)
{
    int y_bar = x_max;
    int x_bar = y_min;

    infa.resize(2);
    for (int i = 0; i < 2; i++)
    {
        infa[i].resize(2);
    }

    for (int i = 0; i < punkty.size(); i++)
    {
        w_x_przes.push_back(punkty[i].first < x_bar);
        w_y_przes.push_back(punkty[i].second > y_bar);
        infa[w_x_przes[i]][w_y_przes[i]].ile_w_niej_jest++;
        if (czy_przestawilem[i])
            infa[w_x_przes[i]][w_y_przes[i]].koszt_gdy_klasa_jest_u_gory += ciezary[i];
        else
            infa[w_x_przes[i]][w_y_przes[i]].koszt_gdy_klasa_jest_na_dole += ciezary[i];
    }
}

int main()
{
    vector<pair<int, int>> punkty;
    vector<bool> czy_przestawilem;
    vector<int> ciezary;
    int x_min;
    int x_max;
    int y_min;
    int y_max;

    wczytywanie(punkty, czy_przestawilem, ciezary, x_min, x_max, y_min, y_max);
    vector<bool> w_x_przes;
    vector<bool> w_y_przes;
    vector<vector<Info_o_klasie>> infa;
    podziel_i_zeskatystykuj(punkty, czy_przestawilem, ciezary, x_min, x_max, y_min, y_max, w_x_przes, w_y_przes, infa);

    //teraz latwo
    long long int obwod = (x_max - x_min) + (y_max - y_min);
    obwod *= 2;
    int koszt = 0;
    vector<vector<bool>> jest_u_gory;
    jest_u_gory.resize(2);
    for (int i = 0; i < 2; i++)
        jest_u_gory[i].resize(2, false);

    if (infa[true][true].ile_w_niej_jest > 0)
    {
        int calkowity_koszt_u_gory = 0;
        calkowity_koszt_u_gory += infa[true][true].koszt_gdy_klasa_jest_u_gory;
        calkowity_koszt_u_gory += infa[true][false].koszt_gdy_klasa_jest_u_gory;
        calkowity_koszt_u_gory += infa[false][true].koszt_gdy_klasa_jest_u_gory;

        int calkowity_koszt_na_dole = 0;
        calkowity_koszt_na_dole += infa[true][true].koszt_gdy_klasa_jest_na_dole;
        calkowity_koszt_na_dole += infa[true][false].koszt_gdy_klasa_jest_na_dole;
        calkowity_koszt_na_dole += infa[false][true].koszt_gdy_klasa_jest_na_dole;

        bool te_3_u_gory = (calkowity_koszt_na_dole > calkowity_koszt_u_gory);
        if (te_3_u_gory)
            koszt = calkowity_koszt_u_gory;
        else
            koszt = calkowity_koszt_na_dole;
        jest_u_gory[true][true] = te_3_u_gory;
        jest_u_gory[true][false] = te_3_u_gory;
        jest_u_gory[false][true] = te_3_u_gory;
    }
    else
    {
        //te 2 niezaleznie sie robia
        jest_u_gory[true][false] = (infa[true][false].koszt_gdy_klasa_jest_na_dole > infa[true][false].koszt_gdy_klasa_jest_u_gory);
        jest_u_gory[false][true] = (infa[false][true].koszt_gdy_klasa_jest_na_dole > infa[false][true].koszt_gdy_klasa_jest_u_gory);

        koszt += min(infa[true][false].koszt_gdy_klasa_jest_na_dole, infa[true][false].koszt_gdy_klasa_jest_u_gory);
        koszt += min(infa[false][true].koszt_gdy_klasa_jest_na_dole, infa[false][true].koszt_gdy_klasa_jest_u_gory);
    }

    //teraz trzeba wypisac output
    cout << obwod << " " << koszt << endl;
    for (int i = 0; i < punkty.size(); i++)
    {
        if (w_x_przes[i] != false || w_y_przes[i] != false)
            if (jest_u_gory[w_x_przes[i]][w_y_przes[i]] == czy_przestawilem[i])
                cout << "1";
            else
                cout << "0";
        else
            cout<<"0";
    }

    //cout << endl
        // << "doszlo do return 0" << endl;
    return 0;
}
