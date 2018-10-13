#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

int next_power_of_2(int n)
{
    int wynik = 1;
    while (n > wynik)
        wynik *= 2;
    return wynik;
}

class Node
{
  public:
    int ile_weszlo;
    int najwiekszy; //prawdziwy max w reprezentowanym odcinku to jest to plus ile_wezlo od pszodkow
    int p_ile_weszlo;
    int p_najwiekszy; //to sa wartosci na probe;
    Node *prawy;
    Node *lewy;
    Node *ojciec;
    Node()
    {
        ile_weszlo = 0;
        najwiekszy = 0;
        p_ile_weszlo = 0;
        p_najwiekszy = 0;
        prawy = 0;
        lewy = 0;
        ojciec = 0;
    }
    void dorzuc_synow(Node *l, Node *p);
    void dodaj_pasazerow(int ilu, vector<Node *> &ruszane); //robi dobre tentative zmienne
    void domaxuj(vector<Node *> & ruszane);
    void zapisz();
    void cofnij();
};

void Node::zapisz(){
    najwiekszy=p_najwiekszy;
    ile_weszlo=p_ile_weszlo;
}

void Node::cofnij(){
    p_najwiekszy=najwiekszy;
    p_ile_weszlo=ile_weszlo;
}

void Node::domaxuj(vector<Node *> & ruszane){
    p_najwiekszy=max(lewy->p_najwiekszy,prawy->p_najwiekszy)+p_ile_weszlo;
    ruszane.push_back(this);
}

void Node::dodaj_pasazerow(int ilu, vector<Node *> &ruszane)
{
    p_ile_weszlo = p_ile_weszlo + ilu;
    p_najwiekszy = p_najwiekszy + ilu;
    ruszane.push_back(this);
}

void Node::dorzuc_synow(Node *l, Node *p)
{
    lewy = l;
    prawy = p;
    l->ojciec = this;
    p->ojciec = this;
}

class Kolej
{
  public:
    vector<Node *> podstawa; //podstawa naszego drzewa binarnego
    Node *root;
    int wolne_miejsca;
    Kolej(int n, int wolne_miejsca);               //n to liczba elementow z wartosciami,elementy od 0 do n-1 mozna bedzie rozwazac, wolne miejsca w pociagu
    bool wsadz(int poczatek, int koniec, int ile); // poczatek,koniec sa od 0 do n-1,
    //(poczatkowe i koncowe tez sie licza), zwraca 1 jesli udalo sie wsadzic lub 0 jak nie udalo sie
    void napraw_w_gore(Node *start, vector<Node *> &ruszane); //ruszane zbiera informacje o tym co bylo ruszane.
    //napraw w gore zaklada, ze z start jest ok, a tylko trzeba domaxowac jego pszodkow
    void zapisz_zmiany(vector<Node *> &ruszane);
    void cofnij_zmiany(vector<Node *> &ruszane);//bo wszystko leci na p_values wiec tak byc musi
};

void Kolej::zapisz_zmiany(vector<Node *> &ruszane){
    for(int i=0;i<ruszane.size();i++)
        ruszane[i]->zapisz();
}

void Kolej::cofnij_zmiany(vector<Node *> &ruszane){
    for(int i=0;i<ruszane.size();i++)
        ruszane[i]->cofnij();
}

void Kolej::napraw_w_gore(Node *start, vector<Node *> &ruszane){
    Node * chwilowy=start;
    while(chwilowy->ojciec!=0){
        chwilowy=chwilowy->ojciec;
        chwilowy->domaxuj(ruszane);
    }
}


bool Kolej::wsadz(int poczatek, int koniec, int ile)
{
    Node *a = podstawa[poczatek];
    Node *b = podstawa[koniec];
    vector<Node *> ruszane;
    if (a == b)
    {
        a->dodaj_pasazerow(ile, ruszane);
        napraw_w_gore(a, ruszane);
    }else{
        a->dodaj_pasazerow(ile,ruszane);
        b->dodaj_pasazerow(ile,ruszane);
        while(a->ojciec!= b->ojciec){
            //trzeba isc w gore;
            bool a_idzie_do_srodka= (a->ojciec->lewy==a);
            bool b_idzie_do_srodka= (b->ojciec->prawy==b);
            if(a_idzie_do_srodka)
                a->ojciec->prawy->dodaj_pasazerow(ile,ruszane);
            if(b_idzie_do_srodka)
                b->ojciec->lewy->dodaj_pasazerow(ile,ruszane);
            a=a->ojciec;
            b=b->ojciec;
            a->domaxuj(ruszane);
            b->domaxuj(ruszane);
        }
        //teraz te dwa ziomeczki maja wspolnego ojca;
        a->ojciec->domaxuj(ruszane);
        napraw_w_gore(a->ojciec,ruszane);
    }

    if (root->p_najwiekszy <= wolne_miejsca)
    {
        //jest ok
        zapisz_zmiany(ruszane);
        return true;
    }
    else
    {
        cofnij_zmiany(ruszane);
        return false;
    }
}

Node *dorabiaj_nody(vector<Node *> &podstawa)
{
    if (podstawa.size() > 1)
    {
        vector<Node *> nowa_podstawa;
        for (int i = 0; i < podstawa.size();)
        {
            Node *nowy = new Node;
            nowy->dorzuc_synow(podstawa[i], podstawa[i + 1]);
            nowa_podstawa.push_back(nowy);
            i += 2;
        }
        return dorabiaj_nody(nowa_podstawa);
    }
    else
    {
        return podstawa[0];
    }
}

Kolej::Kolej(int n, int wolne_miejsca)
{
    this->wolne_miejsca = wolne_miejsca;
    int np2 = next_power_of_2(n);
    for (int i = 0; i < np2; i++)
    {
        Node *nowy = new Node;
        podstawa.push_back(nowy);
    }
    root = dorabiaj_nody(podstawa);
}

int main()
{
    
    //ifstream we("dane.txt");
    int liczba_miast_na_trasie;
    int liczba_miejsc_w_pociagu;
    int liczba_zgloszen;
    cin>>liczba_miast_na_trasie;
    cin>>liczba_miejsc_w_pociagu;
    cin>>liczba_zgloszen;
    Kolej kolej(liczba_miast_na_trasie-1, liczba_miejsc_w_pociagu);
    vector<vector<int> > zgloszenia;
    for(int i=0;i<liczba_zgloszen;i++){
        vector<int> nowy;
        int pom;
        cin>>pom;
        nowy.push_back(pom-1);
        cin>>pom;
        nowy.push_back(pom-2);
        cin>>pom;
        nowy.push_back(pom);
        zgloszenia.push_back(nowy);
    }
    for(int i=0;i<zgloszenia.size();i++){
        vector<int> z=zgloszenia[i];
        if(kolej.wsadz(z[0],z[1],z[2])){
            cout<<"T";
        }else cout<<"N";
        if(i!=zgloszenia.size()-1)
            cout<<endl;
    }

    
/*
    cout << endl
         << "doszlo do return 0" << endl;
        */
    return 0;
}
