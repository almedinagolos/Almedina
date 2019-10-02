#include<iostream>
using namespace std;

/*
1. BROJ I VRSTA PARAMETARA MORAJU BITI IDENTICNI KAO U PRIMJERIMA. U SUPROTNOM SE RAD NECE BODOVATI
2. STAVITE KOMENTAR NA DIJELOVE CODE-A KOJE NE BUDETE IMPLEMENTIRALI
3. KREIRAJTE .DOC FAJL SA VAŠIM BROJEM INDEKSA ( NPR. IB160061.DOC BEZ IMENA I PREZIMENA), TE NA KRAJU ISPITA U NJEGA KOPIRAJTE RJEŠENJA VAŠIH ZADATAKA. NE PREDAVATI .TXT ILI .CPP FAJLOVE
4. TOKOM IZRADE ISPITA NIJE DOZVOLJENO KORIŠTENJE HELP-A
5. TOKOM IZRADE ISPITA MOGU BITI POKRENUTA SAMO TRI PROGRAMA: PDF READER (ISPITNI ZADACI), MS VISUAL STUDIO, MS WORD (U KOJI CETE KOPIRATI VAŠA RJEŠENJA)
6. BEZ OBZIRA NA TO DA LI SU ISPITNI ZADACI URAĐENI, SVI STUDENTI KOJI SU PRISTUPILI ISPITU MORAJU PREDATI SVOJ RAD
*/

const char* not_set = "NEMA_VRIJEDNOST";
const char* crt = "\n-------------------------------------------\n";
const int min_polgavlja = 3;
const int max_poglavlja = 15;
const int min_karaktera_po_poglavlju = 30;
const int max_zavrsnih = 30;

char* AlocirajNizKaraktera(const char* sadrzaj) {
	if (sadrzaj == nullptr)
		return nullptr;
	int vel = strlen(sadrzaj) + 1;
	char* temp = new char[vel];
	strcpy_s(temp, vel, sadrzaj);
	return temp;
}

struct Poglavlje {
	char* _naslov;
	char* _sadrzaj;
	bool _prihvaceno;
	int _ocjena;//da bi se poglavlje smatralo prihvacenim ocjena mora biti u opsegu od 6 - 10
	void Unos(const char* naslov = nullptr, const char* sadrzaj = nullptr) {
		_ocjena = 0; _prihvaceno = false;
		_naslov = AlocirajNizKaraktera(naslov);
		_sadrzaj = AlocirajNizKaraktera(sadrzaj);
	}
	void Dealociraj() {
		delete[] _naslov; _naslov = nullptr;
		delete[] _sadrzaj; _sadrzaj = nullptr;
	}
	void Ispis() {
		if (_naslov == nullptr || _sadrzaj == nullptr)
			return;
		cout << endl << "Poglavlje -> " << _naslov << endl << _sadrzaj << endl;
		if (_prihvaceno)
			cout << "Ocjena: " << _ocjena << endl;;
	}
	void OcijeniPoglavlje(int ocjena) {
		_ocjena = ocjena;
		if (ocjena > 5 && ocjena <= 10)
			_prihvaceno = true;
	}
};

struct ZavrsniRad {
	char* _brojIndeksa;
	char* _tema;
	//zavrsni rad ne bi trebao imati vise od 15 poglavlja
	Poglavlje* _poglavljaRada[max_poglavlja] = { nullptr };
	char* _datumOdbrane;
	float _konacnaOcjena; //konacna ocjena zavrsnog rada predstavlja prosjek ocjena svih poglavlja u zavrsnom radu koja se izracunava u momentu zakazivanja odbrane

	void Unos(const char* brojIndeksa, const char* nazivTeme = nullptr) {
		_konacnaOcjena = 0;
		_brojIndeksa = AlocirajNizKaraktera(brojIndeksa);
		_tema = AlocirajNizKaraktera(nazivTeme);
		_datumOdbrane = AlocirajNizKaraktera(not_set);
	}
	void Dealociraj() {
		delete[] _tema; _tema = nullptr;
		delete[] _datumOdbrane; _datumOdbrane = nullptr;
		delete[] _brojIndeksa; _brojIndeksa = nullptr;
		for (size_t i = 0; i < max_poglavlja; i++) {
			if (_poglavljaRada[i] != nullptr) {
				_poglavljaRada[i]->Dealociraj();
				delete _poglavljaRada[i];
				_poglavljaRada[i] = nullptr;
			}
		}
	}
	void Ispis() {
		cout << "Tema rada: " << _tema << endl;
		cout << "Sadrzaj: " << endl;
		for (size_t i = 0; i < max_poglavlja; i++)
			if (_poglavljaRada[i] != nullptr)
				_poglavljaRada[i]->Ispis();
		cout << "Datum odbrane rada: " << _datumOdbrane << endl << " Ocjena: " << _konacnaOcjena << endl;
	}
	void DodajPoglavlje(const char* poglavlje, const char* sadrzaj) {
		for (int i = 0; i < max_poglavlja; i++)
		{
			if (_poglavljaRada[i] != nullptr) {
				if (strcmp(_poglavljaRada[i]->_naslov, poglavlje) == 0) {
					if (_poglavljaRada[i]->_sadrzaj == nullptr)
						_poglavljaRada[i]->_sadrzaj = AlocirajNizKaraktera(sadrzaj);

					else {
						int vel = strlen(_poglavljaRada[i]->_sadrzaj) + strlen(sadrzaj) + 2;
						char* temp = new char[vel];
						strcpy_s(temp, vel, _poglavljaRada[i]->_sadrzaj);
						strcat_s(temp, vel, " ");
						strcat_s(temp, vel, sadrzaj);
						delete[] _poglavljaRada[i]->_sadrzaj;
						_poglavljaRada[i]->_sadrzaj = temp;
						return;
					}
					return;
				}
			}
			else {
				_poglavljaRada[i] = new Poglavlje;
				_poglavljaRada[i]->Unos(poglavlje, sadrzaj);
				return;
			}
		}
		return;
	}
	void OcijeniPoglavlje(const char* poglavlje, int ocjena) {
		for (int i = 0; i < max_poglavlja; i++)
		{
			if (_poglavljaRada[i] != nullptr && strcmp(_poglavljaRada[i]->_naslov, poglavlje) == 0) {
				_poglavljaRada[i]->OcijeniPoglavlje(ocjena);
				return;
			}
		}
		return;
	}
	void Kopiraj(ZavrsniRad rad) {
		Unos(rad._brojIndeksa, rad._tema);
		for (int i = 0; i < max_poglavlja; i++)
		{
			if (rad._poglavljaRada[i] != nullptr) {
				_poglavljaRada[i] = new Poglavlje;
				_poglavljaRada[i]->Unos(rad._poglavljaRada[i]->_naslov, rad._poglavljaRada[i]->_sadrzaj);
				_poglavljaRada[i]->OcijeniPoglavlje(rad._poglavljaRada[i]->_ocjena);
				rad.Dealociraj();
			}
		}
	}
};
struct Nastavnik {
	char* _imePrezime;
	ZavrsniRad* _teme;
	int _trenutnoZavrsnih;

	void Unos(const char* imePrezime) {
		_imePrezime = AlocirajNizKaraktera(imePrezime);
		_teme = nullptr;
		_trenutnoZavrsnih = 0;
	}
	void Dealociraj() {
		delete[] _imePrezime; _imePrezime = nullptr;
		for (size_t i = 0; i < _trenutnoZavrsnih; i++)
			_teme[i].Dealociraj();
		delete[] _teme;
		_teme = nullptr;
	}
	void Ispis() {
		cout << crt << _imePrezime << crt;
		for (size_t i = 0; i < _trenutnoZavrsnih; i++)
			_teme[i].Ispis();
	}
	bool DodajZavrsniRad(ZavrsniRad rad) {
		for (int i = 0; i < _trenutnoZavrsnih; i++)
		{
			if (strcmp(_teme[i]._brojIndeksa, rad._brojIndeksa) == 0 || strcmp(_teme[i]._tema, rad._tema) == 0)
				return false;
		}
		ZavrsniRad* temp = new ZavrsniRad[_trenutnoZavrsnih + 1];
		for (int i = 0; i < _trenutnoZavrsnih; i++)
		{
			temp[i].Kopiraj(_teme[i]);
			
		}
		if (_teme != nullptr)
			delete[] _teme;
		_teme = nullptr;
		temp[_trenutnoZavrsnih].Kopiraj(rad);
		_teme = temp;
		_trenutnoZavrsnih++;
		return true;
	}
	ZavrsniRad* ZakaziOdbranuRada(const char* index, const char* datum) {
		ZavrsniRad* rad = nullptr;
		for (int i = 0; i < _trenutnoZavrsnih; i++)
		{
			if (strcmp(_teme[i]._brojIndeksa, index) == 0) {
				int brojac = 0;
				float prosjek = 0;
				for (int j = 0; j < max_poglavlja; j++)
					if (_teme[i]._poglavljaRada[j] != nullptr)
						brojac++;

				if (brojac > min_polgavlja) {
					for (int j = 0; j < max_poglavlja; j++)
						if (_teme[i]._poglavljaRada[j] != nullptr)
							if (strlen(_teme[i]._poglavljaRada[j]->_sadrzaj) > min_karaktera_po_poglavlju && _teme[i]._poglavljaRada[j]->_prihvaceno)
								prosjek += _teme[i]._poglavljaRada[j]->_ocjena;

					prosjek /= brojac;
					_teme[i]._konacnaOcjena = prosjek;
					_teme[i]._datumOdbrane = AlocirajNizKaraktera(datum);
					rad = _teme + i;
					return rad;
				}
				else
					return nullptr;
			}
		}
		return nullptr;
	}
};

int main() {
	cout << crt << "UPLOAD RADA OBAVEZNO IZVRSITI U ODGOVARAJUCI FOLDER NA FTP SERVERU" << endl;
	cout << "U slucaju da je Upload folder prazan pritisnite tipku F5" << crt;

	const int max = 2;
	Nastavnik* nastavnici[max];

	nastavnici[0] = new Nastavnik; 	nastavnici[0]->Unos("Denis Music");
	nastavnici[1] = new Nastavnik; 	nastavnici[1]->Unos("Emina Junuz");

	ZavrsniRad multimedijalni;
	//parametri: brojIndeksa, tema
	multimedijalni.Unos("IB120021", "Multimedijalni informacijski sistem za visoko - obrazovnu ustanovu");
	ZavrsniRad podrsa_operaterima;
	podrsa_operaterima.Unos("IB130031", "Sistem za podršku rada kablovskog operatera");
	ZavrsniRad analiza_sigurnosti;
	analiza_sigurnosti.Unos("IB140041", "Prakticna analiza sigurnosti bežičnih računarskih mreža");
	ZavrsniRad kriptografija;
	kriptografija.Unos("IB120021", "Primjena teorije informacija u procesu generisanja kriptografskih ključeva");

	/*u zavrsni rad dodaje novo poglavlje i njegov sadrzaj. ukoliko poglavlje vec postoji u zavrsnom radu, funkcija tom poglavlju treba
	dodati novi sadrzaj i pri tome zadrzi postojeci (izmedju postojeceg i novog sadrzaja se dodaje prazan prostor). u slucaju da poglavlje
	ne postoji, ono se dodaje zajedno sa sadrzajem*/
	//parametri: nazivPoglavlja, sadrzajPoglavlja
	multimedijalni.DodajPoglavlje("Uvod", "U ovom poglavlju ce biti rijeci");
	multimedijalni.DodajPoglavlje("Uvod", "o multimedijalnim sistemima koji se danas koriste");
	multimedijalni.DodajPoglavlje("Uvod", "u savremenom poslovanju");
	multimedijalni.DodajPoglavlje("Vrste multimedijalnih sistema", "Danas se moze govoriti o nekoliko vrsta multimedijalnih sistema, a neke od najznacajnijih su ...");
	multimedijalni.DodajPoglavlje("Teorija multimedije", "Sadrzaj koji bi trebao stajati na pocetku treceg poglavlja zavrsnog rada o multimediji studenta IB130011");

	//nazivPoglavlja, ocjena
	multimedijalni.OcijeniPoglavlje("Uvod", 8);
	multimedijalni.Ispis();

	/*funkcija DodajZavrsniRad ima zadatak da odredjenom nastavniku dodijeli mentorstvo na zavrsnom radu. sprijeciti dodavanje zavrsnih
	radova sa istom temom kao i mogucnost da jedan student kod istog nastavnika posjeduje vise zavrsnih radova*/
	// zavrsniRad
	if (nastavnici[0]->DodajZavrsniRad(multimedijalni))
		cout << "Zavrsni rad uspjesno dodat!" << endl;
	if (nastavnici[0]->DodajZavrsniRad(podrsa_operaterima))
		cout << "Zavrsni rad uspjesno dodat!" << endl;
	if (!nastavnici[0]->DodajZavrsniRad(podrsa_operaterima))//dupliranje rada, onemoguciti dodavanje
		cout << "Zavrsni rad nije dodat!" << endl;
	if (!nastavnici[0]->DodajZavrsniRad(kriptografija)) //studentu vec dodijeljen rad, onemoguciti dodavanje
		cout << "Zavrsni rad nije dodat!" << endl;
	if (nastavnici[1]->DodajZavrsniRad(analiza_sigurnosti))
		cout << "Zavrsni rad uspjesno dodat!" << endl;


	/*funkcija ZakaziOdbranuRada ima zadatak da studentu sa proslijedjenim brojem indeksa zakaze odbranu zavrsnog rada sto podrazumijeva
	izracunavanje konacne ocjene i definisanje datuma odbrane. odbrana rada se moze zakazati samo studentu koji je rad prethodno prijavio.
	Za zakazivanje odbrane rada moraju biti zadovoljeni sljedeci uslovi:
	1. zavrsni rad mora imati broj poglavlja veci od minimalnog
	2. svako poglavlje mora imati broj karaktera veci od minimalnog
	3. svako poglavlje mora biti prihvaceno/odobreno
	ukoliko su zadovoljeni prethodni kriteriji, izracunava se konacna ocjena rada (prosjek ocjena svih poglavlja), postavlja datum odbrane
	rada i vraca pokazivac na rad kome je zakazana odbrana.u slucaju da student sa primljenim brojem indeksa nije prijavio zavrsni rad ili
	neki od postavljenih kriterija nije zadovoljen, funkcija vraca nullptr.
	*/

	//paramteri: brojIndeksa, datumOdbrane
	ZavrsniRad* zr1 = nastavnici[0]->ZakaziOdbranuRada("IB120021", "25.09.2019");
	if (zr1 != nullptr)
		zr1->Ispis();

	zr1 = nastavnici[0]->ZakaziOdbranuRada("IB180081", "25.09.2019");//student sa brojem indeksa IB180081 jos uvijek nije prijavio rad
	if (zr1 == nullptr)
		cout << "Odbrana ne moze biti zakazana!" << endl;

	//ispisuje sve podatke o nastavniku i njegovim mentorstvima
	nastavnici[0]->Ispis();
	nastavnici[1]->Ispis();

	///*rekurzivna funkcija PronadjiNajStudenta ima zadatak da pronadje posljednjeg studenta koji je zavrsni rad odbranio kod
	//nastavnika/mentora, te tom prilikom ostvario (odnosi se na studenta) ocjenu vecu od proslijedjene (npr. 8.2)*/
	////parametri PronadjiNajStudenta: nastavnici, brojNastavnika, minimalnaKonacnaOcjena
	//char* indeksStudenta = PronadjiNajStudenta(nastavnici, max, 8.2);
	//if (indeksStudenta != nullptr)
	//	cout << crt << "Posljednji student " << " -> " << indeksStudenta << crt;

	for (int i = 0; i < max; i++) {
		nastavnici[i]->Dealociraj();
		delete nastavnici[i];
		nastavnici[i] = nullptr;
	}

	system("pause");
	return 0;
}
