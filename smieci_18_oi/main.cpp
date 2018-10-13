#include <iostream>
#include <fstream>
#include <vector>
#include <list>
using namespace std;

class Node;

class Krawedz
{
  public:
    //Node *wskaznik_sasiada;
    int index_sasiada;
    list<Krawedz>::iterator moja_pozycja;
    Krawedz(
        int index_sasiada)
    {
        //this->wskaznik_sasiada = wskaznik_sasiada;
        this->index_sasiada = index_sasiada;
    }
};

class Node
{
  public:
    list<Krawedz> sasiedzi;
    bool czy_obrabiany;
    Node()
    {
        czy_obrabiany = false;
    }
    void wypisz_sasiadow()
    {
        for (list<Krawedz>::iterator pom = sasiedzi.begin(); pom != sasiedzi.end(); pom++)
        {
            cout << pom->index_sasiada + 1 << " ";
        }
        cout << " sam siebie ";
        for (list<Krawedz>::iterator pom = sasiedzi.begin(); pom != sasiedzi.end(); pom++)
        {
            cout << pom->moja_pozycja->index_sasiada + 1 << " ";
        }
        cout << " obrabiany " << czy_obrabiany;
    }
};

class Graf
{
  public:
    vector<Node> nody;

    void dodaj(int a, int b)
    {
        nody[a].sasiedzi.push_front(Krawedz(b));
        nody[b].sasiedzi.push_front(Krawedz(a));

        nody[a].sasiedzi.begin()->moja_pozycja = nody[b].sasiedzi.begin();
        nody[b].sasiedzi.begin()->moja_pozycja = nody[a].sasiedzi.begin();
    }
    void idz_do_sasiada(int a, bool &czy_mamy_ugryzienie, vector<int> &obrabiane)
    {
        //jesli a ma gdzie isc to gdzies idzie
        //zakladam, ze nody[a].size()!=0
        int b = nody[a].sasiedzi.begin()->index_sasiada;
        obrabiane.push_back(b);
        list<Krawedz>::iterator pom = nody[a].sasiedzi.begin()->moja_pozycja;
        if (nody[b].czy_obrabiany)
        {
            czy_mamy_ugryzienie = true;
        }
        else
        {
            czy_mamy_ugryzienie = false;
            nody[b].czy_obrabiany = true;
        }
        nody[a].sasiedzi.erase(nody[a].sasiedzi.begin());
        nody[b].sasiedzi.erase(pom);
    }
    void wypisz()
    {
        cout << endl
             << endl
             << "wypisuje graf" << endl;
        for (int i = 0; i < nody.size(); i++)
        {
            cout << "node " << i + 1 << ":  ";
            nody[i].wypisz_sasiadow();
            cout << endl;
        }
        cout << endl;
    }
};

void wczytywanie(Graf &graf)
{
    //ifstream we("dane.txt");
    int n, m;
    cin >> n >> m;
    graf.nody.resize(n);
    int a, b, s, t;
    for (int i = 0; i < m; i++)
    {
        cin >> a >> b >> s >> t;
        a--;
        b--;
        if (s != t)
            graf.dodaj(a, b);
    }
}

bool czy_dobry(Graf &graf)
{
    for (int i = 0; i < graf.nody.size(); i++)
    {
        if (graf.nody[i].sasiedzi.size() % 2 == 1)
            return false;
    }
    return true;
}

void zdejmuj_z_obrabianych(vector<vector<int>> &petle, Graf &graf, vector<int> &obrabiane)
{
    //ostatni na obrabianych juz kiedys byl
    int dwa_razy_wystepujacy = obrabiane[obrabiane.size() - 1];
    petle.resize(petle.size() + 1);
    int index = petle.size() - 1;
    petle[index].push_back(dwa_razy_wystepujacy);
    int i = obrabiane.size() - 2;
    for (; i >= 0; i--)
    {
        if (obrabiane[i] != dwa_razy_wystepujacy)
        {
            petle[index].push_back(obrabiane[i]);
            graf.nody[obrabiane[i]].czy_obrabiany = false;
        }
        else
        {
            obrabiane.resize(i + 1);
            break;
        }
    }
}

void szukaj_petli(vector<vector<int>> &petle, Graf &graf)
{
    int n = graf.nody.size();
    for (int i = 0; i < n; i++)
    {
        vector<int> obrabiane;
        obrabiane.push_back(i);
        graf.nody[i].czy_obrabiany = true;
        bool czy_mamy_ugryzienie = false;
        //graf.idz_do_sasiada(i,czy_mamy_ugryzienie,obrabiane);
        //teraz mamy w obrabianych 2 elementy;
        while (graf.nody[obrabiane[obrabiane.size() - 1]].sasiedzi.size() > 0)
        {
            //to znaczy, ze mamy isc naprzod od chwilowego konca obrabianych;
            //int ostatni_w_obrabianych = obrabiane[obrabiane.size() - 1];
            czy_mamy_ugryzienie = false;

            while (!czy_mamy_ugryzienie)
            {
                graf.idz_do_sasiada(obrabiane[obrabiane.size() - 1], czy_mamy_ugryzienie, obrabiane);
            }

            //teraz mamy ugryzienie wiec mamy zdejmownaie
            zdejmuj_z_obrabianych(petle, graf, obrabiane);
        }
        graf.nody[i].czy_obrabiany = false;
    }
}

int main()
{
    Graf graf;
    wczytywanie(graf);
    if (czy_dobry(graf))
    {
        vector<vector<int>> petle;
        szukaj_petli(petle, graf);
        cout<<petle.size()<<endl;
        for (int i = 0; i < petle.size(); i++)
        {
            cout<<petle[i].size()<<" ";
            for (int j = 0; j < petle[i].size(); j++)
            {
                cout << petle[i][j] + 1<<" ";
            }
            cout<<petle[i][0]+1;
            if(i!=petle.size())
                cout<<endl;
        }
    }else{
        cout<<"NIE";
    }
    //cout << endl
         //<< "doszlo do return 0" << endl;
    return 0;
}
