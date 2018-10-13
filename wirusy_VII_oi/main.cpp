#include <iostream>
#include <vector>
#include <assert.h>
#include <fstream>
using namespace std;

class Node
{
  public:
    int ostatni_znak;                    //ostatni znak tego stringa
    vector<Node *> dzieci;               //index i ma odnosnik do dziecka powstalego z dopisania znaku i
    Node *ojciec;                        //wiadomo
    bool to_jest_wirus;                  //czy ten string jest wirusem?
    bool to_jest_prefix_wirusa;          //wiadomo
    Node *max_non_whole_prefix_suffix;   //tak jak napisane, najwiekszy nie bedacy calym prefix ktory jest suffixem tego
    bool ma_wirus_w_suffixie;            //tu moze to byc caly, to znaczy czy jeden z sufixow jest wirusem?
    bool juz_policzony;                  //to znaczy, ze juz uzupelnilem ma wirus w suffixie oraz maxP_non_whole_prefix_suffix
    vector<Node *> krawedzie_wychodzace; //do jakich stanow mozna dojsc z tego
    bool krawedzie_dorobione;
    bool dfs_zakonczony;
    Node(Node *ojciec, int nowy_znak, bool to_jest_wirus, bool to_jest_prefix_wirusa);
    void wypisz(string nazwa);
};

void Node::wypisz(string nazwa)
{
    cout << endl
         << "nazwa " << nazwa << " znak " << ostatni_znak << " niezerowe dzieci ";
    for (int i = 0; i < 2; i++)
        if (dzieci[i] != 0)
            cout << i;

    cout << " to_jest_wirus " << to_jest_wirus << " to to jest prefix wirusa " << to_jest_prefix_wirusa;
    cout << " ma wirus w suffixie " << ma_wirus_w_suffixie;
}

Node::Node(Node *ojciec, int nowy_znak, bool to_jest_wiru, bool to_jest_prefix_wirusa)
{
    this->ojciec = ojciec;
    this->ostatni_znak = nowy_znak;
    dzieci.push_back(0);
    dzieci.push_back(0);
    this->to_jest_wirus = to_jest_wiru;
    this->to_jest_prefix_wirusa = to_jest_prefix_wirusa;
    juz_policzony = false;
    krawedzie_dorobione = false;
    max_non_whole_prefix_suffix = 0; // dla innych niz root daje root, dla root daje 0;
    ma_wirus_w_suffixie = false;
    dfs_zakonczony=false;
}

class Drzewo_slow
{
  public:
    Node *root;
    Drzewo_slow()
    {
        root = new Node(0, -1, false, true);
        dorob_paddingow(root);
    }
    void wsadz(string s);
    void wypisz(Node *start, string dotychczasowy_napis);
    void wypisz()
    {
        wypisz(root, "");
    }
    void dorob_paddingow(Node *start);
    void dolicz(Node *liczony);
    Node *get_max_non_whole_prefix_suffix(Node *jakiego)
    {
        dolicz(jakiego);
        return jakiego->max_non_whole_prefix_suffix;
    }
    bool get_ma_wirus_w_suffixie(Node *jakiego)
    {
        dolicz(jakiego);
        return jakiego->ma_wirus_w_suffixie;
    }
    string podpisz_sie(Node *kto);
    void dorob_krawedzie_dla_jednego(Node *jaki);
    bool czy_jest_cykl(Node * start);
};

bool Drzewo_slow::czy_jest_cykl(Node * start){
    if(start->krawedzie_dorobione&&(!start->dfs_zakonczony))
        return true;
    if(!(start->krawedzie_dorobione)){
        dorob_krawedzie_dla_jednego(start);
        /*
        cout<<endl<<"node "<<podpisz_sie(start)<<" graniczy z ";
        for(int i=0;i<start->krawedzie_wychodzace.size();i++)
            cout<<podpisz_sie(start->krawedzie_wychodzace[i])<<", ";
        */
        for(int i=0;i<start->krawedzie_wychodzace.size();i++){
            bool cykl_w_podslowie=czy_jest_cykl(start->krawedzie_wychodzace[i]);
            if(cykl_w_podslowie)
                return true;
            
        }
    }
    start->dfs_zakonczony=true;
    return false;
}

void Drzewo_slow::dorob_krawedzie_dla_jednego(Node *jaki)
{
    //to mozna robic tylko dla prefixow
    assert(jaki->to_jest_prefix_wirusa);
    assert(jaki!=0);
    dolicz(jaki);
    if (!jaki->krawedzie_dorobione)
    {
        //sprawdzmy, czy nasz stan to nie jest koniec zabawy
        if (!(jaki->ma_wirus_w_suffixie || jaki->to_jest_wirus)) //chyba tylko pirewszy by wystarczyl
            for (int dorabiany_char = 0; dorabiany_char < 2; dorabiany_char++)
            {
                //ten jaki nie jest koncem zabawy wiec mozna dosc smialo
                Node * przed_zapadnieciem=jaki->dzieci[dorabiany_char];
                dolicz(przed_zapadnieciem);
                if(przed_zapadnieciem->to_jest_prefix_wirusa)
                    jaki->krawedzie_wychodzace.push_back(przed_zapadnieciem);
                else{
                    Node * po_zapadnieciu=get_max_non_whole_prefix_suffix(przed_zapadnieciem);
                    dolicz(po_zapadnieciu);
                    jaki->krawedzie_wychodzace.push_back(po_zapadnieciu);
                }
            }
        jaki->krawedzie_dorobione = true;
    }
}

string int_do_string(int i)
{
    if (i == 0)
        return "0";
    return "1";
}

string Drzewo_slow::podpisz_sie(Node *kto)
{
    string podpis = "";
    Node *chwilowy = kto;
    while (chwilowy != 0 && chwilowy != root)
    {
        podpis = int_do_string(chwilowy->ostatni_znak) + podpis;
        chwilowy = chwilowy->ojciec;
    }
    return podpis;
}

void Drzewo_slow::dolicz(Node *liczony)
{
    //cout<<endl<<endl<<"wykonuje to na "<<podpisz_sie(liczony)<<endl;
    if (!(liczony->juz_policzony))
    {
        if (root == liczony)
        {
            liczony->max_non_whole_prefix_suffix = 0;
            liczony->ma_wirus_w_suffixie = false;
        }
        else
        {
            //czyli nasze slowko ma dlugosc co najmniej jeden
            Node *rozwazany_poprzedni = get_max_non_whole_prefix_suffix(liczony->ojciec);
            if (rozwazany_poprzedni == 0)
            {
                //to znaczy, ze ten ojciec byl rootem, wiec trzeba zrobic
                liczony->max_non_whole_prefix_suffix = root;
                liczony->ma_wirus_w_suffixie = liczony->to_jest_wirus;
            }
            else
            {
                //teraz warto zupdatowac ma_wirus_w_suffixie

                //zupdatowany
                while (rozwazany_poprzedni != 0 && (rozwazany_poprzedni->dzieci[liczony->ostatni_znak]->to_jest_prefix_wirusa == false))
                {
                    //to znaczy, ze nawet tak skrocone jest za dlugie.
                    rozwazany_poprzedni = get_max_non_whole_prefix_suffix(rozwazany_poprzedni);
                }
                //teraz petla sie zakonczyla.
                if (rozwazany_poprzedni == 0)
                {
                    //to znaczy, ze trzeba zwrocic roota, bo tak
                    liczony->max_non_whole_prefix_suffix = root;
                    liczony->ma_wirus_w_suffixie = liczony->to_jest_wirus; //bo nie ma nawet innych prefixow w tym sufixie
                    //ma wirus w sufffixie jest juz zupdatowany
                }
                else
                {
                    //to znaczy, ze udalo nam sie znalezc cos nietrywialnego, nietrywialny prefix mozna by powiedziec.
                    liczony->max_non_whole_prefix_suffix = rozwazany_poprzedni->dzieci[liczony->ostatni_znak];
                    Node *obciety = rozwazany_poprzedni->dzieci[liczony->ostatni_znak]; //to jest taki suffix nie caly nie pusty
                    bool czy_ma_wirus_w_suffixie = liczony->to_jest_wirus || get_ma_wirus_w_suffixie(obciety);
                    liczony->ma_wirus_w_suffixie = czy_ma_wirus_w_suffixie;
                }
            }
        }

        liczony->juz_policzony = true;
    }
}

void Drzewo_slow::wypisz(Node *start, string dotychczasowy_napis)
{
    if (start != 0)
    {
        dolicz(start);
        start->wypisz(dotychczasowy_napis);
        cout << " max_nonempty_prefix_suffix " << podpisz_sie(start->max_non_whole_prefix_suffix);
        wypisz(start->dzieci[0], dotychczasowy_napis + "0");
        wypisz(start->dzieci[1], dotychczasowy_napis + "1");
    }
}

void Drzewo_slow::dorob_paddingow(Node *start)
{
    for (int i = 0; i < 2; i++)
    {
        if (start->dzieci[i] == 0)
        {
            start->dzieci[i] = new Node(start, i, false, false);
        }
    }
}

int char_to_int(char c)
{
    if (c == '0')
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

void Drzewo_slow::wsadz(string s)
{
    //cout << endl
    //   << "wkladam " << s << " jego rozmia to " << s.size() << endl;
    Node *chwilowy = root;
    for (int i = 0; i < s.size(); i++)
    {
        int wkladany = char_to_int(s[i]);
        /*
        if (chwilowy->dzieci[wkladany] == 0)
        {
            //to znaczy, ze nie ma tego nodu
            cout << endl
                 << " i= " << i << " dorabiam nowy node ciekawostka " << (i == (s.size() - 1));
            chwilowy->dzieci[wkladany] = new Node(chwilowy, wkladany, (i == (s.size() - 1)), true);
        }
        */

        //juz jest taki node
        chwilowy = chwilowy->dzieci[wkladany];
        if (!chwilowy->to_jest_wirus)
            chwilowy->to_jest_wirus = (i == s.size() - 1); //bo jak jest wirusem to niech tak zostanie;
        chwilowy->to_jest_prefix_wirusa = true;
        dorob_paddingow(chwilowy);
    }
}

int main()
{
    //ifstream we("dane2.txt");
    int n;
    cin >> n;
    Drzewo_slow wirusy;
    for (int i = 0; i < n; i++)
    {
        string pom;
        cin >> pom;
        wirusy.wsadz(pom);
    }
    //wirusy.wypisz();
    bool wynik=wirusy.czy_jest_cykl(wirusy.root);
    if(wynik)
        cout<<"TAK";
    else
        cout<<"NIE";
    
    //zbieranie danych chyba dziala


    return 0;
}
