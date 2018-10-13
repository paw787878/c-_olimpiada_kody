#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
using namespace std;

class Node
{
  public:
    //vector<vector<int>> odleglosci_minimalne; //pierwsze to reszta z dzielenia, drugie to ta minimalna odleglosc
    vector<int> numery_sasiadow;
    Node(int n)
    {
        //odleglosci_minimalne.resize(2);
    }
    /*
    void wypisz()
    {
        cout << endl
             << endl
             << "numery_sasiadow" << endl;
        for (int i = 0; i < numery_sasiadow.size(); i++)
            cout << numery_sasiadow[i] + 1 << " ";
        cout << endl;
        cout << "odleglosci minimalne do sasiadow" << endl;
        for (int i = 0; i < odleglosci_minimalne[0].size(); i++)
        {
            cout << "do " << i + 1 << " odleglosci " << odleglosci_minimalne[0][i] << " ";
            cout << odleglosci_minimalne[1][i] << endl;
        }
        cout << endl
             << endl;
    }
    */
};

class Wagonik
{
  public:
    int numer_portu;
    int odleglosc;
    Wagonik(int numer_portu, int odleglosc)
    {
        this->numer_portu = numer_portu;
        this->odleglosc = odleglosc;
    }
};

class Opowiesc
{
  public:
    int start;
    int koniec;
    int dlugosc;
};

class Graf
{
  public:
    vector<vector<int>> odleglosci_minimalne;
    int n;
    vector<Node> tab;
    /*
    void wypisz()
    {
        cout << endl
             << endl;
        for (int i = 0; i < tab.size(); i++)
        {
            cout << "info dla portu " << i + 1 << endl;
            tab[i].wypisz();
        }
    }
    */

    void initialize(int n)
    {
        this->n = n;
        odleglosci_minimalne.resize(2);
        for(int i=0;i<2;i++)
            odleglosci_minimalne[i].resize(n);
        for (int i = 0; i < n; i++)
        {
            tab.push_back(Node(n));
        }
    }
    void dodaj(int a, int b)
    {
        //one sa od 0
        tab[a].numery_sasiadow.push_back(b);
        tab[b].numery_sasiadow.push_back(a);
    }

    void uzupelnij_noda(int startowy, vector<vector<int>> &wazne_pytania, vector<bool> &odpowiedzi, vector<Opowiesc> &opowiesci)
    {
        for (int i = 0; i < 2; i++)
        {
            for (int j = 0; j < n; j++)
                odleglosci_minimalne[i][j] = -1;
        }
        queue<Wagonik> kolejka;
        //przy wkladaniu uzupelniam naszego noda, przy zdejmowaniu rekrutuje kolejnych tylko
        kolejka.push(Wagonik(startowy, 0));
        odleglosci_minimalne[0][startowy] = 0;
        while (!kolejka.empty())
        {
            Wagonik zdjety = kolejka.front();
            kolejka.pop();
            int chwil_polo = zdjety.numer_portu;
            int nowy_czas = zdjety.odleglosc + 1;
            for (int i = 0; i < tab[chwil_polo].numery_sasiadow.size(); i++)
            {
                int rozwazany_sasiad = tab[chwil_polo].numery_sasiadow[i];
                if (odleglosci_minimalne[nowy_czas % 2][rozwazany_sasiad] == -1)
                {
                    odleglosci_minimalne[nowy_czas % 2][rozwazany_sasiad] = nowy_czas;
                    kolejka.push(Wagonik(rozwazany_sasiad, nowy_czas));
                }
            }
        }

        for (int i = 0; i < wazne_pytania[startowy].size(); i++)
        {
            odpowiedzi[wazne_pytania[startowy][i]] = czy_sie_da(opowiesci[wazne_pytania[startowy][i]]);
        }


    }
    void uzupelnij_wszystkie(vector<vector<int>> &wazne_pytania, vector<bool> &odpowiedzi, vector<Opowiesc> &opowiesci)
    {
        for (int i = 0; i < tab.size(); i++)
        {
            uzupelnij_noda(i, wazne_pytania, odpowiedzi, opowiesci);
        }
    }

    bool czy_sie_da(Opowiesc &opowiesc)
    {
        if (opowiesc.koniec == opowiesc.start && tab[opowiesc.start].numery_sasiadow.size() == 0)
            return (opowiesc.dlugosc == 0);
        //teraz o 2 mozna wydluzac
        int minimalna = odleglosci_minimalne[opowiesc.dlugosc % 2][opowiesc.koniec];
        if (minimalna == -1)
            return false;
        return minimalna <= opowiesc.dlugosc;
    }
};

void wczytywanie(Graf &graf, int &n, int &m, int &k, vector<Opowiesc> &opowiesci)
{
    //ifstream we("dane.txt");
    cin >> n >> m >> k;
    graf.initialize(n);
    int a, b;
    for (int i = 0; i < m; i++)
    {
        cin >> a >> b;
        a--;
        b--;
        graf.dodaj(a, b);
    }
    for (int i = 0; i < k; i++)
    {
        Opowiesc opo;
        cin >> opo.start >> opo.koniec >> opo.dlugosc;
        opo.start--;
        opo.koniec--;
        opowiesci.push_back(opo);
    }
}

int main()
{
    Graf graf;
    vector<Opowiesc> opowiesci;
    int n, m, k;
    wczytywanie(graf, n, m, k, opowiesci);
    vector<vector<int>> pytania_po_startowym;
    pytania_po_startowym.resize(n);
    for (int i = 0; i < opowiesci.size(); i++)
    {
        pytania_po_startowym[opowiesci[i].start].push_back(i);
    }
    vector<bool> odpowiedzi;
    odpowiedzi.resize(opowiesci.size());

    graf.uzupelnij_wszystkie(pytania_po_startowym, odpowiedzi, opowiesci);
    //graf.wypisz();
    for (int i = 0; i < opowiesci.size(); i++)
    {
        if (odpowiedzi[i])
            cout << "TAK";
        else
            cout << "NIE";
        if (i != opowiesci.size() - 1)
            cout << endl;
    }

    //cout << endl
    //<< "doszlo do return 0" << endl;
    return 0;
}
