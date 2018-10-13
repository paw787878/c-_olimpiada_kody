#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
using namespace std;

class Node
{
  public:
    vector<int> synowie;
    int ile_potrzeba_pomocy;
    int ojciec;
    Node()
    {
        ojciec = -1;
        ile_potrzeba_pomocy = 0;
    }
};

void wczytywanie(vector<Node> &graf)
{
    //ifstream we("dane.txt");
    int n;
    cin >> n;
    graf.resize(n);
    int a, b;
    for (int i = 0; i < n - 1; i++)
    {
        cin >> a >> b;
        a--;
        b--;
        graf[a].synowie.push_back(b);
        graf[b].synowie.push_back(a);
    }
}

class Dfs_item
{
  public:
    int numer;
    bool czy_pierwszy_raz;
    Dfs_item(int numer)
    {
        this->numer = numer;
        czy_pierwszy_raz = true;
    }
};

void rob_postoder_i_znajdz_ojca(vector<Node> &graf, vector<int> &postoder)
{
    stack<Dfs_item> stos;
    stos.push(Dfs_item(0));
    while (!stos.empty())
    {

        int chwilowy_numer = stos.top().numer;
        if (stos.top().czy_pierwszy_raz)
        {
            stos.top().czy_pierwszy_raz = false;
            for (int i = 0; i < graf[chwilowy_numer].synowie.size(); i++)
            {
                int syn = graf[chwilowy_numer].synowie[i];
                if (graf[chwilowy_numer].ojciec != syn)
                {
                    stos.push(Dfs_item(syn));
                    graf[syn].ojciec = chwilowy_numer;
                }
            }
        }
        else
        {
            stos.pop();
            postoder.push_back(chwilowy_numer);
        }
    }
}

bool czy_da_sie_liczby_ekip(int liczba_ekip, vector<Node> &graf, vector<int> &postorder)
{
    for (int i = 0; i < postorder.size(); i++)
    {
        int rozwazany_numer = postorder[i];
        int liczba_synow = graf[rozwazany_numer].synowie.size();
        if (graf[rozwazany_numer].ojciec != -1)
            liczba_synow--;
        int suma_od_synow = 0;
        for (int j = 0; j < graf[rozwazany_numer].synowie.size(); j++)
        {
            int syn = graf[rozwazany_numer].synowie[j];
            if (syn != graf[rozwazany_numer].ojciec)
            {
                suma_od_synow += graf[syn].ile_potrzeba_pomocy;
            }
        }
        graf[rozwazany_numer].ile_potrzeba_pomocy = max(0, liczba_synow - liczba_ekip + suma_od_synow);
    }
    return (graf[0].ile_potrzeba_pomocy == 0);
}

int main()
{
    vector<Node> graf;
    wczytywanie(graf);
    vector<int> postorder;
    rob_postoder_i_znajdz_ojca(graf, postorder);
    int a,b;
    a=-1;//dla a sie nie da
    b=graf.size();//dla b sie da
    while(b-a != 1){
        int srodek= a+ (b-a)/2;
        if(czy_da_sie_liczby_ekip(srodek,graf,postorder)){
            b=srodek;
        }else{
            a=srodek;
        }
    }
    cout<<b;

    //cout << endl
         //<< "doszlo do return 0" << endl;
    return 0;
}
