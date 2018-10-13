#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

void wczytywanie(vector<int > & wysokosci,vector<int> & k_wektor){
    int n,m;
    //ifstream we("dane.txt");
    cin>>n>>m;
    int pom;
    for(int i=0;i<n;i++){
        cin>>pom;
        wysokosci.push_back(pom);
    }
    for(int i=0;i<m;i++){
        cin>>pom;
        k_wektor.push_back(pom);
    }
}

void domaxoj(int & a , int b){
    a=max(a,b);
}

int obrabiaj_jeden_przypadek(int k,vector<int> & wysokosci){
    //na poczatek robimy te nasze te
    vector<long long int> tab;
    long long int suma=0;
    tab.push_back(suma);
    for(int i=0;i<wysokosci.size();i++){
        suma+= (long long int)wysokosci[i]-k;
        tab.push_back(suma);
    }
    //teraz jest juz to nasze tab. teraz szukamy jego indexow
    vector<int> indexy_minimow,indexy_maksimow;    
    long long int minimum=tab[0];
    indexy_minimow.push_back(0);
    for(int i=1;i<tab.size();i++)
        if(tab[i]<minimum){
            indexy_minimow.push_back(i);
            minimum=tab[i];
        }
    long long int maksimum=tab[tab.size()-1];
    indexy_maksimow.push_back(tab.size()-1);
    for(int i=tab.size()-2;i>=0;i--)
        if(tab[i]>maksimum){
            indexy_maksimow.push_back(i);
            maksimum=tab[i];
        }
    //teraz trzeba indexy minimow leciec od poczatku a indexy maksimow od konca
    int odpowiedz=0;
    int rozwazany_minimow=0;
    int rozwazany_maksimow=indexy_maksimow.size()-1;

    while(rozwazany_minimow<indexy_minimow.size()){
        while(rozwazany_maksimow-1>=0 && tab[indexy_maksimow[rozwazany_maksimow-1]] >= tab[indexy_minimow[rozwazany_minimow]] )
            rozwazany_maksimow--;
        //teraz pozostaje tylko domaksowac
        domaxoj(odpowiedz,indexy_maksimow[rozwazany_maksimow]-indexy_minimow[rozwazany_minimow]);
        rozwazany_minimow++;
    }
    return odpowiedz;

}

int main() 
{
    vector<int> wysokosci,k_wektor;
    wczytywanie(wysokosci,k_wektor);
    for(int i=0;i<k_wektor.size();i++){
        cout<<obrabiaj_jeden_przypadek(k_wektor[i],wysokosci)<<" ";
    }
    //cout <<endl<< "doszlo do return 0"<<endl;
    return 0;
}
