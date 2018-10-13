#include <iostream>
#include <vector>
#include <set>
#include <fstream>
#include <algorithm>
#include <list>
#include <stack>
using namespace std;

void wczytywanie(vector<pair<int, int>> &autostrady, int &n, int &m)
{
    //ifstream we("dane.txt");
    cin >> n >> m;
    for (int i = 0; i < m; i++)
    {
        int a, b;
        cin >> a >> b;
        autostrady.push_back(pair<int, int>(a, b));
    }
}

class Koniec
{
  public:
    int ten;
    int drugi;
    int nr_autostrady;
    Koniec(int nr_autostrady, vector<pair<int, int>> &autostrady, bool pocz_czy)
    {
        this->nr_autostrady = nr_autostrady;
        if (pocz_czy)
        {
            ten = autostrady[nr_autostrady].first;
            drugi = autostrady[nr_autostrady].second;
        }
        else
        {
            drugi = autostrady[nr_autostrady].first;
            ten = autostrady[nr_autostrady].second;
        }
    }
    bool czy_pocz() const
    {
        return drugi > ten;
    }
    bool czy_konc() const
    {
        return ten > drugi;
    }

    bool operator<(const Koniec &rhs) const
    {
        if (this->ten != rhs.ten)
            return this->ten < rhs.ten;
        bool lhs_pocz = this->czy_pocz();
        bool rhs_pocz = rhs.czy_pocz();
        if (lhs_pocz != rhs_pocz)
        {
            if (lhs_pocz == false)
                return true;
            else
                return false;
        }
        else
        {
            return this->drugi > rhs.drugi;
        }
    }

    bool operator>(const Koniec &rhs) const
    {
        return !(operator<(rhs));
    }

    void wypisz() const
    {
        cout << "ten " << ten << " drugi " << drugi << endl;
    }
};

void zlepiaj(list<set<Koniec>> &klasy, vector<list<set<Koniec>>::iterator> &rzeczy_do_pozlepiania, vector<list<set<Koniec>>::iterator> &klasa_autostrady)
{

    //cout << "jest do pozlepiania " << rzeczy_do_pozlepiania.size() << endl;
    while (rzeczy_do_pozlepiania.size() > 1)
    {
        int pomn = rzeczy_do_pozlepiania.size();
        list<set<Koniec>>::iterator a = rzeczy_do_pozlepiania[pomn - 1];
        list<set<Koniec>>::iterator b = rzeczy_do_pozlepiania[pomn - 2];
        // wypisz_set(*a);
        //wypisz_set(*b);
        rzeczy_do_pozlepiania.resize(pomn - 2);
        if ((*a).size() < (*b).size())
        {
            list<set<Koniec>>::iterator pom = a;
            a = b;
            b = pom;
        }
        //teraz a wskazuje na duzy, b na maly
        //cout << "przed for" << endl;
        for (set<Koniec>::iterator i = b->begin(); i != b->end(); ++i)
        {
            //kurcze
            a->insert((*i));
            klasa_autostrady[(*i).nr_autostrady] = a;
        }
        //cout << "po for" << endl;
        //cout << (*(b->begin())).ten << endl;
        //cout<<(b==klasy.end())<<endl;
        klasy.erase(b);
        //cout << "po erase" << endl;
        rzeczy_do_pozlepiania.push_back(a);
    }
    //cout << "koniec zlepiaj" << endl;
}

void wypisz_set(set<Koniec> s)
{
    cout << endl
         << "wypisuje set" << endl;
    for (set<Koniec>::iterator i = s.begin(); i != s.end(); ++i)
    {
        (*i).wypisz();
    }
    cout << endl
         << endl;
}

void rozpinajacy_las(vector<vector<int>> &graf, vector<pair<int, int>> &autostrady, vector<Koniec> &konce)
{
    //na poczatek potrzebna nam bedzie lista

    list<set<Koniec>> klasy;
    graf.resize(autostrady.size());
    vector<list<set<Koniec>>::iterator> klasa_autostrady;
    klasa_autostrady.resize(autostrady.size(), klasy.end());
    set<Koniec> najmniejsze_z_klas;
    for (int i = 0; i < konce.size(); i++)
    {
        Koniec rozwazany = konce[i];
        if (rozwazany.czy_konc())
        {
            //wiec on jest teraz koncowy. trzeba go pousuwac
            (*klasa_autostrady[rozwazany.nr_autostrady]).erase((*klasa_autostrady[rozwazany.nr_autostrady]).find(rozwazany));
            //teraz byc moze trzeba usuwnac z tego duzego
            if (najmniejsze_z_klas.find(rozwazany) != najmniejsze_z_klas.end())
            {
                //usuwamy z niego
                najmniejsze_z_klas.erase(najmniejsze_z_klas.find(rozwazany));
                if ((*klasa_autostrady[rozwazany.nr_autostrady]).size() > 0)
                    najmniejsze_z_klas.insert(*(*klasa_autostrady[rozwazany.nr_autostrady]).begin());
            }
        }
        else
        {
            //czyli poczatkowy. jest trudniej
            //teraz wkladam do listy nowy ten
            Koniec odpowiadajacy_koncowy(rozwazany.nr_autostrady, autostrady, false);
            set<Koniec> wkladany_set;
            wkladany_set.insert(odpowiadajacy_koncowy);
            klasy.push_front(wkladany_set);

            klasa_autostrady[rozwazany.nr_autostrady] = klasy.begin();

            //teraz trzeba zrobic porzadek z tym wszystkim
            vector<list<set<Koniec>>::iterator> rzeczy_do_pozlepiania;
            rzeczy_do_pozlepiania.push_back(klasy.begin());

            //wypisz_set(najmniejsze_z_klas);
            while (najmniejsze_z_klas.size() != 0 && (*najmniejsze_z_klas.begin()).ten < rozwazany.drugi && (*najmniejsze_z_klas.begin()).drugi < rozwazany.ten)
            {
                Koniec drugi_koniec_krawedzi = *(najmniejsze_z_klas.begin());
                najmniejsze_z_klas.erase(najmniejsze_z_klas.begin());

                rzeczy_do_pozlepiania.push_back(klasa_autostrady[drugi_koniec_krawedzi.nr_autostrady]);
                graf[rozwazany.nr_autostrady].push_back(drugi_koniec_krawedzi.nr_autostrady);
                graf[drugi_koniec_krawedzi.nr_autostrady].push_back(rozwazany.nr_autostrady);
            }

            //teraz trzeba nam pozlepiac, to bedzie jakas petla

            zlepiaj(klasy, rzeczy_do_pozlepiania, klasa_autostrady);

            //nie chcem ale muszem

            //nie chcem ale muszem

            //teraz potrzeba nam koniecznie wrzucic do tego najmniejsze z najmniejszych

            //wypisz_set(najmniejsze_z_klas);
            najmniejsze_z_klas.insert((*rzeczy_do_pozlepiania[0]->begin()));
        }
    }
}

void wypisz_pare(pair<int, int> a)
{
    cout << "( " << a.first << " " << a.second << " )";
}

void rob_konce_autostrad(vector<pair<int, int>> &autostrady, vector<Koniec> &konce)
{
    for (int i = 0; i < autostrady.size(); i++)
    {
        konce.push_back(Koniec(i, autostrady, true));
        konce.push_back(Koniec(i, autostrady, false));
    }
    sort(konce.begin(), konce.end());
}

void dfs_koloroj(vector<int> &kolory, int start, vector<vector<int>> &graf, int kolor_ojca)
{
    if (kolory[start] == 0)
    {
        if (kolor_ojca == 1)
            kolory[start] = 2;
        else
            kolory[start] = 1;
        for (int i = 0; i < graf[start].size(); i++)
            dfs_koloroj(kolory, graf[start][i], graf, kolory[start]);
    }
}

void dfs_petla(vector<int> &kolory, int start, vector<vector<int>> &graf)
{
    for (int i = 0; i < graf.size(); i++)
    {
        dfs_koloroj(kolory, i, graf, 1);
    }
}

void rob_gorne_i_dolne(vector<pair<int, int>> &autostrady, vector<int> &kolory, vector<Koniec> &gorne_konce, vector<Koniec> &dolne_konce)
{
    vector<pair<int, int>> jedynkowe_pary;
    vector<pair<int, int>> dwojkowe_pary;
    for (int i = 0; i < kolory.size(); i++)
    {
        if (kolory[i] == 1)
            jedynkowe_pary.push_back(autostrady[i]);
        else
            dwojkowe_pary.push_back(autostrady[i]);
    }
    rob_konce_autostrad(jedynkowe_pary, gorne_konce);
    rob_konce_autostrad(dwojkowe_pary, dolne_konce);
}

bool czy_jest_planarnie(vector<Koniec> &konce)
{
    stack<int> stos_numerow_autostrad;
    for (int i = 0; i < konce.size(); i++)
    {
        Koniec rozwazany = konce[i];
        if (rozwazany.czy_pocz())
            stos_numerow_autostrad.push(rozwazany.nr_autostrady);
        else
        {
            if (rozwazany.nr_autostrady != stos_numerow_autostrad.top())
                return false;
            else
            {
                stos_numerow_autostrad.pop();
            }
        }
    }
    return true;
}

int main()
{
    vector<pair<int, int>> autostrady;
    int n, m;
    wczytywanie(autostrady, n, m);
    vector<Koniec> konce;
    rob_konce_autostrad(autostrady, konce);
    /*
    for (int i = 0; i < konce.size(); i++)
    {
        cout << "i= " << i << " ";
        konce[i].wypisz();
    }
    */

    vector<vector<int>> graf;
    rozpinajacy_las(graf, autostrady, konce);
    /*
    for (int i = 0; i < graf.size(); i++)
    {
        cout << endl;
        wypisz_pare(autostrady[i]);
        cout << "--";
        for (int j = 0; j < graf[i].size(); j++)
        {
            wypisz_pare(autostrady[graf[i][j]]);
        }
    }
    */

    vector<int> kolory;
    kolory.resize(graf.size(), 0);
    dfs_petla(kolory, 0, graf);

    vector<Koniec> gorne_konce;
    vector<Koniec> dolne_konce;

    rob_gorne_i_dolne(autostrady, kolory, gorne_konce, dolne_konce);

    bool czy_sie_da=czy_jest_planarnie(gorne_konce)&&czy_jest_planarnie(dolne_konce);
    if(!czy_sie_da)
        cout<<"NIE";
    else{
        for(int i=0;i<kolory.size();i++){
            if(kolory[i]==1)
                cout<<"N"<<endl;
            else
                cout<<"S"<<endl;
        }
    }
    return 0;
}
