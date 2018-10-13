#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

int char_to_int(char a)
{
    return (int)a - (int)'a';
}

void wczytywanie(vector<int> &slowo)
{
    //ifstream we("dane.txt");
    int n;
    cin >> n;
    string wczytany_string;
    cin >> wczytany_string;
    for (int i = 0; i < n; i++)
    {
        slowo.push_back(char_to_int(wczytany_string[i]));
    }
}

void domaxuj(int &a, int b)
{
    a = max(a, b);
}

class Info_o_dotychczasowych
{
  public:
    int suma_tu_sie_konczaca;
    int z_minustem_suma_tu_sie_konczaca;
    int maksymalna_z_minusem;
    bool czy_juz_byl_minus;
    Info_o_dotychczasowych()
    {
        suma_tu_sie_konczaca = 0;
        z_minustem_suma_tu_sie_konczaca = -1;
        maksymalna_z_minusem = 0;
        czy_juz_byl_minus = false;
    }
    void dodaj_plusa()
    {
        suma_tu_sie_konczaca = max(1, suma_tu_sie_konczaca + 1);
        if (czy_juz_byl_minus)
        {
            z_minustem_suma_tu_sie_konczaca++;
            domaxuj(maksymalna_z_minusem, z_minustem_suma_tu_sie_konczaca);
        }
    }
    void dodaj_minusa()
    {
        int stara_suma_tu_sie_konczaca = suma_tu_sie_konczaca;
        int stara_z_minusem_suma_tu_sie_konczaca = z_minustem_suma_tu_sie_konczaca;
        suma_tu_sie_konczaca = max(-1,stara_suma_tu_sie_konczaca-1);
        if (czy_juz_byl_minus)
        {
            z_minustem_suma_tu_sie_konczaca = max(max(stara_suma_tu_sie_konczaca - 1, stara_z_minusem_suma_tu_sie_konczaca - 1),-1);
        }
        else
        {
            czy_juz_byl_minus = true;
            z_minustem_suma_tu_sie_konczaca = max(-1,stara_suma_tu_sie_konczaca - 1);
        }
        domaxuj(maksymalna_z_minusem, z_minustem_suma_tu_sie_konczaca);
    }
};

class Macierz_infow
{
  public:
    vector<vector<Info_o_dotychczasowych>> tab;
    Macierz_infow()
    {
        int ile_liter = 26;
        tab.resize(ile_liter);
        for (int i = 0; i < ile_liter; i++)
            tab[i].resize(ile_liter);
    }
    Info_o_dotychczasowych &podaj_info(int nr_plusa, int nr_minusa)
    {
        return tab[nr_plusa][nr_minusa];
    }
};

void uzupelnij_macierz(vector<int> &slowo, Macierz_infow &macierz)
{
    for (int i = 0; i < slowo.size(); i++)
    {
        for (int j = 0; j < 26; j++)
        {
            int dochodzaca_litera = slowo[i];
            int druga_litera = j;
            if (dochodzaca_litera != druga_litera)
            {
                //przypadek, gdy dochodzaca jest dodatnia
                macierz.podaj_info(dochodzaca_litera,druga_litera).dodaj_plusa();
                macierz.podaj_info(druga_litera,dochodzaca_litera).dodaj_minusa();
            }
        }
    }
}

int podaj_odpowiedz(Macierz_infow & macierz){
    int odpowiedz=0;
    for(int i=0;i<26;i++){
        for(int j=0;j<26;j++){
            domaxuj(odpowiedz,macierz.podaj_info(i,j).maksymalna_z_minusem);
        }
    }
    return odpowiedz;
}

int main()
{
    vector<int> slowo;
    wczytywanie(slowo);
    Macierz_infow macierz;
    uzupelnij_macierz(slowo,macierz);
    cout<<podaj_odpowiedz(macierz);
    //cout << endl
         //<< "doszlo do return 0" << endl;
    return 0;
}
