#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

void wczytaj(int &liczba_kolorow, int &pierwszy_kolor, int &ostatni_kolor, vector<int> &ile_klockow_tego_koloru)
{
    //ifstream we("klo1a.in");
    cin >> liczba_kolorow >> pierwszy_kolor >> ostatni_kolor;
    pierwszy_kolor--;
    ostatni_kolor--;
    ile_klockow_tego_koloru.resize(liczba_kolorow);
    for (int i = 0; i < liczba_kolorow; i++)
    {
        cin >> ile_klockow_tego_koloru[i];
    }
}

int suma_wektora(vector<int> &tab)
{
    int suma = 0;
    for (int i = 0; i < tab.size(); i++)
    {
        suma += tab[i];
    }
    return suma;
}

void rob_kolejnosc_zwiedzania(int liczba_pustych, vector<int> &kolejnosc_zwiedzania)
{
    if (liczba_pustych % 2 == 0)
    {
        //najpierw parzyste potem nieparzyste
        int liczba_parzysta = 2;
        for (int i = 0; i < liczba_pustych / 2; i++)
        {
            kolejnosc_zwiedzania.push_back(liczba_parzysta);
            liczba_parzysta += 2;
        }
        int liczba_nieparzysta = 1;
        for (int i = 0; i < liczba_pustych / 2; i++)
        {
            kolejnosc_zwiedzania.push_back(liczba_nieparzysta);
            liczba_nieparzysta += 2;
        }
    }
    else
    {
        //najpierw nieparzyste potem parzyste
        int liczba_nieparzysta = 1;
        for (int i = 0; i < liczba_pustych / 2 + 1; i++)
        {
            kolejnosc_zwiedzania.push_back(liczba_nieparzysta);
            liczba_nieparzysta += 2;
        }

        int liczba_parzysta = 2;
        for (int i = 0; i < liczba_pustych / 2; i++)
        {
            kolejnosc_zwiedzania.push_back(liczba_parzysta);
            liczba_parzysta += 2;
        }
    }
}

bool czy_to_jest_w_wektorze(vector<int> & tab,int co){
    for(int i=0;i<tab.size();i++)
        if(tab[i]==co)
            return true;
    return false;
}

void wypelniaj(vector<int> &kolejnosc_zwiedzania, vector<int> &znaleziona_kolejnosc, vector<int> &ile_klockow_tego_koloru, int &pierwszy_nietkniety_index,
               vector<int> &juz_zrobione)
{
    for (int i = 0; i < ile_klockow_tego_koloru.size(); i++)
        if (! czy_to_jest_w_wektorze(juz_zrobione,i))
        {
            for (int j = 0; j < ile_klockow_tego_koloru[i]; j++)
            {
                znaleziona_kolejnosc[kolejnosc_zwiedzania[pierwszy_nietkniety_index]] = i;
                pierwszy_nietkniety_index++;
            }
        }
}

bool przypadek_parzysty_czy_i_jak_sie_da(int pierwszy, int ostatni, vector<int> &kolejnosc_zwiedzania, vector<int> &znaleziona_kolejnosc, vector<int> &ile_klockow_tego_koloru)
{
    //najpierw kryterium
    int liczba_pustych = kolejnosc_zwiedzania.size();
    for (int i = 0; i < ile_klockow_tego_koloru.size(); i++)
        if (ile_klockow_tego_koloru[i] > (liczba_pustych / 2))
            return false;
    if (pierwszy == ostatni && ile_klockow_tego_koloru[pierwszy] > (liczba_pustych / 2 - 1))
        return false;
    //no to teraz mozna przejsc do konstrukcji rozwiazania
    int pierwszy_nietkniety_index = 0;
    vector<int> juz_zrobione;
    juz_zrobione.push_back(pierwszy);
    for (; pierwszy_nietkniety_index < ile_klockow_tego_koloru[pierwszy]; pierwszy_nietkniety_index++)
    {
        znaleziona_kolejnosc[kolejnosc_zwiedzania[pierwszy_nietkniety_index]] = pierwszy;
    }
    if (pierwszy != ostatni)
    {
        //no to trzeba jeszcze uzupelnic ten ostatni;
        juz_zrobione.push_back(ostatni);
        for (int i = 0; i < ile_klockow_tego_koloru[ostatni]; i++)
        {
            znaleziona_kolejnosc[kolejnosc_zwiedzania[kolejnosc_zwiedzania.size() - 1 - i]] = ostatni;
        }
    }
    //teraz jeszcze te pozostale
    wypelniaj(kolejnosc_zwiedzania,znaleziona_kolejnosc,ile_klockow_tego_koloru,pierwszy_nietkniety_index,juz_zrobione);

    return true;
}


bool przypadek_nieparzysty_czy_i_jak_sie_da(int pierwszy, int ostatni, vector<int> &kolejnosc_zwiedzania, vector<int> &znaleziona_kolejnosc, vector<int> &ile_klockow_tego_koloru)
{
    int kolor_ponadpolowowy = -1;
    int liczba_pustych = kolejnosc_zwiedzania.size();
    for (int i = 0; i < ile_klockow_tego_koloru.size(); i++)
    {
        if (ile_klockow_tego_koloru[i] > liczba_pustych / 2 + 1)
            return false;
        if (ile_klockow_tego_koloru[i] == liczba_pustych / 2 + 1)
            kolor_ponadpolowowy = i;
    }
    int pierwszy_nieruszony_index = 0;
    if (kolor_ponadpolowowy != -1)
    {
        if (pierwszy == kolor_ponadpolowowy || ostatni == kolor_ponadpolowowy)
            return false;
        for (int i = 0; i < liczba_pustych / 2 + 1; i++)
        {
            znaleziona_kolejnosc[kolejnosc_zwiedzania[pierwszy_nieruszony_index]] = kolor_ponadpolowowy;
            pierwszy_nieruszony_index++;
        }
        vector<int> juz_zrobione;
        juz_zrobione.push_back(kolor_ponadpolowowy);
        wypelniaj(kolejnosc_zwiedzania,znaleziona_kolejnosc,ile_klockow_tego_koloru,pierwszy_nieruszony_index,juz_zrobione);
    }
    else
    {
        //liczebnosc zadnego koloru nie przekraca polowy
        vector<int> juz_zrobione;
        juz_zrobione.push_back(pierwszy);
        for(int i=0;i<ile_klockow_tego_koloru[pierwszy];i++){
            znaleziona_kolejnosc[kolejnosc_zwiedzania[kolejnosc_zwiedzania.size()-1-i]]=pierwszy;
        }
        if(pierwszy!=ostatni){
            juz_zrobione.push_back(ostatni);
            for(int i=0;i<ile_klockow_tego_koloru[ostatni];i++){
                znaleziona_kolejnosc[kolejnosc_zwiedzania[pierwszy_nieruszony_index]]=ostatni;
                pierwszy_nieruszony_index++;
            }
        }
        //teraz jeszcze te pozostale wypelnaimy
        wypelniaj(kolejnosc_zwiedzania,znaleziona_kolejnosc,ile_klockow_tego_koloru,pierwszy_nieruszony_index,juz_zrobione);
    }
    return true;
}

bool czy_i_jak_sie_da(int liczba_kolorow, int pierwszy_kolor, int ostatni_kolor, vector<int> &ile_klockow_tego_koloru, vector<int> &znaleziona_kolejnosc)
{
    //na poczatek trzeba pousuwac wiadomo
    int liczba_klockow = suma_wektora(ile_klockow_tego_koloru);
    if(liczba_klockow==1){
        if(pierwszy_kolor!=ostatni_kolor)
            return false;
        znaleziona_kolejnosc.push_back(pierwszy_kolor);
        return true;
    }
    int liczba_pustych = liczba_klockow - 2;
    ile_klockow_tego_koloru[pierwszy_kolor]--;
    ile_klockow_tego_koloru[ostatni_kolor]--;
    if (ile_klockow_tego_koloru[pierwszy_kolor] < 0 || ile_klockow_tego_koloru[ostatni_kolor] < 0)
        return false;

    znaleziona_kolejnosc.resize(liczba_klockow, -1);
    znaleziona_kolejnosc[0] = pierwszy_kolor;
    znaleziona_kolejnosc[liczba_klockow - 1] = ostatni_kolor;
    vector<int> kolejnosc_zwiedzania;
    rob_kolejnosc_zwiedzania(liczba_pustych, kolejnosc_zwiedzania);
    if(liczba_pustych%2==0){
        return przypadek_parzysty_czy_i_jak_sie_da(pierwszy_kolor,ostatni_kolor,kolejnosc_zwiedzania,znaleziona_kolejnosc,ile_klockow_tego_koloru);
    }else{
        return przypadek_nieparzysty_czy_i_jak_sie_da(pierwszy_kolor,ostatni_kolor,kolejnosc_zwiedzania,znaleziona_kolejnosc,ile_klockow_tego_koloru);
    }
}

int main()
{
    int liczba_kolorow, pierwszy_kolor, ostatni_kolor;
    vector<int> ile_klockow_tego_koloru;
    wczytaj(liczba_kolorow, pierwszy_kolor, ostatni_kolor, ile_klockow_tego_koloru);
    vector<int> znaleziona_kolejnosc;
    if(czy_i_jak_sie_da(liczba_kolorow,pierwszy_kolor,ostatni_kolor,ile_klockow_tego_koloru,znaleziona_kolejnosc)){
        for(int i=0;i<znaleziona_kolejnosc.size();i++){
            cout<<znaleziona_kolejnosc[i]+1;
            if(i!=znaleziona_kolejnosc.size()-1)
                cout<<" ";
        }
    }else{
        cout<<0;
    }
    //cout << endl
         //<< "doszlo return 0 doszlo" << endl;
    return 0;
}
