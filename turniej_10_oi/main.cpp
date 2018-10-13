#include <iostream>
#include <list>
#include <stack>
#include <fstream>
#include <vector>
using namespace std;

void wczytywanie(vector<vector<int>> &graf)
{
    ifstream we("dobre_testy/tur10.in");
    int n;
    we >> n;
    graf.resize(n);
    for (int i = 0; i < n; i++)
    {
        int pom;
        we >> pom;
        for (int j = 0; j < pom; j++)
        {
            int a;
            we >> a;
            graf[i].push_back(a - 1);
        }
    }
}

void transponuj(vector<vector<int>> &graf, vector<vector<int>> &transponowany)
{
    transponowany.resize(graf.size());
    for (int i = 0; i < graf.size(); i++)
    {
        for (int j = 0; j < graf[i].size(); j++)
        {
            transponowany[graf[i][j]].push_back(i);
        }
    }
}

void wypisz_graf(vector<vector<int>> &z_kim_wygrywa)
{
    for (int i = 0; i < z_kim_wygrywa.size(); i++)
    {
        cout << i << " z niego wychodzi do ";
        for (int j = 0; j < z_kim_wygrywa[i].size(); j++)
        {
            cout << z_kim_wygrywa[i][j] << " ";
        }
        cout << endl;
    }
}

void dfs_obrob_jeden(vector<vector<int>> &gdzie_nie_isc, vector<bool> &wrzucony_do_stosu,
                     vector<bool> &wrzucil_dzieci, vector<list<int>::iterator> &gdzie_w_liscie, stack<int> &stos,
                     list<int> *lista_nr_1, list<int> *lista_nr_2, bool &lista_nr_1_jest_pelna_nietknietych, int &ostatni_zdejmowany)
{
    //zalozenie jest takie, ze na stos nie jest pusty
    int obrabiany = stos.top();
    if (wrzucil_dzieci[obrabiany])
    {
        //wiec jest teraz zdejmowany ze stosu chyba
        stos.pop();
        ostatni_zdejmowany = obrabiany;
    }
    else
    {
        //trzeba przejsc do dzieci i je powrzucac do stosu
        wrzucil_dzieci[obrabiany] = true;
        list<int> *lista_nietknietych = lista_nr_1;
        list<int> *nowa_lista_nietknietych = lista_nr_2;
        if (!lista_nr_1_jest_pelna_nietknietych)
        {
            list<int> *pom = lista_nietknietych;
            lista_nietknietych = nowa_lista_nietknietych;
            nowa_lista_nietknietych = pom;
        }
        lista_nr_1_jest_pelna_nietknietych = !lista_nr_1_jest_pelna_nietknietych;

        for (int i = 0; i < gdzie_nie_isc[obrabiany].size(); i++)
        {
            int nie_idz_do = gdzie_nie_isc[obrabiany][i];
            if (!wrzucony_do_stosu[nie_idz_do])
            {
                //wiec trzeba go uratowac
                (*lista_nietknietych).erase(gdzie_w_liscie[nie_idz_do]);
                (*nowa_lista_nietknietych).push_front(nie_idz_do);
                gdzie_w_liscie[nie_idz_do] = (*nowa_lista_nietknietych).begin();
            }
        }
        //to teraz trzeba wszystkich ziomkow poprzerzucac do stosu
        for (list<int>::iterator i = (*lista_nietknietych).begin(); i != (*lista_nietknietych).end(); ++i)
        {
            int spadajacy_z_listy = *i;
            stos.push(spadajacy_z_listy);
            wrzucony_do_stosu[spadajacy_z_listy] = true;
        }
        //teraz trzeba ta liste niektnietych jakos oczyscic
        (*lista_nietknietych).clear();
    }
}

void dfs_wrzuc_jednego_na_stosa(int &poczatkowy, vector<bool> &wrzucony_do_stosu,
                                vector<list<int>::iterator> &gdzie_w_liscie, stack<int> &stos,
                                list<int> *lista_nr_1, list<int> *lista_nr_2, bool &lista_nr_1_jest_pelna_nietknietych)
{
    list<int> *pelna_lista;
    if (lista_nr_1_jest_pelna_nietknietych)
        pelna_lista = lista_nr_1;
    else
        pelna_lista = lista_nr_2;
    while (poczatkowy < wrzucony_do_stosu.size())
    {
        if (wrzucony_do_stosu[poczatkowy] == false)
        {
            stos.push(poczatkowy);
            wrzucony_do_stosu[poczatkowy] = true;
            pelna_lista->erase(gdzie_w_liscie[poczatkowy]);
            break;
        }
        poczatkowy++;
    }
}

void dfs_obrob_wszystkie(vector<vector<int>> &gdzie_nie_isc, int &ostatni_zdejmowany)
{
    vector<bool> wrzucony_do_stosu;
    vector<bool> wrzucil_dzieci;
    vector<list<int>::iterator> gdzie_w_liscie;
    stack<int> stos;
    list<int> *lista_nr_1 = new list<int>;
    list<int> *lista_nr_2 = new list<int>;
    bool lista_nr_1_jest_pelna_nietknietych = true;
    //teraz dorobic te wejsciowe
    wrzucony_do_stosu.resize(gdzie_nie_isc.size(), false);
    ostatni_zdejmowany = -1;
    wrzucil_dzieci.resize(gdzie_nie_isc.size(), false);
    for (int i = 0; i < gdzie_nie_isc.size(); i++)
    {
        lista_nr_1->push_front(i);
        gdzie_w_liscie.push_back(lista_nr_1->begin());
    }
    int poczatkowy = 0;
    dfs_wrzuc_jednego_na_stosa(poczatkowy, wrzucony_do_stosu, gdzie_w_liscie, stos, lista_nr_1, lista_nr_2, lista_nr_1_jest_pelna_nietknietych);
    while (!stos.empty())
    {
        while (!stos.empty())
        {
            dfs_obrob_jeden(gdzie_nie_isc, wrzucony_do_stosu,
                            wrzucil_dzieci, gdzie_w_liscie, stos,
                            lista_nr_1, lista_nr_2, lista_nr_1_jest_pelna_nietknietych, ostatni_zdejmowany);
        }
        dfs_wrzuc_jednego_na_stosa(poczatkowy, wrzucony_do_stosu, gdzie_w_liscie, stos, lista_nr_1, lista_nr_2, lista_nr_1_jest_pelna_nietknietych);
    }
    delete lista_nr_1;
    delete lista_nr_2;
}

void dfs_obrob_te_co_sie_da_osiagnac_z_danego(int &startowy, vector<vector<int>> &gdzie_nie_isc, vector<bool> &wrzucony_do_stosu, int &ostatni_zdejmowany, bool &czy_wszystkie)
{
    vector<bool> wrzucil_dzieci;
    vector<list<int>::iterator> gdzie_w_liscie;
    stack<int> stos;
    list<int> *lista_nr_1 = new list<int>;
    list<int> *lista_nr_2 = new list<int>;
    bool lista_nr_1_jest_pelna_nietknietych = true;
    //teraz dorobic te wejsciowe
    wrzucony_do_stosu.resize(gdzie_nie_isc.size(), false);
    ostatni_zdejmowany = -1;
    wrzucil_dzieci.resize(gdzie_nie_isc.size(), false);
    for (int i = 0; i < gdzie_nie_isc.size(); i++)
    {
        lista_nr_1->push_front(i);
        gdzie_w_liscie.push_back(lista_nr_1->begin());
    }

    stos.push(startowy);
    wrzucony_do_stosu[startowy] = true;
    lista_nr_1->erase(gdzie_w_liscie[startowy]);

    while (!stos.empty())
    {
        dfs_obrob_jeden(gdzie_nie_isc, wrzucony_do_stosu,
                        wrzucil_dzieci, gdzie_w_liscie, stos,
                        lista_nr_1, lista_nr_2, lista_nr_1_jest_pelna_nietknietych, ostatni_zdejmowany);
    }
    czy_wszystkie = !(lista_nr_1->size() != 0 || lista_nr_2->size() != 0);
    delete lista_nr_1;
    delete lista_nr_2;
}

int main()
{
    vector<vector<int>> z_kim_wygrywa;
    wczytywanie(z_kim_wygrywa);
    //cout << "z_kim_wygrywa" << endl;
    //wypisz_graf(z_kim_wygrywa);
    vector<vector<int>> z_kim_przegrywa;
    transponuj(z_kim_wygrywa, z_kim_przegrywa);
    //cout << "z kim przegrywa" << endl;
    //wypisz_graf(z_kim_przegrywa);
    int ostatni_zdejmowany = -1;
    dfs_obrob_wszystkie(z_kim_przegrywa, ostatni_zdejmowany);
    bool czy_wszystkie = false;
    int niewazny = -1;
    vector<bool> wrzucony_do_stosu;
    //dfs_obrob_te_co_sie_da_osiagnac_z_danego(ostatni_zdejmowany,z_kim_przegrywa,wrzucony_do_stosu,niewazny,czy_wszystkie);
    //cout<<"czy wszystkie "<<czy_wszystkie;

    //to mozna wypisywac te ktore moga wygrac
    bool niewazny_bool;
    dfs_obrob_te_co_sie_da_osiagnac_z_danego(ostatni_zdejmowany, z_kim_wygrywa, wrzucony_do_stosu, niewazny, niewazny_bool);
    vector<int> wygrywajace;
    for (int i = 0; i < wrzucony_do_stosu.size(); i++)
    {
        if (wrzucony_do_stosu[i])
            wygrywajace.push_back(i + 1);
    }
    cout<< wygrywajace.size();
    /*
    for(int i=0;i<wygrywajace.size();i++){
        cout<<" "<< wygrywajace[i];
    }
    */

    //cout << endl
         //<< "doszlo do return 0" << endl;
    return 0;
}
