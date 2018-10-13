#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

int suma_roczna(int a, int b)
{
    int c = a + b;
    int nies = 36500;
    if (c > nies)
        c = nies;
    return c;
}

class Node
{
  public:
    Node();
    //int numer;
    int ilosc_sposobow;
    vector<Node *> outgoing;
    vector<Node *> incoming;
    vector<Node *> &wych(bool czy_transpozycja);
    int informacja_o_stanie; //0 to nieruszony, 1 to w trakcie bycia obrabianym, 2 to zakonczone robienie
    int bliznowatosc;        // 0 to brak blizny, 1 to powstala nie rozprzestrzeniona, 2 to rozprzestrzeniona
};

Node::Node()
{
    //numer = nr;
    ilosc_sposobow = -1;
    informacja_o_stanie = 0;
    bliznowatosc = 0;
}

vector<Node *> &Node::wych(bool czy_transpozycja)
{
    if (czy_transpozycja)
        return incoming;
    else
        return outgoing;
}

void dfs_z_gmachu(Node *start, bool &czy_cykl)
{
    if (start->informacja_o_stanie == 2)
    {
        //tu nic sie nie robi w sumie
    }
    else if (start->informacja_o_stanie == 1)
    {
        czy_cykl = true;
        start->bliznowatosc = 1;
    }
    else
    {
        start->informacja_o_stanie = 1;
        for (int i = 0; i < start->wych(true).size(); i++)
        {
            dfs_z_gmachu(start->wych(true)[i], czy_cykl);
        }

        start->informacja_o_stanie = 2;
    }
}

void dfs_rozprzestrzeniajacy_bliznowatosc(Node *start)
{
    if (start->bliznowatosc != 2)
    {
        start->bliznowatosc = 2;
        for (int i = 0; i < start->wych(true).size(); i++)
        {
            dfs_rozprzestrzeniajacy_bliznowatosc(start->wych(true)[i]);
        }
    }
}

int na_ile_sposobow(Node *start, Node *gmach)
{
    if (start->informacja_o_stanie != 2)
        return 0;
    if (start == gmach)
        return 1;
    if (start->ilosc_sposobow != -1)
        return start->ilosc_sposobow;
    if (start->bliznowatosc == 2)
        return 36500;
    //to znaczy, ze trzeba dzialac
    int wyrzut = 0;
    for (int i = 0; i < start->wych(false).size(); i++)
    {
        wyrzut = suma_roczna(wyrzut, na_ile_sposobow(start->wych(false)[i], gmach));
    }
    start->ilosc_sposobow = wyrzut;
    return wyrzut;
}

int main()
{
    ifstream we("in/pro10.in");
    int n;
    we >> n;
    int m;
    we >> m;
    vector<Node *> tab_wierzcholkow;
    for (int i = 0; i < n + 1; i++)
    {
        Node *nowy = new Node;
        tab_wierzcholkow.push_back(nowy);
    }
    for (int i = 0; i < m; i++)
    {
        int a, b;
        we >> a >> b;
        tab_wierzcholkow[a - 1]->outgoing.push_back(tab_wierzcholkow[b - 1]);
        tab_wierzcholkow[b - 1]->incoming.push_back(tab_wierzcholkow[a - 1]);
    }
    //teraz mozna przejsc do wykonywania algorytmu;
    bool czy_cykl = false;
    dfs_z_gmachu(tab_wierzcholkow[n], czy_cykl);

    //jest cykl wiec trzeba tylko znalezc tych nieskonczonych;
    
    for (int i = 0; i < n; i++)
        if (tab_wierzcholkow[i]->bliznowatosc == 1)
            dfs_rozprzestrzeniajacy_bliznowatosc(tab_wierzcholkow[i]);
        vector<int> maksymalne;
        int wartosc_maksymalnego=-1;
        for(int i=0;i<n;i++){
            int pom=na_ile_sposobow(tab_wierzcholkow[i],tab_wierzcholkow[n]);
            if(pom>wartosc_maksymalnego){
                maksymalne.resize(0);
                wartosc_maksymalnego=pom;
                maksymalne.push_back(i+1);
            }else if(pom==wartosc_maksymalnego){
                maksymalne.push_back(i+1);
            }
        }
        if(wartosc_maksymalnego<36500)
        cout<<wartosc_maksymalnego<<endl;
        else
            cout<<"zawsze"<<endl;
        cout<<maksymalne.size()<<endl;
        for(int i=0;i<maksymalne.size();i++){
            cout<<maksymalne[i]<<" ";
        }
        cout<<endl<<endl<<maksymalne.size();
    /*
    {
        //tu trzeba dokonczyc
        vector<int> maksymalne;
        int wartosc_maksymalnego=-1;
        for(int i=0;i<n;i++){
            int pom=na_ile_sposobow(tab_wierzcholkow[i],tab_wierzcholkow[n]);
            if(pom>wartosc_maksymalnego){
                maksymalne.resize(0);
                wartosc_maksymalnego=pom;
                maksymalne.push_back(i+1);
            }else if(pom==wartosc_maksymalnego){
                maksymalne.push_back(i+1);
            }
        }
        if(wartosc_maksymalnego<36500)
        cout<<wartosc_maksymalnego<<endl;
        else
            cout<<"zawsze"<<endl;
        cout<<maksymalne.size()<<endl;
        for(int i=0;i<maksymalne.size();i++){
            cout<<maksymalne[i]<<" ";
        }
    }
    */
    //cout << endl
        // << "doszlo do return 0" << endl;
    return 0;
}
