#include<iostream>
#include<string>
#include<vector>
#include<sstream>
using namespace std;

/****************************************************************************
1. SVE KLASE TREBAJU POSJEDOVATI ADEKVATAN DESTRUKTOR
2. NAMJERNO IZOSTAVLJANJE KOMPLETNIH I/ILI POJEDINIH DIJELOVA DESTRUKTORA KOJI UZROKUJU RUNTIME ERROR ÆE BITI OZNACENO KAO "RE"
3. SPAŠAVAJTE PROJEKAT KAKO BI SE SPRIJEÈILO GUBLJENJE URAÐENOG ZADATKA
4. NAZIVI FUNKCIJA, TE BROJ I TIP PARAMETARA MORAJU BITI IDENTIÈNI ONIMA KOJI SU KORIŠTENI U TESTNOM CODE-U, OSIM U SLUÈAJU DA POSTOJI ADEKVATAN RAZLOG ZA NJIHOVU MODIFIKACIJU. OSTALE, POMOÆNE FUNKCIJE MOŽETE IMENOVATI I DODAVATI PO ŽELJI.
5. IZUZETAK BACITE U FUNKCIJAMA U KOJIMA JE TO NAZNAÈENO.
****************************************************************************/
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

	/* Dodatne */
	bool AddElement(T1 el1, T2 el2) {
		T1* temp1 = new T1[_trenutno+1];
		T2* temp2 = new T2[_trenutno+1];

		for (int i = 0; i < _trenutno; i++)
		{
			temp1[i] = _elementi1[i];
			temp2[i] = _elementi2[i];
		}
		temp1[_trenutno] = el1;
		temp2[_trenutno] = el2;

		delete[] _elementi1;
		delete[] _elementi2;

		_elementi1 = temp1;
		_elementi2 = temp2;
		_trenutno++;

		return true;
	}

	Kolekcija(Kolekcija& k) {
		for (int i = 0; i < k._trenutno; i++)
		{
			_elementi1[i] = k._elementi1[i];
			_elementi2[i] = k._elementi2[i];
		}
		_trenutno = k._trenutno;
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
	Poglavlje() {
		delete[] _naslov; _naslov = nullptr;
		delete[] _sadrzaj; _sadrzaj = nullptr;
	}
	friend ostream& operator<<(ostream& COUT, Poglavlje& obj) {
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

	void OcijeniPoglavlje(int ocjena) {
		_ocjena = ocjena;
		if (_ocjena > 5 && ocjena <= 10)
			_prihvaceno = true;
	}

	/* Dodatne */

	bool SetNoviSadrzaj(const char* sadrzaj) {
		if (sadrzaj == nullptr)
			return false;

		stringstream ss;
		ss << _sadrzaj << " " << sadrzaj;
		delete _sadrzaj;

		_sadrzaj = AlocirajNizKaraktera(ss.str().c_str());

		return true;
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

	~ZavrsniRad() {
		delete[] _tema; _tema = nullptr;
	}
	char* GetNazivTeme()const { return _tema; }
	vector<Poglavlje>& GetPoglavlja() { return _poglavljaRada; };
	string GetDatumOdbrane()const { return _datumOdbrane; }
	float GetOcjena() { return _konacnaOcjena; }
	void SetDatumOdbrane(string datumOdbrane) { _datumOdbrane = datumOdbrane; }

	ZavrsniRad& operator=(const ZavrsniRad& org) {
		if (this != &org) {
			delete[] _tema;
			_tema = AlocirajNizKaraktera(org._tema);
			_datumOdbrane = org._datumOdbrane;
			_poglavljaRada = org._poglavljaRada;
			_konacnaOcjena = org._konacnaOcjena;
		}
		return *this;
	}

	friend ostream& operator<<(ostream& COUT, ZavrsniRad& obj) {
		COUT << "Tema rada: " << obj._tema << endl;
		COUT << "Sadrzaj: " << endl;
		for (size_t i = 0; i < obj._poglavljaRada.size(); i++)
			COUT << obj._poglavljaRada[i] << endl;
		COUT << "Datum odbrane rada: " << obj._datumOdbrane << endl << " Ocjena: " << obj._konacnaOcjena << endl;
		return COUT;
	}

	bool DodajPoglavlje(const char* nazivPoglavlja, const char* sadrzajPoglavlja) {

		bool imaPoglavlje = false;
		for (int i = 0; i < GetPoglavlja().size(); i++)
		{
			// Ako poglavlje postoji samo update
			if (strcmp(GetPoglavlja()[i].GetNaslov(), nazivPoglavlja)==0) {
				imaPoglavlje = true;

				GetPoglavlja()[i].SetNoviSadrzaj(sadrzajPoglavlja);
				break;
			}
		}
		if (!imaPoglavlje) {
			GetPoglavlja().push_back(Poglavlje(nazivPoglavlja, sadrzajPoglavlja));
		}

		return true;
	}

	bool OcijeniPoglavlje(const char* nazivPoglavlja, int ocjena) {

		bool imaPoglavlje = false;
		for (int i = 0; i < GetPoglavlja().size(); i++)
		{
			// Ako poglavlje postoji samo update
			if (strcmp(GetPoglavlja()[i].GetNaslov(), nazivPoglavlja) == 0) {
				imaPoglavlje = true;

				GetPoglavlja()[i].OcijeniPoglavlje(ocjena);
				break;
			}
		}
		if (!imaPoglavlje) {
			throw exception("Poglavlje ne postoji!");
		}
	}

	void SetKonacnaOcjena(float konacna) {
		_konacnaOcjena = konacna;
	}
};

class Mentor {
	string _imePrezime;
	//Parametar string predstavlja broj indeksa studenta koji prijavljuje zavrsni rad kod odredjenog Mentora
	Kolekcija<string, ZavrsniRad> _teme;
public:
	Mentor(string imePrezime) :_imePrezime(imePrezime) {}
	Kolekcija<string, ZavrsniRad>& GetTeme() { return _teme; };
	string GetImePrezime() { return _imePrezime; }
	void Info() {
		cout << _imePrezime << endl << _teme << endl;
	}

	bool DodajZavrsniRad(string brojIndexa, ZavrsniRad& z) {

		for (int i = 0; i < GetTeme().getTrenutno(); i++)
		{
			if (GetTeme().getElement1(i) == brojIndexa) {
				cout << "!!! Student vec postoji \n";
				return false;
			}
			if (strcmp(z.GetNazivTeme(), GetTeme().getElement2(i).GetNazivTeme()) == 0) {
				cout << "!!! Tema vec postoji! \n";
				return false;
			}
		}
		GetTeme().AddElement(brojIndexa, z);

		return true;
	}


	/*funkcija ZakaziOdbranuRada ima zadatak da studentu sa proslijedjenim brojem indeksa zakaze odbranu zavrsnog rada sto podrazumijeva definisanje
	datuma odbrane. odbrana rada se moze zakazati samo studentu koji je rad prethodno prijavio i pri tom su zadovoljeni sljedeci uslovi:
		1. zavrsni rad ima broj poglavlja veci od minimalnog
		2. svako poglavlje ima broj karaktera veci od minimalnog
		3. svako poglavlje je prihvaceno/odobreno
	ukoliko su zadovoljeni prethodni kriteriji, izracunava se konacna ocjena rada (prosjek ocjena svih poglavlja), postavlja datum odbrane rada i vraca pokazivac na rad kome je zakazan odbrana.
	u slucaju da student sa primljenim brojem indeksa nije prijavio zavrsni rad ili neki od postavljenih kriterija nije zadovoljen, funkcija vraca nullptr.
*/

//parametri:brojIndeksa, datumOdbrane
	//ZavrsniRad* zr1 = mentori[0]->ZakaziOdbranuRada("IB130011", "25.09.2018");

	ZavrsniRad * ZakaziOdbranuRada(string brojIndexa, string datumOdbrane) {

		int brPoglavlja = 0;
		float prosjecnaOcjena = 0;

		for (int i = 0; i < GetTeme().getTrenutno(); i++)
		{
			// da li je prijavio rad ikako
			if (GetTeme().getElement1(i) == brojIndexa) {
				// brojac poglavlja i da li su sva pogljavlja zadovoljila uslov 
				for (int j = 0; j < GetTeme().getElement2(i).GetPoglavlja().size(); j++) {

					if (strlen(GetTeme().getElement2(i).GetPoglavlja()[j].GetSadrzaj()) < min_karaktera_po_poglavlju || !GetTeme().getElement2(i).GetPoglavlja()[j].GetPrihvaceno()) {
						return nullptr;
					}
					prosjecnaOcjena += GetTeme().getElement2(i).GetPoglavlja()[j].GetOcjena();
					brPoglavlja++;
				}
				cout << "\n\n" << brojIndexa << " " << " BROJ POGAVLJA " << brPoglavlja << "\n\n";
				// ako ne zadovoljava uslov minimum poglavlja vratiti nullptr
				if (brPoglavlja < min_polgavlja) {
					cout << "\n Greska: Nema minimalno poglavlja ! \n\n";
					return nullptr;
				}

				prosjecnaOcjena /= brPoglavlja;
				GetTeme().getElement2(i).SetDatumOdbrane(datumOdbrane);
				GetTeme().getElement2(i).SetKonacnaOcjena(prosjecnaOcjena);

				return &GetTeme().getElement2(i);
	
			}
		}
		cout << "\n\n" << brojIndexa << " nije prijavio !! \n\n ";

		return nullptr;
	}
};

int main() {
	cout << crt << "UPLOAD RADA OBAVEZNO IZVRSITI U ODGOVARAJUCI FOLDER NA FTP SERVERU" << endl;
	cout << "U slucaju da je Upload folder prazan pritisnite tipku F5" << crt;

	const int max = 4;
	Mentor* mentori[max];

	mentori[0] = new Mentor("Armina Hubana");
	mentori[1] = new Mentor("Zanin Vejzovic");
	mentori[2] = new Mentor("Jasmin Azemovic");
	mentori[3] = new Mentor("Emina Junuz");
	//parametri: naziv zavrsnog rada
	ZavrsniRad multimedijalni("Multimedijalni informacijski sistem za visoko - obrazovnu ustanovu");
	ZavrsniRad podrsa_operaterima("Sistem za podršku rada kablovskog operatera");
	ZavrsniRad analiza_sigurnosti("Prakticna analiza sigurnosti bežiènih raèunarskih mreža");
	ZavrsniRad kriptografija("Primjena teorije informacija u procesu generisanja kriptografskih kljuèeva");



	cout << "MULTIMEDIJA \n\n";
	/*u zavrsni rad dodaje novo poglavlje i njegov sadrzaj. ukoliko poglavlje vec postoji u zavrsnom radu, funkcija tom poglavlju treba dodati novi sadrzaj i pri tome zadrzi postojeci (izmedju postojeceg i novog sadrzaja se dodaje prazan prostor). u slucaju da poglavlje ne postoji, ono se dodaje zajedno sa sadrzaje*/
	//parametri: nazivPoglavlja, sadrzajPoglavlja
	multimedijalni.DodajPoglavlje("Uvod", "U ovom poglavlju ce biti rijeci");
	multimedijalni.DodajPoglavlje("Uvod", "o multimedijalnim sistemima koji se danas koriste");
	multimedijalni.DodajPoglavlje("Uvod", "u savremenom poslovanju");
	multimedijalni.DodajPoglavlje("Vrste multimedijalnih sistema", "Danas se moze govoriti o nekoliko vrsta multimedijalnih sistema, a neke od najznacajnijih su ...");
	multimedijalni.DodajPoglavlje("Teorija multimedije", "Sadrzaj koji bi trebao stajati na pocetku treceg poglavlja zavrsnog rada o multimediji studenta IB130011");
	multimedijalni.DodajPoglavlje("Zakljucak", "U ovom radu su predstavljeni osnovni koncepti i prakticna primjena...");


	try {
		/*funkcija OcijeniPoglavlje, na osnovu naziva poglavlja, dodjeljuje ocjenu poglavlju te ukoliko je ocjena pozitivna (6 - 10) onda poglavlje oznacava prihvacenim. U slucaju da ocjena nije validna ili poglavlje ne postoji, funkcija baca izuzetak sa odgovarajucom porukom*/
		//parametri:nazivPoglavlja, ocjena

		multimedijalni.OcijeniPoglavlje("Uvod", 8);
		multimedijalni.OcijeniPoglavlje("Vrste multimedijalnih sistema", 8);
		multimedijalni.OcijeniPoglavlje("Teorija multimedije", 9);
		multimedijalni.OcijeniPoglavlje("Zakljucak", 7);
		multimedijalni.OcijeniPoglavlje("Naziv poglavlja ne postoji", 8);
	}
	catch (exception& err) {
		cout << "Greska -> " << err.what() << endl;
	}

	cout << "PODRSKA \n\n";

	podrsa_operaterima.DodajPoglavlje("Uvod", "U ovom poglavlju necemo nista pisati zato sto ne znam sta da pisem");
	podrsa_operaterima.DodajPoglavlje("Uvod", "i sta dalje pisati ne znam koliko karaktera mora biti");
	podrsa_operaterima.DodajPoglavlje("NekoPoglavlje", "Ovdje nesto pisem reda radi neko poglavlje");
	podrsa_operaterima.DodajPoglavlje("TrecePoglavlje", "Trece poglavlje pisem samo da bi moglo proci uslove...");


	try {
		podrsa_operaterima.OcijeniPoglavlje("Uvod", 9);
		podrsa_operaterima.OcijeniPoglavlje("NekoPoglavlje", 8);
		podrsa_operaterima.OcijeniPoglavlje("TrecePoglavlje", 9);
		podrsa_operaterima.OcijeniPoglavlje("nema ga", 9);

	}
	catch (exception& err) {
		cout << "Greska -> " << err.what() << endl;
	}



	/*funkcija DodajZavrsniRad ima zadatak da odredjenom Mentoru dodijeli mentorstvo na zavrsnom radu. zavrsni rad se dodaje studentu sa brojem indeksa proslijedjenim kao prvi parametar.
	sprijeciti dodavanje zavrsnih radova sa istom temom*/
	//parametri:brojIndeksa, zavrsniRad
	if (mentori[0]->DodajZavrsniRad("IB130011", multimedijalni))
		cout << "Zavrsni rad uspjesno dodat!" << endl;
	if (mentori[0]->DodajZavrsniRad("IB120051", podrsa_operaterima))
		cout << "Zavrsni rad uspjesno dodat!" << endl;
	if (!mentori[0]->DodajZavrsniRad("IB120056", podrsa_operaterima))//dupliranje rada
		cout << "Zavrsni rad nije dodat!" << endl;
	if (!mentori[0]->DodajZavrsniRad("IB120051", kriptografija)) //studentu vec dodijeljen rad
		cout << "Zavrsni rad nije dodat!" << endl;
	if (mentori[1]->DodajZavrsniRad("IB140102", analiza_sigurnosti))
		cout << "Zavrsni rad uspjesno dodat!" << endl;
	if (mentori[2]->DodajZavrsniRad("IB140002", kriptografija))
		cout << "Zavrsni rad uspjesno dodat!" << endl;

	/*funkcija ZakaziOdbranuRada ima zadatak da studentu sa proslijedjenim brojem indeksa zakaze odbranu zavrsnog rada sto podrazumijeva definisanje
	datuma odbrane. odbrana rada se moze zakazati samo studentu koji je rad prethodno prijavio i pri tom su zadovoljeni sljedeci uslovi:
		1. zavrsni rad ima broj poglavlja veci od minimalnog
		2. svako poglavlje ima broj karaktera veci od minimalnog
		3. svako poglavlje je prihvaceno/odobreno
	ukoliko su zadovoljeni prethodni kriteriji, izracunava se konacna ocjena rada (prosjek ocjena svih poglavlja), postavlja datum odbrane rada i vraca pokazivac na rad kome je zakazan odbrana.
	u slucaju da student sa primljenim brojem indeksa nije prijavio zavrsni rad ili neki od postavljenih kriterija nije zadovoljen, funkcija vraca nullptr.
*/

//parametri:brojIndeksa, datumOdbrane
	ZavrsniRad* zr1 = mentori[0]->ZakaziOdbranuRada("IB130011", "25.09.2018");
	if (zr1 != nullptr)
		cout << *zr1 << endl;

	// IB120051
	zr1 = mentori[0]->ZakaziOdbranuRada("IB120051", "03.10.2019");//student sa brojem indeksa IB130111 jos uvijek nije prijavio rad
	if (zr1 != nullptr)
		cout << *zr1 << endl;

	zr1 = mentori[0]->ZakaziOdbranuRada("IB130111", "25.09.2018");//student sa brojem indeksa IB130111 jos uvijek nije prijavio rad
	if (zr1 != nullptr)
		cout << *zr1 << endl;

	//ispisuje sve podatke o Mentoru i njegovim mentorstvima
	mentori[0]->Info();

	for (int i = 0; i < max; i++) {
		delete mentori[i];
		mentori[i] = nullptr;
	}
	system("pause>0");
	return 0;
}
