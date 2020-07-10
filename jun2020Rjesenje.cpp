#include<iostream>
#include<vector>
#include<string>
#include<exception>
#include<regex>
using namespace std;

/*const char* PORUKA = "\n-------------------------------------------------------------------------------\n"
"1. SVE KLASE TREBAJU POSJEDOVATI ADEKVATAN DESTRUKTOR\n"
"2. NAMJERNO IZOSTAVLJANJE KOMPLETNIH I/ILI POJEDINIH DIJELOVA DESTRUKTORA CE BITI OZNACENO KAO RE\n"
"3. SPASAVAJTE PROJEKAT KAKO BI SE SPRIJECILO GUBLJENJE URADJENOG ZADATKA\n"
"4. NAZIVI FUNKCIJA, TE BROJ I TIP PARAMETARA MORAJU BITI IDENTICNI ONIMA KOJI SU KORISTENI U TESTNOM CODE-U,\n"
"\tOSIM U SLUCAJU DA POSTOJI ADEKVATAN RAZLOG ZA NJIHOVU MODIFIKACIJU. OSTALE\n"
"\tPOMOCNE FUNKCIJE MOZETE IMENOVATI I DODAVATI PO ZELJI.\n"
"5. IZUZETAK BACITE SAMO U FUNKCIJAMA U KOJIMA JE TO NAZNACENO.\n"
"6. FUNKCIJE KOJE NE IMPLEMENTIRATE TREBAJU BITI OBRISANE!\n"
"7. RJESENJA ZADATKA POSTAVITE NA FTP SERVER U ODGOVARAJUCI FOLDER!\n"
"-------------------------------------------------------------------------------\n";
*/
const char* crt = "\n-------------------------------------------\n";
enum eRazred { PRVI = 1, DRUGI, TRECI, CETVRTI };

char* GetNizKaraktera(const char* sadrzaj, bool dealociraj = false) {
    if (sadrzaj == nullptr)return nullptr;
    int vel = strlen(sadrzaj) + 1;
    char* temp = new char[vel];
    strcpy_s(temp, vel, sadrzaj);
    if (dealociraj)
        delete[]sadrzaj;
    return temp;
}
template<class T1, class T2>
class Kolekcija {
    T1* _elementi1;
    T2* _elementi2;
    int _trenutno;
    bool _omoguciDupliranje;
public:
    Kolekcija(bool omoguciDupliranje = true) {
        _elementi1 = nullptr;
        _elementi2 = nullptr;
        _omoguciDupliranje = omoguciDupliranje;
        _trenutno = 0;
    }
    Kolekcija(const Kolekcija& k) {
        _trenutno = k._trenutno;
        _omoguciDupliranje = k._omoguciDupliranje;
        _elementi1 = new T1[k._trenutno];
        _elementi2 = new T2[k._trenutno];
        for (size_t i = 0; i < k._trenutno; i++)
        {
            _elementi1[i] = k._elementi1[i];
            _elementi2[i] = k._elementi2[i];
        }
    }
    Kolekcija& operator=(const Kolekcija& k) {
        if (this != &k) {
            _trenutno = k._trenutno;
            _omoguciDupliranje = k._omoguciDupliranje;
            delete[] _elementi1;
            delete[] _elementi2;
            _elementi1 = new T1[k._trenutno];
            _elementi2 = new T2[k._trenutno];
            for (size_t i = 0; i < k._trenutno; i++)
            {
                _elementi1[i] = k._elementi1[i];
                _elementi2[i] = k._elementi2[i];
            }
        }
     return *this;
    }
    ~Kolekcija() {
        delete[]_elementi1; _elementi1 = nullptr;
        delete[]_elementi2; _elementi2 = nullptr;
    }
    T1& getElement1(int lokacija)const { return _elementi1[lokacija]; }
    T2& getElement2(int lokacija)const { return _elementi2[lokacija]; }
    int getTrenutno()const { return _trenutno; }
    friend ostream& operator<< (ostream& COUT, const Kolekcija& obj) {
        for (size_t i = 0; i < obj._trenutno; i++)
            COUT << obj.getElement1(i) << " " << obj.getElement2(i) << endl;
        return COUT;
    }
    void AddElement(T1 el1, T2 el2) {
        if (!_omoguciDupliranje) {
            for (size_t i = 0; i < _trenutno; i++)
            {
                if(_elementi1[i] == el1 && _elementi2[i] == el2) {
                throw exception("Nije dozvoljeno duliranje elemenata."); return;
            }
            }
        }
        T1* temp1 = new T1[_trenutno + 1];
        T2* temp2 = new T2[_trenutno + 1];
        for (size_t i = 0; i < _trenutno; i++)
        {
            temp1[i] = _elementi1[i];
            temp2[i] = _elementi2[i];
        }
        delete[] _elementi1;
        delete[] _elementi2;
        temp1[_trenutno] = el1;
        temp2[_trenutno++] = el2;
        _elementi1 = temp1; temp1 = nullptr;
        _elementi2 = temp2; temp2 = nullptr;
    }
    Kolekcija<T1, T2> operator()(int i, int j) {
        if (i < 0 || j < 0) throw exception("Opseg nije validanNNN."); 
        if (i > j) throw exception("Opseg nije validan.");
        if(i>=_trenutno || j>= _trenutno) throw exception("Opseg nije validan.");
        Kolekcija<T1, T2> temp;
        for (size_t k = i; k <= j; k++)
        {
            temp.AddElement(_elementi1[k], _elementi2[k]);
        }
        return temp;
    }
    bool operator==(const Kolekcija<T1, T2>& k) {
        int brojac = 0;
        for (size_t i = 0; i < _trenutno; i++)
        {
            if (_elementi1[i] == k._elementi1[i] && _elementi2[i] == k._elementi2[i])
                brojac++;
        }
        if (brojac == _trenutno)return true;
        else
            return false;
    }
};


class Datum {
    int* _dan, * _mjesec, * _godina;
public:
    Datum(int dan = 1, int mjesec = 1, int godina = 2000) {
        _dan = new int(dan);
        _mjesec = new int(mjesec);
        _godina = new int(godina);
    }
    Datum(const Datum &d) {
        _dan = new int(*d._dan);
        _mjesec = new int(*d._mjesec);
        _godina = new int(*d._godina);
    }
    Datum& operator=(const Datum& d) {
        if (this != &d) {
            *_dan = *d._dan;
            *_mjesec = *d._mjesec;
            *_godina = *d._godina;
        }
        return *this;
    }
    ~Datum() {
        delete _dan; _dan = nullptr;
        delete _mjesec; _mjesec = nullptr;
        delete _godina; _godina = nullptr;
    }
    friend ostream& operator<< (ostream& COUT, const Datum& obj) {
        COUT << *obj._dan << "." << *obj._mjesec << "." << *obj._godina;
        return COUT;
    }
    bool operator==(const Datum& d) {
        return *_dan == *d._dan && *_mjesec == *d._mjesec && *_godina == *d._godina;
    }
    bool operator<(const Datum& d) {
        return *_dan < *d._dan && *_mjesec < *d._mjesec && *_godina < *d._godina;
    }
    bool operator>(const Datum& d) {
        return *_dan > *d._dan && *_mjesec > *d._mjesec && *_godina > *d._godina;
    }
};

class Predmet {
    char* _naziv;
    //int se odnosi na ocjenu u opsegu od 1 – 5, a datum na momenat postizanja ocjene
    Kolekcija<int, Datum> _ocjene;
public:
    Predmet(const char* naziv = "", int ocjena = 0, Datum datum = Datum()) {
        _naziv = GetNizKaraktera(naziv);
        if (ocjena > 0)
            _ocjene.AddElement(ocjena, datum);
    }
    Predmet(const Predmet& p) {
        _naziv = GetNizKaraktera(p._naziv);
        _ocjene = p._ocjene;
    }
    Predmet& operator=(const Predmet& p) {
        if (this != &p) {
            delete[] _naziv;
            _naziv = GetNizKaraktera(p._naziv);
            _ocjene = p._ocjene;
        } return *this;
    }
    ~Predmet() {
        delete[] _naziv; _naziv = nullptr;
    }
    void AddOcjena(int ocjena, Datum datum) {
        _ocjene.AddElement(ocjena, datum);
    }
    char* GetNaziv() { return _naziv; }
    Kolekcija<int, Datum>& GetOcjene() { return _ocjene; }
    friend ostream& operator<< (ostream& COUT, const Predmet& obj) {
        COUT << obj._naziv << " " << obj._ocjene << endl;
        float prosjek = 0;
        if (obj._ocjene.getTrenutno() == 0)
            COUT << "Prosjek je nula" << endl;
        for (size_t i = 0; i < obj._ocjene.getTrenutno(); i++)
        {
            prosjek += obj._ocjene.getElement1(i);
        }
        prosjek /= obj._ocjene.getTrenutno();

        COUT << "Prosjek je: " << prosjek << endl;
        return COUT;
    }
    bool operator==(const Predmet& p) {
        if ((strcmp(_naziv, p._naziv) == 0) && _ocjene == p._ocjene) return true; else return false;
    }
};
class Uspjeh {
    eRazred _razred;
    //string se odnosi na napomenu o polozenom predmetu
    Kolekcija<Predmet, string>* _polozeniPredmeti;
public:
    Uspjeh(eRazred razred = PRVI) {
        _razred = razred;
        _polozeniPredmeti = new  Kolekcija<Predmet, string>;
    }
    Uspjeh(const Uspjeh& u):Uspjeh(u._razred) {
        _polozeniPredmeti = new Kolekcija<Predmet, string>(*u._polozeniPredmeti);
    }
    ~Uspjeh() { delete _polozeniPredmeti; _polozeniPredmeti = nullptr; }
    void DodajPredmet(Predmet p, string s) {
        _polozeniPredmeti->AddElement(p, s);
    }
    Kolekcija<Predmet, string>* GetPredmeti() { return _polozeniPredmeti; }
    eRazred GetERazred() { return _razred; }
    friend ostream& operator<< (ostream& COUT, const Uspjeh& obj) {
        COUT << obj._razred << " " << *obj._polozeniPredmeti << endl;
        return COUT;
    }
};
bool ValidirajEmail(string mail) {
    if (regex_match(mail, regex("([a-zA-Z//.]{1,})[@](edu.fit|fit)[.](ba|com|org)")))return true; return false;
}
const char* not_set = "not_set@edu.fit.ba";
class Kandidat {
    char* _imePrezime;
    string _emailAdresa;
    string _brojTelefona;
    vector<Uspjeh> _uspjeh;
public:
    Kandidat(const char* imePrezime, string emailAdresa, string brojTelefona) {
        _imePrezime = GetNizKaraktera(imePrezime);
        if (ValidirajEmail(emailAdresa)) _emailAdresa = emailAdresa;
        else {
            _emailAdresa = not_set;
        }
        
        _brojTelefona = brojTelefona;
    }
    Kandidat(const Kandidat& k) {
        _imePrezime = GetNizKaraktera(k._imePrezime);
        _emailAdresa = k._emailAdresa;
        _brojTelefona = k._brojTelefona;
        _uspjeh = k._uspjeh;
    }
    ~Kandidat() {
        delete[] _imePrezime; _imePrezime = nullptr;
    }
    friend ostream& operator<< (ostream& COUT, Kandidat& obj) {
        COUT << obj._imePrezime << " " << obj._emailAdresa << " " << obj._brojTelefona << endl;
        for (size_t i = 0; i < obj._uspjeh.size(); i++)
            COUT << obj._uspjeh[i];
        return COUT;
    }
    vector<Uspjeh>& GetUspjeh() { return _uspjeh; }
    string GetEmail() { return _emailAdresa; }
    string GetBrojTelefona() { return _brojTelefona; }
    char* GetImePrezime() { return _imePrezime; }
    friend ostream& operator<< (ostream& COUT, const Kandidat& obj) {
        COUT << obj._imePrezime << " " << obj._emailAdresa << " " << obj._brojTelefona << endl;
        return COUT;
    }
    bool AddPredmet(eRazred r, Predmet p, string napomena) {
        for (vector<Uspjeh>::iterator i = _uspjeh.begin(); i != _uspjeh.end(); i++)
        {
            if (i->GetERazred() == r) {
                for (size_t j = 0; j < i->GetPredmeti()->getTrenutno(); j++)
                {
                    if (i->GetPredmeti()->getElement1(j) == p) return false;
                    if (i->GetPredmeti()->getTrenutno() > 5)return false;
                    float prosjek = 0;
                    for (size_t k = 0; k < i->GetPredmeti()->getElement1(j).GetOcjene().getTrenutno(); k++)
                    {
                        prosjek += i->GetPredmeti()->getElement1(j).GetOcjene().getElement1(k);
                    }
                    prosjek /= i->GetPredmeti()->getElement1(j).GetOcjene().getTrenutno();
                    if (prosjek < 2.5) return false;
                }
            }
        }
        Uspjeh u(r);
        u.GetPredmeti()->AddElement(p, napomena);
        _uspjeh.push_back(u);
        return true;
    }
    // zadnja moze i ovako
    /* Uspjeh* operator[](eRazred r) {
        for (size_t i = 0; i < _uspjeh.size(); i++) {
            if (_uspjeh[i].GetERazred() == r) {
                return &_uspjeh[i];
            }
        }
        return nullptr;
    } */
    /*
   uspjeh (tokom srednjoskolskog obrazovanja) se dodaje za svaki predmet na nivou razreda.
   tom prilikom onemoguciti:
   - dodavanje istih (moraju biti identicne vrijednosti svih clanova) predmeta na nivou jednog razreda,
   - dodavanje predmeta kod kojih je prosjecna ocjena manja od 2.5
   - dodavanje vise od 5 predmeta na nivou jednog razreda
   razredi (predmeti ili uspjeh) ne moraju biti dodavani sortiranim redoslijedom (npr. prvo se moze dodati uspjeh za II razred, pa onda za I razred i sl.).
   Funkcija vraca true ili false u zavisnosti od (ne)uspjesnost izvrsenja
   */
   //vraca kolekciju predmeta koji sadrze najmanje jednu ocjenu evidentiranu u periodu izmedju proslijedjenih datuma
   //float se odnosi na prosjecan broj dana izmedju ostvarenih ocjena na predmetu
    Kolekcija<Predmet, float> operator()(Datum d1, Datum d2) {
        Kolekcija<Predmet, float> temp;
        for (vector<Uspjeh>::iterator i = _uspjeh.begin(); i != _uspjeh.end(); i++)
        {
            for (size_t j = 0; j < i->GetPredmeti()->getTrenutno(); j++)
            {
                for (size_t k = 0; k < i->GetPredmeti()->getElement1(j).GetOcjene().getTrenutno(); k++)
                {
                    if ((i->GetPredmeti()->getElement1(j).GetOcjene().getElement2(k) > d1 || i->GetPredmeti()->getElement1(j).GetOcjene().getElement2(k) < d2) && i->GetPredmeti()->getElement1(j).GetOcjene().getTrenutno() > 0)
                        temp.AddElement(i->GetPredmeti()->getElement1(j), 5);
                }
            }
        }
        return temp;
    }
    Uspjeh* operator[](eRazred r){
        Uspjeh* temp = new Uspjeh(r);
        for (vector<Uspjeh>::iterator i = _uspjeh.begin(); i != _uspjeh.end(); i++) {
            if (i->GetERazred() == r) {
                for (size_t j = 0; j < i->GetPredmeti()->getTrenutno(); j++)
                {
                    temp->DodajPredmet(i->GetPredmeti()->getElement1(j), i->GetPredmeti()->getElement2(j));
                }
           }
        }

        return temp;
    }
};
const char* GetOdgovorNaPrvoPitanje() {
    cout << "Pitanje -> Na sta se odnosi pojam reprezentacije tipa?\n";
    return "Odgovor -> OVDJE UNESITE VAS ODGOVOR";
}
const char* GetOdgovorNaDrugoPitanje() {
    cout << "Pitanje -> Na koji nacin se moze izbjeci pojavljivanje vise podobjekata bazne klase u slucaju visestrukog nasljedjivanja?\n";
    return "Odgovor -> OVDJE UNESITE VAS ODGOVOR";
}

void main() {

    //cout << PORUKA;
    cin.get();

    cout << GetOdgovorNaPrvoPitanje() << endl;
    cin.get();
    cout << GetOdgovorNaDrugoPitanje() << endl;
    cin.get();

    Datum temp,
        datum19062019(19, 6, 2019),
        datum20062019(20, 6, 2019),
        datum30062019(30, 6, 2019),
        datum05072019(5, 7, 2019);

    int kolekcijaTestSize = 9;
    Kolekcija<int, int> kolekcija1(false);
    for (int i = 0; i <= kolekcijaTestSize; i++)
        kolekcija1.AddElement(i, i);

    try {
        //ukoliko nije dozvoljeno dupliranje elemenata (provjeravaju se T1 i T2), metoda AddElement baca izuzetak
        kolekcija1.AddElement(3, 3);
    }
    catch (exception & err) {
        cout << err.what() << crt;
    }
    cout << kolekcija1 << crt;

    /*objekat kolekcija2 ce biti inicijalizovan elementima koji se u objektu kolekcija1 nalaze na lokacijama 1 - 4
    ukljucujuci i te lokacije. u konkretnom primjeru to ce biti parovi sa vrijednostima: 1 1 2 2 3 3 4 4*/
    Kolekcija<int, int> kolekcija2 = kolekcija1(1, 4);
    cout << kolekcija2 << crt;
    try {
        //primjeri u kojima opseg nije validan, te bi funkcija trebala baciti izuzetak
      //  Kolekcija<int, int> temp1 = kolekcija1(1, 14);//imamo 10 elemenata
        Kolekcija<int, int> temp2 = kolekcija1(-1, 8);//lokacija -1 ne postoji
    }
    catch (exception & err) {
        cout << err.what() << crt;
    }

    //parametri: nazivPredmeta, prva ocjena, datum
    Predmet Matematika("Matematika", 5, datum19062019),
        Fizika("Fizika", 5, datum20062019),
        Hemija("Hemija", 2, datum30062019),
        Engleski("Engleski", 5, datum05072019);

    Matematika.AddOcjena(3, datum05072019);
    Matematika.AddOcjena(5, datum05072019);

    // ispisuje: naziv predmeta, ocjene (zajedno sa datumom polaganja) i prosjecnu ocjenu na predmetu
    // ukoliko predmet nema niti jednu ocjenu prosjecna treba biti 0
    cout << Matematika << endl;

    if (ValidirajEmail("text.text@edu.fit.ba"))
        cout << "Email validan" << crt;
    if (ValidirajEmail("texttext@edu.fit.ba"))
        cout << "Email validan" << crt;
    if (ValidirajEmail("texttext@fit.ba"))
        cout << "Email validan" << crt;
    if (ValidirajEmail("texttext@fit.com"))
        cout << "Email validan" << crt;
    if (ValidirajEmail("texttext@edu.fit.org"))
        cout << "Email validan" << crt;

    /*
    email adresa mora biti u formatu text.text (sa ili bez tacke),
    dok domena moze biti fit.ba ili edu.fit.ba
    nakon institucije (fit ili edu.fit), osim drzavne (.ba), dozvoljene su domene .com i .org.

    za provjeru validnosti email adrese koristiti globalnu funkciju ValidirajEmail, a unutar nje regex metode.
    validacija email adrese ce se vrsiti unutar konstruktora klase Kandidat, a u slucaju da nije validna
    postaviti je na defaultnu adresu: notSet@edu.fit.ba
    */

   Kandidat jasmin("Jasmin Azemovic", "jasmin@fit.ba", "033 281 172");
    Kandidat adel("Adel Handzic", "adel@edu.fit.ba", "033 281 170");
    Kandidat emailNotValid("Ime Prezime", "korisnik@klix.ba", "033 281 170");

   
    if (jasmin.AddPredmet(DRUGI, Fizika, "Napomena 1"))
        cout << "Predmet uspjesno dodan!" << crt;
    if (jasmin.AddPredmet(DRUGI, Hemija, "Napomena 2"))
        cout << "Predmet uspjesno dodan!" << crt;
    if (jasmin.AddPredmet(PRVI, Engleski, "Napomena 3"))
        cout << "Predmet uspjesno dodan!" << crt;
    if (jasmin.AddPredmet(PRVI, Matematika, "Napomena 4"))
        cout << "Predmet uspjesno dodan!" << crt;
    //ne treba dodati Matematiku jer je vec dodana u prvom razredu
    if (jasmin.AddPredmet(PRVI, Matematika, "Napomena 5"))
        cout << "Predmet uspjesno dodan!" << crt;

    /*nakon evidentiranja uspjeha na bilo kojem predmetu kandidatu se salje email sa porukom:
    FROM:info@fit.ba
    TO: emailKorisnika
    Postovani ime i prezime, evidentirali ste uspjeh za X razred. Dosadasnji uspjeh (prosjek)
    na nivou X razreda iznosi Y, a ukupni uspjeh u toku skolovanja iznosi Z.
    Pozdrav.
    FIT Team.
    ukoliko je prosjek na nivou tog razreda veci od 4.5 kandidatu se salje SMS sa porukom: "Svaka cast za uspjeh 4.X u X razredu".
    slanje poruka i emailova implemenitrati koristeci zasebne thread-ove.
    */
    cout << jasmin << crt;

    
    Kolekcija<Predmet, float> jasminUspjeh = jasmin(Datum(18, 06, 2019), Datum(21, 06, 2019));
    cout << jasminUspjeh << crt;

    Uspjeh* uspjeh_Irazred = jasmin[PRVI];//vraca uspjeh kandidata ostvaren u prvom razredu
    if (uspjeh_Irazred != nullptr)
        cout << *uspjeh_Irazred << crt;

    cin.get();
    system("pause>0");
}
