#include<iostream>
#include<string>
#include<regex>
#include<mutex>
#include<thread>
#include<vector>
using namespace std;
/*const char* PORUKA = "\n-------------------------------------------------------------------------------\n"
"0. PROVJERITE DA LI PREUZETI ZADACI PRIPADAJU VASOJ GRUPI (G1/G2)\n"
"1. SVE KLASE TREBAJU POSJEDOVATI ADEKVATAN DESTRUKTOR\n"
"2. NAMJERNO IZOSTAVLJANJE KOMPLETNIH I/ILI POJEDINIH DIJELOVA DESTRUKTORA CE BITI OZNACENO KAO TM\n"
"3. SPASAVAJTE PROJEKAT KAKO BI SE SPRIJECILO GUBLJENJE URADJENOG ZADATKA\n"
"4. NAZIVI FUNKCIJA, TE BROJ I TIP PARAMETARA MORAJU BITI IDENTICNI ONIMA KOJI SU KORISTENI U TESTNOM CODE-U,\n"
"\tOSIM U SLUCAJU DA POSTOJI ADEKVATAN RAZLOG ZA NJIHOVU MODIFIKACIJU. OSTALE\n"
"\tPOMOCNE FUNKCIJE MOZETE IMENOVATI I DODAVATI PO ZELJI.\n"
"5. IZUZETAK BACITE SAMO U FUNKCIJAMA U KOJIMA JE TO NAZNACENO.\n"
"6. FUNKCIJE KOJE NE IMPLEMENTIRATE TREBAJU BITI OBRISANE (KAKO POZIV TAKO I DEFINICIJA)!\n"
"7. NA KRAJU ISPITA SVOJE RJESENJE KOPIRATE U .DOCX FAJL (IMENOVAN BROJEM INDEKSA)!\n"
"8. RJESENJA ZADATKA POSTAVITE NA FTP SERVER U ODGOVARAJUCI FOLDER!\n"
"9. NEMOJTE POSTAVLJATI VISUAL STUDIO PROJEKTE, VEC SAMO .DOCX FAJL SA VASIM RJESENJEM!\n"
"-------------------------------------------------------------------------------\n";
*/
const char* crt = "\n-------------------------------------------\n";
enum Pojas { BIJELI, ZUTI, NARANDZASTI, ZELENI, PLAVI, SMEDJI, CRNI };
const int brojTehnika = 6;
const char* NIJE_VALIDNA = "<VRIJEDNOST_NIJE_VALIDNA>";
const char* PojasChar[] = { "BIJELI", "ZUTI", "NARANDZASTI", "ZELENI", "PLAVI", "SMEDJI", "CRNI" };

char* GetNizKaraktera(const char* sadrzaj, bool dealociraj = false) {
    if (sadrzaj == nullptr)return nullptr;
    int vel = strlen(sadrzaj) + 1;
    char* temp = new char[vel];
    strcpy_s(temp, vel, sadrzaj);
    if (dealociraj)
        delete[]sadrzaj;
    return temp;
}

template<class T1, class T2, int max = 10>
class Kolekcija {
    T1* _elementi1[max] = { nullptr };
    T2* _elementi2[max] = { nullptr };
    int  _trenutno;
public:
    Kolekcija() { _trenutno = 0; }
    ~Kolekcija() {
        for (size_t i = 0; i < _trenutno; i++) {
            delete _elementi1[i]; _elementi1[i] = nullptr;
            delete _elementi2[i]; _elementi2[i] = nullptr;
        }
    }
    Kolekcija& operator=(const Kolekcija& k) {
        if (this != &k) {
            _trenutno = k._trenutno;
            for (size_t i = 0; i < _trenutno; i++) {
                delete _elementi1[i]; _elementi1[i] = nullptr;
                delete _elementi2[i]; _elementi2[i] = nullptr;
            }
            for (size_t i = 0; i < k._trenutno; i++)
            {
                _elementi1[i] = new T1(*k._elementi1[i]);
                _elementi2[i] = new T2(*k._elementi2[i]);
            }
        }
        return *this;
    }
    Kolekcija(const Kolekcija& k) {
        _trenutno = k._trenutno;
        for (size_t i = 0; i < k._trenutno; i++)
        {
            if (k._elementi1[i] != nullptr && k._elementi2[i] != nullptr) {
                _elementi1[i] = new T1(*k._elementi1[i]);
                _elementi2[i] = new T2(*k._elementi2[i]);
            }
            else {
                _elementi1[i] = nullptr;
                _elementi2[i] = nullptr;
            }
        }
    }
    T1& getElement1(int lokacija)const { return *_elementi1[lokacija]; }
    T2& getElement2(int lokacija)const { return *_elementi2[lokacija]; }
    int getTrenutno() { return _trenutno; }
    friend ostream& operator<< (ostream& COUT, const Kolekcija& obj) {
        for (size_t i = 0; i < obj._trenutno; i++)
            COUT << obj.getElement1(i) << " " << obj.getElement2(i) << endl;
        return COUT;
    }
    void AddElement(T1 el1, T2 el2, int lokacija = -1) {
        if (max == _trenutno) {
            throw exception("Prekoracen max br el.");
        }
        if (lokacija <= -1) {
            {
                _elementi1[_trenutno] = new T1(el1);
                _elementi2[_trenutno++] = new T2(el2);
            }
        }
        else if (lokacija > -1 && lokacija < max){
            _elementi1[_trenutno] = new T1;
            _elementi2[_trenutno] = new T2;
            for (size_t i = _trenutno; i >= lokacija; i--)
            {
                *_elementi1[i] = *_elementi1[i - 1];
                *_elementi2[i] = *_elementi2[i - 1];
            }
            *_elementi1[lokacija] = el1;
            *_elementi2[lokacija] = el2;
            _trenutno++;
        }
    }
    void RemoveAt(int indeks) {
        for (int i = indeks; i < _trenutno - 1; i++)
        {
            delete _elementi1[i];
            delete _elementi2[i];
            _elementi1[i] = new T1(*_elementi1[i + 1]);
            _elementi2[i] = new T2(*_elementi2[i + 1]);
        }
        delete _elementi1[_trenutno - 1]; _elementi1[_trenutno - 1] = nullptr;
        delete _elementi2[_trenutno - 1]; _elementi2[_trenutno - 1] = nullptr;
        _trenutno--;

    }
    T2& operator[](int i) {
        for (size_t j = 0; j < _trenutno; j++)
        {
            if (*_elementi1[j] == i)
                return *_elementi2[j];
        }
    }
    bool operator==(const Kolekcija& k) {
        //if (_trenutno != k._trenutno)return false;
        ////int brojac = 0;
        //for (size_t i = 0; i < k._trenutno; i++)
        //{
        //    if (!(*_elementi1[i] == *k._elementi1[i] && *_elementi2[i] == *k._elementi2[i])) 
        //        return false;

        //        //brojac++;
        //}
        ////if (brojac == _trenutno)
        return true;
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
    ~Datum() {
        delete _dan; _dan = nullptr;
        delete _mjesec; _mjesec = nullptr;
        delete _godina; _godina = nullptr;
    }
    friend ostream& operator<< (ostream& COUT, const Datum& obj) {
        COUT << *obj._dan << "." << *obj._mjesec << "." << *obj._godina;
        return COUT;
    }
    int operator-(const Datum& d) {
        return (*_dan + *_mjesec * 30 + *_godina * 365) - (*d._dan + *d._mjesec * 30 + *d._godina * 365);
    }
    bool operator==(const Datum &d)
    {
        return *_dan == *d._dan && *_mjesec == *d._mjesec && *_godina == *d._godina;
    }
};

class Tehnika {
    char* _naziv;
    //int se odnosi na ocjenu u opsegu od 1 – 5, a Datum na datum kada je ocijenjena odredjena tehnika 
    Kolekcija<int, Datum, brojTehnika>* _ocjene;
public:
    Tehnika(const char* naziv) {
        _naziv = GetNizKaraktera(naziv);
        _ocjene = new Kolekcija<int, Datum, brojTehnika>();
    }
    Tehnika(const Tehnika& t) {
        _naziv = GetNizKaraktera(t._naziv);
        _ocjene = new Kolekcija<int, Datum, brojTehnika>(*t._ocjene);
    }
    ~Tehnika() {
        delete[] _naziv; _naziv = nullptr;
        delete _ocjene; _ocjene = nullptr;
    }
    float GetProsjek()const {
        float prosjek = 0;
        for (size_t i = 0; i < _ocjene->getTrenutno(); i++)
        {
            prosjek += _ocjene->getElement1(i);
        }
        prosjek /= _ocjene->getTrenutno();
        if (_ocjene->getTrenutno() == 0) return 0;
        return prosjek;
    }
    char* GetNaziv() { return _naziv; }
    Kolekcija<int, Datum, brojTehnika>& GetOcjene() { return *_ocjene; }
    friend ostream& operator<< (ostream& o, const Tehnika& obj) {
        o << "Naziv -> " << obj._naziv << " ";
        for (size_t i = 0; i < obj._ocjene->getTrenutno(); i++)
        {
            o << "Ocjena -> " << obj._ocjene->getElement1(i) << " Datum -> " << obj._ocjene->getElement2(i) << " ";
        }
        o << "Prosjek -> " << obj.GetProsjek() << endl;
        return o;
    }
    bool AddOcjena(int ocjena, Datum d) {
        for (size_t i = 0; i < _ocjene->getTrenutno(); i++)
        {
            if (d - _ocjene->getElement2(i) < 3)return false;
            //if (d < _ocjene->getElement2(i)) return false;
        }
        _ocjene->AddElement(ocjena, d);
        return true;
    }
    /*svaka tehnika moze imati vise ocjena tj. moze se polagati u vise navrata.
       -   razmak izmedju polaganja dvije tehnike mora biti najmanje 3 dana
       -   nije dozvoljeno dodati ocjenu sa ranijim datumom u odnosu na vec evidentirane (bez obzira sto je stariji od 3 dana)
   */
    bool operator==(const Tehnika& t) {
        return (strcmp(_naziv, t._naziv) == 0 && *_ocjene == *t._ocjene);
    }
};

class Polaganje {
    Pojas _pojas;
    vector<Tehnika*> _polozeneTehnike;
public:
    Polaganje(Pojas pojas = BIJELI) {
        _pojas = pojas;
    }
    Polaganje(const Polaganje& p) {
        _pojas = p._pojas;
        for (size_t i = 0; i < p._polozeneTehnike.size(); i++)
        {
            _polozeneTehnike.push_back(new Tehnika(*p._polozeneTehnike[i]));
        }
    }
    Polaganje& operator=(const Polaganje& p) {
        if (this != &p) {
            for (size_t i = 0; i < _polozeneTehnike.size(); i++) {
                delete _polozeneTehnike[i];
                _polozeneTehnike[i] = nullptr;
            }
            _pojas = p._pojas;
            for (size_t i = 0; i < p._polozeneTehnike.size(); i++)
            {
                _polozeneTehnike.push_back(new Tehnika(*p._polozeneTehnike[i]));
            }
        }
        return *this;
    }
    ~Polaganje() {
        for (size_t i = 0; i < _polozeneTehnike.size(); i++) {
            delete _polozeneTehnike[i];
            _polozeneTehnike[i] = nullptr;
        }
    }
    float GetProsjek()const {
        float prosjek = 0;
        int brojac = 0;
        for (size_t i = 0; i < _polozeneTehnike.size(); i++)
        {
            if (_polozeneTehnike[i]->GetProsjek() > 0) {
                prosjek += _polozeneTehnike[i]->GetProsjek();
                brojac++;
            }
        }
        prosjek /= brojac;
        if (_polozeneTehnike.size() == 0) return 0;
        return prosjek;
    }
    vector<Tehnika*>& GetTehnike() { return _polozeneTehnike; }
    Pojas GetPojas() { return _pojas; }
    friend ostream& operator<< (ostream& COUT, const Polaganje& obj) {
        COUT << obj._pojas << endl;
        for (size_t i = 0; i < obj._polozeneTehnike.size(); i++)
            COUT << *obj._polozeneTehnike[i];
        return COUT;
    }
};
bool ValidirajLozinku(string loz) {
    if (regex_search(loz, regex("(?=.{7,})(?=.*[a-z])(?=.*[A-Z])(?=.*[0-9])(?=.*\\W)"))) return true; return false;
}
class Korisnik {
    char* _imePrezime;
    string _emailAdresa;
    string _lozinka;
public:
    Korisnik(const char* imePrezime, string emailAdresa, string lozinka)
    {
        _imePrezime = GetNizKaraktera(imePrezime);
        _emailAdresa = emailAdresa;
        _lozinka = ValidirajLozinku(lozinka) ? lozinka : NIJE_VALIDNA;
    }
   virtual ~Korisnik() { delete[] _imePrezime; _imePrezime = nullptr; }
    string GetEmail() { return _emailAdresa; }
    string GetLozinka() { return _lozinka; }
    char* GetImePrezime() { return _imePrezime; }
    virtual void Info() = 0;
};
mutex m;
class KaratePolaznik:public Korisnik {
    vector<Polaganje> _polozeniPojasevi;
public:
    KaratePolaznik(const char* imePrezime, string emailAdresa, string lozinka):Korisnik(imePrezime, emailAdresa, lozinka) {
    }
   virtual ~KaratePolaznik() {
        cout << crt << "DESTRUKTOR -> KaratePolaznik" << crt;
    }
   float GetProsjek()const {
       float prosjek = 0;
       int brojac = 0;
       for (size_t i = 0; i < _polozeniPojasevi.size(); i++)
       {
           if (_polozeniPojasevi[i].GetProsjek() > 0) {
               prosjek += _polozeniPojasevi[i].GetProsjek();
               brojac++;
           }
       }
       prosjek /= brojac;
       if (_polozeniPojasevi.size() == 0) return 0;
       return prosjek;
   }
    friend ostream& operator<< (ostream& COUT, KaratePolaznik& obj) {
        COUT << obj.GetImePrezime() << " " << obj.GetEmail() << " " << obj.GetLozinka() << endl;
        for (size_t i = 0; i < obj._polozeniPojasevi.size(); i++)
            COUT << obj._polozeniPojasevi[i];
        return COUT;
    }
    void Infoo(Pojas p, Tehnika t, Polaganje pol) {
        m.lock();
        cout << "FROM:info@karate.ba\nTO: " << GetEmail() << "\nPostovani " << GetImePrezime() << ", evidentirana vam je thenika " << t << " za " << PojasChar[p] << " pojas.\nDosadasnji uspjeh(prosjek ocjena) na pojasu " << PojasChar[p] << " iznosi " << pol.GetProsjek() << ", a ukupni uspjeh(prosjek ocjena) na svim pojasevima iznosi " << GetProsjek() << "\nPozdrav.\nKARATE Team." << endl;
        m.unlock();
    }
    vector<Polaganje>& GetPolozeniPojasevi() { return _polozeniPojasevi; }
   
    bool AddTehniku(Pojas p, Tehnika t) {
        for (vector<Polaganje>::iterator i = _polozeniPojasevi.begin(); i != _polozeniPojasevi.end(); i++)
        {
            if (p > i->GetPojas() && (i->GetTehnike().size() < 3 || i->GetProsjek() < 3.5))return false;
            if (i->GetPojas() == p) {
                for (size_t j = 0; j < i->GetTehnike().size(); j++)
                {
                    if (*i->GetTehnike()[j] == t) return false;
                }
                i->GetTehnike().push_back(new Tehnika(t));
                thread t_sms(&KaratePolaznik::Infoo, this, p, t, *i);
                t_sms.join();
                return true;
            }
        }
        Polaganje poj(p);
        poj.GetTehnike().push_back(new Tehnika(t));
        _polozeniPojasevi.push_back(poj);
        return true;
    }
    /*
    sve tehnike na nivou jednog pojasa (ZUTI, ZELENI ... ) se evidentiraju unutar istog objekta tipa Polaganje,
    tom prilikom onemoguciti:
    - dodavanje istih (moraju biti identicne vrijednosti svih atributa) tehnika na nivou jednog pojasa,
    - dodavanje tehnika za visi pojas ako prethodni pojas nema evidentirane najmanje 3 tehnike ili nema prosjecnu ocjenu svih tehnika vecu od 3.5
    (onemoguciti dodavanje tehnike za NARANDZASTI ako ne postoji najmanje 3 tehnike za ZUTI pojas ili njihov prosjek nije veci od 3.5)
    funkcija vraca true ili false u zavisnosti od (ne)uspjesnost izvrsenja
    */
    /*nakon evidentiranja tehnike na bilo kojem pojasu kandidatu se salje email sa porukom:
  FROM:info@karate.ba
  TO: emailKorisnika
  Postovani ime i prezime, evidentirana vam je thenika X za Y pojas. Dosadasnji uspjeh (prosjek ocjena)
  na pojasu Y iznosi F, a ukupni uspjeh (prosjek ocjena) na svim pojasevima iznosi Z.
  Pozdrav.
  KARATE Team.
  slanje email poruka implemenitrati koristeci zasebne thread-ove.
  */
    void Info() { cout << *this; }
};


const char* GetOdgovorNaPrvoPitanje() {
    cout << "Pitanje -> Pojasnite ulogu operatora reinterpret_cast.\n";
    return "Odgovor -> OVDJE UNESITE VAS ODGOVOR";
}
const char* GetOdgovorNaDrugoPitanje() {
    cout << "Pitanje -> Ukratko opisite znacaj i vrste pametnih pokazivaca.\n";
    return "Odgovor -> OVDJE UNESITE VAS ODGOVOR";
}
void main() {

  /*  cout << PORUKA;
    cin.get();

    cout << GetOdgovorNaPrvoPitanje() << endl;
    cin.get();
    cout << GetOdgovorNaDrugoPitanje() << endl;
    cin.get();*/

    Datum   datum19062020(19, 6, 2020),
        datum20062020(20, 6, 2020),
        datum30062020(30, 6, 2020),
        datum05072020(5, 7, 2020);

    int kolekcijaTestSize = 10;

    Kolekcija<int, int> kolekcija1;
    for (int i = 0; i < kolekcijaTestSize; i++)
        kolekcija1.AddElement(i, i);

    cout << kolekcija1 << endl;

    try {
        /*metoda AddElement baca izuzetak u slucaju da se pokusa
        dodati vise od maksimalnog broja elemenata*/
        kolekcija1.AddElement(11, 11);
    }
    catch (exception & err) {
        cout << crt << "Greska -> " << err.what() << crt;
    }
    cout << kolekcija1 << crt;


    kolekcija1.RemoveAt(2);
    /*uklanja par (T1 i T2) iz kolekcije koji se nalazi na lokaciji sa proslijedjenim indeksom.
    nakon uklanjanja vrijednosti onemoguciti pojavu praznog prostora unutar kolekcije tj.
    pomjeriti sve elemente koji se nalaze nakon proslijedjene lokacije za jedno mjesto unazad
    npr. ako unutar kolekcije postoje elementi
    0 0
    1 1
    2 2
    3 3
    nakon uklanjanja vrijednosti na lokaciji 1, sadrzaj kolekcije ce biti sljedeci
    0 0
    2 2
    3 3
    */

    cout << kolekcija1 << crt;

    kolekcija1.AddElement(9, 9, 2);
    /*funkciji AddElement se, kao treci parametar, moze proslijediti i lokacija na koju se dodaju
    nove vrijednosti pri cemu treba zadrzati postojece vrijednosti pomjerene za jedno mjesto unaprijed
    u odnosu na definisanu lokaciju npr. ako unutar kolekcije postoje elementi
    0 0
    1 1
    2 2
    3 3
    nakon dodavanja vrijednosti 9 i 9 na lokaciju 1, sadrzaj kolekcije ce biti sljedeci
    0 0
    9 9
    1 1
    2 2
    3 3
    */

    cout << kolekcija1 << crt;

    Kolekcija<int, int> kolekcija2 = kolekcija1;
    cout << kolekcija1 << crt;

    //na osnovu vrijednosti T1 mijenja vrijednost T2. 
    kolekcija1[9] = 78;
    /* npr.ako unutar kolekcije postoje elementi:
    0 0
    9 9
    1 1
    2 2
    3 3
    nakon promjene vrijednosti sadrzaj kolekcije ce biti sljedeci
    0 0
    9 2
    1 1
    2 2
    3 3
    */

    Tehnika choku_zuki("choku_zuki"),
        gyaku_zuki("gyaku_zuki"),
        kizami_zuki("kizami_zuki"),
        oi_zuki("oi_zuki");
    cout << kolekcija1 << endl;
   
    if (choku_zuki.AddOcjena(1, datum19062020))
        cout << "Ocjena evidentirana!" << endl;
    if (!choku_zuki.AddOcjena(5, datum20062020))
        cout << "Ocjena NIJE evidentirana!" << endl;
    if (choku_zuki.AddOcjena(5, datum30062020))
        cout << "Ocjena evidentirana!" << endl;

    /* ispisuje: naziv tehnike, ocjene (zajedno sa datumom) i prosjecnu ocjenu za tu tehniku
       ukoliko tehnika nema niti jednu ocjenu prosjecna treba biti 0*/
    cout << choku_zuki << endl;

    if (ValidirajLozinku("john4Do*e"))
        cout << "OK" << crt;
    if (!ValidirajLozinku("john4Doe"))
        cout << "Specijalni znak?" << crt;
    if (!ValidirajLozinku("jo*4Da"))
        cout << "7 znakova?" << crt;
    if (!ValidirajLozinku("4jo-hnoe"))
        cout << "Veliko slovo?" << crt;
    if (ValidirajLozinku("@john2Doe"))
        cout << "OK" << crt;

    /*
    za autentifikaciju svaki korisnik mora posjedovati lozinku koja sadrzi:
    -   najmanje 7 znakova
    -   velika i mala slova
    -   najmanje jedan broj
    -   najmanje jedan specijalni znak
    za provjeru validnosti lozinke koristiti globalnu funkciju ValidirajLozinku, a unutar nje regex metode.
    validacija lozinke se vrsi unutar konstruktora klase Korisnik, a u slucaju da nije validna
    postaviti je na podrazumijevanu vrijednost: <VRIJEDNOST_NIJE_VALIDNA>
    */

    Korisnik* jasmin = new KaratePolaznik("Jasmin Azemovic", "jasmin@karate.ba", "j@sm1N*");
    Korisnik* adel = new KaratePolaznik("Adel Handzic", "adel@edu.karate.ba", "4Ade1*H");
    Korisnik* emailNijeValidan = new KaratePolaznik("John Doe", "john.doe@google.com", "johndoe");

    //doraditi klase da nacin da omoguce izvrsenje naredne linije koda
    KaratePolaznik* jasminPolaznik = dynamic_cast<KaratePolaznik*>(jasmin);

    if (jasminPolaznik != nullptr) {
        if (jasminPolaznik->AddTehniku(ZUTI, gyaku_zuki))
            cout << "Tehnika uspjesno dodan!" << crt;
        //ne treba dodati kizami_zuki jer ne postoje evidentirane 3 tehnike za ZUTI pojas
        if (!jasminPolaznik->AddTehniku(NARANDZASTI, kizami_zuki))
            cout << "Tehnika NIJE uspjesno dodana!" << crt;
        if (jasminPolaznik->AddTehniku(ZUTI, choku_zuki))
            cout << "Tehnika uspjesno dodan!" << crt;
        //ne treba dodati choku_zuki jer je vec dodana za zuti pojas
        if (!jasminPolaznik->AddTehniku(ZUTI, choku_zuki))
            cout << "Tehnika NIJE uspjesno dodana!" << crt;

        //ispisuje sve dostupne podatke o karate polazniku
        cout << *jasminPolaznik << crt;
    }

  

    //osigurati da se u narednim linijama poziva i destruktor klase KaratePolaznik
    delete jasmin;
    delete adel;
    delete emailNijeValidan;

    cin.get();
    system("pause>0");
}
