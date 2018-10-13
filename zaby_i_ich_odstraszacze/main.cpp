#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

namespace Red_black_tree
{

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
    void wyrzuc_wszystko_do_wektora(vector<pair<T, S>> &wektor, Node<T, S> *start);
    void wyrzuc_wszystko_do_wektora(vector<pair<T, S>> &wektor);
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
void RBTree<T, S>::wyrzuc_wszystko_do_wektora(vector<pair<T, S>> &wektor, Node<T, S> *start)
{
    if (start != sent)
    {
        Node<T, S> *lewy, *prawy;
        lewy = start->left;
        prawy = start->right;
        wyrzuc_wszystko_do_wektora(wektor, lewy);
        wektor.push_back(pair<T, S>(start->value, start->satelite));
        delete start;
        wyrzuc_wszystko_do_wektora(wektor, prawy);
    }
    root = sent;
}

template <class T, class S>
void RBTree<T, S>::wyrzuc_wszystko_do_wektora(vector<pair<T, S>> &wektor)
{
    wyrzuc_wszystko_do_wektora(wektor, root);
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
        //cout << "nie znaleziono";
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

} // namespace Red_black_tree

namespace Priority_heap
{

template <class T, class S>
class Node
{
  public:
    T war;
    S sat;
    int i;
    Node(T war, S sat, int i)
    {
        this->sat = sat;
        this->i = i;
        this->war = war;
    }
};

template <class T, class S>
class Heap
{
  public:
    vector<Node<T, S> *> tab;
    int w_srodku;
    int ile_w_srodku()
    {
        return w_srodku;
    }
    bool (*mniejszy)(T, T);
    Heap(bool (*mniejszy)(T, T), T dummy_war, S dummy_sat)
    {
        this->mniejszy = mniejszy;
        tab.push_back(new Node<T, S>(dummy_war, dummy_sat, 0));
        w_srodku = 0;
        //bo zerowy element ni ejest istotny
    }
    bool empty()
    {
        return tab.size() == 0 || tab.size() == 1;
    }

    T min()
    {
        return tab[1]->war;
    }
    pair<T, S> pop_min();
    void zamien(int a, int b)
    {
        Node<T, S> *pom = tab[a];
        tab[a] = tab[b];
        tab[b] = pom;
        tab[a]->i = a;
        tab[b]->i = b;
    }
    Node<T, S> *wsadz(T elem, S sat);
    void minimalize(Node<T, S> *co, T ile_teraz_wynosi);
    void heap_up(int start);
    void wypisz();
};

template <class T, class S>
void Heap<T, S>::wypisz()
{
    cout << endl
         << "wypisuje heap prawdziwy" << endl;

    for (int i = 1; i < tab.size(); i++)
    {
        cout << "index " << i << "(" << tab[i]->i << ") wart " << tab[i]->war << " sat+1 " << tab[i]->sat + 1 << endl;
    }
}

template <class T, class S>
void Heap<T, S>::minimalize(Node<T, S> *co, T ile_teraz_wynosi)
{
    if (mniejszy(ile_teraz_wynosi, co->war))
    {
        int start = co->i;
        co->war = ile_teraz_wynosi;
        heap_up(start);
    }
}

template <class T, class S>
pair<T, S> Heap<T, S>::pop_min()
{
    w_srodku--;
    pair<T, S> wyrzut;
    wyrzut.first = tab[1]->war;
    wyrzut.second = tab[1]->sat;
    zamien(1, tab.size() - 1);
    delete (tab[tab.size() - 1]);
    tab.resize(tab.size() - 1);
    //teraz trzeba spuscic ziomeczka w dol;
    int chwilowy = 1;
    while (true)
    {
        //vector<T *> synowie;
        vector<int> jaki_k;
        for (int k = 0; k < 2; k++)
            if (2 * chwilowy + k < tab.size())
            {
                //synowie.push_back(&(tab[2 * chwilowy + k]->war));
                jaki_k.push_back(k);
            }
        if (jaki_k.size() == 0)
            break;
        int index_min_k = 0;
        for (int i = 0; i < jaki_k.size(); i++)
            if (mniejszy(tab[2 * chwilowy + jaki_k[i]]->war, tab[2 * chwilowy + jaki_k[index_min_k]]->war))
                index_min_k = i;
        //teraz mamy juz tego mniejszego.
        if (mniejszy(tab[2 * chwilowy + jaki_k[index_min_k]]->war, tab[chwilowy]->war))
        {
            zamien(chwilowy, 2 * chwilowy + jaki_k[index_min_k]);
            chwilowy = 2 * chwilowy + jaki_k[index_min_k];
        }
        else
            break;
    }

    return wyrzut;
}

template <class T, class S>
void Heap<T, S>::heap_up(int start)
{
    int chwilowy = start;
    while (chwilowy != 1)
    {
        int ten = chwilowy;
        int ojc = chwilowy / 2;
        if (mniejszy(tab[ten]->war, tab[ojc]->war))
        {
            zamien(ten, ojc);
            chwilowy = ojc;
        }
        else
        {
            break;
        }
    }
}

template <class T, class S>
Node<T, S> *Heap<T, S>::wsadz(T elem, S sat)
{
    w_srodku++;
    Node<T, S> *wyrzut = new Node<T, S>(elem, sat, tab.size());
    tab.push_back(wyrzut);
    //teraz trzebawsadzic
    int chwilowy = tab.size() - 1;
    heap_up(chwilowy);
    return wyrzut;
}
} // namespace Priority_heap

//teraz zaczynam kodzic to co jest wtym zadaniu

bool mniejszy(int a, int b)
{
    return a < b;
}

int kwadrat(int a)
{
    return a * a;
}

int l2_odleglosc(pair<int, int> a, pair<int, int> b)
{
    return kwadrat(a.first - b.first) + kwadrat(a.second - b.second);
}

void rob_sasiadow(pair<int, int> ja, vector<pair<int, int>> &sasiedzi, int roz_x, int roz_y);

class Licz_odleglosci
{
  public:
    int roz_x, roz_y;
    vector<vector<Red_black_tree::RBTree<int, bool>>> jaki_pozar;
    Priority_heap::Heap<int, pair<int, int>> heap_drzew;
    vector<vector<Priority_heap::Node<int, pair<int, int>> *>> wskaznik_do_heap;
    //vector<vector<int>> odleglosci; //nalezy go wyrzucic z praogramu
    vector<pair<int, int>> odstraszacze;
    Licz_odleglosci(int a, int b); //a,b to rozmiary planszy
    void dodaj_odstraszacz(int x, int y);
    void rob_petle(vector<pair<int, pair<int, int>>> &punkty_w_kolejnosci);
};

bool czy_dobry(pair<int, int> p, int roz_x, int roz_y)
{
    return p.first < roz_x && p.first >= 0 && p.second < roz_y && p.second >= 0;
}

void rob_sasiadow(pair<int, int> ja, vector<pair<int, int>> &sasiedzi, int roz_x, int roz_y)
{
    vector<pair<int, int>> moze_sasiedzi;
    moze_sasiedzi.push_back(pair<int, int>(ja.first + 1, ja.second));
    moze_sasiedzi.push_back(pair<int, int>(ja.first - 1, ja.second));
    moze_sasiedzi.push_back(pair<int, int>(ja.first, ja.second + 1));
    moze_sasiedzi.push_back(pair<int, int>(ja.first, ja.second - 1));
    for (int i = 0; i < 4; i++)
        if (czy_dobry(moze_sasiedzi[i], roz_x, roz_y))
            sasiedzi.push_back(moze_sasiedzi[i]);
}

void Licz_odleglosci::rob_petle(vector<pair<int, pair<int, int>>> &punkty_w_kolejnosci)
{
    while (!heap_drzew.empty())
    {
        pair<int, pair<int, int>> zdjety = heap_drzew.pop_min();
        punkty_w_kolejnosci.push_back(zdjety);
        pair<int, int> wsporzedne_zdjetego = zdjety.second;
        //odleglosci[wsporzedne_zdjetego.first][wsporzedne_zdjetego.second] = zdjety.first;
        wskaznik_do_heap[wsporzedne_zdjetego.first][wsporzedne_zdjetego.second] = 0;
        //teraz trzeba to porobic dla wszystkich sasiadow
        vector<pair<int, int>> sasiedzi;
        rob_sasiadow(wsporzedne_zdjetego, sasiedzi, roz_x, roz_y);
        vector<pair<int, bool>> jakie_tu_byly_pozary;
        jaki_pozar[wsporzedne_zdjetego.first][wsporzedne_zdjetego.second].wyrzuc_wszystko_do_wektora(jakie_tu_byly_pozary);
        for (int i = 0; i < jakie_tu_byly_pozary.size(); i++)
        {
            for (int j = 0; j < sasiedzi.size(); j++)
            {
                //trzeba dodac ten pozar do tamtego
                pair<int, int> som = sasiedzi[j];
                int nr_wkladanego_pozaru = jakie_tu_byly_pozary[i].first;
                if (wskaznik_do_heap[som.first][som.second] != 0 && jaki_pozar[som.first][som.second].odnajdz(nr_wkladanego_pozaru) == jaki_pozar[som.first][som.second].sent)
                {
                    //nie bylo tu tego pozaru i teraz przejmuje go
                    jaki_pozar[som.first][som.second].wsadz(nr_wkladanego_pozaru, true);
                    int rozwazana_odleglosc = l2_odleglosc(odstraszacze[nr_wkladanego_pozaru], som);
                    //cout<<"rozwazana odleglosc to "<<rozwazana_odleglosc<<endl;
                    heap_drzew.minimalize(wskaznik_do_heap[som.first][som.second], rozwazana_odleglosc);
                }
            }
        }
    }
}

void Licz_odleglosci::dodaj_odstraszacz(int x, int y)
{
    int nr_dodawanego_odstraszacza = odstraszacze.size();
    odstraszacze.push_back(pair<int, int>(x, y));
    jaki_pozar[x][y].wsadz(nr_dodawanego_odstraszacza, true);
    heap_drzew.minimalize(wskaznik_do_heap[x][y], 0);
}

Licz_odleglosci::Licz_odleglosci(int a, int b) : heap_drzew(&mniejszy, -1, pair<int, int>(-1, -1))
{
    roz_x = a;
    roz_y = b;
    jaki_pozar.resize(roz_x);
    //odleglosci.reserve(roz_x);
    for (int i = 0; i < roz_x; i++)
    {
        jaki_pozar[i].resize(roz_y, Red_black_tree::RBTree<int, bool>(&mniejszy));
        //odleglosci[i].resize(roz_y, 0);
    }
    //teraz trzeba dodac wszystkie elementy do priority que
    int nies = 1000000000;
    wskaznik_do_heap.resize(roz_x);
    for (int i = 0; i < roz_x; i++)
        for (int j = 0; j < roz_y; j++)
        {
            wskaznik_do_heap[i].push_back(heap_drzew.wsadz(nies, pair<int, int>(i, j)));
        }
}

void wypisz_ladnie_int(int a)
{
    if (a < 10)
        cout << " ";
    cout << a;
}

//teraz trzeba zrobic ta klase smieszna bo tak;
class Klasy_rownowaznosci
{
  public:
    vector<int> ojcowie; //-1 to znaczy ze nie ma ojca
    vector<int> ile_pod; //liczac jego, dobre wartosci tylko dla ziomeczkow ktore sa rootami
    Klasy_rownowaznosci(int n)
    {
        ojcowie.resize(n, -1);
        ile_pod.resize(n, 1);
    }
    void przyrownaj(int a, int b); // a oraz b od 0 do n-1
    bool czy_rowny(int a, int b);
    int znajdz_korzen(int a);
};

bool Klasy_rownowaznosci::czy_rowny(int a, int b)
{
    return znajdz_korzen(a) == znajdz_korzen(b);
}

int Klasy_rownowaznosci::znajdz_korzen(int a)
{
    int chwilowy = a;
    while (ojcowie[chwilowy] != -1)
        chwilowy = ojcowie[chwilowy];
    return chwilowy;
}

void Klasy_rownowaznosci::przyrownaj(int a, int b)
{
    int korz_a = znajdz_korzen(a);
    int korz_b = znajdz_korzen(b);
    if (korz_a != korz_b)
        if (ile_pod[korz_a] > ile_pod[korz_b])
        {
            ojcowie[korz_b] = korz_a;
            ile_pod[korz_a] += ile_pod[korz_b];
        }
        else
        {
            ojcowie[korz_a] = korz_b;
            ile_pod[korz_b] += ile_pod[korz_a];
        }
}

int para_do_liczba(pair<int, int> para, int roz_x, int roz_y)
{
    return para.first + roz_x * para.second;
}

int minimalna_odleglosc(vector<pair<int, pair<int, int>>> &punkty, int roz_x, int roz_y, pair<int, int> pocz, pair<int, int> konc)
{
    Klasy_rownowaznosci perkolacja(roz_x * roz_y);
    vector<bool> czy_rozwalony;
    czy_rozwalony.resize(roz_x * roz_y, false);
    for (int i = punkty.size() - 1; i >= 0; i--)
    {
        int chwilowy_min = punkty[i].first;
        pair<int, int> usuwany_punkt = punkty[i].second;
        czy_rozwalony[para_do_liczba(usuwany_punkt, roz_x, roz_y)] = true;
        vector<pair<int, int>> sasiedzi;
        rob_sasiadow(usuwany_punkt, sasiedzi, roz_x, roz_y);
        for (int j = 0; j < sasiedzi.size(); j++)
            if (czy_rozwalony[para_do_liczba(sasiedzi[j], roz_x, roz_y)])
            {
                perkolacja.przyrownaj(para_do_liczba(sasiedzi[j], roz_x, roz_y), para_do_liczba(usuwany_punkt, roz_x, roz_y));
            }
        if (perkolacja.czy_rowny(para_do_liczba(pocz, roz_x, roz_y), para_do_liczba(konc, roz_x, roz_y)))
            return chwilowy_min;
    }
    return 0;
}

int main()
{
    //teraz trzeba
    //ifstream we("in/zab0.in");
    int roz_x, roz_y;
    cin >> roz_x >> roz_y;
    int pocz_x, pocz_y, konc_x, konc_y;
    cin >> pocz_x >> pocz_y >> konc_x >> konc_y;
    pocz_x--;
    pocz_y--;
    konc_x--;
    konc_y--;
    pair<int, int> pocz(pocz_x, pocz_y);
    pair<int, int> konc(konc_x, konc_y);
    int n;
    cin >> n;
    Licz_odleglosci liczenie_odleglosci(roz_x, roz_y);
    for (int i = 0; i < n; i++)
    {
        int a, b;
        cin >> a >> b;
        a--;
        b--;
        liczenie_odleglosci.dodaj_odstraszacz(a, b);
    }
    vector<pair<int, pair<int, int>>> punkty_w_kolejnosci;
    liczenie_odleglosci.rob_petle(punkty_w_kolejnosci);
    cout << minimalna_odleglosc(punkty_w_kolejnosci, roz_x, roz_y, pocz, konc);
    //teraz trzeba sprawdzić co nam wyszlo
    /*
    cout << endl
         << "tablica odleglosci" << endl;
    for (int j = roz_y - 1; j >= 0; j--)
    {
        for (int i = 0; i < roz_x; i++)
        {
            wypisz_ladnie_int(liczenie_odleglosci.odleglosci[i][j]);
            cout << " ";
        }
        cout << endl;
    }
    */

    //cout << endl
         //<< "doszlo do return 0" << endl;
    return 0;
}
