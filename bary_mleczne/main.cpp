#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
using namespace std;

class Node
{
  public:
    int ojciec;
    vector<int> synowie;
    Node()
    {
        ojciec = -1;
    }
};

void usun_z_wektora(vector<int> &tab, int &index)
{
    if (index == tab.size() - 1)
        tab.resize(tab.size() - 1);
    else
    {
        tab[index] = tab[tab.size() - 1];
        tab.resize(tab.size() - 1);
    }
    index--;
}

class Graf
{
  public:
    int n;
    vector<Node> nody;
    Graf() {}
    void inicjoj(int n)
    {
        this->n = n;
        nody.resize(n);
    }
    void dodaj(int a, int b)
    {
        nody[a].synowie.push_back(b);
        nody[b].synowie.push_back(a);
    }
    void rob_dfs_z_konca()
    {
        int startowy = n - 1;
        stack<int> stos;
        stos.push(startowy);
        while (!stos.empty())
        {
            int zdjety = stos.top();
            stos.pop();
            for (int i = 0; i < nody[zdjety].synowie.size(); i++)
            {
                int sasiad = nody[zdjety].synowie[i];
                if (sasiad != nody[zdjety].ojciec)
                {
                    nody[sasiad].ojciec = zdjety;
                    stos.push(sasiad);
                }
                else
                {
                    usun_z_wektora(nody[zdjety].synowie, i);
                }
            }
        }
    }
    bool droga_od_korzenia_do_syna(int start, vector<int> &droga)
    {
        //daje true jesli znajdzie taka droge
        //bedzie rekurencja a co mi tam
        if (nody[start].synowie.size() == 0)
        {
            cout << endl
                 << "poszlo nie tak" << endl;
            return false;
        }
        droga.push_back(start);
        int ktory_syn_nietrywialny = -1;
        for (int i = 0; i < nody[start].synowie.size(); i++)
        {
            int syn = nody[start].synowie[i];
            if (nody[syn].synowie.size() != 0)
            {
                if (ktory_syn_nietrywialny == -1)
                {
                    ktory_syn_nietrywialny = syn;
                    vector<int> od_syna;
                    if (droga_od_korzenia_do_syna(syn, od_syna))
                        for (int j = od_syna.size() - 1; j >= 0; j--)
                        {
                            droga.push_back(od_syna[j]);
                        }
                    else
                        return false;
                }
                else
                {
                    return false;
                }
            }
        }
        //no to teraz trzeba pozbierac te wszystkie trywialniaczki
        for (int i = 0; i < nody[start].synowie.size(); i++)
        {
            int syn = nody[start].synowie[i];
            if (syn != ktory_syn_nietrywialny)
            {
                droga.push_back(syn);
            }
        }
        return true;
    }
    bool krok_do_przodu(int chwilowy_krag,vector<int> & droga){
        //chwilowy_krag nie jest jeszcze na drodze
        //zakladam, ze chwilowy_krag nie jest ostanim
        if(nody[chwilowy_krag].synowie.size() > 0){
            int nowy_chwilowy_krag=nody[chwilowy_krag].ojciec;
            for(int i=0;i<nody[nowy_chwilowy_krag].synowie.size();i++){
                int rozwazany_sasiad=nody[nowy_chwilowy_krag].synowie[i];
                if(rozwazany_sasiad==chwilowy_krag)
                    usun_z_wektora(nody[nowy_chwilowy_krag].synowie,i);
            }
            return droga_od_korzenia_do_syna(chwilowy_krag,droga);
        }else{
            droga.push_back(chwilowy_krag);
            int nowy_chwilowy_krag=nody[chwilowy_krag].ojciec;
            //teraz trzeba zjesc wszystkie trywialne
            //vector<int> usuniete_indexy;
            for(int i=0;i<nody[nowy_chwilowy_krag].synowie.size();i++){
                int rozwazany_sasiad=nody[nowy_chwilowy_krag].synowie[i];
                if(rozwazany_sasiad==chwilowy_krag){
                    //usuniete_indexy.push_back(i);
                    usun_z_wektora(nody[nowy_chwilowy_krag].synowie,i);
                }
                else if(nody[rozwazany_sasiad].synowie.size()==0){
                    //usuniete_indexy.push_back(i);
                    usun_z_wektora(nody[nowy_chwilowy_krag].synowie,i);
                    droga.push_back(rozwazany_sasiad);
                }
            }
            //teraz szukasz jakiegos wiekszego celu
            bool czy_sie_powiodlo=true; 
            for(int i=0;i<nody[nowy_chwilowy_krag].synowie.size();i++){
                int rozwazany_sasiad=nody[nowy_chwilowy_krag].synowie[i];
                if(nody[rozwazany_sasiad].synowie.size()>0){
                    czy_sie_powiodlo=droga_od_korzenia_do_syna(rozwazany_sasiad,droga);
                    //usuniete_indexy.push_back(i);
                    usun_z_wektora(nody[nowy_chwilowy_krag].synowie,i);
                    break;
                }
            }
            return czy_sie_powiodlo;
        }
    }
    bool szukaj_tej_drogi(vector<int> & droga){
        int chwilowy_krag=0;
        while(chwilowy_krag != n-1){
            bool pom=krok_do_przodu(chwilowy_krag,droga);
            if(! pom)
                return pom;
            chwilowy_krag=nody[chwilowy_krag].ojciec;
        }
        //teraz ten chwilowy krag jest ostatni.
        droga.push_back(n-1);
        if(nody[n-1].synowie.size()==0)
            return true;
        else
            return false;
    }
};

void wczytaj(Graf &graf)
{
    //ifstream we("dane1.txt");
    int n;
    cin >> n;
    graf.inicjoj(n);
    int a, b;
    for (int i = 0; i < n - 1; i++)
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
    graf.rob_dfs_z_konca();
    //teraz juz jest dobrze ojciec oraz synowie to nie ojciec

    vector<int> proba;
    if (graf.szukaj_tej_drogi(proba))
        for (int i = 0; i < proba.size(); i++)
        {
            cout << proba[i] + 1;
            if(i!=proba.size()-1)
                cout<<endl;
        }
    else
    {
        cout << "BRAK" << endl;
    }
    //cout << endl
         //<< "doszlo do return 0" << endl;
    return 0;
}
