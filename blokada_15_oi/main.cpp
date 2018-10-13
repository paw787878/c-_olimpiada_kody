#include <iostream>
#include <vector>
#include <fstream>
#include <stack>
using namespace std;

class Node
{
  public:
    int wielkosc_poddrzewa;
    int inorder;
    int min_backedge_inorder;
    int ostatni_robiony_graph_syn_index;
    vector<vector<Node>::iterator> graf_synowie;
    vector<vector<Node>::iterator> dfs_synowie;
    Node()
    {
        ostatni_robiony_graph_syn_index = -1;
        wielkosc_poddrzewa = 1;
        inorder = -1;
        min_backedge_inorder = 1000 * 1000 * 1000;
    }
    void wypisz()
    {
        cout << endl;
        cout << "wielkosc poddrzewa " << wielkosc_poddrzewa << endl;
        cout << "inorder " << inorder << endl;
        cout << "min_backage " << min_backedge_inorder << endl;
        cout << "ostatni_robiony graf index " << ostatni_robiony_graph_syn_index << endl;
        cout << "dfs synowie inordery" << endl;
        for (int i = 0; i < dfs_synowie.size(); i++)
        {
            cout << dfs_synowie[i]->inorder << " ";
        }
    }
};

void wczytywanie(vector<Node> &graf)
{
    int n, m;
    //ifstream we("dane.txt");
    cin >> n >> m;
    graf.resize(n);
    int a, b;
    for (int i = 0; i < m; i++)
    {
        cin >> a >> b;
        a -= 1;
        b -= 1;
        graf[a].graf_synowie.push_back(graf.begin() + b);
        graf[b].graf_synowie.push_back(graf.begin() + a);
    }
}

void dominuj(int &a, int b)
{
    a = min(a, b);
}

void dfs_iteratywny(vector<Node> &graf)
{
    //zaczynam w tym o indeksie 0
    stack<vector<Node>::iterator> stos;
    int chwilowy_inorder = 0;
    stos.push(graf.begin());

    while (!stos.empty())
    {
        vector<Node>::iterator chwilowy = stos.top();
        if (chwilowy->ostatni_robiony_graph_syn_index == -1)
        {
            //pierwszy kontakt. daj inorder numer
            chwilowy->inorder = chwilowy_inorder;
            chwilowy_inorder++;
        }
        else
        {
            //znow go widze, wiec robilem mu dziecko. wiec
            chwilowy->wielkosc_poddrzewa += chwilowy->graf_synowie[chwilowy->ostatni_robiony_graph_syn_index]->wielkosc_poddrzewa;
            dominuj(chwilowy->min_backedge_inorder, chwilowy->graf_synowie[chwilowy->ostatni_robiony_graph_syn_index]->min_backedge_inorder);
        }
        chwilowy->ostatni_robiony_graph_syn_index++;
        while (chwilowy->ostatni_robiony_graph_syn_index < chwilowy->graf_synowie.size() && chwilowy->graf_synowie[chwilowy->ostatni_robiony_graph_syn_index]->inorder != -1)
        {
            //wskazuje do backedga. mozna dominaowac
            dominuj(chwilowy->min_backedge_inorder, chwilowy->graf_synowie[chwilowy->ostatni_robiony_graph_syn_index]->inorder);
            chwilowy->ostatni_robiony_graph_syn_index++;
        }
        if (chwilowy->ostatni_robiony_graph_syn_index == chwilowy->graf_synowie.size())
        {
            //to znaczy, ze nie ma nowych sasiadow. czas go usunac
            stos.pop();
            continue;
        }
        else
        {
            //to znaczy, ze znalezlismy jego dfs dziecko
            chwilowy->dfs_synowie.push_back(chwilowy->graf_synowie[chwilowy->ostatni_robiony_graph_syn_index]);
            stos.push(chwilowy->graf_synowie[chwilowy->ostatni_robiony_graph_syn_index]);
        }
    }
}

void licz_ile_uniemozliwi(vector<long long int> &ile_uniemozliwi, vector<Node> &graf)
{
    long long int n = graf.size();
    ile_uniemozliwi.resize(n, n - 1); // na razie tyle co by ugoscil ten ziomek
    for (int i = 0; i < n; i++)
    {
        long long int suma = 1; // bo tu jest ten jeden ziomek
        for (int j = 0; j < graf[i].dfs_synowie.size(); j++)
            if (graf[i].dfs_synowie[j]->min_backedge_inorder >= graf[i].inorder)
            {
                suma += graf[i].dfs_synowie[j]->wielkosc_poddrzewa;
                ile_uniemozliwi[i] += (n - graf[i].dfs_synowie[j]->wielkosc_poddrzewa) * graf[i].dfs_synowie[j]->wielkosc_poddrzewa;
            }
        ile_uniemozliwi[i] += (n - suma) * suma;
    }
}

int main()
{
    vector<Node> graf;
    wczytywanie(graf);
    dfs_iteratywny(graf);
    vector<long long int> ile_uniemozliwi;
    licz_ile_uniemozliwi(ile_uniemozliwi, graf);
    /*
    for(int i=0;i<graf.size();i++){
        cout<<endl<<endl<<"info dla wierzcholka nr "<<i+1;
        graf[i].wypisz();
    }
    */
    //wypisywanie
    for (int i = 0; i < graf.size(); i++)
    {
        cout << ile_uniemozliwi[i];
        if (i < graf.size() - 1)
            cout << endl;
    }

    //cout << endl
    //<< "doszlo do return 0" << endl;
    return 0;
}
