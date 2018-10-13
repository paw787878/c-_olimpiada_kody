#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

class Node
{
  public:
    int glebokosc;
    int dfs_numer;
    int ograniczenie_synow_dfs_numberow;
    vector<Node *> synowie;
    Node *ojciec;
    vector<Node *> teleporty;
    //int moj_numer;
    Node()
    {
        ojciec = 0;
        dfs_numer = -1;
        ograniczenie_synow_dfs_numberow = -1;
        //moj_numer=-1;
        glebokosc = -1;
    }
};

class Miasta
{
  public:
    vector<Node *> tab_miast;
    Node *root;
    vector<int> droga;
    int log_n;
    Miasta();
    //void wypisz(Node* start);
    int odpowiedz();
};

bool a_jest_przodkiem_b(Node *a, Node *b)
{
    int num_b = b->dfs_numer;
    int apocz = a->dfs_numer;
    int akonc = a->ograniczenie_synow_dfs_numberow;
    // cout<<endl<<"num_b"<<num_b<<" a_pocz"<<apocz<<" b_pocz"<<akonc<<endl;
    return (num_b < akonc && num_b > apocz);
}

Node *lca(Node *a, Node *b, int log_n)
{
    if (a_jest_przodkiem_b(a, b))
        return a;
    if (a_jest_przodkiem_b(b, a))
    {
        return b;
    }
    Node *chwilowy = a;
    int do_przodu = log_n;
    //jest zachowywany taki niezmiennik, ze chwilowy jest a lub jego przodkiem,
    //ale nie jest w sumie przodkiem b, zas a przeteleportowany o do_przodu jest przodkiem b;
    while (do_przodu != 0)
    {
        do_przodu--;
        if (a_jest_przodkiem_b(chwilowy->teleporty[do_przodu], b))
        //nic w sumie nie trzeba robic
        {
        }
        else
        {
            chwilowy = chwilowy->teleporty[do_przodu];
        }
    }
    return chwilowy->teleporty[0];
}

int odleglosc(Node *a, Node *b, int log_n)
{
    return a->glebokosc + b->glebokosc - 2 * lca(a, b, log_n)->glebokosc;
}

int Miasta::odpowiedz()
{
    int suma = 0;
    for (int i = 0; i < droga.size() - 1; i++)
    {
        suma += odleglosc(tab_miast[droga[i]], tab_miast[droga[i + 1]], log_n);
    }
    return suma;
}

void dorob_wszystko(Node *start, int gl_start, int &gl_max,int & licznik)
{
    start->glebokosc = gl_start;
    if (gl_start > gl_max)
        gl_max = gl_start;
    start->dfs_numer = licznik;
    licznik++;
    for (int i = 0; i < start->synowie.size(); i++) if(start->ojciec!=start->synowie[i])
    {
        start->synowie[i]->ojciec=start;
        dorob_wszystko(start->synowie[i], gl_start + 1,gl_max,licznik);
    }
    start->ograniczenie_synow_dfs_numberow=licznik;
}
/*

void ukorzen(Node *start)
{
    for (int i = 0; i < start->synowie.size(); i++)
    {
        Node *syn = start->synowie[i];
        int j = 0;
        for (j = 0; j < syn->synowie.size(); j++)
        {
            if (syn->synowie[j] == start)
                break;
        }
        //teraz j to index ojca w tej tablicy
        syn->synowie.erase(syn->synowie.begin() + j);
        syn->ojciec = start;
        ukorzen(syn);
    }
}

void dfs_numerowanie(Node *start, int &licznik)
{
    int pocz_licznik = licznik;
    start->dfs_numer = licznik;
    licznik++;
    for (int i = 0; i < start->synowie.size(); i++)
        dfs_numerowanie(start->synowie[i], licznik);
    start->ograniczenie_synow_dfs_numberow = licznik;
}
*/

/*

void Miasta::wypisz(Node * start){
    cout<<endl<<start->moj_numer<<" synowie ";
    for(int i=0;i<start->synowie.size();i++)
        cout<<start->synowie[i]->moj_numer<<" ";
    cout<<" ojciec ";
    if(start->ojciec!=0)
        cout<<start->ojciec->moj_numer;
    cout<<" dfs_n "<<start->dfs_numer<<" ogr_synow "<<start->ograniczenie_synow_dfs_numberow;
    cout<<" teleporty ";
    for(int i=0;i<start->teleporty.size();i++){
        cout<<start->teleporty[i]->moj_numer<<" ";
    }
    cout<<" glebokosc "<<start->glebokosc;
    for(int i=0;i<start->synowie.size();i++){
        wypisz(start->synowie[i]);
    }
}
*/





void dorob_teleporty(vector<Node *> &tab_miast, int log_n)
{
    //log_n to maksymalny indeks jaki ma byc;
    for (int i = 0; i <= log_n; i++)
    {
        for (int j = 1; j < tab_miast.size(); j++)
        {
            Node *obrabiany = tab_miast[j];
            if (i == 0)
            {
                if (obrabiany->ojciec != 0)
                    obrabiany->teleporty.push_back(obrabiany->ojciec);
                else
                    obrabiany->teleporty.push_back(obrabiany);
            }
            else
            {
                obrabiany->teleporty.push_back(obrabiany->teleporty[i - 1]->teleporty[i - 1]);
            }
        }
    }
}

Miasta::Miasta()
{
    // ifstream we("dane.txt");
    int n;
    cin >> n;

    for (int i = 0; i <= n; i++)
    {
        tab_miast.push_back(new Node);
        // tab_miast[i]->moj_numer=i;
    }
    root = tab_miast[1];
    for (int i = 0; i < n - 1; i++)
    {
        int pocz;
        int konc;
        cin >> pocz;
        cin >> konc;
        tab_miast[pocz]->synowie.push_back(tab_miast[konc]);
        tab_miast[konc]->synowie.push_back(tab_miast[pocz]);
    }
    
    int gl_max = 0;
    //dorob_glebokosc(root, 0, gl_max);
    log_n = 0;
    int licznik=0;
    dorob_wszystko(root,0,gl_max,licznik);
    int potega = 1;
    while (potega < gl_max)
    {
        log_n++;
        potega *= 2;
    }


    //dfs_numerowanie(root, licznik);
    dorob_teleporty(tab_miast, log_n);

    int m;
    cin >> m;
    for (int i = 0; i < m; i++)
    {
        int pom;
        cin >> pom;
        droga.push_back(pom);
    }
}

int main()
{

    Miasta a;
    cout << a.odpowiedz();
    // cout<<endl<<"doszlo do return 0";
    return 0;
}
