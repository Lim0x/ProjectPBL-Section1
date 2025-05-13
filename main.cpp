#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <vector>
#include "json.hpp"
#include <ctime>
#include <iomanip>
#include <stdexcept>
#include <sstream>



using namespace std;
using json = nlohmann::json;


/**
 * @class Karta
 * @brief Reprezentuje karte.
 *
 *
 *  Klasa przechowuje podstawowe dane i funkcje zwiazane z kartami
 */
class Karta {
private:
	string numerKarty; ///< Numer karty
	string dataWaznosci; ///< Data waznosci karty
	string kodCVC; ///< Kod CVC karty
public:
	/**
	 * @brief Konstruktor klasy Karta.
	 *
	 * Inicjalizuje obiekt Karta z podanymi parametrami.
	 *
	 * @param numer Numer karty
	 * @param data Data waznosci karty
	 * @param kod Kod CVC karty
	 */
	Karta(string numer, string data, string kod) {
		this->numerKarty = numer;
		this->dataWaznosci = data;
		this->kodCVC = kod;
		
	}
	/**
	 * @brief Zwraca numer karty.
	 *
	 * @return Numer karty
	 */
	string getNumerKarty() const { return numerKarty; }
	/**
	 * @brief Zwraca date waznosci karty.
	 *
	 * @return Data waznosci karty
	 */
	string getDataWaznosci() const
	{
		return dataWaznosci.substr(0, 2) + "/20" + dataWaznosci.substr(2, 2);
	}
	/**
	 * @brief Zwraca kod CVC karty.
	 *
	 * @return Kod CVC karty
	 */
	string getKodCVC() const { return kodCVC; }
	/**
	 * @brief Ustala numer karty.
	 *
	 * @param numer Numer karty
	 */
	void setNumerKarty(string numer) { numerKarty = numer; }
	/**
	 * @brief Ustala date waznosci karty.
	 *
	 * @param data Data waznosci karty
	 */
	void setDataWaznosci(string data)
	{
		if (data.length() ==4) // Format "MMRR"
		{
			dataWaznosci = data;
		}
		else if (data.length()==7 && data[2] == '/') // Format "MM/YYYY"
		{
			dataWaznosci = data.substr(0, 2) + data.substr(5, 2);
		}
		else
		{
			throw invalid_argument("Niepoprawny format daty. Oczekiwano formatu MMRR lub MM/YYYY");
		}
	}
	/**
	 * @brief Ustala kod CVC karty.
	 *
	 * @param kod Kod CVC karty
	 */
	void setKodCVC(string kod) { kodCVC = kod; }
	/**
	 * @brief Zwraca typ karty.
	 * 
	 * @return Typ karty (np. Debetowa, Kredytowa)
	 */
	virtual string getTypKarty() const = 0;
	/**
	 * @brief Sprawdza, czy karta jest wazna.
	 * @return true jeśli karta jest wazna, false w przeciwnym razie
	 */
	virtual bool czyWazna() const
	{
		if (dataWaznosci.length() != 4) {
			return false; // Niepoprawny format daty
		}
		try{
			// Pobiera aktualny czas i konwertuje na lokalny dzien, miesiac
		time_t now = time(nullptr);
		tm today;
		localtime_s(&today, &now);

		int miesiac = stoi(dataWaznosci.substr(0, 2));
		int rok = 2000 + stoi(dataWaznosci.substr(2, 2));

		return (rok > today.tm_year + 1900 || (rok == today.tm_year + 1900 && miesiac >= today.tm_mon + 1));
		}
		catch (...) {
			return false; // Niepoprawny format daty
		}
	}
	/**
	 * @brief Wyswietla informacje o karcie.
	 *
	 * Funkcja wyswietla szczegóły karty, takie jak numer karty, data waznosci, kod CVC oraz status karty.
	 */
	virtual void wyswietlInformacje() const
	{
		cout << "===== DANE KARTY =====" << endl;
		cout << "Typ karty: " << getTypKarty() << endl;
		cout << "Numer karty: " << getNumerKarty() << endl;
		cout << "Data waznosci: " << getDataWaznosci() << endl;
		cout << "Kod CVC: " << getKodCVC() << endl;
		cout << "Status: " << (czyWazna() ? "Wazna" : "Niewazna") << endl;
		cout << "======================" << endl;
	}
};


/**
 * @class Lokata
 * @brief Reprezentuje lokate.
 *
 * Klasa przechowuje kwote, oprocentowanie oraz date oddania.
 */
class Lokata {
	float kwota; ///< Kwota lokaty
	float oprocentowanie; ///< Oprocentowanie lokaty
	string dataOddania; ///< Data oddania lokaty
	string powiazaneKonto; ///< Powiązane konto bankowe

public:
	/**
	 * @brief Konstruktor klasy Lokata.
	 *
	 * Inicjalizuje obiekt Lokata z podanymi parametrami.
	 *
	 * @param kwota Kwota lokaty
	 * @param oprocentowanie Oprocentowanie lokaty
	 * @param dataOddania Data oddania lokaty
	 * @param wlasciciel Wlasciciel lokaty
	 */
	Lokata(float kwota, float oprocentowanie, const string& dataOddania, string wlasciciel) {
		this->kwota = kwota;
		this->oprocentowanie = oprocentowanie;
		this->dataOddania = dataOddania;
		this->powiazaneKonto = wlasciciel;
	}
	/**
	 * @brief Konstruktor domyslny klasy Lokata.
	 *
	 * Inicjalizuje obiekt Lokata z domyslnymi wartosciami.
	 */
	Lokata() = default;
	/**
	 * @brief Ustala kwote lokaty.
	 *
	 * @param kwota Kwota lokaty
	 */
	void setKwota(float kwota) { this->kwota = kwota; }
	/**
	 * @brief Ustala oprocentowanie lokaty.
	 *
	 * @param oprocentowanie Oprocentowanie lokaty
	 */
	void setOprocentowanie(float oprocentowanie) { this->oprocentowanie = oprocentowanie; }
	/**
	 * @brief Ustala date oddania lokaty.
	 *
	 * @param dataOddania Data oddania lokaty
	 */
	void setDataOddania(const string& dataOddania) { this->dataOddania = dataOddania; }
	/**
	 * @brief Zwraca kwote lokaty.
	 *
	 * @return Kwota lokaty
	 */
	float getKwota() const { return kwota; }

	/**
	 * @brief Zwraca oprocentowanie lokaty.
	 *
	 * @return Oprocentowanie lokaty
	 */
	float getOprocentowanie() const { return oprocentowanie; }

	/**
	 * @brief Zwraca date oddania lokaty.
	 *
	 * @return Data oddania lokaty
	 */
	const string& getDataOddania() const { return dataOddania.substr(0, 2) + "/20"
		+ dataOddania.substr(2, 2); }

	/**
	 * @brief Zwraca konto powiazane z dana lokata.
	 *
	 * @return Powiazane konto
	 */
	string getPowiazaneKonto() const { return powiazaneKonto; }

	/**
	 * @brief Sprawdza czy lokata jest aktywna.
	 *
	 * @return true jeśli lokata jest aktywna, false w przeciwnym razie
	 */
	bool czyAktywna() const
	{
		if (dataOddania.length() != 4)
		{
			return false; // Niepoprawny format daty
		}
		try
		{
			// Pobiera aktualny czas i konwertuje na lokalny dzien, miesiac
			time_t now = time(nullptr);
			tm today;
			localtime_s(&today, &now);

			int miesiac = stoi(dataOddania.substr(0, 2));
			int rok = 2000 + stoi(dataOddania.substr(2, 2));

			return (rok > today.tm_year + 1900 || (rok == today.tm_year + 1900 && miesiac >= today.tm_mon + 1));
		}
		catch (...)
		{
			return false; // Niepoprawny format daty
		}
	}
	/**
	 * @brief Oblicza zysk z lokaty.
	 *
	 * @return Zysk z lokaty
	 */
	double obliczZysk() const
	{
		if (oprocentowanie < 0) return 0.0; // Oprocentowanie nie moze byc ujemne
		return kwota * (oprocentowanie / 100);
	}

	/**
 * @brief Wyswietla informacje o lokacie.
 *
 * Funkcja wyswietla szczegóły lokaty, takie jak kwota, oprocentowanie, data oddania,
 * zysk roczny oraz status lokaty.
 */
	void wyswietlInformacje() const
	{
		cout << "===== DANE LOKATY =====" << endl;
		cout << "Kwota: " << fixed<<setprecision(2)<<getKwota() <<"PLN"<< endl;
		cout << "Oprocentowanie: " << getOprocentowanie() << "%" << endl;
		cout << "Data oddania: " << getDataOddania() << endl;
		cout << "Zysk roczny: " << obliczZysk() << " PLN" << endl;
		cout << "Status: " << (czyAktywna() ? "Aktywna" : "Nieaktywna") << endl;
		cout << "Konto powiazane: " << getPowiazaneKonto() << endl;
		cout << "======================" << endl;
	}
};

/**
 * @class KontoGlowne
 * @brief Reprezentuje konto.
 * 
 * 
 *  Klasa przechowuje podtsawowe dane dane i funkcje zwiazane z kontami 
 */
class KontoGlowne {
private:
	string numerKonta; ///< Numer konta
	string typKonta; ///< Typ konta (np. Osobiste, Oszczędnościowe)
	float saldoKonta; ///< Saldo konta

public:
	/**
	 * @brief Konstruktor klasy Konto.
	 *
	 * Inicjalizuje obiekt Konto z podanymi parametrami.
	 *
	 * @param numer Numer konta
	 * @param typ Typ konta (np. Osobiste, Oszczędnościowe)
	 * @param saldo Saldo konta
	 */
	KontoGlowne(string numer, string typ, float saldo) {
			this->numerKonta = numer;
			this->typKonta = typ;
			this->saldoKonta = saldo;
	}
	/**
	 * @brief Konstruktor domyslny klasy Konto.
	 *
	 * Inicjalizuje obiekt Konto z domyslnymi wartosciami.
	 */
	KontoGlowne() = default;
	/**
	 * @brief Zwraca numer konta.
	 *
	 * @return Numer konta
	 */
	string getNumerKonta() const { return numerKonta; }
	/**
	 * @brief Zwraca typ konta.
	 *
	 * @return Typ konta (np. Osobiste, Oszczędnościowe)
	 */
	string getTypKonta() const { return typKonta; }
	/**
	 * @brief Zwraca saldo konta.
	 *
	 * @return Saldo konta
	 */
	float getSaldoKonta() const { return saldoKonta; }
	/**
	 * @brief Ustala numer konta.
	 *
	 * @param numer Numer konta
	 */
	void setNumerKonta(string numer) { numerKonta = numer; }
	/**
	 * @brief Ustala typ konta.
	 *
	 * @param typ Typ konta (np. Osobiste, Oszczędnościowe)
	 */
	void setTypKonta(string typ) { typKonta = typ; }
	/**
	 * @brief Ustala saldo konta.
	 *
	 * @param saldo Saldo konta
	 */
	void setSaldoKonta(float saldo)
	{
		if (saldo < 0) {
			cout << "Saldo nie moze byc ujemne." << endl;
			return;
		}
		saldoKonta = saldo;
	}
	/**
	 * @brief Wplaca pieniadze na konto.
	 *
	 * @param kwota Kwota do wplaty
	 * @return true jeśli wplata powiodła się, false w przeciwnym razie
	 */
	virtual bool wplac(float kwota)
	{
		if (kwota <= 0) {
			cout << "Kwota do wplaty musi byc wieksza od zera." << endl;
			return false;
		}
		saldoKonta += kwota;
		cout << "Wplata zakonczona sukcesem. Nowe saldo: " << saldoKonta << " PLN" << endl;
		return true;

	}
	/**
	 * @brief Wyplaca pieniadze z konta.
	 *
	 * @param kwota Kwota do wyplaty
	 * @return true jeśli wyplata powiodła się, false w przeciwnym razie
	 */
	virtual bool wyplac(float kwota)
	{
		if (kwota <= 0) {
			cout << "Kwota do wyplaty musi byc wieksza od zera." << endl;
			return false;
		}
		if (kwota > saldoKonta) {
			cout << "Niewystarczajace srodki na koncie." << endl;
			return false;
		}
		saldoKonta -= kwota;
		cout << "Wyplata zakonczona sukcesem. Nowe saldo: " << saldoKonta << " PLN" << endl;
		return true;

	}

	/**
	 * @brief Wyswietla informacje o koncie.
	 *
	 * Funkcja wyswietla szczegóły konta, takie jak numer konta, typ konta i saldo.
	 */
	virtual void wyswietlInformacje() const
	{
		cout << "===== DANE KONTA =====" << endl;
		cout << "Typ konta: " << getTypKonta() << endl;
		cout << "Numer konta: " << getNumerKonta() << endl;
		cout << "Saldo: " << fixed << setprecision(2) << getSaldoKonta() << "PLN" << endl;
		cout << "======================" << endl;
	}
};

/**
 * @class Transakcja
 * @brief Reprezentuje transakcje.
 *
 * Klasa przechowuje dane o transakcji, takie jak kwota, data, typ transakcji oraz zaangażowane konto.
 */
class Transakcja
{
private:
	float kwota; ///< Kwota transakcji
	string typTransakcji; ///< Typ transakcji (np. Wpłata, wypłata, przelew)
	string dataTransakcji; ///< Data transakcji
	string kontoNadawcy; ///< Numer konta nadawcy
	string kontoOdbiorcy; ///< Numer konta odbiorcy
public:
	/**
	 * @brief Domyslny konstruktor klasy Transakcja.
	 *
	 * Inicjalizuje obiekt Transakcja z domyslnymi wartosciami.
	 */
	Transakcja() = default;
	/**
	 * @brief  Ustala kwote transakcji.
	 * 
	 * @param kwota Kwota transakcji
	 */
	void setKwota(float kwota)
	{
		if (kwota < 0) {
			cout << "Kwota transakcji nie moze byc ujemna." << endl;
			return;
		}
		this->kwota = kwota;
	}
	/**
	 * @brief Ustala typ transakcji.
	 *
	 * @param typTransakcji Typ transakcji (np. Wpłata, wypłata, przelew)
	 */
	void setTypTransakcji(const string &typTransakcji)
	{
		if (typTransakcji != "wplata" && typTransakcji != "wypata" && typTransakcji != "przelew") {
			cout << "Niepoprawny typ transakcji." << endl;
			return;
		}
		this->typTransakcji = typTransakcji;
	}
	/**
	 * @brief Ustala konto nadawcy.
	 *
	 * @param kontoNadawcy Numer konta nadawcy
	 */
	void setKontoNadawcy(const string &kontoNadawcy)
	{
		if (kontoNadawcy.empty() && typTransakcji == "przelew")
		{
			throw invalid_argument("Niepoprawny numer konta nadawcy.");
		}
		this->kontoNadawcy = kontoNadawcy;
	}
	/**
	 * @brief Ustala konto odbiorcy.
	 *
	 * @param kontoOdbiorcy Numer konta odbiorcy
	 */
	void setKontoOdbiorcy(const string &kontoOdbiorcy)
	{
		if (kontoOdbiorcy.empty() && typTransakcji == "przelew")
		{
			throw invalid_argument("Niepoprawny numer konta odbiorcy.");
		}
		this->kontoOdbiorcy = kontoOdbiorcy;
	}
	/**
	* @brief Zwraca bieżącą datę w formacie "DD-MM-YYYY".
	*
	* @return Bieżąca data w formacie "DD-MM-YYYY"
	*/
	string getAktualnaData()
	{
		time_t now = time(nullptr);
		tm today;
		localtime_s(&today, &now);

		ostringstream ss;

		ss << put_time(&today, "%d-%m-%Y %H:%M:%S");
		return ss.str();
	}
	/**
	 * @brief Zwraca kwote transakcji.
	 *
	 * @return Kwota transakcji
	 */
	float getKwota() const { return kwota; }
	/**
	 * @brief Zwraca typ transakcji.
	 *
	 * @return Typ transakcji (np. Wpłata, wypłata, przelew)
	 */
	string getTypTransakcji() const { return typTransakcji; }
	/**
	 * @brief Zwraca date transakcji.
	 *
	 * @return Data transakcji
	 */
	string getDataTransakcji() const { return dataTransakcji; }
	/**
	 * @brief Zwraca numer konta nadawcy.
	 *
	 * @return Numer konta nadawcy
	 */
	string getKontoNadawcy() const { return kontoNadawcy; }
	/**
	 * @brief Zwraca numer konta odbiorcy.
	 *
	 * @return Numer konta odbiorcy
	 */
	string getKontoOdbiorcy() const { return kontoOdbiorcy; }
	/**
	 * @ brief Ustala szczegóły transakcji.
	 *
	 * Funckja ustala szczegóły transakcji, takie jak kwota, typ transakcji, data transakcji oraz numery kont nadawcy i odbiorcy.
	 */
	void szczegolyTransakcji()
	{
		cout << "Podaj kwote transakcji: ";
		float kwotaWpisana;
		cin >> kwotaWpisana;
		setKwota(kwotaWpisana);

		cout << "Podaj typ transakcji (wplata/wypata/przelew): ";
		string typTransakcjiWpisany;
		cin >> typTransakcjiWpisany;
		setTypTransakcji(typTransakcjiWpisany);

		dataTransakcji = getAktualnaData();

		if (typTransakcji == "przelew")
		{
			cout << "Podaj numer konta nadawcy: ";
			cin >> kontoNadawcy;
			cout << "Podaj numer konta odbiorcy: ";
			cin >> kontoOdbiorcy;
		}
	}
	/**
	 * @brief Wyświetla szczegóły transakcji.
	 *
	 * Funckja wyświetla szczegóły transakcji, takie jak kwota, typ transakcji, data transakcji oraz numery kont nadawcy i odbiorcy.
	 */
	void wyswietlSzczegolyTransakcji() const
	{
		cout << "===== SZCZEGOLY TRANSAKCJI =====" << endl;
		cout << "Data transakcji: " << getDataTransakcji() << endl;
		cout << "Typ transakcji: " << getTypTransakcji() << endl;
		cout << "Kwota: " << fixed << setprecision(2) << getKwota() << " PLN" << endl;
		if (typTransakcji == "przelew")
		{
			cout << "Konto nadawcy: " << getKontoNadawcy() << endl;
			cout << "Konto odbiorcy: " << getKontoOdbiorcy() << endl;
		}
		cout << "===============================" << endl;
	}
};

/**
 * @class KartaDebetowa
 * @brief Reprezentuje karte debetowa.
 * @extends Karta
 *
 * Implementacja konkretnego typu karty - karty debetowej,
 * która jest powiązana z kontem bankowym i posiada dzienny limit transakcji.
 */
class KartaDebetowa : public Karta {
private:
	string powiazaneKonto; ///< Powiązane konto bankowe
	float dziennyLimit; ///< Dzienny limit transakcji
public:
	/**
	 * @brief Konstruktor klasy KartaDebetowa.
	 *
	 * Inicjalizuje obiekt KartaDebetowa z podanymi parametrami.
	 *
	 * @param numer Numer karty
	 * @param data Data waznosci karty
	 * @param kod Kod CVC karty
	 * @param powiazaneKonto Powiązane konto bankowe
	 * @param limit Dzienny limit transakcji
	 */
	KartaDebetowa(string numer, string data, string kod, string powiazaneKonto, float limit)
		: Karta(numer, data, kod), powiazaneKonto(powiazaneKonto), dziennyLimit(limit) {}

	/**
	 * @brief Zwraca typ karty
	 * @return Typ karty
	 *
	 * Implementacja metody czysto wirtualnej z klasy bazowej Karta.
	 */
	string getTypKarty() const override { return "Debetowa"; }
	/**
	 * @brief Pobiera numer powiązanego konta.
	 * @return Identyfikator konta
	 */
	string getPowiazaneKonto() const { return powiazaneKonto; }
	/**
	 * @brief  Pobiera dzienny limit transakcji
	 * @return Wartość dziennego limitu
	 */
	float getDziennyLimit() const { return dziennyLimit; }
	/**
	 * @brief Ustawia numer powiązanego konta
	 * @param konto Nowy identyfikator konta
	 */
	void setPowiazaneKonto(string konto) { powiazaneKonto = konto; }
	/**
	 * @brief Ustawia dzienny limit transakcji
	 * @param limit Nowa wartość dziennego limitu
	 */
	void setDziennyLimit(float limit) { dziennyLimit = limit; }
	/**
	 * @brief Wykonuje płatność kartą
	 * @param kwota Kwota transakcji
	 * @return true jeśli płatność została wykonana pomyślnie, false w przeciwnym razie
	 *
	 * Metoda weryfikuje czy żądana kwota nie przekracza dziennego limitu transakcji
	 * i czy karta jest ważna, a następnie wykonuje płatność.
	 */
	bool wykonajPlatnosc(float kwota);
};
/**
 * @class KontoOszczednosciowe
 * @brief Reprezentuje konto oszczędnościowe.
 *
 * Klasa dziedziczy po klasie Konto i dodaje funkcjonalności specyficzne dla kont oszczędnościowych.
 */
class KontoOszczednosciowe : public KontoGlowne {
private:
	float oprocentowanie; ///< Oprocentowanie konta oszczędnościowego
	string dataOstatniejKapitalizacji; ///< Data ostatniej kapitalizacji odsetek
	int ograniczenieWyplat; ///< Ograniczenie liczby wypłat w miesiącu
public:
	/**
	 * @brief Konstruktor klasy KontoOszczednosciowe.
	 *
	 * Inicjalizuje obiekt KontoOszczednosciowe z podanymi parametrami.
	 *
	 * @param numer Numer konta
	 * @param saldo Saldo konta
	 * @param oprocentowanie Oprocentowanie konta oszczędnościowego
	 * @param dataKapitalizacji Data ostatniej kapitalizacji odsetek
	 * @param limitWyplat Ograniczenie liczby wypłat w miesiącu
	 */
	KontoOszczednosciowe(string numer, float saldo, float oprocentowanie, string dataKapitalizacji, int limitWyplat)
		: KontoGlowne(numer, "Oszczędnościowe", saldo), oprocentowanie(oprocentowanie), dataOstatniejKapitalizacji(dataKapitalizacji), ograniczenieWyplat(limitWyplat) {}

	/**
	 * @brief Zwraca oprocentowanie konta oszczędnościowego.
	 *
	 * @return Oprocentowanie konta oszczędnościowego
	 */
	float getOprocentowanie() const { return oprocentowanie; }
	/**
	 * @brief Zwraca datę ostatniej kapitalizacji odsetek.
	 *
	 * @return Data ostatniej kapitalizacji odsetek
	 */
	string getDataOstatniejKapitalizacji() const { return dataOstatniejKapitalizacji; }

	/**
	* @brief Zwraca ograniczenie liczby wypłat w miesiącu.
	*
	* @return Ograniczenie liczby wypłat w miesiącu
	*/
	int getOgraniczenieWyplat() const { return ograniczenieWyplat; }
	/**
	 * @brief Ustala oprocentowanie konta oszczędnościowego.
	 *
	 * @param oprocentowanie Oprocentowanie konta oszczędnościowego
	 */
	void setOprocentowanie(float oprocentowanie) { this->oprocentowanie = oprocentowanie; }
	/**
	 * @brief Ustala datę ostatniej kapitalizacji odsetek.
	 *
	 * @param dataKapitalizacji Data ostatniej kapitalizacji odsetek
	 */
	void setDataOstatniejKapitalizacji(string data) { this->dataOstatniejKapitalizacji = data; }
	/**
	 * @brief Ustala ograniczenie liczby wypłat w miesiącu.
	 *
	 * @param limitWyplat Ograniczenie liczby wypłat w miesiącu
	 */
	void setOgraniczenieWyplat(int limitWyplat) { this->ograniczenieWyplat = limitWyplat; }
	/**
	 * @brief Oblicza i dodaje odsetki do konta.
	 * 
	 * @return 
	 */
	float kapitalizujOdsetki();
	/**
	 * @brief Wplaca pieniadze na konto oszczędnościowe.
	 *
	 * @param kwota Kwota do wplaty
	 * @return true jeśli wplata powiodła się, false w przeciwnym razie
	 */
	bool wplac(float kwota) override;
	/**
	 * @brief Wyplaca pieniadze z konta oszczędnościowego.
	 *
	 * @param kwota Kwota do wyplaty
	 * @return true jeśli wyplata powiodła się, false w przeciwnym razie
	 */
	bool wyplac(float kwota) override;

};
/**
 * @class Klient
 * @brief Reprezentuje klienta banku.
 * 
 * 
 *  Klasa przechowuje dane osobowe oraz udostepnia podstawowe funkcje zwiazane z uzytkownikiem 
 */
class Klient
{
private:
	string imie; ///< Imie klienta
	string nazwisko; ///< Nazwisko klienta
	string pesel; ///< Numer PESEL klienta
	string login; ///< Login klienta
	string haslo; ///< Haslo klienta

	vector<KontoGlowne*> kontaUzytkownika; ///< Tablica przechowujaca konta uzytkownika
	vector<Karta*> kartyUzytkownika; ///< Tablica przechowujaca karty uzytkownika
	vector<Lokata> lokatyUzytkownika; ///< Tablica przechowujaca lokaty uzytkownika

public:
	/**
	 *@brief Domyslny konstruktor klasy Klient
	 *
	 * Inicjalizuje obiekt Klient z domyslnymi wartosciami.
	 */
	Klient() = default;
	/**
	 *@brief Ustala imie klienta.
	 *
	 *@param imie Imie klienta
	 */
	void setImie(string imie) { this->imie = imie; }
	/**
	 * @brief Ustala nazwisko klienta.
	 *
	 * @param nazwisko Nazwisko klienta
	 */
	void setNazwisko(string nazwisko) { this->nazwisko = nazwisko; }
	/**
	 * @brief Ustala PESEL klienta.
	 *
	 * @param pesel Numer PESEL klienta
	 */
	void setPesel(string pesel) { this->pesel = pesel; }
	/**
	 * @brief Ustala login klienta.
	 *
	 * @param login Login klienta
	 */
	void setLogin(string login) { this->login = login; }
	/**
	 * @brief Ustala haslo klienta.
	 *
	 * @param haslo Haslo klienta
	 */
	void setHaslo(string haslo) { this->haslo = haslo; }
	/**
	 * @brief Zwraca imie klienta.
	 *
	 * @return Imie klienta
	 */
	string getImie() const { return imie; }
	/**
	 * @brief Zwraca nazwisko klienta.
	 *
	 * @return Nazwisko klienta
	 */
	string getNazwisko() const { return nazwisko; }
	/**
	 * @brief Zwraca PESEL klienta.
	 *
	 * @return Numer PESEL klienta
	 */
	string getPesel() const { return pesel; }
	/**
	 * @brief Zwraca login klienta.
	 *
	 * @return Login klienta
	 */
	string getLogin() const { return login; }
	/**
	 * @brief Zwraca haslo klienta.
	 *
	 * @return Haslo klienta
	 */
	string getHaslo() const { return haslo; }
    /**
     * @brief Edytuje dane klienta.
     *
     * Funkcja umozliwia edycje danych klienta, takich jak imie, nazwisko, czy PESEL,
     * zapisujac zmiany do pliku JSON.
     */
    void edytujDane();
	/**
	 * @brief Wyswietla dane klienta.
	 *
	 * Funkcja wyswietla dane klienta, takie jak imie, nazwisko, PESEL, login i haslo.
	 */
	void wyswietlDane();
	
};
/**
 * @class FileManager
 * @brief Klasa do zarządzania plikami.
 *
 * Klasa FileManager jest odpowiedzialna za operacje na plikach, takie jak odczyt i zapis danych.
 */
class FileManager
{
private:
	string nazwaPliku; ///< Nazwa pliku do odczytu/zapisu
	string typ; ///< Typ konta bankowego lub karty
	/**
	 * @brief Zwraca informacje o kliencie w formacie JSON.
	 *
	 * Ta funkcja konwertuje obiekt Klient na format JSON, aby ułatwić zapis do pliku.
	 *
	 * @param j Obiekt JSON, do którego zostaną zapisane informacje o kliencie
	 * @param klient Obiekt Klient, którego informacje mają zostać zapisane
	 */
	void to_json_Klient(json& j, const Klient& klient);
	/**
	 * @brief Wczytuje informacje o kliencie z formatu JSON.
	 *
	 * Ta funkcja konwertuje dane w formacie JSON na obiekt Klient.
	 *
	 * @param j Obiekt JSON, z którego zostaną odczytane informacje o kliencie
	 * @param klient Obiekt Klient, do którego zostaną zapisane informacje
	 */
	void from_json_Klient(const json& j, Klient& klient);
	/**
	 * @brief Zwraca informacje o transakcji w formacie JSON.
	 *
	 * Ta funkcja konwertuje obiekt Transakcja na format JSON, aby ułatwić zapis do pliku.
	 *
	 * @param j Obiekt JSON, do którego zostaną zapisane informacje o transakcji
	 * @param transakcja Obiekt Transakcja, którego informacje mają zostać zapisane
	 */
	void to_json_Transakcja(json& j, const Transakcja& transakcja);
	/**
	 * @brief Wczytuje informacje o transakcji z formatu JSON.
	 *
	 * Ta funkcja konwertuje dane w formacie JSON na obiekt Transakcja.
	 *
	 * @param j Obiekt JSON, z którego zostaną odczytane informacje o transakcji
	 * @param transakcja Obiekt Transakcja, do którego zostaną zapisane informacje
	 */
	void from_json_Transakcja(const json& j, Transakcja& transakcja);
	/**
	 * @brief Zwraca informacje o karcie w formacie JSON.
	 *
	 * Ta funkcja konwertuje obiekt Karta na format JSON, aby ułatwić zapis do pliku.
	 *
	 * @param j Obiekt JSON, do którego zostaną zapisane informacje o karcie
	 * @param karta Obiekt Karta, którego informacje mają zostać zapisane
	 */
	void to_json_Karta(json& j, const Karta& karta);
	/**
	 * @brief Wczytuje informacje o karcie z formatu JSON.
	 *
	 * Ta funkcja konwertuje dane w formacie JSON na obiekt Karta.
	 *
	 * @param j Obiekt JSON, z którego zostaną odczytane informacje o karcie
	 * @param karta Obiekt Karta, do którego zostaną zapisane informacje
	 */
	void from_json_Karta(const json& j, Karta& karta);
	/**
	 * @brief Zwraca informacje o lokacie w formacie JSON.
	 *
	 * Ta funkcja konwertuje obiekt Lokata na format JSON, aby ułatwić zapis do pliku.
	 *
	 * @param j Obiekt JSON, do którego zostaną zapisane informacje o lokacie
	 * @param lokata Obiekt Lokata, którego informacje mają zostać zapisane
	 */
	void to_json_Lokata(json& j, const Lokata& lokata);
	/**
	 * @brief Wczytuje informacje o lokacie z formatu JSON.
	 *
	 * Ta funkcja konwertuje dane w formacie JSON na obiekt Lokata.
	 *
	 * @param j Obiekt JSON, z którego zostaną odczytane informacje o lokacie
	 * @param lokata Obiekt Lokata, do którego zostaną zapisane informacje
	 */
	void from_json_Lokata(const json& j, Lokata& lokata);
	/**
	 * @brief Zwraca informacje o koncie w formacie JSON.
	 *
	 * Ta funkcja konwertuje obiekt Konto na format JSON, aby ułatwić zapis do pliku.
	 *
	 * @param j Obiekt JSON, do którego zostaną zapisane informacje o koncie
	 * @param konto Obiekt Konto, którego informacje mają zostać zapisane
	 */
	void to_json_Konto(json& j, const KontoGlowne& konto);
	/**
	 * @brief Wczytuje informacje o koncie z formatu JSON.
	 *
	 * Ta funkcja konwertuje dane w formacie JSON na obiekt Konto.
	 *
	 * @param j Obiekt JSON, z którego zostaną odczytane informacje o koncie
	 * @param konto Obiekt Konto, do którego zostaną zapisane informacje
	 */
	void from_json_Konto(const json& j, KontoGlowne& konto);

public:
	/**
	 * @brief Zapisuje dane klientów do pliku JSON.
	 * @param klienci Wektor klientów do zapisania
	 */
	void zapiszKlientow(const vector<Klient>& klienci);
	/**
	 * @brief Odczytuje dane klientów z pliku JSON.
	 * @return Wektor klientów odczytanych z pliku
	 */
	vector<Klient> wczytajKlientow();
	/**
	 * @brief Zapisuje historię transakcji do pliku JSON.
	 * @param transakcje Wektor transakcji do zapisania
	 */
	void zapiszTransakcje(const vector<Transakcja>& transakcje);
	/**
	 * @brief Wczytuje historię transakcji z pliku JSON.
	 * @return Wektor transakcji odczytanych z pliku
	 */
	vector<Transakcja> wczytajTransakcje();
	/**
	 * @brief Zapisuje dane kart do pliku JSON.
	 * @param karty Wektor kart do zapisania
	 */
	void zapiszKarty(const vector<Karta*>& karty);
	/**
	 * @brief Wczytuje dane kart z pliku JSON.
	 * @return Wektor kart odczytanych z pliku
	 */
	vector<Karta*> wczytajKarty();
	/**
	 * @brief Zapisuje dane lokat do pliku JSON.
	 * @param lokaty Wektor lokat do zapisania
	 */
	void zapiszLokaty(const vector<Lokata>& lokaty);
	/**
	 * @brief Wczytuje dane lokat z pliku JSON.
	 * @return Wektor lokat odczytanych z pliku
	 */
	vector<Lokata> wczytajLokaty();
	/**
	 * @brief Zapisuje dane kont do pliku JSON.
	 * @param konta Wektor kont do zapisania
	 */
	void zapiszKonta(const vector<KontoGlowne*>& konta);
	/**
	 * @brief Wczytuje dane kont z pliku JSON.
	 * @return Wektor kont odczytanych z pliku
	 */
	vector<KontoGlowne*> wczytajKonta();

};
/**
* @class SystemBankowy
* @brief Główny kontroler systemu bankowego.
*
* Klasa zarządzająca wszystkimi funkcjonalnościami systemu bankowego,
* odpowiedzialna za obsługę klientówm, kont, transakcji, i innych operacji bankowych.
*/
class SystemBankowy
{
private:
	vector<Klient> klienci; ///< Wektor przechowujący klientów
	vector<Transakcja> transakcje; ///< Wektor przechowujący transakcje
	Klient* zalogowanyKlient; ///< Wskaźnik na aktualnie zalogowanego klienta
	FileManager menedzerPlikow; ///< Obiekt do zarządzania plikami
public:
	/**
	 * @brief Konstruktor klasy SystemBankowy.
	 *
	 * Inicjalizuje system bankowy, wczytując dane klientów i transakcji z plików
	 * oraz ustawia brak zalogowanego klienta.
	 */
	SystemBankowy();
	/**
	 * @brief Uruchamia system bankowy.
	 *
	 * Główna metoda sterująca przepływem programu. Wyświetla menu główne
	 * i obsługuje wybory użytkownika do moment wyjścia z programu.
	 */
	void uruchom();
	/**
	 * @brief Wyświetla menu główne systemu bankowego.
	 *
	 * Pokjazuje dostępne opcje dla niezalogowanego użytkownika,
	 * takiej jak rejestracja, logowanie, czy wyjście z programu.
	 */
	void wyswietlMenuGlowne();
	/**
	 * @brief Obsługuje zalogowanego użytkownika.
	 *
	 * Wyświtla menu dla zalogowanego użytkownika i obsługuje
	 * jego wybory do momentu wylogowania lub zakończenia programu.
	 */
	void obslugaZalogowanegoUzytkownika();
	/**
	 * @brief Wyświetla menu dla zalogowanego użytkownika.
	 *
	 * Pokazuje dostępne opcje dla zalogowanego użytkownika, takie jak
	 * zarządzanie danymi osobowymi, kontami, kartami, lokatami, czy transakcjami.
	 */
	void wyswietlMenuUzytkownika();
	/**
	 * @brief Obsługuje proces rejestracji nowego klienta.
	 *
	 * Zbiera dane od użytkownika, tworzy noy obiekt Klient
	 * i dodaje go do listy klientów banku.
	 */
	void zarejestrujKlienta();
	/**
	 * @brief Obsługuje proces logowania klienta.
	 *
	 * Sprawdza poprawność podanych danych logowania i
	 * ustawia wskaźnik zalogowanegoKlienta.
	 *
	 * @return true jeśli logowanie powiodło się, false w przeciwnym razie
	 */
	bool zalogujKlienta();
	/**
	 * @brief Dodaje nowe konto dla zalogowanego klienta.
	 *
	 * Umożliwia zalogowanemu klientowi dodanie nowego konta
	 * bankowego wybranego typu
	 */
	void dodajKonto();
	/**
	 * @brief Dodaje nową kartę dla zalogowanego klienta.
	 *
	 * Umożliwia zalogowanemu klientowi dodanie nowej karty
	 * bankowej wybranego typu
	 */
	void dodajKarte();
	/**
	 * @brief Zakłada nową lokatę dla zalogowanego klienta.
	 *
	 * Umożliwia zalogowanemu klientowi dodanie nowej lokaty
	 * na wybraną kwotę i okres
	 */
	void zalozLokate();
	/**
	 * @brief Wykonuje przelew z konta zalogowanego klienta.
	 *
	 * Umożliwia zalogowanemu klientowi wykonanie przelewu
	 * z jednego konta na drugie, tworząc odpowiedni rekord transakcji.
	 */
	void wykonajPrzelew();
	/**
	 * @brief Wyświetla historię transakcji dla zalogowanego klienta.
	 *
	 * Pokazuje listę wszystkich transakcji związanych z kotami
	 * zalogowanego kllienta.
	 */
	void wyswietlHistorieTransakcji();
	/**
	 * @brief Sprawdza, czy podany login jesy już zajęty.
	 *
	 * Weryfikuje, czy podany login nie jest już używany przez
	 * innego klienta w systemie.
	 * 
	 * @param login Login do sprawdzenia 
	 * @return true jeśli login istnieje, false w przeciwnym razie
	 */
	bool sprawdzCzyLoginIstnieje(const string& login);
};








int main(int argc, char** argv) {

	return 0;
}
