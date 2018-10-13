#include <iostream>
#include <vector>
#include <fstream>
using namespace std;



int do_int(char a){
    return (int) a-48;
}

int pin_do_int(string a){
    int wyrzut=0;
    int potega=1;
    for(int i=3;i>=0;i--){
        wyrzut+=do_int(a[i])*potega;
        potega*=10;
    }   
    return wyrzut;
}

void dorzuc_literke(char litera,vector<vector<vector<int> > >& maszynka_pinow,vector<int> &piny){
    int index_literki=do_int(litera);
    for(int i=0;i<4;i++){
        for(int j=0;j<maszynka_pinow[i][index_literki].size();j++){
            int pin_dodawany=maszynka_pinow[i][index_literki][j]*10+index_literki;
            if(i<3){
                //to trzeba rozdystrybudować do nastepnych tych. 
                for(int k=0;k<10;k++){
                    maszynka_pinow[i+1][k].push_back(pin_dodawany);
                }
            }else{
                //to trzreba dodac do prawdziwych 4 cyfrowych pinow
                piny[pin_dodawany]++;
            }
        }
        maszynka_pinow[i][index_literki].resize(0);
    }
}

void dorzuc_slowa(string & slowo, vector<int> & piny){
    //tworze pomocnicze rzeczy. bedzie to wektor wektorow wektorow stringow.
    vector<vector<vector<int> > > maszynka_pinow;
    //pierwszy index od 0 do 3 to dlugosc tego czegos, drugi to jakia cyfra dojdzie w kolejnym kroku, reszta to slowa ktore tam sa. 
    for(int i=0;i<4;i++){
        vector<vector<int> > pom0;
        for(int j=0;j<10;j++){
            vector<int> pom1;
            pom0.push_back(pom1);
        }
        maszynka_pinow.push_back(pom0);
    }
    for(int i=0;i<10;i++)
        maszynka_pinow[0][i].push_back(0);
    //teraz mozna rozpoczac przedsiewziecie
    for(int i=0;i<slowo.size();i++){
        dorzuc_literke(slowo[i],maszynka_pinow,piny);
    }

}

int main() 
{   
    vector<int> piny;//trzyma w ilu slowach z listy sie pojawilo
    for(int i=0;i<10000;i++){
        piny.push_back(0);
    }
    
    //ifstream we("dane.txt");
    int n;
    cin>>n;
    for(int i=0;i<n;i++){
        string wczytywany;
        int pom;
        cin>>pom>>wczytywany;
        dorzuc_slowa(wczytywany,piny);
    }

    int ile_pinow=0;
    for(int i=0;i<10000;i++)
        if(piny[i]==n)
            ile_pinow++;
    cout<<ile_pinow;
    


    //cout<<endl<<"doszlo do return 0"<<endl;
    return 0;
}
