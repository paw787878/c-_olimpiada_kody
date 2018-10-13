#include <iostream>
#include <fstream>
#include <list>
#include <queue>
using namespace std;

void wczytywanie(vector<vector<int>> &graf)
{
    //ifstream we("dane.txt");
    int n, a, b;
    cin >> n;
    graf.resize(n);
    for (int i = 0; i < n - 1; i++)
    {
        cin >> a >> b;
        a--;
        b--;
        graf[a].push_back(b);
        graf[b].push_back(a);
    }
}

long long int nie_drzewiasta_wersja(vector<int> &liczebnosci)
{
    long long int ile_1 = 0;
    long long int ile_2 = 0;
    long long int ile_3 = 0;
    for (int i = 0; i < liczebnosci.size(); i++)
    {
        ile_3 += (long long int)liczebnosci[i] * ile_2;
        ile_2 += (long long int)liczebnosci[i] * ile_1;
        ile_1 += (long long int)liczebnosci[i];
    }
    return ile_3;
}

class Kolejko_item
{
  public:
    int ja;
    int ojciec;
    Kolejko_item(int ja, int ojciec)
    {
        this->ja = ja;
        this->ojciec = ojciec;
    }
};

void rob_krok_bfs(vector<vector<int> > & graf, queue<Kolejko_item> & kolejka){
    kolejka.push(Kolejko_item(-1,-1));
    while(kolejka.front().ja!=-1){
        Kolejko_item zdjety=kolejka.front();
        kolejka.pop();
        int ja=zdjety.ja;
        int ojciec=zdjety.ojciec;
        for(int i=0;i<graf[ja].size();i++)
            if(graf[ja][i]!=ojciec){
                kolejka.push(Kolejko_item(graf[ja][i],ja));
            }
    }
    kolejka.pop();//usuwam tego fummy itema
}

void dolicz_przyczynek_dla_jednego(int start, long long int &liczba_wszystkich, vector<vector<int>> &graf)
{
    list<queue<Kolejko_item>> lista_bfsow;
    //trzeba ja jakos zainicjowac
    for (int i = 0; i < graf[start].size(); i++)
    {
        queue<Kolejko_item> pom_kol;
        pom_kol.push(Kolejko_item(graf[start][i], start));
        //pom_kol.push(Kolejko_item(-1, -1));
        lista_bfsow.push_back(pom_kol);
    }
    while (lista_bfsow.size() >= 3){
        //na poczatek dolicz te przyczynki
        vector<int> liczebnosci;
        for(list<queue<Kolejko_item>>::iterator listo_iterator=lista_bfsow.begin();listo_iterator!=lista_bfsow.end();){
            liczebnosci.push_back(listo_iterator->size());
            rob_krok_bfs(graf,(*listo_iterator));
            if(listo_iterator->size()==0){
                list<queue<Kolejko_item>>::iterator pom=listo_iterator;
                ++listo_iterator;
                lista_bfsow.erase(pom);
            }else{
                ++listo_iterator;
            }
        }
        liczba_wszystkich+= nie_drzewiasta_wersja(liczebnosci);
    }
}

long long int odpowiedz(vector<vector<int> > & graf){
    long long int liczba_wszystkich=0;
    for(int i=0;i<graf.size();i++){
        dolicz_przyczynek_dla_jednego(i,liczba_wszystkich,graf);
    }
    return liczba_wszystkich;
}

int main()
{
    vector<vector<int>> graf;
    wczytywanie(graf);
    cout<<odpowiedz(graf);
    //cout << endl
         //<< "doszlo do return 0" << endl;
    return 0;
}
