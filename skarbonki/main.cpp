#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

void dfs_jednej_skladowej(vector<vector<int> > & graf,vector<bool> & odwiedzone,int start){
    odwiedzone[start]=true;
    for(int i=0;i<graf[start].size();i++)
        if(odwiedzone[graf[start][i]]==false)
            dfs_jednej_skladowej(graf,odwiedzone,graf[start][i]);
    
}

int main() 
{
    //ifstream we("dane.txt");
    int n;
    cin>>n;
    vector<vector<int> > graf;
    vector<bool> odwiedzone;
    for(int i=0;i<n;i++){
        vector<int> pom;
        graf.push_back(pom);
        odwiedzone.push_back(false);
    }

    for(int i=0;i<n;i++){
        int a;
        cin>>a;
        a--;
        graf[a].push_back(i);
        graf[i].push_back(a);
    }
    //teraz mam juz graf.
    int wynik=0;
    for(int i=0;i<n;i++)
        if(odwiedzone[i]==false){
            wynik++;
            dfs_jednej_skladowej(graf,odwiedzone,i);
        }
    
    cout<<wynik;

    //cout<<endl<<"doszlo do return 0"<<endl;
    return 0;
}
