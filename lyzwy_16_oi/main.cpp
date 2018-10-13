#include <iostream>
#include <fstream>
#include <vector>
using namespace std;


class Node
{
  public:
    long long int od_lewej;
    long long int od_prawej;
    long long int max_suma;
    long long int suma;
    Node()
    {
        od_lewej = 0;
        od_prawej = 0;
        suma = 0;
        max_suma = 0;
    }
    void inicjoj_liczba(long long int a)
    {
        max_suma = max((long long int)0, a);
        od_lewej = max_suma;
        od_prawej = max_suma;
        suma = a;
    }
    void popraw(Node &lewy, Node &prawy)
    {
        suma = lewy.suma + prawy.suma;
        max_suma = max(max(lewy.max_suma, prawy.max_suma), lewy.od_prawej + prawy.od_lewej);
        od_lewej = max(lewy.od_lewej, lewy.suma + prawy.od_lewej);
        od_prawej = max(prawy.od_prawej, prawy.suma + lewy.od_prawej);
    }
};

class Dynamiczna_max_suma
{
  public:
    vector<Node> tab;
    int potega_2;
    Dynamiczna_max_suma(long long int k,int n)
    {
        int ile_danych = n;
        potega_2 = 1;
        while (potega_2 < ile_danych)
            potega_2 *= 2;
        tab.resize(2 * potega_2);
        for (int i = 0; i < ile_danych; i++)
        {
            tab[i + potega_2].inicjoj_liczba(-k);
        }

        for (int i = potega_2 - 1; i > 0; i--)
        {
            tab[i].popraw(tab[2 * i], tab[2 * i + 1]);
        }
    }
    void dodaj(long long int gdzie,long long int ile){
        //to gdzie to jest od 0
        gdzie+=potega_2;
        tab[gdzie].inicjoj_liczba(tab[gdzie].suma+(long long int) ile);
        gdzie/=2;
        while(gdzie!=0){
            tab[gdzie].popraw(tab[2*gdzie],tab[2*gdzie+1]);
            gdzie/=2;
        }
    }
    long long int maksymalne(){
        return tab[1].max_suma;
    }
};

void wczytywanie(vector<int> & gdzie,vector<int> & ile,int & n,
int & m, int & k, int & d){
    //ifstream we("dane.txt");
    cin>>n>>m>>k>>d;
    int r,x;
    for(int i=0;i<m;i++){
        cin>>r>>x;
        gdzie.push_back(r);
        ile.push_back(x);
    }
}

int main()
{
    vector<int> gdzie,ile;
    int n,m,k,d;
    wczytywanie(gdzie,ile,n,m,k,d);
    Dynamiczna_max_suma dynamiczna (k,n-d);
    long long int poprzeczka=(long long int) d*(long long int)k;
    for(int i=0;i<m;i++){
        dynamiczna.dodaj(gdzie[i]-1,ile[i]);
        if(dynamiczna.maksymalne()>poprzeczka)
            cout<<"NIE";
        else
            cout<<"TAK";
        if(i!=m-1)
            cout<<endl;
    }


    //cout << endl
         //<< "doszlo do return 0" << endl;
    return 0;
}
