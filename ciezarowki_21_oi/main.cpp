#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
using namespace std;

class Event
{
  public:
    bool czy_zmiana;
    int pierwsza_pozycja;            //kto lub ile ciezarowek
    int druga_pozycja;               //nowa wartosc lub jak daleko
    int index_do_ktorego_sie_odnosi; // w przypadku zamiany
    Event()
    {
        pierwsza_pozycja = -1;
        druga_pozycja = -1;
        index_do_ktorego_sie_odnosi = -1;
        czy_zmiana = false;
    }
    void inicjoj(bool czy_zmiana, int pierwsza_pozycja, int druga_pozycja)
    {
        this->czy_zmiana = czy_zmiana;
        this->pierwsza_pozycja = pierwsza_pozycja;
        this->druga_pozycja = druga_pozycja;
    }
};

void wczytywanie(Event *&eventy, int &n, int &m)
{
    //ifstream we("dane.txt");
    cin >> n >> m;
    eventy = new Event[m];
    char litera;
    int poz1, poz2;
    for (int i = 0; i < m; i++)
    {
        cin >> litera >> poz1 >> poz2;
        if (litera == 'U')
        {
            eventy[i].inicjoj(true, poz1 - 1, poz2);
        }
        else
        {
            eventy[i].inicjoj(false, poz1, poz2);
        }
    }
}

class Sortowaniowy_item
{
  public:
    int wartosc;
    int *wskazywany;
    Sortowaniowy_item()
    {
    }
    Sortowaniowy_item(int wartosc, int *wskazywany)
    {
        this->wartosc = wartosc;
        this->wskazywany = wskazywany;
    }

    bool operator>(const Sortowaniowy_item &str) const
    {
        return (wartosc < str.wartosc);
    }
    bool operator<(const Sortowaniowy_item &str) const
    {
        return (wartosc > str.wartosc);
    }
};

void dorob_co_gdzie(int m, Event *eventy, vector<Sortowaniowy_item> &rzeczy_do_sortowania)
{
    for (int i = 0; i < m; i++)
        if (eventy[i].czy_zmiana)
            rzeczy_do_sortowania.push_back(Sortowaniowy_item(eventy[i].druga_pozycja, &(eventy[i].index_do_ktorego_sie_odnosi)));
    //wiec teraz trzeba posorotwac
    sort(rzeczy_do_sortowania.begin(), rzeczy_do_sortowania.end());
    for (int i = 0; i < rzeczy_do_sortowania.size(); i++)
    {
        *(rzeczy_do_sortowania[i].wskazywany) = i;
    }
}

class Node
{
  public:
    int wartosc;
    int ile_pod;
    long long int suma_zapalonych_pod_spodem;
    int ziomek_na_lewym_koncu;
    Node()
    {
        ziomek_na_lewym_koncu = 0;
        wartosc = 0; //dla nie lisci to jest taka fakowa wartosc
        ile_pod = 0;
        suma_zapalonych_pod_spodem = 0;
    }
    void przyjmij_wartosc_jako_lisc(int wartosc)
    {
        this->wartosc = wartosc;
        ziomek_na_lewym_koncu = wartosc;
    }
    void obczaj_synow(Node &lewy, Node &prawy)
    {
        wartosc = prawy.ziomek_na_lewym_koncu;
        ile_pod = lewy.ile_pod + prawy.ile_pod;
        suma_zapalonych_pod_spodem = (long long int)lewy.suma_zapalonych_pod_spodem + prawy.suma_zapalonych_pod_spodem;
        ziomek_na_lewym_koncu = lewy.ziomek_na_lewym_koncu;
    }
    long long int policz_to_b(int k, Node &lewy, Node &prawy, int ile_ostro_na_lewo, long long int suma_ostro_na_prawo)
    {
        long long int wyrzut = 0;
        wyrzut = (long long int)(k - lewy.ile_pod - ile_ostro_na_lewo) * wartosc;
        wyrzut -= (long long int)prawy.suma_zapalonych_pod_spodem + suma_ostro_na_prawo;
        return wyrzut;
    }
    long long int policz_to_b_w_lisciu(int k, int ile_ostro_na_lewo, long long int suma_ostro_na_prawo)
    {
        long long int wyrzut = 0;
        wyrzut = (long long int)(k - ile_ostro_na_lewo - ile_pod) * wartosc;
        wyrzut -= suma_ostro_na_prawo;
        return wyrzut;
    }
};

class Drzewo_przedzialowe
{
  public:
    vector<Node> nody;
    int rozmiar_potega_2;
    Drzewo_przedzialowe(vector<Sortowaniowy_item> &tab)
    {
        rozmiar_potega_2 = 1;
        while (rozmiar_potega_2 < tab.size())
            rozmiar_potega_2 *= 2;
        nody.resize(2 * rozmiar_potega_2);
        for (int i = 0; i < tab.size(); i++)
        {
            nody[i + rozmiar_potega_2].przyjmij_wartosc_jako_lisc(tab[i].wartosc);
        }
        tab.resize(0);
        for (int i = rozmiar_potega_2 - 1; i >= 1; i--)
            nody[i].obczaj_synow(nody[2 * i], nody[2 * i + 1]);
    }
    long long int jak_daleko_maksymalnie_pojedziesz(int k)
    {
        int ile_ostro_na_lewo = 0;
        long long int suma_ostro_na_prawo = 0;
        long long int b;
        int i = 1;
        while (i < rozmiar_potega_2)
        {
            b = nody[i].policz_to_b(k, nody[2 * i], nody[2 * i + 1], ile_ostro_na_lewo, suma_ostro_na_prawo);
            if (b > 0)
            {
                i = 2 * i + 1;
                ile_ostro_na_lewo += nody[i - 1].ile_pod;
            }
            else
            {
                i = 2 * i;
                suma_ostro_na_prawo += nody[i + 1].suma_zapalonych_pod_spodem;
            }
        }
        //tu zakonczyles w jakims, ale byc moze nie jest dodatni.
        if (nody[i].policz_to_b_w_lisciu(k, ile_ostro_na_lewo, suma_ostro_na_prawo) > 0)
        {
            int mianownik = k - (ile_ostro_na_lewo + nody[i].ile_pod);
            long long int licznik = suma_ostro_na_prawo;
            return licznik / mianownik;
        }else{
            int mianownik=k;
            long long int licznik=suma_ostro_na_prawo + nody[i].suma_zapalonych_pod_spodem;
            return licznik/mianownik;
        }
    }
    void dodaj_wartosc_na_pozycji(int pozycja)
    {
        pozycja += rozmiar_potega_2;
        nody[pozycja].ile_pod = 1;
        nody[pozycja].suma_zapalonych_pod_spodem = nody[pozycja].wartosc;
        pozycja /= 2;
        while (pozycja != 0)
        {
            nody[pozycja].obczaj_synow(nody[2 * pozycja], nody[2 * pozycja + 1]);
            pozycja /= 2;
        }
    }
    void usun_wartosc_na_pozycji(int pozycja)
    {
        pozycja += rozmiar_potega_2;
        nody[pozycja].ile_pod = 0;
        nody[pozycja].suma_zapalonych_pod_spodem = 0;
        pozycja /= 2;
        while (pozycja != 0)
        {
            nody[pozycja].obczaj_synow(nody[2 * pozycja], nody[2 * pozycja + 1]);
            pozycja /= 2;
        }
    }
};

void rob_symulacje(int n,vector<bool> & odpowiedzi, Event * eventy, int m, Drzewo_przedzialowe & drzewo){
    vector<int> ostatnie_wytrzymalosci;
    ostatnie_wytrzymalosci.resize(n,-1);
    for(int i=0;i<m;i++){
        Event rozwazany=eventy[i];
        if(rozwazany.czy_zmiana){
            int kto=rozwazany.pierwsza_pozycja;
            if(ostatnie_wytrzymalosci[kto]!= -1)
                drzewo.usun_wartosc_na_pozycji(ostatnie_wytrzymalosci[kto]);
            ostatnie_wytrzymalosci[kto]=rozwazany.index_do_ktorego_sie_odnosi;
            drzewo.dodaj_wartosc_na_pozycji(rozwazany.index_do_ktorego_sie_odnosi);
        }else{
            int k=rozwazany.pierwsza_pozycja;
            int odleglosc=rozwazany.druga_pozycja;
            long long int jak_daleko= drzewo.jak_daleko_maksymalnie_pojedziesz(k);
            odpowiedzi.push_back(jak_daleko >= odleglosc);
        }
    }
}

int main()
{
    Event *eventy;
    int n, m;
    wczytywanie(eventy, n, m);
    vector<Sortowaniowy_item> tab;
    dorob_co_gdzie(m, eventy, tab);
    /*
    cout << "dotkniete wartoscil" << endl;
    for (int i = 0; i < tab.size(); i++)
    {
        cout << "i " << i << " wartoc " << tab[i].wartosc << endl;
    }
    

    cout << "indexy w eventach" << endl;
    for (int i = 0; i < m; i++)
        if (eventy[i].czy_zmiana)
        {
            cout << eventy[i].index_do_ktorego_sie_odnosi << endl;
        }
    */
    Drzewo_przedzialowe drzewo(tab);
    vector<bool> odpowiedzi;
    rob_symulacje(n,odpowiedzi,eventy,m,drzewo);

    //cout<<"odpowiedzi"<<endl<<endl;
    for(int i=0;i<odpowiedzi.size();i++){
        if(odpowiedzi[i])
            cout<<"TAK";
        else
            cout<<"NIE";
        if(i!=odpowiedzi.size()-1)
            cout<<endl;
    }

    //cout << endl
         //<< "doszlo do return 0" << endl;
    return 0;
}
