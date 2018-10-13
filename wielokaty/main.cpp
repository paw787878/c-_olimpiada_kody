#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

template <class T>
void merge_sort(vector<T> &tab, bool (*mniejszy)(T, T), int a, int b)
{
    if (a != b && (a + 1) != b)
    {
        int srodek = (b - a) / 2 + a;
        merge_sort(tab, mniejszy, a, srodek);
        merge_sort(tab, mniejszy, srodek, b);
        vector<T> v1;
        vector<T> v2;
        for (int i = a; i < srodek; i++)
            v1.push_back(tab[i]);
        for (int i = srodek; i < b; i++)
            v2.push_back(tab[i]);
        int i1 = 0;
        int i2 = 0;
        for (int i = a; i < b; i++)
        {
            if (i1 == v1.size())
            {
                tab[i] = v2[i2];
                i2++;
            }
            else if (i2 == v2.size())
            {
                tab[i] = v1[i1];
                i1++;
            }
            else if (mniejszy(v1[i1], v2[i2]))
            {
                tab[i] = v1[i1];
                i1++;
            }
            else
            {
                tab[i] = v2[i2];
                i2++;
            }
        }
    }
}

void para_ma_byc_rosnaca(pair<int, int> &para)
{
    if (para.first >= para.second)
    {
        int pom = para.first;
        para.first = para.second;
        para.second = pom;
    }
}

string to_string(pair<int, int> &para)
{
    return " " + to_string(para.first) + ", " + to_string(para.second) + " ";
}

bool mniejsze_para(pair<int, int> a, pair<int, int> b)
{
    if (a.first < b.first)
        return true;
    if (b.first < a.first)
        return false;
    if (a.second < b.second)
        return true;
    return false;
}

int bin_search(vector<pair<int, int>> &wek, pair<int, int> cel)
{
    int a = 0;
    int b = wek.size();
    while (a != b && (a + 1) != b)
    {
        int srodek = (b - a) / 2 + a;
        if (mniejsze_para(cel, wek[srodek]))
            b = srodek;
        else
            a = srodek;
    }
    return a;
}

vector<pair<int, int>> kraw_trojkata(vector<int> &wek)
{
    vector<pair<int, int>> wyrzut;
    for (int j = 0; j < 3; j++)
    {
        vector<int> dwa;
        for (int k = 0; k < 3; k++)
        {
            if (j != k)
            {
                dwa.push_back(wek[k]);
            }
        }
        pair<int, int> para_pom(dwa[0], dwa[1]);
        para_ma_byc_rosnaca(para_pom);
        wyrzut.push_back(para_pom);
    }
    return wyrzut;
}

void wczytywanie(vector<vector<int>> &graf)
{
    //ifstream we("dane.txt");
    int n;
    cin >> n;
    vector<vector<int>> trojkoty;
    for (int i = 0; i < n - 2; i++)
    {
        vector<int> pom;
        for (int j = 0; j < 3; j++)
        {
            int pomint;
            cin >> pomint;
            pom.push_back(pomint);
        }
        trojkoty.push_back(pom);
    }
    vector<pair<int, int>> krawedzie;
    for (int i = 0; i < trojkoty.size(); i++)
    {
        for (int j = 0; j < 3; j++)
        {
            vector<int> dwa;
            for (int k = 0; k < 3; k++)
            {
                if (j != k)
                {
                    dwa.push_back(trojkoty[i][k]);
                }
            }
            pair<int, int> para_pom(dwa[0], dwa[1]);
            para_ma_byc_rosnaca(para_pom);
            krawedzie.push_back(para_pom);
        }
    }
    //krawedzie teraz to jest
    merge_sort(krawedzie, &mniejsze_para, 0, krawedzie.size());
    /*
    for(int i=0;i<krawedzie.size();i++){
        cout<<endl<<to_string(krawedzie[i]);
    }
    */
    //usun duplikaty
    vector<pair<int, int>> bez_duplikatow_kra;
    bez_duplikatow_kra.push_back(krawedzie[0]);
    for (int i = 1; i < krawedzie.size(); i++)
    {
        if (krawedzie[i] != krawedzie[i - 1])
            bez_duplikatow_kra.push_back(krawedzie[i]);
    }
    //teraz bez_duplikatow_kra zawiera krawedzie. mozna

    vector<vector<int>> z_tym_trojkatem_graniczy;
    for (int i = 0; i < bez_duplikatow_kra.size(); i++)
    {
        vector<int> pom;
        z_tym_trojkatem_graniczy.push_back(pom);
    }
    //to zbiera dla kazdej krawedzi z bez_duplikatow_kra trojkaty z ktorymi graniczy;
    vector<vector<int>> z_ta_krawedzia_graniczy;
    for (int i = 0; i < trojkoty.size(); i++)
    {
        vector<int> pom;
        z_ta_krawedzia_graniczy.push_back(pom);
    }

    for (int i = 0; i < trojkoty.size(); i++)
    {
        vector<pair<int, int>> trzy_boki = kraw_trojkata(trojkoty[i]);
        for (int j = 0; j < 3; j++)
        {
            int nr_krawedzi = bin_search(bez_duplikatow_kra, trzy_boki[j]);
            z_ta_krawedzia_graniczy[i].push_back(nr_krawedzi);
            z_tym_trojkatem_graniczy[nr_krawedzi].push_back(i);
        }
    }

    for (int i = 0; i < trojkoty.size(); i++)
    {
        vector<int> pom;
        graf.push_back(pom);
    }
    for (int i = 0; i < z_tym_trojkatem_graniczy.size(); i++)
    {
        vector<int> graniczne_trojkaty = z_tym_trojkatem_graniczy[i];
        if (graniczne_trojkaty.size() == 2)
        {
            graf[graniczne_trojkaty[0]].push_back(graniczne_trojkaty[1]);
            graf[graniczne_trojkaty[1]].push_back(graniczne_trojkaty[0]);
        }
    }
}

class Info_o_poddrzewie
{
  public:
    int max_bez_przedluzania;//max jesli nie ineteresuje nas co jest na zewnatrz
    int max_z_przedluzaniem_jednej_lini;//wymagamy mozliwosc przedluzenia jednej lini
    int max_z_przeduzaniem_i_rozgalezianiem;//wymagamy mozliwosc poprowadzenia jednej lini i jej rozgalezienia
    Info_o_poddrzewie()
    {
        max_bez_przedluzania = 0;
        max_z_przedluzaniem_jednej_lini = 0;
        max_z_przeduzaniem_i_rozgalezianiem = 0;
    }
};

Info_o_poddrzewie policz_info_o_poddrzewie(vector<vector<int>> &graf, int root, int ojciec)
{
    //
    //root moze miec max 2 synkow
    Info_o_poddrzewie wyrzut;
    vector<Info_o_poddrzewie> infa_od_dzieci;
    for (int i = 0; i < graf[root].size(); i++)
        if (graf[root][i] != ojciec)
        {
            int syn = graf[root][i];
            infa_od_dzieci.push_back(policz_info_o_poddrzewie(graf,syn,root));
        }
    if(infa_od_dzieci.size()==0){
        //to znaczy, ze to jest lisciem
        wyrzut.max_bez_przedluzania=1;
        wyrzut.max_z_przedluzaniem_jednej_lini=1;
        wyrzut.max_z_przeduzaniem_i_rozgalezianiem=1;
    }else if(infa_od_dzieci.size()==1){
        Info_o_poddrzewie syna=infa_od_dzieci[0];
        wyrzut.max_z_przedluzaniem_jednej_lini=syna.max_z_przedluzaniem_jednej_lini+1;
        wyrzut.max_z_przeduzaniem_i_rozgalezianiem=syna.max_z_przeduzaniem_i_rozgalezianiem+1;
        wyrzut.max_bez_przedluzania=max(max(syna.max_bez_przedluzania,wyrzut.max_z_przedluzaniem_jednej_lini),
        wyrzut.max_z_przeduzaniem_i_rozgalezianiem);//bo z przedulzaniem i rozgalezianiem jest mniejsze
    }else{
        //jest 2 dzieci
        Info_o_poddrzewie pd1=infa_od_dzieci[0];
        Info_o_poddrzewie pd2=infa_od_dzieci[1];
        wyrzut.max_z_przeduzaniem_i_rozgalezianiem=1+max(pd1.max_z_przeduzaniem_i_rozgalezianiem,pd2.max_z_przeduzaniem_i_rozgalezianiem);
        wyrzut.max_z_przedluzaniem_jednej_lini=1+max(max(pd1.max_z_przedluzaniem_jednej_lini,pd2.max_z_przedluzaniem_jednej_lini),
        pd1.max_z_przeduzaniem_i_rozgalezianiem+pd2.max_z_przeduzaniem_i_rozgalezianiem);//byc moze trzeba tu zadbac by bylo wieksze niz linia wyzej
        wyrzut.max_bez_przedluzania=max(max(max(wyrzut.max_z_przedluzaniem_jednej_lini,wyrzut.max_z_przeduzaniem_i_rozgalezianiem),
        max(pd1.max_bez_przedluzania,pd2.max_bez_przedluzania)),1+max(
            pd1.max_z_przedluzaniem_jednej_lini+pd2.max_z_przeduzaniem_i_rozgalezianiem,pd2.max_z_przedluzaniem_jednej_lini+pd1.max_z_przeduzaniem_i_rozgalezianiem  ));
    }
    return wyrzut;
}

int main()
{
    vector<vector<int>> graf;

    wczytywanie(graf);
    //szukam kandydata na roota;
    int root=0;
    for (int i = 0; i < graf.size(); i++)
    {
        if (graf[i].size() == 1)
        {
            root = i;
            break;
        }
    }
    Info_o_poddrzewie info_roota=policz_info_o_poddrzewie(graf,root,-1);
    cout<<info_roota.max_bez_przedluzania;


    return 0;
}
