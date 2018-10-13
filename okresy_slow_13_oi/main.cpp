#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

void zrob_pi(string & slowo, vector<int> & pi){
    //pi to bedzie takie cos, pi[i] to tak, ze max sufix ktory jest prefixem ktory nie jest calym,
    //dla i==0 to jest -1. 
    //pi na poczatku jest pusty.
    pi.push_back(-1);
    for(int i=1;i<slowo.size()+1;i++){
        int dl_wczesniejszego=pi[i-1];
        while(dl_wczesniejszego!=-1){
            if(slowo[i-1]==slowo[dl_wczesniejszego]){
                pi.push_back(dl_wczesniejszego+1);
                break;
            }
            dl_wczesniejszego=pi[dl_wczesniejszego];
        }
        if(dl_wczesniejszego==-1)
            pi.push_back(0);
    }

}

void zrob_alpha(vector<int> & pi,vector<int> & alpha){
    //alpha[i] to minimalny nie bedacy 0 sufix(prefixu slowa odlugosic i) ktory jest prefixem.
    //dla i==0 jest -13, bo do tego argumentu sie nie odwolujemy
    alpha.push_back(-13);
    for(int i=1;i<pi.size();i++){
        if(pi[i]==0)
            alpha.push_back(i);
        else
            alpha.push_back(alpha[pi[i]]);
    }

}
long long int ostateczny_wynik(vector<int> &alpha){
    long long int suma=0;
    for(int i=1;i<alpha.size();i++)
        if(alpha[i]<=i/2)
            suma+= i-alpha[i];
    return suma;
}

int main() 
{
    //ifstream we("dane.txt");
    int n;
    cin>>n;
    string slowo;
    cin>>slowo;
    vector<int> pi;
    vector<int> alpha;
    zrob_pi(slowo,pi);
    zrob_alpha(pi,alpha);
    cout<<ostateczny_wynik(alpha);
    //cout<<endl<<"doszlo do return 0"<<endl;
    return 0;

}
