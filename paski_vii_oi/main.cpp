#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

vector<pair<int,int> > pary_do_ktorych_mozna_dojsc(vector<int> dlugosci_3_paskow,int startowa_pozycja){
    vector<pair<int,int> > wyrzut;
    if(startowa_pozycja==0)
        return wyrzut;//nie mozna wykonac ruchu
    for(int i=0;i<3;i++){
        int kolorowy_pasek=dlugosci_3_paskow[i];
        if(startowa_pozycja>=kolorowy_pasek){
            for(int j=0;j<=(startowa_pozycja-kolorowy_pasek);j++){
                wyrzut.push_back(pair<int,int>(j,startowa_pozycja-kolorowy_pasek-j));
            }
        }
    }
    return wyrzut;
}

void wyfalsuj(vector<bool> & tab){
    for(int i=0;i<tab.size();i++){
        tab[i]=false;
    }
}

int main() 
{
    //ifstream we("dane.txt");
    vector<int> dlugosci_3_paskow;
    for(int i=0;i<3;i++){
        int pom;
        cin>>pom;
        dlugosci_3_paskow.push_back(pom);
    }
    int n_pytan;
    cin>>n_pytan;
    vector<int> pytania;
    int max_pytanie=0;
    for(int i=0;i<n_pytan;i++){
        int pom;
        cin>>pom;
        if(pom>max_pytanie)
            max_pytanie=pom;
        pytania.push_back(pom);
    }
    //cout<<"pytania size"<<pytania.size();

    //teraz trzeba zrobic nasz playing ground;

    vector<int> policzone_klasy;// ita pozycja to klasa planszy zlozonej z jednego obszaru wielkosci i
    vector<bool> czy_trafiony;// taki brudnopis.
    for(int i=0;i<5;i++){
        czy_trafiony.push_back(false);
    }
    int rozmiar=max_pytanie+1;
    
    for(int i=0;i<rozmiar;i++){
        //dorabiamy info dla tego ziomeczka x_i to znaczy pusty obszar o dlugosci i
        vector<pair<int,int> > pary=pary_do_ktorych_mozna_dojsc(dlugosci_3_paskow,i);
        wyfalsuj(czy_trafiony);
        for(int j=0;j<pary.size();j++){
            pair<int,int> p=pary[j];
            int klasa_dziecka=(policzone_klasy[p.first])^(policzone_klasy[p.second]);
            czy_trafiony[klasa_dziecka]=true;
        }
        int j=0;
        for(;j<czy_trafiony.size()&&czy_trafiony[j];j++);
        
        //teraz ten j powinien byc numerem klasy tego naszego wchodzacego
        policzone_klasy.push_back(j);
        for(int k=0;k<4;k++){
            czy_trafiony.push_back(false);//dorabiam ramu.
        }
    }
    //cout<<endl<<"teraz wypisze klasy"<<endl;

    //cout<<endl<<"teraz odpowiedzi"<<endl;
    //cout<<"pytania size"<<pytania.size();
    for(int i=0;i<pytania.size();i++){
        int a=policzone_klasy[pytania[i]];
        if(a==0){
            cout<<2;
        }else cout<<1;
        if(i!=pytania.size()-1)
            cout<<endl;
    }
    




    //cout<<endl<<"doszlo do return 0"<<endl;
    return 0;
}
