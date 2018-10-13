#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
using namespace std;

bool dominuj(int &a, int b)
{
    bool wyrzut = (b < a);
    a = min(a, b);
    return wyrzut;
}

class Podkaktus
{
  public:
    int nr_noda;
    int index_tree_syna;
    Podkaktus() {}
    Podkaktus(int nr_noda, int index_tree_syna)
    {
        this->nr_noda = nr_noda;
        this->index_tree_syna = index_tree_syna;
    }
};

class Para_parzystosci
{
  public:
    bool wymusi_parzyste;
    bool wymusi_nieparzyste;
    Para_parzystosci()
    {
        wymusi_nieparzyste = false;
        wymusi_parzyste = false;
    }
    void wypisz()
    {
        cout << "wymusi parzyste " << wymusi_parzyste << endl;
        cout << "wymusi nieparzyste " << wymusi_nieparzyste << endl;
    }
};

class Tree_syn
{
  public:
    int nr;
    Para_parzystosci info_podkaktusa;
    //bool rozpocz_wymusi_nieparzyste_w_plecach;
    Tree_syn()
    {
        nr = -1;
        //rozpocz_wymusi_parzyste_w_podkaktusie = 0;
        //rozpocz_wymusi_nieparzyste_w_podkaktusie = 0;
        //rozpocz_wymusi_nieparzyste_w_plecach = 0;
    }
    Tree_syn(int nr)
    {
        this->nr = nr;
        //rozpocz_wymusi_nieparzyste_w_plecach = false;
        //rozpocz_wymusi_nieparzyste_w_podkaktusie = false;
        //rozpocz_wymusi_parzyste_w_podkaktusie = false;
    }
    void wypisz()
    {
        cout << "nr " << nr + 1 << endl;
        //cout << "plecy_nieparzyste " << rozpocz_wymusi_nieparzyste_w_plecach << endl;
        cout << "podkaktus " << endl;
        info_podkaktusa.wypisz();
    }
};

int nies = 1000000000;

class Node
{
  public:
    int inorder;
    int min_back_inorder_w_poddrzewie;
    vector<int> sasiedzi;
    vector<Tree_syn> tree_synowie;
    int dfs_ojciec;
    int ile_super_wygrywaczy;
    int ile_parzystych;
    int ile_nieparzystych;
    Tree_syn kapatybilny_do_dfs_ojciec;
    Node()
    {
        ile_super_wygrywaczy = 0;
        ile_nieparzystych = 0;
        ile_parzystych = 0;
        dfs_ojciec = -1;
        inorder = -1;
        min_back_inorder_w_poddrzewie = nies;
    }
    /*
    void policz_ile_od_tree_synow_ale_nie_od_kapatybilny()
    {
        for (int i = 0; i < tree_synowie.size(); i++)
        {
            bool czy_wymusi_parzyste = tree_synowie[i].info_podkaktusa.wymusi_parzyste;
            bool czy_wymusi_nieparzyste = tree_synowie[i].info_podkaktusa.wymusi_nieparzyste;
            if (czy_wymusi_nieparzyste && czy_wymusi_parzyste)
                ile_super_wygrywaczy++;
            if (czy_wymusi_nieparzyste && !czy_wymusi_parzyste)
                ile_nieparzystych++;
            if ((!czy_wymusi_nieparzyste) && czy_wymusi_parzyste)
                ile_parzystych++;
        }
    }
    */
    void wypisz()
    {
        cout <<endl<<endl<< "teraz opisuje jeden node" << endl;
        cout << "inorder " << inorder << endl;
        cout << "min inorder " << min_back_inorder_w_poddrzewie << endl;
        cout << "dfs ojciec " << dfs_ojciec + 1 << endl;
        cout<<"ile super wygrywaczy "<<ile_super_wygrywaczy<<endl;
        cout<<"ile parzystych "<<ile_parzystych<<endl;
        cout<<"ile nieparzystych "<<ile_nieparzystych<<endl;
        cout << "kapatybilny do dfs ojciec " << endl;
        kapatybilny_do_dfs_ojciec.wypisz();
        cout << "tree synowie" << endl;
        for (int i = 0; i < tree_synowie.size(); i++)
        {
            tree_synowie[i].wypisz();
            cout << endl<<endl;
        }
        cout << "koniec wypisywania node" << endl;
    }
    bool czy_plecy_daja_mozliwosc_przejscia_nieparzystego(Tree_syn &rozwazany)
    {
        int ile_super_wygrywaczy_2 = ile_super_wygrywaczy - (int)(rozwazany.info_podkaktusa.wymusi_nieparzyste && rozwazany.info_podkaktusa.wymusi_parzyste);
        int ile_nieparzystych_2 = ile_nieparzystych - (int)(rozwazany.info_podkaktusa.wymusi_nieparzyste && !rozwazany.info_podkaktusa.wymusi_parzyste);
        int ile_parzystych_2 = ile_parzystych - (int)((!rozwazany.info_podkaktusa.wymusi_nieparzyste) && rozwazany.info_podkaktusa.wymusi_parzyste);
        if (ile_super_wygrywaczy_2)
            return true;
        if (ile_nieparzystych_2%2==1)
            return true;
        return false;
    }
    bool czy_wygrywajacy()
    {
        if (ile_super_wygrywaczy)
            return true;
        if (ile_nieparzystych%2==1)
            return true;
        return false;
    }
};

class Dfs_item_1
{
  public:
    int nr;
    int ostatni_robiony_index;
    Dfs_item_1(int nr)
    {
        this->nr = nr;
        ostatni_robiony_index = -1;
    }
};


class Graf
{
  public:
    vector<Node> nody;
    int startowy; //stopnia 2
    Graf()
    {
        startowy = -1;
    }
    void inicjoj(int n)
    {
        nody.resize(n);
    }
    void dodaj(int a, int b)
    {
        //juz od 0 do n-1
        nody[a].sasiedzi.push_back(b);
        nody[b].sasiedzi.push_back(a);
    }
    void szukaj_startowego()
    {
        int i = 0;
        while (nody[i].sasiedzi.size() != 2)
            i++;
        startowy = i;
    }
    void rob_dfs_tree()
    {
        stack<Dfs_item_1> stos;
        stos.push(Dfs_item_1(startowy));
        int czas = 0;
        nody[startowy].inorder = czas;
        nody[startowy].min_back_inorder_w_poddrzewie = czas;
        czas++;
        while (!stos.empty())
        {
            Dfs_item_1 obrabiany = stos.top();
            if (obrabiany.ostatni_robiony_index != -1)
            {
                //musimy dominowac
                int nr_syna = nody[obrabiany.nr].sasiedzi[obrabiany.ostatni_robiony_index];
                if (dominuj(nody[obrabiany.nr].min_back_inorder_w_poddrzewie, nody[nr_syna].min_back_inorder_w_poddrzewie))
                {
                    nody[obrabiany.nr].kapatybilny_do_dfs_ojciec.nr = nr_syna;
                    nody[obrabiany.nr].tree_synowie.resize(nody[obrabiany.nr].tree_synowie.size() - 1);
                }
            }
            stos.top().ostatni_robiony_index++;
            while (stos.top().ostatni_robiony_index < nody[stos.top().nr].sasiedzi.size() && nody[nody[obrabiany.nr].sasiedzi[stos.top().ostatni_robiony_index]].inorder != -1)
            {
                if (nody[obrabiany.nr].sasiedzi[stos.top().ostatni_robiony_index] != nody[obrabiany.nr].dfs_ojciec)
                {
                    dominuj(nody[obrabiany.nr].min_back_inorder_w_poddrzewie, nody[nody[obrabiany.nr].sasiedzi[stos.top().ostatni_robiony_index]].inorder);
                }
                stos.top().ostatni_robiony_index++;
            }

            if (stos.top().ostatni_robiony_index == nody[stos.top().nr].sasiedzi.size())
            {
                //trzeba zdjac
                stos.pop();
            }
            else
            {
                int nr_syna = nody[stos.top().nr].sasiedzi[stos.top().ostatni_robiony_index];
                nody[nr_syna].dfs_ojciec = stos.top().nr;
                nody[nr_syna].inorder = czas;
                nody[nr_syna].min_back_inorder_w_poddrzewie = czas;
                czas++;
                nody[stos.top().nr].tree_synowie.push_back(Tree_syn(nr_syna));
                stos.push(Dfs_item_1(nr_syna));
            }
        }
    }
    void znajdz_odrosty_na_petli_co_nie_sa_na_starcie(Podkaktus &startowy_node, vector<Podkaktus> &podkaktusy)
    {
        int podroznik = nody[startowy_node.nr_noda].tree_synowie[startowy_node.index_tree_syna].nr;
        while (podroznik != -1)
        {
            for (int i = 0; i < nody[podroznik].tree_synowie.size(); i++)
            {
                podkaktusy.push_back(Podkaktus(podroznik, i));
            }

            podroznik = nody[podroznik].kapatybilny_do_dfs_ojciec.nr;
        }
    }
    void znajdz_petle_bez_startowego(vector<int> &petla, Podkaktus &startowy_node)
    {
        int podroznik = nody[startowy_node.nr_noda].tree_synowie[startowy_node.index_tree_syna].nr;
        while (podroznik != -1)
        {
            petla.push_back(podroznik);

            podroznik = nody[podroznik].kapatybilny_do_dfs_ojciec.nr;
        }
    }
    void wypisz()
    {
        cout << "wypisuje graf" << endl;
        cout << "startowy " << startowy + 1 << endl<<endl;
        for (int i = 0; i < nody.size(); i++)
        {
            cout <<endl<<endl<< "teraz node nr " << i + 1 << endl<<endl<<endl;
            nody[i].wypisz();
            cout << endl
                 << endl;
        }
    }
    bool czy_plecy_moga_byc_nieparzyscie_od_itego_tree_syna(int nr_noda, int index_tree_syna)
    {
        return nody[nr_noda].czy_plecy_daja_mozliwosc_przejscia_nieparzystego(nody[nr_noda].tree_synowie[index_tree_syna]);
    }
    bool czyplecy_moga_byc_nieparzyscie_od_kapatybilnego(int nr_noda)
    {
        return nody[nr_noda].czy_plecy_daja_mozliwosc_przejscia_nieparzystego(nody[nr_noda].kapatybilny_do_dfs_ojciec);
    }
    void podaj_odpowiedzi_z_zadania()
    {
        for (int i = 0; i < nody.size(); i++)
        {
            if (nody[i].czy_wygrywajacy())
                cout << 1;
            else
                cout << 2;
            if (i != nody.size() - 1)
                cout << endl;
        }
    }
};

void faza_1_dla_petli(Graf &graf, Podkaktus &startowy)
{
    //rob problemy plecow na calej petli
    vector<int> rzeczy_do_tkniecia;
    graf.znajdz_petle_bez_startowego(rzeczy_do_tkniecia, startowy);
    /*
    for (int i = 0; i < rzeczy_do_tkniecia.size(); i++)
    {
        graf.nody[rzeczy_do_tkniecia[i]].policz_ile_od_tree_synow_ale_nie_od_kapatybilny();
    }
    */

    //rob problem prostszy dla wywolujacego
    int pierwszy_od_lewej = -1;
    int pierwszy_od_prawej = -1;
    for (int i = 0; i < rzeczy_do_tkniecia.size(); i++)
        if (graf.czyplecy_moga_byc_nieparzyscie_od_kapatybilnego(rzeczy_do_tkniecia[i]))
        {
            pierwszy_od_lewej = i + 1;
            break;
        }
    for (int i = rzeczy_do_tkniecia.size() - 1; i >= 0; i--)
    {
        if (graf.czyplecy_moga_byc_nieparzyscie_od_kapatybilnego(rzeczy_do_tkniecia[i]))
        {
            pierwszy_od_prawej = rzeczy_do_tkniecia.size() - i;
            break;
        }
    }
    if (pierwszy_od_lewej == -1)
    {
        //to znaczy, ze to zalezy od parzystoci tej petli
        if (rzeczy_do_tkniecia.size() % 2 == 0)
        {
            //petla jest nieparzysta
            graf.nody[startowy.nr_noda].tree_synowie[startowy.index_tree_syna].info_podkaktusa.wymusi_nieparzyste = true;
            graf.nody[startowy.nr_noda].tree_synowie[startowy.index_tree_syna].info_podkaktusa.wymusi_parzyste = false;
            graf.nody[startowy.nr_noda].ile_nieparzystych++;
        }
        else
        {
            graf.nody[startowy.nr_noda].tree_synowie[startowy.index_tree_syna].info_podkaktusa.wymusi_nieparzyste = false;
            graf.nody[startowy.nr_noda].tree_synowie[startowy.index_tree_syna].info_podkaktusa.wymusi_parzyste = true;
            graf.nody[startowy.nr_noda].ile_parzystych++;
        }
    }
    else
    {
        //sa jakies na petli.
        if (pierwszy_od_lewej % 2 == 0 || pierwszy_od_prawej % 2 == 0)
        {
            graf.nody[startowy.nr_noda].tree_synowie[startowy.index_tree_syna].info_podkaktusa.wymusi_nieparzyste = true;
            graf.nody[startowy.nr_noda].tree_synowie[startowy.index_tree_syna].info_podkaktusa.wymusi_parzyste = true;
            graf.nody[startowy.nr_noda].ile_super_wygrywaczy++;
        }
        else
        {
            graf.nody[startowy.nr_noda].tree_synowie[startowy.index_tree_syna].info_podkaktusa.wymusi_nieparzyste = false;
            graf.nody[startowy.nr_noda].tree_synowie[startowy.index_tree_syna].info_podkaktusa.wymusi_parzyste = false;
        }
    }
}

class Dfs_item_faza_1
{
  public:
    Podkaktus ja;
    bool pierwsze_dotkniecie;
    Dfs_item_faza_1(Podkaktus ja)
    {
        this->ja = ja;
        pierwsze_dotkniecie = true;
    }
};

void pierwsza_faza(Graf &graf)
{
    stack<Dfs_item_faza_1> stos;
    stos.push(Podkaktus(graf.startowy, 0));
    while (!stos.empty())
    {

        if (stos.top().pierwsze_dotkniecie)
        {
            stos.top().pierwsze_dotkniecie = false;
            vector<Podkaktus> podkaktusy;
            graf.znajdz_odrosty_na_petli_co_nie_sa_na_starcie(stos.top().ja, podkaktusy);
            for (int i = 0; i < podkaktusy.size(); i++)
            {
                stos.push(Dfs_item_faza_1(podkaktusy[i]));
            }
        }
        else
        {
            Podkaktus startowy = stos.top().ja;
            stos.pop();
            faza_1_dla_petli(graf, startowy);
        }
    }
}

void znajduj_odleglosci(vector<bool> &wektor_booli, vector<int> &ile_w_lewo, vector<int> &ile_w_prawo)
{
    ile_w_lewo.resize(wektor_booli.size(), -1);
    ile_w_prawo.resize(wektor_booli.size(), -1);
    int licznik = 0;
    for (int a = 0; a < 2; a++)
    {
        for (int i = 0; i < wektor_booli.size(); i++)
        {
            ile_w_lewo[i] = licznik;
            if (wektor_booli[i])
                licznik = 0;
            licznik++;
        }
    }
    licznik = 0;
    for (int a = 0; a < 2; a++)
    {
        for (int i = wektor_booli.size() - 1; i >= 0; i--)
        {
            ile_w_prawo[i] = licznik;
            if (wektor_booli[i])
                licznik = 0;
            licznik++;
        }
    }
}

void faza_2_dla_petli(Graf &graf, Podkaktus &start)
{
    vector<int> petla;
    graf.znajdz_petle_bez_startowego(petla, start);
    vector<bool> tab_czy_nieparzyste_plecy;
    for (int i = 0; i < petla.size(); i++)
    {
        tab_czy_nieparzyste_plecy.push_back(graf.czyplecy_moga_byc_nieparzyscie_od_kapatybilnego(petla[i]));
    }
    //petla.push_back(start.nr_noda);
    tab_czy_nieparzyste_plecy.push_back(graf.czy_plecy_moga_byc_nieparzyscie_od_itego_tree_syna(start.nr_noda, start.index_tree_syna));
    //to teraz tab_czy_nieparzyste_plecy jest spoko.
    vector<int> ile_w_lewo;
    vector<int> ile_w_prawo;
    znajduj_odleglosci(tab_czy_nieparzyste_plecy, ile_w_lewo, ile_w_prawo);
    for (int i = 0; i < petla.size(); i++)
    {

        if (ile_w_lewo[i] >= tab_czy_nieparzyste_plecy.size())
        {
            //to znaczy, ze to zalezy od parzystoci tej petli
            if (tab_czy_nieparzyste_plecy.size() % 2 == 1)
            {
                //petla jest nieparzysta
                graf.nody[petla[i]].kapatybilny_do_dfs_ojciec.info_podkaktusa.wymusi_nieparzyste = true;
                graf.nody[petla[i]].kapatybilny_do_dfs_ojciec.info_podkaktusa.wymusi_parzyste = false;
                graf.nody[petla[i]].ile_nieparzystych++;
            }
            else
            {
                graf.nody[petla[i]].kapatybilny_do_dfs_ojciec.info_podkaktusa.wymusi_nieparzyste = false;
                graf.nody[petla[i]].kapatybilny_do_dfs_ojciec.info_podkaktusa.wymusi_parzyste = true;
                graf.nody[petla[i]].ile_parzystych++;
            }
        }
        else
        {
            //sa jakies na petli.
            if (ile_w_lewo[i] % 2 == 0 || ile_w_prawo[i] % 2 == 0)
            {
                graf.nody[petla[i]].kapatybilny_do_dfs_ojciec.info_podkaktusa.wymusi_nieparzyste = true;
                graf.nody[petla[i]].kapatybilny_do_dfs_ojciec.info_podkaktusa.wymusi_parzyste = true;
                graf.nody[petla[i]].ile_super_wygrywaczy++;
            }
            else
            {
                graf.nody[petla[i]].kapatybilny_do_dfs_ojciec.info_podkaktusa.wymusi_nieparzyste = false;
                graf.nody[petla[i]].kapatybilny_do_dfs_ojciec.info_podkaktusa.wymusi_parzyste = false;
            }
        }
        
    }
}

void druga_faza(Graf &graf)
{
    stack<Podkaktus> stos;
    stos.push(Podkaktus(graf.startowy, 0));
    while (!stos.empty())
    {
        Podkaktus startowy = stos.top();
        stos.pop();
        faza_2_dla_petli(graf, startowy);
        vector<Podkaktus> podkaktusy;
        graf.znajdz_odrosty_na_petli_co_nie_sa_na_starcie(startowy, podkaktusy);
        for (int i = 0; i < podkaktusy.size(); i++)
        {
            stos.push(podkaktusy[i]);
        }
    }
}

void wczytaj(Graf &graf)
{
    //ifstream we("dane3.txt");
    int n, m, a, b;
    cin >> n >> m;
    graf.inicjoj(n);
    for (int i = 0; i < m; i++)
    {
        cin >> a >> b;
        a--;
        b--;
        graf.dodaj(a, b);
    }
}

int main()
{
    Graf graf;
    wczytaj(graf);
    graf.szukaj_startowego();
    graf.rob_dfs_tree();
    pierwsza_faza(graf);
    //graf.wypisz();
    druga_faza(graf);
    //graf.wypisz();
    graf.podaj_odpowiedzi_z_zadania();
    //cout << endl
         //<< "doszlo do return 0" << endl;
    return 0;
}
