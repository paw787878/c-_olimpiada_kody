#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

void wczytywanie(int &n, int &k, vector<int> &domy, vector<int> &przestepca_lewy,
                 vector<int> &przestepca_prawy)
{
    //ifstream we("in/prz2c.in");
    cin >> n >> k;
    int pom;
    for (int i = 0; i < n; i++)
    {
        cin >> pom;
        pom--;
        domy.push_back(pom);
    }
    int m, l;
    cin >> m >> l;
    for (int i = 0; i < m; i++)
    {
        cin >> pom;
        pom--;
        przestepca_lewy.push_back(pom);
    }
    for (int i = 0; i < l; i++)
    {
        cin >> pom;
        pom--;
        przestepca_prawy.push_back(pom);
    }
}

void uzupelnij_gdzie_zaczac(vector<int> &kolory_domow, vector<int> &gdzie_zaczac, vector<int> &slownik, vector<int> &kolejne_rabowane_kolory, vector<int> &gdzie_ostatnio_ten_podciag,
                            int startowy, int zakoncowy, int increment)
{
    //najpierw przygotujmy sobie miejsce do pracy
    for (int i = 0; i < slownik.size(); i++)
        slownik[i] = -1;
    for (int i = 0; i < kolejne_rabowane_kolory.size(); i++)
        gdzie_ostatnio_ten_podciag[i] = -1;
    //teraz ten slownik pouzupelniaj
    for (int i = 0; i < kolejne_rabowane_kolory.size(); i++)
        slownik[kolejne_rabowane_kolory[i]] = i;
    //dobra to teraz jestesmy gotowi na to
    for (int i = startowy; i != zakoncowy; i += increment)
    {
        int nr_tego_wyrazu_ciagu = slownik[kolory_domow[i]];
        if (nr_tego_wyrazu_ciagu != -1)
            if (nr_tego_wyrazu_ciagu == 0)
            {
                gdzie_ostatnio_ten_podciag[nr_tego_wyrazu_ciagu] = i;
            }
            else
            {
                gdzie_ostatnio_ten_podciag[nr_tego_wyrazu_ciagu] = gdzie_ostatnio_ten_podciag[nr_tego_wyrazu_ciagu - 1];
            }
        gdzie_zaczac[i] = gdzie_ostatnio_ten_podciag[kolejne_rabowane_kolory.size() - 1];
    }
}

class Klasa_szukajaca_par
{
  public:
    vector<int> *ilosci_z_lewej;
    vector<int> *ilosci_z_prawej;
    vector<int> *kolory_domow;
    int pythonowy_koniec_lewej_czesci;
    int pythonowy_poczatek_prawej_czesci;
    int ile_jest_par;
    Klasa_szukajaca_par(vector<int> &kolory_domow, vector<int> *pamiec1, vector<int> *pamiec2)
    {
        this->kolory_domow = &kolory_domow;
        pythonowy_koniec_lewej_czesci = 0;
        pythonowy_poczatek_prawej_czesci = 0;
        ile_jest_par = 0;
        ilosci_z_lewej = pamiec1;
        ilosci_z_prawej = pamiec2;
        for (int i = 0; i < ilosci_z_lewej->size(); i++)
        {
            (*ilosci_z_lewej)[i] = 0;
        }
        for (int i = 0; i < ilosci_z_prawej->size(); i++)
        {
            (*ilosci_z_prawej)[i] = 0;
        }
        for (int i = 0; i < kolory_domow.size(); i++)
        {
            (*ilosci_z_prawej)[kolory_domow[i]]++;
        }
    }
    void ustanow_pythonowy_koniec_lewej_czesci(int index)
    {
        while (pythonowy_koniec_lewej_czesci < index)
        {
            int i = pythonowy_koniec_lewej_czesci;
            int stara = ((*ilosci_z_lewej)[(*kolory_domow)[i]] >= 1 && (*ilosci_z_prawej)[(*kolory_domow)[i]] >= 1);
            (*ilosci_z_lewej)[(*kolory_domow)[i]]++;
            int nowa = ((*ilosci_z_lewej)[(*kolory_domow)[i]] >= 1 && (*ilosci_z_prawej)[(*kolory_domow)[i]] >= 1);
            ile_jest_par -= stara;
            ile_jest_par += nowa;
            pythonowy_koniec_lewej_czesci++;
        }
    }
    void ustanow_pythonowy_poczatek_prawej_czesci(int index)
    {
        while (pythonowy_poczatek_prawej_czesci < index)
        {
            int i = pythonowy_poczatek_prawej_czesci;
            int stara = ((*ilosci_z_lewej)[(*kolory_domow)[i]] >= 1 && (*ilosci_z_prawej)[(*kolory_domow)[i]] >= 1);
            (*ilosci_z_prawej)[(*kolory_domow)[i]]--;
            int nowa = ((*ilosci_z_lewej)[(*kolory_domow)[i]] >= 1 && (*ilosci_z_prawej)[(*kolory_domow)[i]] >= 1);
            ile_jest_par -= stara;
            ile_jest_par += nowa;
            pythonowy_poczatek_prawej_czesci++;
        }
    }
    bool czy_jest_para()
    {
        return (ile_jest_par > 0);
    }
};

void szukaj_tych_domow(vector<int> &kolory_domow, vector<int> &mozliwe_domy, Klasa_szukajaca_par &szukanie_par, vector<int> &gdzie_zaczac_od_prawej, vector<int> &gdzie_zaczac_od_lewej, int kolor_domu_spotkania)
{
    for (int i = 0; i < kolory_domow.size(); i++)
        if (kolory_domow[i] == kolor_domu_spotkania)
        {
            //najpierw doteguj
            int nowy_koniec_lewego, nowy_poczatek_prawego;
            if (gdzie_zaczac_od_lewej[i] == -1)
                nowy_koniec_lewego = 0;
            else
                nowy_koniec_lewego = gdzie_zaczac_od_lewej[i];

            if (gdzie_zaczac_od_prawej[i] == -1)
                nowy_poczatek_prawego = kolory_domow.size();
            else
                nowy_poczatek_prawego = gdzie_zaczac_od_prawej[i] + 1;
            szukanie_par.ustanow_pythonowy_koniec_lewej_czesci(nowy_koniec_lewego);
            szukanie_par.ustanow_pythonowy_poczatek_prawej_czesci(nowy_poczatek_prawego);
            if (szukanie_par.czy_jest_para())
                mozliwe_domy.push_back(i);
        }
}

int main()
{
    int n;
    int k;
    vector<int> domy;
    vector<int> przestepca_lewy;
    vector<int> przestepca_prawy;
    wczytywanie(n, k, domy, przestepca_lewy, przestepca_prawy);
    vector<int> gdzie_trzeba_zaczac_od_lewej;
    gdzie_trzeba_zaczac_od_lewej.resize(n, -1);
    vector<int> gdzie_trzeba_zaczac_od_prawej;
    gdzie_trzeba_zaczac_od_prawej.resize(n, -1);
    vector<int> pamiec_rozmiaru_k;
    pamiec_rozmiaru_k.resize(k);
    vector<int> pamiec_rozmiaru_max_m_l;
    pamiec_rozmiaru_max_m_l.resize(max(przestepca_lewy.size(), przestepca_prawy.size()));
    uzupelnij_gdzie_zaczac(domy, gdzie_trzeba_zaczac_od_lewej, pamiec_rozmiaru_k, przestepca_lewy, pamiec_rozmiaru_max_m_l, 0, domy.size(), 1);
    uzupelnij_gdzie_zaczac(domy, gdzie_trzeba_zaczac_od_prawej, pamiec_rozmiaru_k, przestepca_prawy, pamiec_rozmiaru_max_m_l, domy.size() - 1, -1, -1);
    
    /*
    for (int i = 0; i < domy.size(); i++)
    {
        cout << i << "dom " << domy[i] + 1 << " zacznij od lewej " << gdzie_trzeba_zaczac_od_lewej[i] << endl;
    }
    cout << endl
         << endl
         << endl;

    for (int i = 0; i < domy.size(); i++)
    {
        cout << i << "dom " << domy[i] + 1 << " zacznij od prawej " << gdzie_trzeba_zaczac_od_prawej[i] << endl;
    }
    */
    
    pamiec_rozmiaru_max_m_l.resize(k);
    //teraz ta pamiec ma juz dobry rozmiar;
    Klasa_szukajaca_par szukanie_par(domy, &pamiec_rozmiaru_k, &pamiec_rozmiaru_max_m_l);
    vector<int> mozliwe_domy;
    szukaj_tych_domow(domy, mozliwe_domy, szukanie_par, gdzie_trzeba_zaczac_od_prawej, gdzie_trzeba_zaczac_od_lewej, przestepca_lewy[przestepca_lewy.size() - 1]);

    cout << mozliwe_domy.size();
    if (mozliwe_domy.size() > 0)
    {
        cout << endl;
        for (int i = 0; i < mozliwe_domy.size(); i++)
        {
            cout << mozliwe_domy[i] + 1 ;
            if(i!=mozliwe_domy.size()-1)
                cout<<" ";
        }
    }

    //cout << endl
         //<< "doszlo do return 0" << endl;
    return 0;
}
