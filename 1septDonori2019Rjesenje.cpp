#include <iostream>
#include <vector>
#include<string>
#include<thread>
#include<mutex>
#include<regex>
#include<sstream>

using namespace std;
const char* crt = "\n-------------------------------------------\n";
enum OznakaKrvneGrupe { O, A, B, AB };
const char* OznakaKrvneGrupeString[] = { "O", "A", "B", "AB" };
mutex m;
char* Alociraj(const char* sadrzaj) {
	if (sadrzaj == nullptr)return nullptr;
	int vel = strlen(sadrzaj) + 1;
	char* temp = new char[vel];
	strcpy_s(temp, vel, sadrzaj);
	return temp;
}

class Datum {
	int _dan, _mjesec, _godina;
public:
	Datum(int dan, int mjesec, int godina) :
		_dan(dan), _mjesec(mjesec), _godina(godina) {}
	friend ostream& operator<<(ostream& COUT, const Datum& obj) {
		COUT << obj._dan << " " << obj._mjesec << "." << obj._godina << endl;
		return COUT;
	}
	string ToString(string separator = ".")const{
		stringstream ss;
		ss << _dan << separator << _mjesec << separator << _godina;
		return ss.str();
	}
	int operator -(const Datum& d) {
		return (_dan + _mjesec * 30 + _godina * 365) - (d._dan + d._mjesec * 30 + d._godina * 365);
	}
};
template <class T1, class T2, int max>
class Kolekcija {
	T1 _elementi1[max];
	T2 _elementi2[max];
	int* _trenutnoElemenata;
	int _keyElement; // kljucni element, a podrazumijevano T1 predstavlja kljucni element i tada je vrijednost 1, a u slucaju da je T2 onda ce vrijednost biti 2
public:
	Kolekcija(int keyElement = 1) {
		_trenutnoElemenata = new int(0);
		_keyElement = keyElement;
	}
	Kolekcija(const Kolekcija &k){
		_trenutnoElemenata = new int(*k._trenutnoElemenata);
		_keyElement = k._keyElement;
		for (size_t i = 0; i < *k._trenutnoElemenata; i++)
		{
			_elementi1[i] = k._elementi1[i];
			_elementi2[i] = k._elementi2[i];
		}
	}
	~Kolekcija() {
		delete _trenutnoElemenata;
		_trenutnoElemenata = nullptr;
	}
	bool AddElement(T1 ele1, T2 ele2) {
		if (*_trenutnoElemenata == max)
			return false;
		_elementi1[*_trenutnoElemenata] = ele1;
		_elementi2[*_trenutnoElemenata] = ele2;
		(*_trenutnoElemenata)++;
		return true;
	}
	void SetElement2(int lokacija, T2 ele2) {
		_elementi2[lokacija] = ele2;
	}
	void SetKeyElement(int keyElement) {
		_keyElement = keyElement;
	}
	int GetTrenutno() const { return *_trenutnoElemenata; }
	int GetMax() const { return max; }

	T1 GetElement1(int lokacija) const {
		return _elementi1[lokacija];
	}
	T2 GetElement2(int lokacija) const {
		return _elementi2[lokacija];
	}
	friend ostream& operator<<(ostream& COUT, const Kolekcija& obj) {
		for (size_t i = 0; i < obj.GetTrenutno(); i++)
			COUT << obj.GetElement1(i) << " " << obj.GetElement2(i) << endl;
		return COUT;
	}
	void Sort(string type){
		bool prolaz = true;
		while (prolaz) {
			prolaz = false;
			for (size_t i = 0; i < *_trenutnoElemenata - 1; i++)
			{
				if (type == "ASC" && _keyElement == 1 && _elementi1[i] > _elementi1[i + 1]) {
					swap<T1, T1>(_elementi1[i], _elementi1[i + 1]);
					swap<T2, T2>(_elementi2[i], _elementi2[i + 1]);
					prolaz = true;
				}
				else if (type == "DESC" && _keyElement == 1 && _elementi1[i] < _elementi1[i + 1]) {
					swap<T1, T1>(_elementi1[i], _elementi1[i + 1]);
					swap<T2, T2>(_elementi2[i], _elementi2[i + 1]);
					prolaz = true;
				}
				else if (type == "ASC" && _keyElement == 2 && _elementi2[i] > _elementi2[i + 1]) {
					swap<T1, T1>(_elementi1[i], _elementi1[i + 1]);
					swap<T2, T2>(_elementi2[i], _elementi2[i + 1]);
					prolaz = true;
				}
				else if (type == "DESC" && _keyElement == 2 && _elementi2[i] < _elementi2[i + 1]) {
					swap<T1, T1>(_elementi1[i], _elementi1[i + 1]);
					swap<T2, T2>(_elementi2[i], _elementi2[i + 1]);
					prolaz = true;
				}
			}
		}
	}
};

class KrvnaGrupa {
	/*postoje 4 krvne grupe koje su navedene u enumeracije OznakaKrvneGrupe, pri cemu svaka od navedenih moze imati pozitivan (+) i negativan (-) Rh faktor*/
	OznakaKrvneGrupe _oznaka;//npr: AB
	char _rhFaktor; // + ili -
	/*prethodno pomenuto je izuzetno bitno iz razloga sto postoje jasna pravila vezana za darivanje krvi tj. koji primalac moze primiti krv od kojeg donatora sto je prikazano u tabeli, a naredna dva vector-a su zaduzena da cuvaju informacije o tome, npr. za krvnu grupu A+ vector donori ce cuvati vrijednosti: A+ i AB+ */
	vector<KrvnaGrupa> _donori; //krvne grupe kojima odredjena krvna grupa moze donirati krv.
	vector<KrvnaGrupa> _primaoci; //krvne grupe od kojih odredjena krvna grupa moze primiti krv.
public:
	KrvnaGrupa(OznakaKrvneGrupe oznaka, char rhFaktor) :
		_oznaka(oznaka), _rhFaktor(rhFaktor) {}
	void SetDonori(vector<KrvnaGrupa> donori) { _donori = donori; }
	void SetPrimaoci(vector<KrvnaGrupa> primaoci) { _primaoci = primaoci; }
	OznakaKrvneGrupe& GetOznakaKrvneGrupe() { return _oznaka; }
	vector<KrvnaGrupa>& GetDonori() { return _donori; }
	vector<KrvnaGrupa>& GetPrimaoci() { return _primaoci; }
	string ToString()const {
		stringstream ss;
		ss << crt;
		ss << "Krvna grupa -> " << _oznaka << _rhFaktor;
		ss << crt;
		ss << "Donori-> ";
		for (vector<KrvnaGrupa>::const_iterator i = _donori.begin(); i != _donori.end(); i++)
		{
			ss << _oznaka << _rhFaktor << ", ";
		}
		ss << endl;
		ss << "Primaoci -> ";
		for (vector<KrvnaGrupa>::const_iterator i = _primaoci.begin(); i != _primaoci.end(); i++)
		{
			ss << _oznaka << _rhFaktor << ", ";
		}
		ss << crt;
		return ss.str();
	}
	/*Ispis podataka o krvnoj grupi treba biti u formatu:
	-------------------------------------------
	Krvna grupa -> 0+
	-------------------------------------------
	Donori  ->  0+, A+, B+, AB+
	Primaoci ->	0+, 0-
	-------------------------------------------
	*/
	bool operator==(const KrvnaGrupa& k) {
		if (_oznaka == k._oznaka && _rhFaktor == k._rhFaktor) return true; return false;
	}
};

class Osoba {
protected:
	char* _imePrezime;
	KrvnaGrupa _krvnaGrupa;
public:
	Osoba(const char* imePrezime, KrvnaGrupa krvnaGrupa) : _krvnaGrupa(krvnaGrupa) {
		_imePrezime = Alociraj(imePrezime);
	}
	Osoba(const Osoba &o):_krvnaGrupa(o._krvnaGrupa){ _imePrezime = Alociraj(o._imePrezime); }
	~Osoba() {
		delete[] _imePrezime;
		_imePrezime = nullptr;
	}
	char* GetImePrezime() { return _imePrezime; }
	KrvnaGrupa& GetKrvnaGrupa() { return _krvnaGrupa; }
	virtual void Info() = 0;
};
bool ValidanFormat(string tel) {
	if (regex_match(tel, regex("(\\d{3})([/-])(\\d{3})([-](\\d{3}))"))) return true; else return false;
}
class Donor:public Osoba {
	//u slucaju da broj telefona nije u validnom formatu potrebno ga je postaviti na podrazumijevanu vrijednost: 000-000-000
	//ValidanFormat treba biti globalna funkcija, a ne èlanica
	string _telefon; //regex: 000/000-000 ili 000-000-000
	Datum _datumPosljednjegDoniranja;
	bool _podsjetiMe;
	bool _kontaktirajMe;//u slucaju potrebe za doniranjem krvi, donor se slaze da bude kontaktiran
public:
	Donor(const char* imePrezime, KrvnaGrupa krvnaGrupa, string telefon, Datum dpd, bool remind = true, bool contact = true):Osoba(imePrezime, krvnaGrupa), _telefon(telefon), _datumPosljednjegDoniranja(dpd), _podsjetiMe(remind), _kontaktirajMe(contact)  {
		if (ValidanFormat(telefon))
			_telefon = telefon;
		else
			_telefon = "000-000-000";
	}
	Donor(const Donor &d):Osoba(d), _telefon(d._telefon), _datumPosljednjegDoniranja(d._datumPosljednjegDoniranja), _podsjetiMe(d._podsjetiMe), _kontaktirajMe(d._kontaktirajMe) {}
	Datum& GetDatumPosljednjegDoniranja() { return _datumPosljednjegDoniranja; }
	friend ostream& operator<<(ostream& o, const Donor& d) {
		o << "Ime i prezime -> " << d._imePrezime << endl;
		o << "Telefon -> " << d._telefon << endl;
		return o;
	}
	void Info() { cout << *this; }
	void SetDatumPosljednjegDoniranja(Datum d) { _datumPosljednjegDoniranja = d; }
	bool GetKontaktirajMe() { return _kontaktirajMe; }
	string GetBrojTelefona() { return _telefon; }
};

class Zahtjev {
	string _ustanova;
	Datum _datumZahtjeva;
	KrvnaGrupa _krvnaGrupa;
	double _kolicina;
public:
	Zahtjev(string ustanova, Datum datum, KrvnaGrupa krvnaGrupa, double kolicina) :
		_ustanova(ustanova), _datumZahtjeva(datum), _krvnaGrupa(krvnaGrupa), _kolicina(kolicina) { }
	string GetUstanova() { return _ustanova; }
	Datum GetDatumZahtjeva() { return _datumZahtjeva; }
	KrvnaGrupa GetKrvnaGrupa() { return _krvnaGrupa; }
	double GetKolicina() { return _kolicina; }

	friend ostream& operator<<(ostream& COUT, const Zahtjev& obj) {
		COUT << "Ustanova: " << obj._ustanova << endl;
		COUT << "Datum: " << obj._datumZahtjeva.ToString() << endl;//kreirati metodu koja vraća datum kao niz karaktera
		COUT << "Krvna grupa: " << obj._krvnaGrupa.ToString() << endl;
		COUT << "Kolicina: " << obj._kolicina << endl;
		return COUT;
	}
};

class TransfuzijskiCentar {
	//stanje zaliha za svaku pojedinu krvnu grupu
	Kolekcija<KrvnaGrupa*, double, 8> _zalihe;
	//evidentira svaku donaciju krvi
	Kolekcija<Osoba*, double, 100> _donacije;
	vector<Zahtjev> _zahtjevi;
public:
	Kolekcija<KrvnaGrupa*, double, 8>& GetZalihe() { return _zalihe; }
	Kolekcija<Osoba*, double, 100>& GetDonacije() { return _donacije; }
	vector<Zahtjev>& GetZahtjeve() { return _zahtjevi; }
	
	bool AddDonaciju(Datum d, Osoba* o, float kolicina) {
		bool postoji = false;
		for (size_t i = 0; i < _zalihe.GetTrenutno(); i++)
		{
			if (*_zalihe.GetElement1(i) == o->GetKrvnaGrupa()) {
				_zalihe.SetElement2(i, _donacije.GetElement2(i) + kolicina);
				postoji = true;
			}
		}
		if (!postoji)
			_zalihe.AddElement(new KrvnaGrupa(o->GetKrvnaGrupa()), kolicina);
		Donor* donator = dynamic_cast<Donor*>(o);
		for (size_t i = 0; i < _donacije.GetTrenutno(); i++)
		{
			if (_donacije.GetElement1(i) == o) {
				dynamic_cast<Donor*>(_donacije.GetElement1(i))->SetDatumPosljednjegDoniranja(d);
				_donacije.SetElement2(i, _donacije.GetElement2(i) + kolicina);
				return true;
			}
		}
		_donacije.AddElement(new Donor(*donator), kolicina);
		return true;
	}
	/*
	prilikom svake donacije je potrebno povecati zalihe za tu krvnu grupu
	ukoliko krvna grupa vec postoji na zalihi onda se uvecava samo njena kolicina
	takodjer, donoru se postavlja nova vrijednost datuma posljednje donacije
	*/
	//datum donacije, donor, kolicina
	bool DodajZahtjev(Zahtjev z) {
		bool postoji = false;
		for (size_t i = 0; i < _zalihe.GetTrenutno(); i++)
		{
			for (size_t j = 0; j < z.GetKrvnaGrupa().GetPrimaoci().size(); j++)
			{
				if (*_zalihe.GetElement1(i) == z.GetKrvnaGrupa().GetPrimaoci()[j] && _zalihe.GetElement2(i) == z.GetKolicina())
					_zalihe.SetElement2(i, _zalihe.GetElement2(i) - z.GetKolicina());
				postoji = true;
				break;
			}
		}
		if (!postoji) {
			thread t([this, &z]() {
				m.lock();
				for (size_t i = 0; i < this->GetDonacije().GetTrenutno(); i++)
				{
					Donor* d = dynamic_cast<Donor*>(this->GetDonacije().GetElement1(i));
					for (size_t j = 0; j < d->GetKrvnaGrupa().GetDonori().size(); j++)
					{
						if (d->GetKrvnaGrupa().GetDonori()[j] == z.GetKrvnaGrupa() && d->GetKontaktirajMe() && (d->GetDatumPosljednjegDoniranja() - z.GetDatumZahtjeva()) >= 90)
							cout << "Saljem sms na broj telefona -> " << endl;
							cout << "Br tel -> " << d->GetBrojTelefona();
							cout << "Potrebne zalihe krvi.." << endl;
							this_thread::sleep_for(chrono::seconds(2));
					}
				}
				m.unlock();
			});
			t.join();
			return false;

		}
		_zahtjevi.push_back(z);
		return true;
	}
	/*ukoliko transfuzijski centar posjeduje zahtijevane kolicine na stanju, odmah ce ih ustupiti prema zahtjevu,
	a u slucaju da ne posjeduje onda ce (koristeci multithreading) kontaktirati (poslati SMS poruku sa odgovarajucim sadrzajem)
	sve donore koji zadovoljavaju sljedece uslove:
	- mogu donirati krv zahtijevanoj krvnoj grupi
	- oznacili su da zele biti kontaktirani
	- nisu davali krv u posljednjih 90 dana ili 3 mjeseca
	*/
	~TransfuzijskiCentar() {
		for (size_t i = 0; i < _zalihe.GetTrenutno(); i++)
			delete _zalihe.GetElement1(i);
		for (size_t i = 0; i < _donacije.GetTrenutno(); i++)
			delete _donacije.GetElement1(i);
	}
	Datum* GetDatumPosljednjegDoniranja(string ime, string telefon) {
		Datum *datumPosljednjeg = nullptr;
		for (size_t i = 0; i < _donacije.GetTrenutno(); i++)
		{
			Donor* d = dynamic_cast<Donor*>(_donacije.GetElement1(i));
			for (size_t j = 0; j < _donacije.GetElement1(i)->GetKrvnaGrupa().GetDonori().size(); j++)
			{
				if (d->GetImePrezime() == ime.c_str() || d->GetBrojTelefona() == telefon.c_str()) {
					datumPosljednjeg = &d->GetDatumPosljednjegDoniranja();
					return datumPosljednjeg;
				}
			}			
		}
		return nullptr;
	}
	/*
	parametri: imePrezime, brojTelefona
	vraca pokazivac na datum posljednjeg darivanja krvi. u slucaju da ne postoje trazeni podaci, metoda vraca nullptr
	*/
	void PrintajZahvalnice(int broj) {
		Kolekcija<Osoba*, double, 100> donatoriKolicinaTemp(_donacije);
		donatoriKolicinaTemp.SetKeyElement(2);
		donatoriKolicinaTemp.Sort("DESC");
		if (broj > donatoriKolicinaTemp.GetTrenutno()) throw exception("Ne postoji toliko donatora na spisku.");
		for (size_t i = 0; i < broj; i++)
		{
			cout << "Zahvaljujemo se donatorima -> " << *dynamic_cast<Donor*>(donatoriKolicinaTemp.GetElement1(i));
			cout << " na donaciji kolicine -> " << donatoriKolicinaTemp.GetElement2(i) << endl;
		}

	}
};

void main() {

	/****************************************************************************
	1. SVE KLASE TREBAJU POSJEDOVATI ADEKVATAN DESTRUKTOR
	2. NAMJERNO IZOSTAVLJANJE KOMPLETNIH I/ILI POJEDINIH DIJELOVA DESTRUKTORA KOJI UZROKUJU RUNTIME ERROR CE BITI OZNACENO KAO "RE"
	3. SPAŠAVAJTE PROJEKAT KAKO BI SE SPRIJECILO GUBLJENJE URA?ENOG ZADATKA
	4. PROGRAMSKI CODE SE TAKO?ER NALAZI U FAJLU CODE.TXT
	5. NAZIVI FUNKCIJA, TE BROJ I TIP PARAMETARA MORAJU BITI IDENTIENI ONIMA KOJI SU KORIŠTENI U TESTNOM CODE-U, OSIM U SLUCAJU DA POSTOJI ADEKVATAN RAZLOG ZA NJIHOVU MODIFIKACIJU. OSTALE, POMOANE FUNKCIJE MOŽETE IMENOVATI I DODAVATI PO ŽELJI.
	6. IZUZETAK BACITE U FUNKCIJAMA U KOJIMA JE TO NAZNACENO.
	7. IMATE POTPUNU SLOBODU DA U MAIN FUNKCIJI, ZA POTREBE TESTIRANJA, DODAJETE VISE POZIVA ILI SALJETE DRUGE VRIJEDNOSTI PARAMETARA
	****************************************************************************/

	Datum dat_12_01_2019(12, 1, 2019), dat_01_02_2019(1, 2, 2019);

	Kolekcija<int, int, 10> kolekcija1;
	for (size_t i = 0; i < kolekcija1.GetMax(); i++)
		if (!kolekcija1.AddElement(i, 170000 + i))
			cout << "Elementi " << i << " i " << 170000 + i << " nisu dodati u kolekciju" << endl;

	cout << kolekcija1.GetElement1(0) << " " << kolekcija1.GetElement2(0) << endl;

	kolekcija1.Sort("ASC"); //kljucne rijeci za sortiranje su ASC i DESC, a sortiranje se vrsi prema kljucnom elementu
	cout << kolekcija1 << endl << endl << endl;
	kolekcija1.Sort("DESC");
	cout << kolekcija1 << endl;

	kolekcija1.SetKeyElement(2);//postavlja kljucni element na T2

	kolekcija1.Sort("DESC"); //sada se sortiranje vrsi po elementu T2
	cout << kolekcija1 << endl;

	KrvnaGrupa
		O_poz(O, '+'), O_neg(O, '-'),
		A_poz(A, '+'), A_neg(A, '-'),
		B_poz(B, '+'), B_neg(B, '-'),
		AB_poz(AB, '+'), AB_neg(AB, '-');

	vector<KrvnaGrupa> donori_O_poz{ O_poz, A_poz, B_poz, AB_poz };
	O_poz.SetDonori(donori_O_poz);
	//ili krace napisano
	O_poz.SetPrimaoci(vector<KrvnaGrupa>{O_poz, O_neg});
	A_poz.SetDonori(vector<KrvnaGrupa>{A_poz, AB_poz});
	A_poz.SetPrimaoci(vector<KrvnaGrupa>{O_poz, A_poz, O_neg, A_neg});

	B_poz.SetDonori(vector<KrvnaGrupa>{B_poz, AB_poz});
	B_poz.SetPrimaoci(vector<KrvnaGrupa>{O_poz, B_poz, O_neg, B_neg});

	
	cout << O_poz.ToString() << endl;

	Osoba* jasmin = new Donor("Jasmin Azemovic", B_poz, "061-111-222", Datum(12, 2, 2018), true, true);
	Osoba* adel = new Donor("Adel Handzic", A_neg, "061-222-333", Datum(9, 1, 2018), true, true);
	Osoba* goran = new Donor("Goran Skondric", B_neg, "061-333-444", Datum(9, 3, 2018), true, true);

	TransfuzijskiCentar tcMostar;
	
	tcMostar.AddDonaciju(Datum(20, 5, 2018), jasmin, 2.5);
	tcMostar.AddDonaciju(Datum(20, 5, 2018), adel, 3);
	tcMostar.AddDonaciju(Datum(6, 5, 2018), goran, 1.2);

	tcMostar.AddDonaciju(Datum(10, 9, 2018), jasmin, 2);
	tcMostar.AddDonaciju(Datum(18, 10, 2018), adel, 1.8);
	tcMostar.AddDonaciju(Datum(15, 9, 2018), goran, 3.8);
	cout << crt;
	cout << crt;
	cout << crt;
	cout << crt;
	Datum* posljednjeDoniranje = tcMostar.GetDatumPosljednjegDoniranja("Jasmin Azemovic", "061-111-222");

	Zahtjev zahtjev_0_poz("Tranfuziologija KCUS", Datum(18, 2, 2019), O_poz, 15),
		zahtjev_0_neg("Tranfuziologija Bakir Nakas", Datum(20, 2, 2019), O_neg, 8);

	
	if (tcMostar.DodajZahtjev(zahtjev_0_poz))
		cout << "Zahtijevane zalihe na stanju i bit ce vam ustupljene!" << endl;
	if (tcMostar.DodajZahtjev(zahtjev_0_neg))
		cout << "Zahtijevane zalihe na stanju i bit ce vam ustupljene!" << endl;
		
	//printa zahvalnice (zahvaljujemo se ime i prezime donoru na ukupno doniranih X doza krvi) za TOP 2 donatora krvi
	tcMostar.PrintajZahvalnice(2);

	delete adel;
	delete jasmin;
	delete goran;
	cin.get();
}
