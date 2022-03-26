
#include <iostream>
using namespace std;



#define Befor_Refact  // do wyboru kompilacji kodu nalezy rowniez "After_Refact" ||||Befor_Refact




#ifdef After_Refact

class Zamowienie;
enum typKlienta { nowy, staly, zaprzyjazniony, vip };
class Klient
{
	Zamowienie* m_zamowienie = nullptr;
	typKlienta m_typ;
	double m_odlegoscDostawy;
	const double dystans[3]{ 10.0,100.0,500.0 };
	const double koszty_dostawy[4]{ 1.0,2.0,3.0,3.5 };
	double cenaBrutto{ 0 };
public:
	Klient() {};
	Klient(typKlienta typ, double odl) : m_typ(typ), m_odlegoscDostawy(odl) {}

	virtual double rabat() {return 0.0;  } //domyslne dla nowego

	//pomocnicze gettery
	typKlienta get_typ() { return m_typ; }
	int get_odleglosc() { return m_odlegoscDostawy; }
	double get_naliczonaNaleznosc() { cenaBrutto = m_zamowienie->get_cenaBrutto(); }

	//pomocnicze settery
	void set_naleznosc(double naleznosc) { cenaBrutto = naleznosc; }
	void set_typ(typKlienta _typ) { m_typ = _typ; }
	void set_(int _odleglosc) { m_odlegoscDostawy = _odleglosc; }


	double odleglModyfKosztuDostawy()
	{
		if (m_odlegoscDostawy < dystans[0]) // <10 km		// tabelaryzacja
			return koszty_dostawy[0];
		else if (m_odlegoscDostawy < dystans[1]) // <100 km
			return koszty_dostawy[1];
		else if (m_odlegoscDostawy < dystans[2]) // <500 km
			return koszty_dostawy[2];

		else return koszty_dostawy[3]; // >500
	}
};
class NowyKlient : public Klient { // polimorfizm
public:
	NowyKlient(int typ, double odl) {
	};

	double rabat() override {return 0.00;}
};
class StalyKlient : public Klient {
	StalyKlient();
	double rabat() override {return 0.05;}
};
class ZaprzyjaznionyKlient : public Klient {
	ZaprzyjaznionyKlient(int typ, double odl) {};

	double rabat() override {return 0.10;}
};
class VIPKlient : public Klient {
	VIPKlient();
	double rabat() override {return 0.15;}
};
class Podatek {
protected:

	double masaDetaliczna;

	double Netto;

public:
	Podatek() {};
	Podatek(double cena, double masa) :Netto(cena), masaDetaliczna(masa) {};

	virtual double podatek() {return 0.0;} //domyslna wartosc

};

class Vat7 : public Podatek {

public:
	Vat7(double cena, double masa) {
	};
	double podatek()override {return 0.07;}
};

class Vat11 : public Podatek {

public:
	Vat11(double cena, double masa) {};
	double podatek()override {return 0.11;}
};

class Vat15 : public Podatek {

public:
	Vat15(double cena, double masa) {};
	double podatek()override {return 0.15;}
};

class Vat22 : public Podatek {

public:
	Vat22(double cena, double masa) {};
	double podatek()override {return 0.22;}
};
class Zamowienie
{
private:
	Klient* klient = nullptr;	//asocjacja
	double cenaBrutto{ 0 }, cenaNetto, marza{ 0.5 };
	int ilosc, typPodatku;
	double kosztDojazdu{ 5.0 };
public:
	//Zamowienie() {};
	Zamowienie(int ilosc, double cenaNetto, double masaDetaliczna, int typPodatku, Klient  Klienta, double odelgosc)
	{
		setKlient(Klienta);
		/* Obliczenie ceny brutto: */
		switch (typPodatku)
		{
		default: case 0: //zwykły vat
			cenaBrutto = cenaNetto * marza * 0.22; break;
		case 1: // vat 7%
			cenaBrutto = cenaNetto * marza * 0.07; break;
		case 2: // vat 11&
			cenaBrutto = cenaNetto * marza * 0.11; break;
		case 3: // vat 15%
			cenaBrutto = cenaNetto * marza * 0.15; break;
		}
		/* Obliczenie sumy zamówienia: */
		cenaBrutto *= ilosc;

		Klient temp(klient->get_typ(), klient->get_odleglosc());
		/* Doliczenie kosztów transportu (5.0 to koszt stały)*/
		
		cenaBrutto += (kosztDojazdu + 3.0 * masaDetaliczna * temp.odleglModyfKosztuDostawy());

		/*Odliczenie rabatu: */
		cenaBrutto -= temp.rabat() * cenaBrutto;
		set_cenaBrutto(cenaBrutto);

	}
	void setKlient(Klient m_kl) { klient = &m_kl; }
	void set_cenaBrutto(double cena) { cenaBrutto = cena; }

	double get_cenaBrutto() { return cenaBrutto; }
	~Zamowienie() {}


};
void test_pierwszy() {
	cout << "Test Pierwszy (Nowy, <10km):";
	Klient pierwszy(nowy, 9);
	Zamowienie zamowienie_klienta(200, 1500, 10, 0.5, 0, pierwszy.get_typ(), pierwszy.get_odleglosc());
	zamowienie_klienta.setKlient(pierwszy);
	double znizka = pierwszy.rabat();
	double obliczonaNaleznoscDrugi = (200, 1500, 10, 0.5, 0, pierwszy.get_typ(), pierwszy.get_odleglosc());
	if (obliczonaNaleznoscDrugi == 29799 && znizka == 0.1) {
		cerr << " OK";
	}
	else 		cerr << " FAILED!";

};

void test_drugi() {
	cout << "Test Drugi (Zaprzyjazniony, >500km):";
	ZaprzyjaznionyKlient drugi(zaprzyjazniony, 5000);
	Zamowienie zamowienie_klienta2(200, 1500, 10, 0.5, 0, drugi.get_typ(), drugi.get_odleglosc());
	zamowienie_klienta2.setKlient(drugi);
	double znizka = drugi.rabat();
	if (zamowienie_klienta2.get_cenaBrutto() == 29799 && znizka == 0.1) {
		cerr << " OK";
	}
	else 		cerr << " FAILED!";

};
void main(void)
{
	test_pierwszy();
	cout << endl;
	test_drugi();
}


#endif 

#ifdef Befor_Refact


class Klient
{
	int m_typ;
	double m_odlegoscDostawy;
public:
	Klient(int typ, double odl)
		: m_typ(typ), m_odlegoscDostawy(odl)
	{}
	double wysokoscRabatu()
	{
		switch (m_typ)
		{
		default: case 0: // Nowy Klient
			return 0.00;
		case 1: //Stały Klient
			return 0.05;
		case 2: //Zaprzyjaxiony Kliejnt
			return 0.10;
		case 3: // V.I.P.
			return 0.15;
		}
	}
	double odleglModyfKosztuDostawy()
	{
		if (m_odlegoscDostawy < 10.0) //10 km
			return 1.0;
		else if (m_odlegoscDostawy < 100.0) // 100 km
			return 2.0;
		else if (m_odlegoscDostawy < 500.0) // 500 km
			return 3.0;
		else return 3.5;
	}
	int get_typ() { return m_typ; }
	int get_odleglosc() { return m_odlegoscDostawy; }
};

double naliczNaleznosc(int ilosc, double cenaNetto, double masaDetaliczna,
	double marza, int typPodatku, int typKlienta, double odelgosc)
{
	/* Obliczenie ceny brutto: */
	double cenaBrutto = 0;
	switch (typPodatku)
	{
	default: case 0: //zwykły vat
		cenaBrutto = cenaNetto * marza * 0.22; break;
	case 1: // vat 7%
		cenaBrutto = cenaNetto * marza * 0.07; break;
	case 2: // vat 11&
		cenaBrutto = cenaNetto * marza * 0.11; break;
	case 3: // vat 15%
		cenaBrutto = cenaNetto * marza * 0.15; break;
	}
	/* Obliczenie sumy zamówienia: */
	cenaBrutto *= ilosc;

	Klient temp(typKlienta, odelgosc);
	/* Doliczenie kosztów transportu (5.0 to koszt stały)*/
	cenaBrutto += (5.0 + 3.0 * masaDetaliczna * temp.odleglModyfKosztuDostawy());

	/*Odliczenie rabatu: */
	cenaBrutto -= temp.wysokoscRabatu() * cenaBrutto;
	return cenaBrutto;
}

void test_pierwszy() {
	cout << "Test Pierwszy (NEW, <10km):";
	Klient pierwszy(0, 9);
	double znizka = pierwszy.wysokoscRabatu();
	double obliczonaNaleznoscPierwszy = naliczNaleznosc(200, 1500, 10, 0.5, 0, pierwszy.get_typ(), pierwszy.get_odleglosc());
	if (obliczonaNaleznoscPierwszy == 33035 && znizka == 0) {
		cerr << " OK";
	}
	else 		cerr << " FAILED!";


};
void test_drugi() {
	cout << "Test Pierwszy (Zaprzyjazniony, >500km):";
	Klient drugi(2, 5000);
	double znizka = drugi.wysokoscRabatu();
	double obliczonaNaleznoscDrugi = naliczNaleznosc(200, 1500, 10, 0.5, 0, drugi.get_typ(), drugi.get_odleglosc());
	if (obliczonaNaleznoscDrugi == 29799 && znizka == 0.1) {
		cerr << " OK";
	}
	else 		cerr << " FAILED!";

};
void main(void)
{
	test_pierwszy();
	cout << endl;
	test_drugi();
}

#endif 




