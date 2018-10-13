#include <iostream>
#include <time.h>
#include <vector>
using namespace std;
//najpierw nasz node
template <class T>
class Node
{
  public:
    bool red;
    Node *left;
    Node *right;
    Node *father;
    T value;
    Node(){};
    Node(T wartosc, bool czy_czerwony, Node *sentinel)
    {
        red = czy_czerwony;
        left = right = father = sentinel;
        value = wartosc;
    }
    void rodzina(Node *left, Node *right, Node *father)
    {
        this->left = left;
        this->right = right;
        this->father = father;
    }
};

template <class T>
class RBTree
{
  public:
    Node<T> *sent;
    Node<T> *root;
    bool (*mniejszy)(T, T);
    RBTree(bool (*f)(T, T));
    void wsadz(T wartosc);
    void dwa_z_rzedu(Node<T> *start);
    void wypisz_dokladnie(Node<T> *start);
    Node<T> *minimum(Node<T> *start);
    Node<T> *maximum(Node<T> *start);
    Node<T> *nastepny(Node<T> *start);
    Node<T> *poprzedni(Node<T> *start);
    Node<T> *odnajdz(T wartosc);
    void rotate_right(Node<T> *zawias);
    void rotate_left(Node<T> *zawias);
    void usun(Node<T> *element);
    void przesadz(Node<T> *co, Node<T> *gdzie);
    void wsadz_fixup(Node<T> *nowy);
    int black_height(Node<T> *start);
    void usun_fixup(Node<T> *chwilowy);
};

template <class T>
void RBTree<T>::usun_fixup(Node<T> *chwilowy)
{
    typedef Node<T> *nod;
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
                    nod ch_oj=chwilowy->father;
                    rotate_right(brat);
                    brat = brat->father;
                    brat->red = false;
                    brat->right->red = true;
                    chwilowy->father=ch_oj;
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
                    nod ch_oj=chwilowy->father;
                    rotate_left(brat);
                    brat = brat->father;
                    brat->red = false;
                    brat->left->red = true;

                    chwilowy->father=ch_oj;
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
}

template <class T>
int RBTree<T>::black_height(Node<T> *start)
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

template <class T>
void RBTree<T>::wsadz_fixup(Node<T> *nowy)
{
    typedef Node<T> *nod;
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

template <class T>
void RBTree<T>::rotate_right(Node<T> *zawias)
{
    //zaklada, ze jest lewy nietrywialny syn zawiasu
    //prawy bedzie miec dobrego ojca nawet jesli jest sentem.
    //kolory sie nie zmieniaja oczywiscie.
    typedef Node<T> *nod;
    Node<T> *a = zawias;
    Node<T> *b = zawias->left;
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

template <class T>
void RBTree<T>::rotate_left(Node<T> *zawias)
{
    //zaklada, ze jest prawy nietrywialny syn zawiasu
    //lewy syn zawiasu bedzie miec dobrego ojca nawet jesli jest sentem
    //kolory sie nie zmieniaja oczywiscie

    typedef Node<T> *nod;
    Node<T> *b = zawias;
    Node<T> *a = zawias->right;
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

template <class T>
void RBTree<T>::wypisz_dokladnie(Node<T> *start)
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
        cout << start->value;
        if (start->red)
            cout << " red ";
        else
            cout << " black ";
        cout << " father " << start->father->value << " sons " << start->left->value << " " << start->right->value << endl;
        wypisz_dokladnie(start->right);
    }
}

template <class T>
void RBTree<T>::przesadz(Node<T> *co, Node<T> *gdzie)
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

template <class T>
void RBTree<T>::usun(Node<T> *element)
{
    typedef Node<T> *nod;
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
        Node<T> *wpychajacy = nastepny(element);
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

template <class T>
Node<T> *RBTree<T>::odnajdz(T wartosc)
{
    Node<T> *ojciec_to_be = root;
    while (ojciec_to_be != sent && ojciec_to_be->value != wartosc)
        if (mniejszy(wartosc, ojciec_to_be->value))
            ojciec_to_be = ojciec_to_be->left;
        else
            ojciec_to_be = ojciec_to_be->right;
    if (ojciec_to_be == sent)
    {
        cout << "nie znaleziono";
        return sent;
    }
    return ojciec_to_be;
}

template <class T>
Node<T> *RBTree<T>::poprzedni(Node<T> *start)
{
    //zwraca sentinela jak trzeba
    //zakladam, ze start to nie sent
    if (start->left != sent)
        return maximum(start->left);
    else
    {
        Node<T> *chwilowy = start;
        while (chwilowy->father != sent && chwilowy->father->left == chwilowy)
            chwilowy = chwilowy->father;
        return chwilowy->father;
    }
}

template <class T>
Node<T> *RBTree<T>::nastepny(Node<T> *start)
{
    //zwraca sentinela jak trzeba
    //zakladam, ze start to nie sent
    if (start->right != sent)
        return minimum(start->right);
    else
    {
        Node<T> *chwilowy = start;
        while (chwilowy->father != sent && chwilowy->father->right == chwilowy)
            chwilowy = chwilowy->father;
        return chwilowy->father;
    }
}

template <class T>
Node<T> *RBTree<T>::maximum(Node<T> *start)
{
    Node<T> *kandydat = start;
    while (kandydat->right != sent)
        kandydat = kandydat->right;
    return kandydat;
}

template <class T>
Node<T> *RBTree<T>::minimum(Node<T> *start)
{
    Node<T> *kandydat = start;
    while (kandydat->left != sent)
        kandydat = kandydat->left;
    return kandydat;
}

template <class T>
void RBTree<T>::dwa_z_rzedu(Node<T> *start)
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

bool intowa(int a, int b)
{
    return a < b;
}

template <class T>
void RBTree<T>::wsadz(T wartosc)
{
    Node<T> *nowy = new Node<T>(wartosc, true, sent);
    Node<T> *ojciec_to_be = sent;
    Node<T> *pod_nim = root;
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
}

template <class T>
RBTree<T>::RBTree(bool (*f)(T, T))
{
    sent = new Node<T>;
    sent->value = -77;
    sent->red = false;
    root = sent;
    mniejszy = f;
}

int main()
{
    srand(time(NULL));
    //cout<<rand()<<endl;
    RBTree<int> drzewo(&intowa);
    vector<int> wsadzone;
    
    for (int i = 0; i < 1000000; i++)
    {
        int wsadzany = rand() % 10000000;
        // cout<<endl<<"wsadzany to "<<wsadzany<<endl;
        drzewo.wsadz(wsadzany);
        wsadzone.push_back(wsadzany);
        //cout << wsadzany << " ";
        // drzewo.black_height(drzewo.root);
        //drzewo.dwa_z_rzedu(drzewo.root);
        //cout<<endl;
        
        //cout<<endl;
        //drzewo.wypisz_dokladnie(drzewo.root);
    }
    cout << endl
         << "usuwane wartosci to ";
    int ile_nie_znalezionych=0;
    for (int i = 0; i < 10000; i++)
    {
        int usuwana_wartosc = wsadzone[rand() % wsadzone.size()];
        Node<int> *odnaleziony = drzewo.odnajdz(usuwana_wartosc);
        if (odnaleziony != drzewo.sent)
        {
           // cout << " " << usuwana_wartosc << " ";
            drzewo.usun(odnaleziony);

        }
        else
            ile_nie_znalezionych++;
    }

    drzewo.black_height(drzewo.root);
    drzewo.dwa_z_rzedu(drzewo.root);
    if (drzewo.root->red)
        cout << "zly kolor roota";
    if (drzewo.sent->red)
        cout << "zly kolor senta";

    //teraz trzeba to przetestwoac usuwanie
    //cout<<endl<<"po wszystkim drzewo wyglada tak"<<endl;
    //drzewo.wypisz_dokladnie(drzewo.root);
    cout << endl<<"zakonczylo sie poprawnie nie znaleziono "<<ile_nie_znalezionych<<endl;
    
/*
    drzewo.wsadz(46);
    drzewo.wsadz(98);
    drzewo.wsadz(84);
    drzewo.wsadz(29);
    drzewo.wsadz(97);
    drzewo.wsadz(45);
    drzewo.wsadz(21);
    drzewo.wsadz(39);
    drzewo.wsadz(93);
    drzewo.wsadz(1);
    drzewo.usun(drzewo.odnajdz(45));
    cout << endl
         << "komentarze do formy" << endl;
    drzewo.black_height(drzewo.root);
    drzewo.dwa_z_rzedu(drzewo.root);
    if (drzewo.root->red)
        cout << "zly kolor roota";
    if (drzewo.sent->red)
        cout << "zly kolor senta";

    cout<<endl<<"teraz usuwam 39";
    drzewo.usun(drzewo.odnajdz(39));

       cout << endl
         << "komentarze do formy" << endl;
    drzewo.black_height(drzewo.root);
    drzewo.dwa_z_rzedu(drzewo.root);
    if (drzewo.root->red)
        cout << "zly kolor roota";
    if (drzewo.sent->red)
        cout << "zly kolor senta";

    cout<<endl<<"tuz przed usunieciem wygladalo tak"<<endl;
    drzewo.wypisz_dokladnie(drzewo.root);
    
    drzewo.usun(drzewo.odnajdz(98));



*/

    return 0;
}
