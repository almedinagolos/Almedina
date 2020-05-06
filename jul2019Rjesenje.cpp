#include<iostream>
#include<vector>
#include<string>
#include<mutex>
#include<regex>
#include<thread>
using namespace std;

/****************************************************************************
1. SVE KLASE TREBAJU POSJEDOVATI ADEKVATAN DESTRUKTOR
2. NAMJERNO IZOSTAVLJANJE KOMPLETNIH I/ILI POJEDINIH DIJELOVA DESTRUKTORA KOJI UZROKUJU RUNTIME ERROR ĆE BITI OZNACENO KAO "RE"
3. SPAŠAVAJTE PROJEKAT KAKO BI SE SPRIJEČILO GUBLJENJE URAĐENOG ZADATKA
4. NAZIVI FUNKCIJA, TE BROJ I TIP PARAMETARA MORAJU BITI IDENTIČNI ONIMA KOJI SU KORIŠTENI U TESTNOM CODE-U, OSIM U SLUČAJU DA POSTOJI ADEKVATAN RAZLOG ZA NJIHOVU MODIFIKACIJU. OSTALE, POMOĆNE FUNKCIJE MOŽETE IMENOVATI I DODAVATI PO ŽELJI.
5. IZUZETAK BACITE U FUNKCIJAMA U KOJIMA JE TO NAZNAČENO.
****************************************************************************/
mutex m;
const char* crt = "\n-------------------------------------------\n";
const char* nedozvoljena_operacija = "Nedozvoljena operacija";
const char* not_set = "NOT_SET";
const int min_polgavlja = 3;
const int min_karaktera_po_poglavlju = 30;

char* AlocirajNizKaraktera(const char* sadrzaj) {
	if (sadrzaj == nullptr)
		return nullptr;
	int vel = strlen(sadrzaj) + 1;
	char* temp = new char[vel];
	strcpy_s(temp, vel, sadrzaj);
	return temp;
}

template<class T1, class T2>
class Kolekcija {
	T1* _elementi1;
	T2* _elementi2;
	int _trenutno;
public:
	Kolekcija() :_trenutno(0), _elementi1(nullptr), _elementi2(nullptr) { }
	~Kolekcija() {
		delete[] _elementi1; _elementi1 = nullptr;
		delete[] _elementi2; _elementi1 = nullptr;
	}
	T1* getElementi1Pok() { return _elementi1; }
	T2* getElementi2Pok() { return _elementi2; }
	T1& getElement1(int lokacija) { return _elementi1[lokacija]; }
	T2& getElement2(int lokacija) { return _elementi2[lokacija]; }
	int getTrenutno() { return _trenutno; }

	friend ostream& operator<<(ostream& COUT, Kolekcija<T1, T2>& obj) {
		for (size_t i = 0; i < obj._trenutno; i++)
			COUT << obj.getElement1(i) << " " << obj.getElement2(i) << endl;
		return COUT;
	}
	bool AddElement(T1 el1, T2 el2) {
		for (size_t i = 0; i < _trenutno; i++)
		{
			if (_elementi1[i] == el1 || _elementi2[i] == el2) throw exception("Elementi vec postoje.");
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
		return true;
	}
};
class Poglavlje {
	char* _naslov;
	char* _sadrzaj;
	bool _prihvaceno;
	int _ocjena;//da bi se poglavlje smatralo prihvacenim ocjena mora biti u opsegu od 6 - 10
public:
	Poglavlje(const char* naslov = nullptr, const char* sadrzaj = nullptr)
		:_ocjena(0), _prihvaceno(false) {
		_naslov = AlocirajNizKaraktera(naslov);
		_sadrzaj = AlocirajNizKaraktera(sadrzaj);
	}
	Poglavlje(const Poglavlje &p):_prihvaceno(p._prihvaceno), _ocjena(p._ocjena){
		_naslov = AlocirajNizKaraktera(p._naslov);
		_sadrzaj = AlocirajNizKaraktera(p._sadrzaj);
	}
	Poglavlje& operator=(const Poglavlje& p) {
		if (this != &p) {
			delete[] _naslov;
			delete[] _sadrzaj;
			_naslov = AlocirajNizKaraktera(p._naslov);
			_sadrzaj = AlocirajNizKaraktera(p._sadrzaj);
			_prihvaceno = p._prihvaceno;
			_ocjena = p._ocjena;
		} return *this;
	}
	~Poglavlje() {
		delete[] _naslov; _naslov = nullptr;
		delete[] _sadrzaj; _sadrzaj = nullptr;
	}
	friend ostream& operator<<(ostream& COUT, const Poglavlje& obj) {
		if (obj._naslov == nullptr || obj._sadrzaj == nullptr)
			return COUT;
		COUT << endl << obj._naslov << endl << obj._sadrzaj << endl;
		if (obj._prihvaceno)
			COUT << "Ocjena: " << obj._ocjena << endl;;
		return COUT;
	}
	char* GetNaslov() { return _naslov; }
	char* GetSadrzaj() { return _sadrzaj; }
	bool GetPrihvaceno() { return _prihvaceno; }
	int GetOcjena() { return _ocjena; }
	void SetSadrzaj(string sadrzaj){
		char* temp = AlocirajNizKaraktera((sadrzaj + string(_sadrzaj)).c_str());
		delete[] _sadrzaj;
		_sadrzaj = temp;
	}
	void OcijeniPoglavlje(int ocjena) {
		_ocjena = ocjena;
		if (_ocjena > 5 && _ocjena <= 10)
			_prihvaceno = true;
	}
};

class ZavrsniRad {
	char* _tema;
	vector<Poglavlje> _poglavljaRada;
	string _datumOdbrane;
	float _konacnaOcjena; //prosjek ocjena svih poglavlja u zavrsnom radu koja se izracunava u momentu zakazivanja odbrane
public:
	ZavrsniRad(const char* nazivTeme = nullptr) : _konacnaOcjena(0), _datumOdbrane(not_set) {
		_tema = AlocirajNizKaraktera(nazivTeme);
	}

	ZavrsniRad(const ZavrsniRad& org) : _poglavljaRada(org._poglavljaRada), _konacnaOcjena(org._konacnaOcjena), _datumOdbrane(org._datumOdbrane) {
		_tema = AlocirajNizKaraktera(org._tema);
	}
	ZavrsniRad& operator=(const ZavrsniRad& z) {
		if (this != &z) {
			delete[] _tema;
			_tema = AlocirajNizKaraktera(z._tema);
			_poglavljaRada = z._poglavljaRada;
			_datumOdbrane = z._datumOdbrane;
			_konacnaOcjena = z._konacnaOcjena;
		}
		return *this;
	}
	float GetKonacnaOcjena() { return _konacnaOcjena; }
	bool operator==(const ZavrsniRad& z) {
		if(_tema != nullptr)
		return strcmp(_tema, z._tema) == 0;
	}
	~ZavrsniRad() {
		delete[] _tema; _tema = nullptr;
	}
	char* GetNazivTeme()const { return _tema; }
	vector<Poglavlje>& GetPoglavlja() { return _poglavljaRada; };
	string GetDatumOdbrane()const { return _datumOdbrane; }
	float GetOcjena() { return _konacnaOcjena; }
	void SetDatumOdbrane(string datumOdbrane) { _datumOdbrane = datumOdbrane; }
	void SetKonacnaOcjena(float ocjena) { _konacnaOcjena = ocjena; }
	friend ostream& operator<<(ostream& COUT, const ZavrsniRad& obj) {
		COUT << "Tema rada: " << obj._tema << endl;
		COUT << "Sadrzaj: " << endl;
		for (size_t i = 0; i < obj._poglavljaRada.size(); i++)
			COUT << obj._poglavljaRada[i] << endl;
		COUT << "Datum odbrane rada: " << obj._datumOdbrane << endl << " Ocjena: " << obj._konacnaOcjena << endl;
		return COUT;
	}
	void DodajPoglavlje(string nazivPoglavlja, string sadrzajPoglavlja){
		for (vector<Poglavlje>::iterator i = _poglavljaRada.begin(); i != _poglavljaRada.end(); i++)
		{
			if (strcmp(i->GetNaslov(), nazivPoglavlja.c_str()) == 0) {
				i->SetSadrzaj(sadrzajPoglavlja);
				return;
			}
		}
		_poglavljaRada.push_back(Poglavlje(nazivPoglavlja.c_str(), sadrzajPoglavlja.c_str()));
	}
	/*u zavrsni rad dodaje novo poglavlje i njegov sadrzaj. ukoliko poglavlje vec postoji u zavrsnom radu,
	funkcija tom poglavlju treba dodati novi sadrzaj i pri tome zadrzi postojeci (izmedju postojeceg i novog sadrzaja se dodaje prazan prostor).
	u slucaju da poglavlje ne postoji, ono se dodaje zajedno sa sadrzaje*/
	//parametri: nazivPoglavlja, sadrzajPoglavlja
	bool OcijeniPoglavlje(string naziv, int ocjena) {
		bool postoji = false;
		for (vector<Poglavlje>::iterator i = _poglavljaRada.begin(); i != _poglavljaRada.end(); i++)
		{
			if (strcmp(i->GetNaslov(), naziv.c_str()) == 0)
			{
				postoji = true;
				i->OcijeniPoglavlje(ocjena);
				break;
			}
		}
		if (!postoji) {
			throw exception("Poglavlje ne postoji.");
		}
	}
};

class Osoba {
protected:
	string _imePrezime;
public:
	Osoba(string imePrezime) : _imePrezime(imePrezime) {}
	string GetImePrezime() { return _imePrezime; }
	friend ostream& operator <<(ostream& o, const Osoba& os) {
		o << os._imePrezime << endl;
		return o;
	}
	virtual void Info() = 0;
};

class Nastavnik : public Osoba{
	//Parametar string predstavlja broj indeksa studenta koji prijavljuje zavrsni rad kod odredjenog nastavnika
	Kolekcija<string, ZavrsniRad> _teme;
public:
	Nastavnik(string imePrezime):Osoba(imePrezime){}
	Kolekcija<string, ZavrsniRad>& GetTeme() { return _teme; };
	friend ostream& operator<<(ostream& o, Nastavnik& n) {
		o << dynamic_cast<Osoba&>(n);
		o << n._teme << endl;
		return o;
	}
	void Info() { cout << *this; }
	bool DodajZavrsniRad(string indeks, ZavrsniRad z) {
		try {
			_teme.AddElement(indeks, z);
			return true;
		}
		catch (const std::exception & e) {
			return false;
		}
		return false;
	}
	ZavrsniRad* ZakaziOdbranuRada(string indeks, string datum){
		float suma = 0;
		for (size_t i = 0; i < _teme.getTrenutno(); i++)
		{
			if (_teme.getElement1(i) == indeks) {
				if ((_teme.getElement2(i).GetPoglavlja().size()) < min_polgavlja) return nullptr;
				for (vector<Poglavlje>::iterator j = _teme.getElement2(i).GetPoglavlja().begin(); j != _teme.getElement2(i).GetPoglavlja().end(); j++)
				{
					if (strlen(j->GetSadrzaj()) < min_karaktera_po_poglavlju || !j->GetPrihvaceno()) return nullptr;
					suma += _teme.getElement2(i).GetOcjena();
				}
				suma /= _teme.getElement2(i).GetPoglavlja().size();
				_teme.getElement2(i).SetDatumOdbrane(datum);
				_teme.getElement2(i).SetKonacnaOcjena(suma);
				return &_teme.getElement2(i);
			}
		}
		return nullptr;
	}
	/*funkcija ZakaziOdbranuRada ima zadatak da studentu sa proslijedjenim brojem indeksa zakaze odbranu zavrsnog rada sto podrazumijeva definisanje
	datuma odbrane. odbrana rada se moze zakazati samo studentu koji je rad prethodno prijavio i pri tom su zadovoljeni sljedeci uslovi:
		1. zavrsni rad ima broj poglavlja veci od minimalnog
		2. svako poglavlje ima broj karaktera veci od minimalnog
		3. svako poglavlje je prihvaceno/odobreno
	ukoliko su zadovoljeni prethodni kriteriji, izracunava se konacna ocjena rada (prosjek ocjena svih poglavlja), postavlja datum odbrane rada i vraca pokazivac na rad kome je zakazan odbrana.
	u slucaju da student sa primljenim brojem indeksa nije prijavio zavrsni rad ili neki od postavljenih kriterija nije zadovoljen, funkcija vraca nullptr.
*/

//parametri: brojIndeksa, datumOdbrane
};
/*Funkcija PosaljiPozivZaDodjeluNagrada ima zadatak da svim studentima koji su uspjesno okoncali svoj zavrsni rad kod odredjenog nastavnika/mentora
	i tom prilikom ostvarili ocjenu vecu od proslijedjene, u zasebnom thread.u, posalje email poruku (mail adresa: brojIndeksa@edu.fit.ba) 
	sa sadrzajem:
	"Postovani {brojIndeksa}, uzimajuci u obzir cinjenicu da ste kod mentora {imePrezimeMentora} uspjesno odbranili rad sa ocjenom {ocjena}
	cast nam je pozvati vas na dodjelu nagrada za najbolje studente koja ce se odrzatu u na FIT-u 03.07.2019. godine."	.
	funkcija treba da vrati sadrzaj svih poslatih email poruka, a ukoliko niti jedan od nastavnika ne posjeduje evidentirano mentorstvo na
	zavrsnom radu, funkcija vraca not_set*/
string PosaljiPozivZaDodjeluNagrada(Nastavnik** nastavnici, int max, int ocjena) {
	string poruka = not_set;
	float prosjek = 0;
	int brojac = 0;
	Nastavnik* n = nullptr;
	for (size_t i = 0; i < max; i++)
	{
		float trenutniProsjek = 0;
		for (size_t j = 0; j < nastavnici[i]->GetTeme().getTrenutno(); j++)
		{
			if (nastavnici[i]->GetTeme().getElement2(j).GetOcjena() > 5) {
				trenutniProsjek += nastavnici[i]->GetTeme().getElement2(j).GetOcjena();
				brojac++;
			}
		}
		if (brojac != 0) {
			trenutniProsjek /= brojac;
			if (prosjek > trenutniProsjek) {
				prosjek = trenutniProsjek;
				n = nastavnici[i];
			}
		}
	}
	if (n != nullptr) {
		for (size_t i = 0; i < n->GetTeme().getTrenutno(); i++)
		{
			if (n->GetTeme().getElement2(i).GetKonacnaOcjena() >= ocjena) {
				string temp = "Postovani" + n->GetTeme().getElement1(i) + ", uzimajuci u obzir cinjenicu da ste kod mentora " + n->GetImePrezime() + " uspjesno odbranili rad sa ocjenom " + to_string(n->GetTeme().getElement2(i).GetKonacnaOcjena()) + " cast nam je pozvati vas na dodjelu nagrada za najbolje studente koja ce se odrzatu u na FIT - u 20.09.2018.godine.";
				poruka += "\n" + temp + "\n";
				thread t([temp](string email) {
					m.lock();
					cout << "Saljem mail -> " << email << endl;
					cout << temp << endl;
					this_thread::sleep_for(chrono::seconds(2));
					m.unlock();
					}, n->GetTeme().getElement1(i) + "@edu.fit.ba");
				t.join();
			}
		}
	}
	return poruka;
}

int main() {
	cout << crt << "UPLOAD RADA OBAVEZNO IZVRSITI U ODGOVARAJUCI FOLDER NA FTP SERVERU" << endl;
	cout << "U slucaju da je Upload folder prazan pritisnite tipku F5" << crt;

	const int max = 4;
	Nastavnik* nastavnici[max];

	nastavnici[0] = new Nastavnik("Denis Music");
	nastavnici[1] = new Nastavnik("Zanin Vejzovic");
	nastavnici[2] = new Nastavnik("Jasmin Azemovic");
	nastavnici[3] = new Nastavnik("Emina Junuz");
	//parametri: naziv zavrsnog rada
	ZavrsniRad multimedijalni("Multimedijalni informacijski sistem za visoko - obrazovnu ustanovu");
	ZavrsniRad podrsa_operaterima("Sistem za podršku rada kablovskog operatera");
	ZavrsniRad analiza_sigurnosti("Prakticna analiza sigurnosti bežičnih računarskih mreža");
	ZavrsniRad kriptografija("Primjena teorije informacija u procesu generisanja kriptografskih ključeva");

	
	multimedijalni.DodajPoglavlje("Uvod", "U ovom poglavlju ce biti rijeci");
	multimedijalni.DodajPoglavlje("Uvod", "o multimedijalnim sistemima koji se danas koriste");
	multimedijalni.DodajPoglavlje("Uvod", "u savremenom poslovanju");
	multimedijalni.DodajPoglavlje("Vrste multimedijalnih sistema", "Danas se moze govoriti o nekoliko vrsta multimedijalnih sistema, a neke od najznacajnijih su ...");
	multimedijalni.DodajPoglavlje("Teorija multimedije", "Sadrzaj koji bi trebao stajati na pocetku treceg poglavlja zavrsnog rada o multimediji studenta IB130011");
	multimedijalni.DodajPoglavlje("Zakljucak", "U ovom radu su predstavljeni osnovni koncepti i prakticna primjena...");
	
	try {
		/*funkcija OcijeniPoglavlje, na osnovu naziva poglavlja, dodjeljuje ocjenu poglavlju te ukoliko je ocjena pozitivna (6 - 10) 
		onda poglavlje oznacava prihvacenim. U slucaju da ocjena nije validna ili poglavlje ne postoji, funkcija baca izuzetak sa 
		odgovarajucom porukom*/
		//parametri:nazivPoglavlja, ocjena

		multimedijalni.OcijeniPoglavlje("Uvod", 8);
		multimedijalni.OcijeniPoglavlje("Vrste multimedijalnih sistema", 8);
		multimedijalni.OcijeniPoglavlje("Teorija multimedije", 9);
		multimedijalni.OcijeniPoglavlje("Zakljucak", 7);
		multimedijalni.OcijeniPoglavlje("Naziv poglavlja ne postoji", 8);
	}
	catch (exception & err) {
		cout << "Greska -> " << err.what() << endl;
	}

	/*funkcija DodajZavrsniRad ima zadatak da odredjenom nastavniku dodijeli mentorstvo na zavrsnom radu.
	zavrsni rad se dodaje studentu sa brojem indeksa proslijedjenim kao prvi parametar.
	sprijeciti dodavanje zavrsnih radova sa istom temom*/
	//parametri: brojIndeksa, zavrsniRad
	if (nastavnici[0]->DodajZavrsniRad("IB130011", multimedijalni))
		cout << "Zavrsni rad uspjesno dodat!" << endl;
	if (nastavnici[0]->DodajZavrsniRad("IB120051", podrsa_operaterima))
		cout << "Zavrsni rad uspjesno dodat!" << endl;
	if (!nastavnici[0]->DodajZavrsniRad("IB120056", podrsa_operaterima))//dupliranje rada
		cout << "Zavrsni rad nije dodat!" << endl;
	if (!nastavnici[0]->DodajZavrsniRad("IB120051", kriptografija)) //studentu vec dodijeljen rad
		cout << "Zavrsni rad nije dodat!" << endl;
	if (nastavnici[1]->DodajZavrsniRad("IB140102", analiza_sigurnosti))
		cout << "Zavrsni rad uspjesno dodat!" << endl;
	if (nastavnici[2]->DodajZavrsniRad("IB140002", kriptografija))
		cout << "Zavrsni rad uspjesno dodat!" << endl;

	
	ZavrsniRad* zr1 = nastavnici[0]->ZakaziOdbranuRada("IB130011", "25.09.2018");
	if (zr1 != nullptr)
		cout << *zr1 << endl;

	zr1 = nastavnici[0]->ZakaziOdbranuRada("IB130111", "25.09.2018");//student sa brojem indeksa IB130111 jos uvijek nije prijavio rad
	if (zr1 != nullptr)
		cout << *zr1 << endl;

	//ispisuje sve podatke o nastavniku i njegovim mentorstvima
	nastavnici[0]->Info();

	
	cout << "Studenti za dodjelu nagrada: " << PosaljiPozivZaDodjeluNagrada(nastavnici, max, 9.5) << endl;

	for (int i = 0; i < max; i++) {
		delete nastavnici[i];
		nastavnici[i] = nullptr;
	}
	system("pause>0");
	return 0;
}
