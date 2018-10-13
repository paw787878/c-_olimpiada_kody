#include <iostream>
#include <stack>
#include <fstream>
#include <vector>
using namespace std;

void wczytywanie(string &s)
{
    //ifstream we("dane.txt");
    int n;
    cin >> n;
    cin >> s;
}

void rob_nastepny(vector<int> &nastepny, string &slowo)
{
    //dziala dobrze.
    stack<int> stos;
    nastepny.resize(slowo.size(), -1);
    for (int i = 0; i < slowo.size(); i++)
    {
        if (slowo[i] == '(')
        {
            //otwieramy nawias
            stos.push(i);
        }
        else if (stos.empty())
        {
            //wiec jest ) i stos jest pusty wiec nie ma dla niego cookies
        }
        else
        {
            //wiec jest ) i stos jest cos ma
            int poprzedni = stos.top();
            stos.pop();
            nastepny[poprzedni] = i;
        }
    }
}

void dorob_dla_zwartego_kawalka(vector<int> &nastepny, long long int &licznik_ile, int pocz, int konc)
{
    //pocz oraz konc to jest jak w pythonie, fajnie

    //to jest jakby nietrywialne
    int nawias_otwierajacy = pocz;
    int ile_stoi_obok_siebie = 0;
    while (nawias_otwierajacy < konc)
    {
        //wiec jest fajnie
        ile_stoi_obok_siebie++;
        int nawias_zamykajacy = nastepny[nawias_otwierajacy];
        dorob_dla_zwartego_kawalka(nastepny, licznik_ile, nawias_otwierajacy + 1, nawias_zamykajacy);
        nawias_otwierajacy = nawias_zamykajacy + 1;
    }
    //teraz trzeba dodac
    licznik_ile += ((long long int)ile_stoi_obok_siebie) * ((long long int)(ile_stoi_obok_siebie + 1)) / 2;
}

void odnajdz_kawalki_zwarte(vector<int> &nastepny, long long int &licznik_ile)
{
    int obecny_index = 0;
    while (obecny_index < nastepny.size())
        if (nastepny[obecny_index] == -1)
        {
            obecny_index++;
        }
        else
        {
            //to znaczy, ze cos sie zaczyna;
            int pocz = obecny_index;
            do
            {
                obecny_index = nastepny[obecny_index] + 1;
            } while (obecny_index < nastepny.size() && nastepny[obecny_index] != -1);
            //teraz obecny index to jest jeden za tym co sie konczy
            dorob_dla_zwartego_kawalka(nastepny,licznik_ile,pocz,obecny_index);
        }
}

void wypisz_wektor(vector<int> &w)
{
    cout << endl;
    for (int i = 0; i < w.size(); i++)
    {
        cout << i << " " << w[i] << endl;
    }
    cout << endl;
}

int main()
{
    string slowo;
    wczytywanie(slowo);
    long long int licznik_ile = 0;
    vector<int> nastepny;
    rob_nastepny(nastepny, slowo);
    //wypisz_wektor(nastepny);
    odnajdz_kawalki_zwarte(nastepny,licznik_ile);
    cout << licznik_ile;
    //cout << endl
        // << "doszlo do return 0" << endl;
    return 0;
}
