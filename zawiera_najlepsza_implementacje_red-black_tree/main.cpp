#include <iostream>
#include <time.h>
#include <vector>
#include <fstream>
using namespace std;
//najpierw nasz node

string to_string(pair<int, int> a)
{
    return to_string(a.first) + ", " + to_string(a.second);
}

template <class T, class S>
class Node
{
  public:
    bool red;
    Node *left;
    Node *right;
    Node *father;
    T value;
    S satelite;
    Node(){};
    Node(T wartosc, S satelitarna, bool czy_czerwony, Node *sentinel)
    {
        red = czy_czerwony;
        left = right = father = sentinel;
        value = wartosc;
        satelite = satelitarna;
    }
};

string to_string(Node<long long int, pair<int, int>> *a)
{
    pair<int, int> pom = a->satelite;
    return " " + to_string(pom.first) + ", " + to_string(pom.second) + " ";
}

template <class T, class S>
class RBTree
{
  public:
    Node<T, S> *sent;
    Node<T, S> *root;
    bool (*mniejszy)(T, T); // jak nazwa wskazuje to znaczy ze pierwszy argument jest ostro mniejszy niz drugi.
    RBTree(bool (*f)(T, T));
    Node<T, S> *wsadz(T wartosc, S satelitarna);
    void dwa_z_rzedu(Node<T, S> *start);
    void wypisz_dokladnie(Node<T, S> *start);
    void wypisz_dokladnie();
    void wypisz_zbior(Node<T, S> *start);
    void wypisz_zbior();
    Node<T, S> *minimum(Node<T, S> *start);
    Node<T, S> *minimum();
    Node<T, S> *maximum(Node<T, S> *start);
    Node<T, S> *maximum();
    Node<T, S> *nastepny(Node<T, S> *start);
    Node<T, S> *poprzedni(Node<T, S> *start);
    Node<T, S> *odnajdz(T wartosc); //jak jest wiele to znajduje najmniejszy w kolejnosci obchodzenia drzewa.
    void rotate_right(Node<T, S> *zawias);
    void rotate_left(Node<T, S> *zawias);
    void usun(Node<T, S> *element);
    void przesadz(Node<T, S> *co, Node<T, S> *gdzie);
    void wsadz_fixup(Node<T, S> *nowy);
    int black_height(Node<T, S> *start);
    void usun_fixup(Node<T, S> *chwilowy);
    Node<T, S> *wiekszy_lub_rowny_od(T wartosc); //jesli jest wiele wartosci to zwroci wskaznik do pierwszsego w "kolejnosci drzewa", wiec uzywajac
    //funkcji nast mozna obejsc je wszystkie sobie.
    Node<T, S> *mniejszy_lub_rowny_od(T wartosc); //tutaj jak jest wiele to jes tak zriione, by poprzedni porzeszlo wszystkie wartosci ktore sa takie same.
};

template <class T, class S>
Node<T, S> *RBTree<T, S>::wiekszy_lub_rowny_od(T wartosc)
{
    //Node<T, S> *nowy = new Node<T, S>(wartosc, satelitarna, true, sent);
    Node<T, S> *ojciec_to_be = sent;
    Node<T, S> *pod_nim = root;
    bool ostatnio_na_lewo;
    while (pod_nim != sent)
    {
        ojciec_to_be = pod_nim;
        if (mniejszy(pod_nim->value, wartosc))
        {
            pod_nim = pod_nim->right;
            ostatnio_na_lewo = false;
        }
        else
        {
            pod_nim = pod_nim->left;
            ostatnio_na_lewo = true;
        }
    }
    if (ojciec_to_be == sent) //to znaczy, ze bylo puste
        return sent;
    if (ostatnio_na_lewo)
        return ojciec_to_be;
    else
        return nastepny(ojciec_to_be);
}

template <class T, class S>
Node<T, S> *RBTree<T, S>::mniejszy_lub_rowny_od(T wartosc)
{
    //Node<T, S> *nowy = new Node<T, S>(wartosc, satelitarna, true, sent);
    Node<T, S> *ojciec_to_be = sent;
    Node<T, S> *pod_nim = root;
    bool ostatnio_na_prawo;
    while (pod_nim != sent)
    {
        ojciec_to_be = pod_nim;
        if (mniejszy(wartosc, pod_nim->value))
        {
            pod_nim = pod_nim->left;
            ostatnio_na_prawo = false;
        }
        else
        {
            pod_nim = pod_nim->right;
            ostatnio_na_prawo = true;
        }
    }
    if (ojciec_to_be == sent) //to znaczy, ze bylo puste
        return sent;
    if (ostatnio_na_prawo)
        return ojciec_to_be;
    else
        return poprzedni(ojciec_to_be);
}

template <class T, class S>
void RBTree<T, S>::wypisz_dokladnie()
{
    wypisz_dokladnie(root);
}

template <class T, class S>
Node<T, S> *RBTree<T, S>::minimum()
{
    return minimum(root);
}

template <class T, class S>
Node<T, S> *RBTree<T, S>::maximum()
{
    return maximum(root);
}

template <class T, class S>
void RBTree<T, S>::usun_fixup(Node<T, S> *chwilowy)
{
    typedef Node<T, S> *nod;
    //teraz chwilowy ma nad soba dodatkowy czarny kolor. ma dobrego ojca nawet jesli jest sentinelem
    while (chwilowy != root)
        if (chwilowy->red)
        {
            chwilowy->red = false;
            break;
        }
        else
        {
            //to znaczy, ze gosc ma swojego brata nietrywialnego.
            bool chwilowy_jest_lewy = (chwilowy->father->left == chwilowy);

            if (chwilowy_jest_lewy)
            {
                nod brat = chwilowy->father->right;
                if (brat->red)
                {
                    //rotacja sprawi, ze brat bedzie jednak czearny
                    rotate_left(chwilowy->father);
                    brat = chwilowy->father->right;
                    chwilowy->father->red = true;
                    chwilowy->father->father->red = false;
                    //dalej kontynuujemy wiec nie ma break;
                }
                else
                    //teraz brat jest czarny
                    if (brat->left->red && (brat->right->red == false))
                {
                    nod ch_oj = chwilowy->father;
                    rotate_right(brat);
                    brat = brat->father;
                    brat->red = false;
                    brat->right->red = true;
                    chwilowy->father = ch_oj;
                    //cout<<endl<<"teraz po tym pierwszym ruchu"<<endl;
                    //wypisz_dokladnie(root);
                }
                else

                    if ((brat->right->red == false) && (brat->left->red == false))
                {
                    //obydwaj synowie brat sa czarni
                    brat->red = true;
                    chwilowy = chwilowy->father;
                }
                else if ((brat->left->red == false) && brat->right->red)
                {
                    //teraz lewy syn brata jest czarny a prawy jest czerwony
                    rotate_left(chwilowy->father);
                    chwilowy->father->father->red = chwilowy->father->red;
                    chwilowy->father->red = true;
                    chwilowy = chwilowy->father->father;
                }
                else
                {
                    //teraz synowie brata sa czerwoni a on jest czarny
                    bool c_is_red = chwilowy->father->red;
                    rotate_left(chwilowy->father);
                    chwilowy->father->red = false;
                    chwilowy->father->father->red = c_is_red;
                    chwilowy->father->right->red = true;
                    chwilowy->father->father->right->red = false;
                    break;
                }
            }
            else
            {

                nod brat = chwilowy->father->left;
                if (brat->red)
                {
                    //rotacja sprawi, ze brat bedzie jednak czearny
                    rotate_right(chwilowy->father);
                    brat = chwilowy->father->left;
                    chwilowy->father->red = true;
                    chwilowy->father->father->red = false;
                    //dalej kontynuujemy wiec nie ma break;
                }
                else
                    //teraz brat jest czarny
                    if (brat->right->red && (brat->left->red == false))
                {
                    nod ch_oj = chwilowy->father;
                    rotate_left(brat);
                    brat = brat->father;
                    brat->red = false;
                    brat->left->red = true;

                    chwilowy->father = ch_oj;
                    //cout<<endl<<"teraz po tym pierwszym ruchu"<<endl;
                    //wypisz_dokladnie(root);
                }
                else

                    if ((brat->left->red == false) && (brat->right->red == false))
                {
                    //obydwaj synowie brat sa czarni
                    brat->red = true;
                    chwilowy = chwilowy->father;
                }
                else if ((brat->right->red == false) && brat->left->red)
                {
                    //teraz lewy syn brata jest czarny a prawy jest czerwony
                    rotate_right(chwilowy->father);
                    chwilowy->father->father->red = chwilowy->father->red;
                    chwilowy->father->red = true;
                    chwilowy = chwilowy->father->father;
                }
                else
                {

                    //teraz synowie brata sa czerwoni a on jest czarny
                    bool c_is_red = chwilowy->father->red;
                    rotate_right(chwilowy->father);
                    chwilowy->father->red = false;
                    chwilowy->father->father->red = c_is_red;
                    chwilowy->father->left->red = true;
                    chwilowy->father->father->left->red = false;
                    break;
                }
            }
        }
    root->red = false;
}

template <class T, class S>
int RBTree<T, S>::black_height(Node<T, S> *start)
{
    if (start == sent)
        return 0;
    int dodawane = 0;
    if (start->red == false)
        dodawane = 1;
    int lewa = black_height(start->left);
    int prawa = black_height(start->right);
    if (lewa != prawa)
        cout << "jest zle bardzo zle " << endl;
    return dodawane + lewa;
}

template <class T, class S>
void RBTree<T, S>::wsadz_fixup(Node<T, S> *nowy)
{
    typedef Node<T, S> *nod;
    nod chwilowy = nowy;

    while (chwilowy != root && chwilowy->red && chwilowy->father->red)
    {

        nod ojciec = chwilowy->father;

        bool rosnaca_linia_z_ojca = (chwilowy->father->father->left == chwilowy->father);
        bool rosnaca_linia_z_chwilowy = (chwilowy->father->left == chwilowy);
        //teraz wyrownuje sobie
        if (rosnaca_linia_z_chwilowy != rosnaca_linia_z_ojca)
        {
            if (rosnaca_linia_z_chwilowy)
                rotate_right(chwilowy->father);
            else
                rotate_left(chwilowy->father);
            nod pom = chwilowy;
            chwilowy = ojciec;
            ojciec = pom;
        }
        nod dziadek = ojciec->father;
        nod unkle;
        if (rosnaca_linia_z_ojca)
            unkle = dziadek->right;
        else
            unkle = dziadek->left;
        if (unkle->red)
        {

            dziadek->red = true;
            ojciec->red = false;
            unkle->red = false;
            chwilowy = dziadek;
        }
        else
        {

            if (rosnaca_linia_z_ojca)
                rotate_right(dziadek);
            else
                rotate_left(dziadek);
            dziadek->red = true;
            ojciec->red = false;
            chwilowy = ojciec;
            break;
        }
    }
    root->red = false;
}

template <class T, class S>
void RBTree<T, S>::rotate_right(Node<T, S> *zawias)
{
    //zaklada, ze jest lewy nietrywialny syn zawiasu
    //prawy bedzie miec dobrego ojca nawet jesli jest sentem.
    //kolory sie nie zmieniaja oczywiscie.
    typedef Node<T, S> *nod;
    Node<T, S> *a = zawias;
    Node<T, S> *b = zawias->left;
    nod alpha = b->left;
    nod beta = b->right;
    nod gamma = a->right;
    przesadz(b, a);
    alpha->father = b;
    b->left = alpha;
    beta->father = a;
    a->left = beta;
    gamma->father = a;
    a->right = gamma;
    a->father = b;
    b->right = a;
    a->right->father = a;
}

template <class T, class S>
void RBTree<T, S>::rotate_left(Node<T, S> *zawias)
{
    //zaklada, ze jest prawy nietrywialny syn zawiasu
    //lewy syn zawiasu bedzie miec dobrego ojca nawet jesli jest sentem
    //kolory sie nie zmieniaja oczywiscie

    typedef Node<T, S> *nod;
    Node<T, S> *b = zawias;
    Node<T, S> *a = zawias->right;
    nod alpha = b->left;
    nod beta = a->left;
    nod gamma = a->right;
    przesadz(a, b);
    alpha->father = b;
    b->left = alpha;
    beta->father = b;
    b->right = beta;
    gamma->father = a;
    a->right = gamma;
    b->father = a;
    a->left = b;
    b->left->father = b;
}

template <class T, class S>
void RBTree<T, S>::wypisz_dokladnie(Node<T, S> *start)
{
    if (start == root)
    {
        cout << endl
             << endl
             << "root " << root->value << endl;
    }
    if (start != sent)
    {
        wypisz_dokladnie(start->left);
        cout << "key " << to_string(start->value) << "satelite " << to_string(start->satelite);
        if (start->red)
            cout << " red ";
        else
            cout << " black ";
        cout << " father " << start->father->value << " sons " << start->left->value << " " << start->right->value << endl;
        wypisz_dokladnie(start->right);
    }
}

template <class T, class S>
void RBTree<T, S>::wypisz_zbior(Node<T, S> *start)
{
    if (start != sent)
    {
        wypisz_zbior(start->left);
        cout << "key " << to_string(start->value) << " satelite " << to_string(start->satelite) << endl;
        wypisz_zbior(start->right);
    }
}

template <class T, class S>
void RBTree<T, S>::wypisz_zbior()
{
    cout << endl
         << endl
         << "wypisuje drzewo czerwono-czarne" << endl
         << endl;
    wypisz_zbior(root);
    cout << endl
         << "koniec wypisywania drzewa czerwono-czarnego" << endl
         << endl;
}

template <class T, class S>
void RBTree<T, S>::przesadz(Node<T, S> *co, Node<T, S> *gdzie)
{
    //kolory wszystkie zachowuja swoje nie ma problemu.
    //przesadz poddrzewo takie tam. dba o takie rzeczy jak root
    if (gdzie == root)
    {
        root = co;
        co->father = sent;
    }
    else
    {
        co->father = gdzie->father;
        if (gdzie == gdzie->father->left)
            gdzie->father->left = co;
        else
            gdzie->father->right = co;
    }
}

template <class T, class S>
void RBTree<T, S>::usun(Node<T, S> *element)
{
    typedef Node<T, S> *nod;
    nod do_fixup;
    bool czy_fixup = false;
    nod ojciec_fixupu;
    if (element->left == sent)
    {
        do_fixup = element->right;
        czy_fixup = (element->red == false);
        ojciec_fixupu = element->father;
        przesadz(element->right, element);
    }
    else if (element->right == sent)
    {
        do_fixup = element->left;
        czy_fixup = (element->red == false);
        ojciec_fixupu = element->father;
        przesadz(element->left, element);
    }
    //czyli sa 2 synowie.
    else
    {
        Node<T, S> *wpychajacy = nastepny(element);
        if (wpychajacy == element->right)
        {
            //trzeba to dorobic.
            do_fixup = wpychajacy->right;
            czy_fixup = (wpychajacy->red == false);
            ojciec_fixupu = wpychajacy;
            wpychajacy->left = element->left;
            wpychajacy->red = element->red;
            element->left->father = wpychajacy;
            przesadz(wpychajacy, element);
        }
        else
        {
            //trzeba najpierw wyrwac wpychajacy
            do_fixup = wpychajacy->right;
            czy_fixup = (wpychajacy->red == false);
            ojciec_fixupu = wpychajacy->father;
            przesadz(wpychajacy->right, wpychajacy);
            wpychajacy->left = element->left;
            wpychajacy->right = element->right;
            wpychajacy->red = element->red;
            element->left->father = wpychajacy;
            element->right->father = wpychajacy;
            przesadz(wpychajacy, element);
        }
    }

    delete element;
    if (czy_fixup)
    {
        do_fixup->father = ojciec_fixupu;
        //cout<<"do_fixiu"<<do_fixup->value<<" ojciec jego "<<do_fixup->father->value<<endl;
        usun_fixup(do_fixup);
    }
    root->father = sent;
}

template <class T, class S>
Node<T, S> *RBTree<T, S>::odnajdz(T wartosc)
{
    Node<T, S> *ojciec_to_be = root;
    while (ojciec_to_be != sent && ojciec_to_be->value != wartosc)
        if (mniejszy(ojciec_to_be->value, wartosc))
            ojciec_to_be = ojciec_to_be->right;
        else
            ojciec_to_be = ojciec_to_be->left;
    if (ojciec_to_be == sent)
    {
        cout << "nie znaleziono";
        return sent;
    }
    return ojciec_to_be;
}

template <class T, class S>
Node<T, S> *RBTree<T, S>::poprzedni(Node<T, S> *start)
{
    //zwraca sentinela jak trzeba
    //zakladam, ze start to nie sent
    if (start->left != sent)
        return maximum(start->left);
    else
    {
        Node<T, S> *chwilowy = start;
        while (chwilowy->father != sent && chwilowy->father->left == chwilowy)
            chwilowy = chwilowy->father;
        return chwilowy->father;
    }
}

template <class T, class S>
Node<T, S> *RBTree<T, S>::nastepny(Node<T, S> *start)
{
    //zwraca sentinela jak trzeba
    //zakladam, ze start to nie sent
    if (start->right != sent)
        return minimum(start->right);
    else
    {
        Node<T, S> *chwilowy = start;
        while (chwilowy->father != sent && chwilowy->father->right == chwilowy)
            chwilowy = chwilowy->father;
        return chwilowy->father;
    }
}

template <class T, class S>
Node<T, S> *RBTree<T, S>::maximum(Node<T, S> *start)
{
    Node<T, S> *kandydat = start;
    while (kandydat->right != sent)
        kandydat = kandydat->right;
    return kandydat;
}

template <class T, class S>
Node<T, S> *RBTree<T, S>::minimum(Node<T, S> *start)
{
    Node<T, S> *kandydat = start;
    while (kandydat->left != sent)
        kandydat = kandydat->left;
    return kandydat;
}

template <class T, class S>
void RBTree<T, S>::dwa_z_rzedu(Node<T, S> *start)
{
    if (start != sent)
    {
        dwa_z_rzedu(start->left);
        //cout << start->value << " ";
        if (start->red && start->father->red)
            cout << "dwa z rzedu takie same kolory";
        dwa_z_rzedu(start->right);
    }
}

bool mniejszy_int(int a, int b)
{
    return a < b;
}

template <class T, class S>
Node<T, S> *RBTree<T, S>::wsadz(T wartosc, S satelitarna)
{
    Node<T, S> *nowy = new Node<T, S>(wartosc, satelitarna, true, sent);
    Node<T, S> *ojciec_to_be = sent;
    Node<T, S> *pod_nim = root;
    while (pod_nim != sent)
    {
        ojciec_to_be = pod_nim;
        if (mniejszy(wartosc, pod_nim->value))
            pod_nim = pod_nim->left;
        else
            pod_nim = pod_nim->right;
    }
    if (ojciec_to_be == sent)
        root = nowy;
    else
    {
        nowy->father = ojciec_to_be;
        if (mniejszy(wartosc, ojciec_to_be->value))
            ojciec_to_be->left = nowy;
        else
            ojciec_to_be->right = nowy;
    }
    //teraz insert fixup
    wsadz_fixup(nowy);
    return nowy;
}

template <class T, class S>
RBTree<T, S>::RBTree(bool (*f)(T, T))
{
    sent = new Node<T, S>;
    sent->value = -77;
    sent->red = false;
    root = sent;
    mniejszy = f;
}

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

//teraz prawdziwe kodzenie

bool mniejszy_long_int(long long int a, long long int b)
{
    return a < b;
}

typedef Node<long long int, pair<int, int>> *Wskaznik_do_area;

class Wektor_3d
{
  public:
    int x, y, z;
    Wektor_3d(int xx, int yy, int zz)
    {
        x = xx;
        y = yy;
        z = zz;
    }
    Wektor_3d()
    {
        x = 0;
        y = 0;
        z = 0;
    }
    long long int objetosc()
    {
        return ((long long int)x) * ((long long int)y) * ((long long int)z);
    }
};

class Plansza_2d
{
  public:
    int bok;
    RBTree<int, Wskaznik_do_area> x_sorted;
    RBTree<long long int, pair<int, int>> area_sorted;
    void wsadz_punkt(Wektor_3d &punkt_3d);
    pair<int, int> max_punkt();
    Plansza_2d();
    void usun(Node<int, Wskaznik_do_area> *jaki);
    long long int max_pole();
    void wsadz_do_drzew(pair<int, int> punkt);
};

void Plansza_2d::wsadz_do_drzew(pair<int, int> punkt)
{
    //cout << endl
      //   << "wsadzam " << to_string(punkt) << endl;
    Wskaznik_do_area ws_do_area = area_sorted.wsadz(((long long int)punkt.first) * ((long long int)punkt.second), punkt);
    x_sorted.wsadz(punkt.first, ws_do_area);
}

void Plansza_2d::usun(Node<int, Wskaznik_do_area> *jaki)
{
   // cout << endl
     //    << "usuwam " << to_string(jaki->satelite->satelite) << endl;
    Wskaznik_do_area ws_do_ar = jaki->satelite;
    area_sorted.usun(ws_do_ar);
    x_sorted.usun(jaki);
}

pair<int, int> Plansza_2d::max_punkt()
{
    return area_sorted.maximum()->satelite;
}

long long int Plansza_2d::max_pole()
{
    return area_sorted.maximum()->value;
}

Plansza_2d::Plansza_2d() : x_sorted(&mniejszy_int), area_sorted(&mniejszy_long_int)
{
    bok = 1000000;
    Wskaznik_do_area ws_do_area = area_sorted.wsadz(((long long int)bok) * ((long long int)bok), pair<int, int>(bok, bok));
    x_sorted.wsadz(bok, ws_do_area);
    area_sorted.sent->satelite = pair<int, int>(bok, bok);
}

void Plansza_2d::wsadz_punkt(Wektor_3d &punkt_3d)
{
    pair<int, int> wsadzany = pair<int, int>(punkt_3d.x, punkt_3d.y);
    Node<int, Wskaznik_do_area> *na_prawo = x_sorted.wiekszy_lub_rowny_od(wsadzany.first);
    int y_sciana = na_prawo->satelite->satelite.second;
    Node<int, Wskaznik_do_area> *poprzedni = x_sorted.mniejszy_lub_rowny_od(wsadzany.first);
    //na pewno na_prawo nie jest teraz sentem.
    int ile_usunelismy = 0;
    if (wsadzany.first == na_prawo->satelite->satelite.first)
        na_prawo = x_sorted.nastepny(na_prawo);
    int x_sciany;
    while (x_sorted.sent != na_prawo && wsadzany.second < na_prawo->satelite->satelite.second)
    {
        Node<int, Wskaznik_do_area> *poprzedni_na_prawo = na_prawo;
        na_prawo = x_sorted.nastepny(na_prawo);
        x_sciany = poprzedni_na_prawo->satelite->satelite.first;
        usun(poprzedni_na_prawo);
        ile_usunelismy++;
    }
    //teraz trzeba wstawic nasze nowe punkty.
    if (ile_usunelismy > 0)
    {

        if (poprzedni == x_sorted.sent)
        {
            wsadz_do_drzew(pair<int, int>(wsadzany.first, bok));
        }
        else if (poprzedni->satelite->satelite.first != wsadzany.first)
            wsadz_do_drzew(pair<int, int>(wsadzany.first, y_sciana));
        if (na_prawo == x_sorted.sent)
            wsadz_do_drzew(pair<int, int>(bok, wsadzany.second));
        else if (na_prawo->satelite->satelite.second != wsadzany.second)
            wsadz_do_drzew(pair<int, int>(x_sciany, wsadzany.second));
    }
}

bool mniejszy_3d_wektor(Wektor_3d a, Wektor_3d b)
{
    return a.z < b.z;
}

int main()
{
    /*
    srand(time(NULL));
    //cout<<rand()<<endl;
    RBTree<int,pair<int,int> > drzewo(&intowa);
    */
    //wczytywanie danych

    //cout<<nowa.area_sorted.root->value;

    ifstream we("dane.txt");
    int n;
    we >> n;
    vector<Wektor_3d> punkty;
    for (int i = 0; i < n; i++)
    {
        vector<int> punkt;
        for (int j = 0; j < 3; j++)
        {
            int pom;
            we >> pom;
            punkt.push_back(pom);
        }
        Wektor_3d nowy(punkt[0], punkt[1], punkt[2]);
        punkty.push_back(nowy);
    }

    //wczytywanie zakonczone
    //posortujmy
    vector<int> pom;
    for (int i = 0; i < 10; i++)
        pom.push_back((i * 7) % 10);
    merge_sort<int>(pom, &mniejszy_int, 0, pom.size());
    int rozmiar = punkty.size();
    merge_sort<Wektor_3d>(punkty, &mniejszy_3d_wektor, 0, rozmiar);

    Plansza_2d x_y_plane;

    Wektor_3d wielkie_pudlo;
    int i = 0;
    while (i < punkty.size())
    {
        int chwilowy_z = punkty[i].z;
        long long int pole_pod = x_y_plane.max_pole();
        if (pole_pod * ((long long int)chwilowy_z) > wielkie_pudlo.objetosc())
        {
            pair<int,int> xy=x_y_plane.max_punkt();
            wielkie_pudlo.x=xy.first;
            wielkie_pudlo.y=xy.second;
            wielkie_pudlo.z=chwilowy_z;
        }

        while (i < punkty.size() && chwilowy_z == punkty[i].z)
        {
            x_y_plane.wsadz_punkt(punkty[i]);
            i++;
        }
    }

        int chwilowy_z = x_y_plane.bok;
        long long int pole_pod = x_y_plane.max_pole();
        if (pole_pod * ((long long int)chwilowy_z) > wielkie_pudlo.objetosc())
        {
            pair<int,int> xy=x_y_plane.max_punkt();
            wielkie_pudlo.x=xy.first;
            wielkie_pudlo.y=xy.second;
            wielkie_pudlo.z=chwilowy_z;
        }

    cout <<wielkie_pudlo.x<<" "<<wielkie_pudlo.y<<" "<<wielkie_pudlo.z;
    return 0;
}
