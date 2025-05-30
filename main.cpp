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
#include <stdexcept>
#include <exception>

class SystemBankowy;
using namespace std;
using json = nlohmann::json;

/**
 * @class Error
 * @brief Odpowiada za wypisywanie wyjątków.
 *
 *
 *	Odpowiada za wypisywanie wyjątków które wydarzyły się podczas działania programu
 */
class Error : public invalid_argument {
public:
	Error(const string& message) : invalid_argument(message) {}
};


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
	string pin; ///< Pin karty
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
		if (dataWaznosci.length() == 4) {
			return dataWaznosci.substr(0, 2) + "/20" + dataWaznosci.substr(2, 2);
		}
		else
		{
			return dataWaznosci;
		}
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
		else if (data.length() == 6)
		{
			dataWaznosci = data.substr(0, 2) + data.substr(4, 2);
		}
		else
		{
			throw Error("Niepoprawny format daty. Oczekiwano formatu MMRR lub MM/YYYY");
		}
	}
	/**
	 * @brief Ustala kod CVC karty.
	 *
	 * @param kod Kod CVC karty
	 */
	void setKodCVC(string kod) { kodCVC = kod; }

	/**
	* @brief Ustala pin karty.
	*
	* @param nowyPin Pin karty
	*/
	void setPin(const string& nowyPin) {
		if (nowyPin.length() == 4) {  // Weryfikacja długości PINu
			pin = nowyPin;
		}
		else {
			throw Error("PIN musi mieć 4 cyfry");
		}
	}
	/**
	 * @brief Zwraca typ karty.
	 *
	 * @return Typ karty (np. Debetowa, Kredytowa)
	 */
	virtual string getTypKarty() const = 0;

	/**
	* @brief Zwraca pin karty.
	*
	* @return Pin karty
	*/
	string getPin() const { return pin; }
	/**
	 * @brief Sprawdza, czy karta jest wazna.
	 * @return true jeśli karta jest wazna, false w przeciwnym razie
	 */
	virtual bool czyWazna() const
	{
		string dataWaznosci = getDataWaznosci();

		if (dataWaznosci.length() == 7 && dataWaznosci[2] == '/')
		{
			dataWaznosci = dataWaznosci.substr(0, 2) + dataWaznosci.substr(5, 2); // Konwersja z formatu MM/YYYY na MMRR
		}

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
	* @brief Ustala konto powiazane z dana lokata.
	*
	* @param wlasciciel Powiazane konto
	*/
	void setPowiazaneKonto(const string& wlasciciel) { this->powiazaneKonto = wlasciciel; }
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
	string getDataOddania() const { return dataOddania.substr(0, 2) + "/20"
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
	string wlasciciel; ///< Właściciel konta

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
	 * @brief Zwraca wlasciciela konta.
	 *
	 * @return Wlasciciel konta
	 */
	string getWlasciciel() const { return wlasciciel; }

	/**
	 * @brief Ustala wlasciciela konta.
	 *
	 * @param wlasciciel Wlasciciel konta
	 */
	void setWlascicielel(string wlasciciel) { this->wlasciciel = wlasciciel; }
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
		if (typTransakcji != "wplata" && typTransakcji != "wyplata" && typTransakcji != "przelew") {
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
			throw Error("Niepoprawny numer konta nadawcy.");
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
			throw Error("Niepoprawny numer konta odbiorcy.");
		}
		this->kontoOdbiorcy = kontoOdbiorcy;
	}

	void setDataTransakcji(const string& dataTransakcji)
	{
		if (dataTransakcji.length() == 4) // Format "MMRR"
		{
			this->dataTransakcji = dataTransakcji;
		}
		else if (dataTransakcji.length() == 7 && dataTransakcji[2] == '/') // Format "MM/YYYY"
		{
			this->dataTransakcji = dataTransakcji.substr(0, 2) + dataTransakcji.substr(5, 2);
		}
		else
		{
			throw Error("Niepoprawny format daty. Oczekiwano formatu MMRR lub MM/YYYY");
		}
	}
	/**
	* @brief Zwraca bieżącą datę w formacie "DD-MM-YYYY".
	*
	* @return Bieżąca data w formacie "DD-MM-YYYY"
	*/
	string getAktualnaData() const
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
	void setDziennyLimit(float limit)
	{
		if (limit<0)
		{
			throw Error("Dzienny limit nie moze byc ujemny.");
		}
		dziennyLimit = limit;
	}
	/**
	 * @brief Wykonuje płatność kartą
	 * @param kwota Kwota transakcji
	 * @return true jeśli płatność została wykonana pomyślnie, false w przeciwnym razie
	 *
	 * Metoda weryfikuje czy żądana kwota nie przekracza dziennego limitu transakcji
	 * i czy karta jest ważna, a następnie wykonuje płatność.
	 */
	bool wykonajPlatnosc(float kwota)
	{
		if (kwota<=0)
		{
			cerr << "Kwota transakcji musi byc wieksza od zera." << endl;
			return false;
		}

		if (!czyWazna())
		{
			cerr << "Karta jest niewazna." << endl;
			return false;

		}
		if (kwota > dziennyLimit)
		{
			cerr << "Kwota transakcji przekracza dzienny limit." << endl;
			return false;
		}

		dziennyLimit -= kwota;
		cout << "Platnosc zakonczona sukcesem. Pozostaly limit: " << dziennyLimit << " PLN" << endl;
		return true;
	}
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
	int wykonaneWyplatywWMiesiacu; ///< Liczba wykonanych wypłat w danym miesiącu
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
		: KontoGlowne(numer, "Oszczednosciowe", saldo), oprocentowanie(oprocentowanie), dataOstatniejKapitalizacji(dataKapitalizacji), ograniczenieWyplat(limitWyplat), wykonaneWyplatywWMiesiacu(0) {}

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
	string getDataOstatniejKapitalizacji() const
	{
		if (dataOstatniejKapitalizacji.length()== 4)
		{
			return dataOstatniejKapitalizacji.substr(0, 2) + "/20" + dataOstatniejKapitalizacji.substr(2, 2);

		}
		return dataOstatniejKapitalizacji;
	}

	/**
	* @brief Zwraca ograniczenie liczby wypłat w miesiącu.
	*
	* @return Ograniczenie liczby wypłat w miesiącu
	*/
	int getOgraniczenieWyplat() const { return ograniczenieWyplat; }

	/**
	* @brief Zwraca liczbę wykonanych wypłat w danym miesiącu.
	*
	* @return Liczba wykonanych wypłat w danym miesiącu
	*/
	int getWykonaneWyplatywWMiesiacu() const { return wykonaneWyplatywWMiesiacu; }
	/**
	 * @brief Ustala oprocentowanie konta oszczędnościowego.
	 *
	 * @param oprocentowanie Oprocentowanie konta oszczędnościowego
	 */
	void setOprocentowanie(float oprocentowanie)
	{
		if (oprocentowanie < 0) {
			cout << "Oprocentowanie nie moze byc ujemne." << endl;
			return;
		}
		this->oprocentowanie = oprocentowanie;
	}
	/**
	 * @brief Ustala datę ostatniej kapitalizacji odsetek.
	 *
	 * @param dataKapitalizacji Data ostatniej kapitalizacji odsetek
	 */
	void setDataOstatniejKapitalizacji(string data)
	{
		if (data.length() == 4) // Format "MMRR"
		{
			dataOstatniejKapitalizacji = data;
		}
		else if (data.length() == 7 && data[2] == '/') // Format "MM/YYYY"
		{
			dataOstatniejKapitalizacji = data.substr(0, 2) + data.substr(5, 2);
		}
		else
		{
			throw Error("Niepoprawny format daty. Oczekiwano formatu MMRR lub MM/YYYY");
		}
	}
	/**
	 * @brief Ustala ograniczenie liczby wypłat w miesiącu.
	 *
	 * @param limitWyplat Ograniczenie liczby wypłat w miesiącu
	 */
	void setOgraniczenieWyplat(int limitWyplat)
	{
		if (limitWyplat < 0) {
			cout << "Ograniczenie wypłat nie może być ujemne." << endl;
			return;
		}
		this->ograniczenieWyplat = limitWyplat;
	}

	/**
	 * @breif Resetuje licznik wypłat w danym miesiącu.
	 *
	 * Funkcja resetuje licznik wypłat w danym miesiącu do zera.
	 */
	void resetujLicznikWyplat() {
		wykonaneWyplatywWMiesiacu = 0;
	}

	bool czyNalezyKapitalizowac() const
	{
		if (dataOstatniejKapitalizacji.length()!=4)
		{
			return false; // Niepoprawny format daty
		}
		try
		{
			time_t now = time(nullptr);
			tm today;
			localtime_s(&today, &now);

			int miesiacKapitalizacji = stoi(dataOstatniejKapitalizacji.substr(0, 2));
			int rokKapitalizacji = 2000 + stoi(dataOstatniejKapitalizacji.substr(2, 2));
			int miesiacAktualny= today.tm_mon + 1;
			int rokAktualny = today.tm_year + 1900;

			if (rokAktualny>rokKapitalizacji)
			{
				return true;
			}
			else if (rokAktualny == rokKapitalizacji && miesiacAktualny > miesiacKapitalizacji)
			{
				return true;
			}
			return false;
		}catch (...)
		{
			return false; // Niepoprawny format daty
		}
	}
	/**
	 * @brief Oblicza i dodaje odsetki do konta.
	 *
	 * @return
	 */
	float kapitalizujOdsetki()
	{
		if (!czyNalezyKapitalizowac())
		{
			cout << "Nie minal jeszcze miesiac od ostatniej kapitalizacji." << endl;
			return 0.0f;
		}

		float saldo = getSaldoKonta();
		float odsetki = saldo * (oprocentowanie / 100.0f) / 12.0f; // Miesieczne odsetki

		setSaldoKonta(saldo + odsetki);

		time_t now = time(nullptr);
		tm today;
		localtime_s(&today, &now);

		string miesiac = to_string(today.tm_mon + 1);
		if (miesiac.length() == 1) {
			miesiac = "0" + miesiac; // Dodaj zero wiodące
		}

		string rok = to_string((today.tm_year + 1900) % 100); // Dwie ostatnie cyfry roku
		if (rok.length() == 1) {
			rok = "0" + rok; // Dodaj zero wiodące
		}
		dataOstatniejKapitalizacji = miesiac + rok; // Ustaw nowa date kapitalizacji

		cout << "Kapitalizacja odsetek wykonana pomyslnie." << endl;
		cout << "Naliczone odsetki: " << fixed << setprecision(2) << odsetki << " PLN" << endl;
		cout << "Nowe saldo: " << fixed << setprecision(2) << getSaldoKonta() << " PLN" << endl;

		return odsetki;
	}
	/**
	 * @brief Wplaca pieniadze na konto oszczędnościowe.
	 *
	 * @param kwota Kwota do wplaty
	 * @return true jeśli wplata powiodła się, false w przeciwnym razie
	 */
	bool wplac(float kwota) override
	{
		return KontoGlowne::wplac(kwota);
	}
	/**
	 * @brief Wyplaca pieniadze z konta oszczędnościowego.
	 *
	 * @param kwota Kwota do wyplaty
	 * @return true jeśli wyplata powiodła się, false w przeciwnym razie
	 */
	bool wyplac(float kwota) override
	{
		if (wykonaneWyplatywWMiesiacu >= ograniczenieWyplat)
		{
			cout << "Przekroczono limit wyplat w tym miesiacu (" << ograniczenieWyplat << ")." << endl;
			return false;
		}

		bool wyplataUdana = KontoGlowne::wyplac(kwota);

		if (wyplataUdana)
		{
			wykonaneWyplatywWMiesiacu++;
			cout << "Dostepna liczba wyplat w tym, miesiacu: " << ograniczenieWyplat - wykonaneWyplatywWMiesiacu << endl;

		}
		return wyplataUdana;
	}

	void wyswietlInformacje() const override
	{
		cout << "===== DANE KONTA OSZCZEDNOSCIOWEGO =====" << endl;
		cout << "Typ konta: " << getTypKonta() << endl;
		cout << "Numer konta: " << getNumerKonta() << endl;
		cout << "Saldo: " << fixed << setprecision(2) << getSaldoKonta() << " PLN" << endl;
		cout << "Oprocentowanie: " << getOprocentowanie() << "%" << endl;
		cout << "Data ostatniej kapitalizacji: " << getDataOstatniejKapitalizacji() << endl;
		cout << "Limit wyplat miesiecznie: " << getOgraniczenieWyplat() << endl;
		cout << "Liczba wyplat w tym miesiacu: " << getWykonaneWyplatywWMiesiacu() << "/" << getOgraniczenieWyplat() << endl;
		cout << "Pozostale wyplaty w tym miesiacu: " << getOgraniczenieWyplat() - getWykonaneWyplatywWMiesiacu() << endl;
		cout << "========================================" << endl;

	}

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
	* @brief Konstruktor parametryczny klasy Klient.
	*
	* Inicjalizuje obiekt Klient z podanymi parametrami.
	*
	* @param imie Imie klienta
	* @param nazwisko Nazwisko klienta
	* @param pesel Numer PESEL klienta
	* @param login Login klienta
	* @param haslo Haslo klienta
	*/
	Klient(string imie, string nazwisko, string pesel, string login, string haslo)
	{
		this->imie = imie;
		this->nazwisko = nazwisko;
		this->pesel = pesel;
		this->login = login;
		this->haslo = haslo;
	}

	/**
	 * @brief Destruktor klasy Klient.
	 *
	 * Uwalnia zasoby zajmowane przez konta i karty uzytkownika.
	 */
	~Klient()
	{
		for (auto& konto : kontaUzytkownika)
		{
			delete konto;
		}
		kontaUzytkownika.clear();

		for (auto& karta : kartyUzytkownika)
		{
			delete karta;
		}
		kartyUzytkownika.clear();
	}
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
	 * @brief Dodaje konto do listy kont uzytkownika.
	 *
	 * @param konto Wskaznik na obiekt konta do doania
	 */
	void dodajKonto(KontoGlowne* konto)
	{
		kontaUzytkownika.push_back(konto);
	}

	/**
	* @brief Dodaje karte do listy kart uzytkownika.
	*
	* @param karta Wskaznik na obiekt karty do dodania
	*/
	void dodajKarte(Karta* karta)
	{
		kartyUzytkownika.push_back(karta);
	}

	/**
	* @brief Dodaje lokate do listy lokat uzytkownika.
	*
	* @param lokata Obiekt lokaty do dodania
	*/
	void dodajLokate(const Lokata& lokata)
	{
		lokatyUzytkownika.push_back(lokata);
	}

	/**
	* @brief Zwraca liste kont uzytkownika.
	*
	* @return Wektor wskaznikow na konta uzytkownika
	*/
	vector<KontoGlowne*>& getKontaUzytkownika()
	{
		return kontaUzytkownika;
	}

	/**
	 * @brief Zwraca liste kart uzytkownika.
	 *
	 * @return Wektor wskaznikow na katry klientow
	 */
	vector<Karta*>& getKartyUzytkownika()
	{
		return kartyUzytkownika;
	}

	/**
	* @brief Zwraca liste lokat uzytkownika.
	*
	* @return Wektor lokat klienta
	*/
	vector<Lokata>& getLokatyUzytkownika()
	{
		return lokatyUzytkownika;
	}


	/**
	 * @brief Znajduje konto po numerze.
	 *
	 * @param numerKonta Numer konta do znalezienia
	 *
	 * @return Wskaznik na znalezione konto lub nullptr jesli nie znaleziono
	 */
	KontoGlowne* znajdzKonto(const string& numerKonta)
	{
		for (auto& konto : kontaUzytkownika)
		{
			if (konto->getNumerKonta() == numerKonta)
			{
				return konto;
			}
		}
		return nullptr; // Konto nie znalezione
	}


	/**
	* @brief Znajduje karte po numerze.
	*
	* @param numerKarty Numer karty do znalezienia
	*
	* @return Wskaznik na znaleziona karte lub nullptr jesli nie znaleziono
	*/
	Karta* znajdzKarte(const string& numerKarty)
	{
		for (auto& karta : kartyUzytkownika)
		{
			if (karta->getNumerKarty() == numerKarty)
			{
				return karta;
			}
		}
		return nullptr; // Karta nie znaleziona
	}

	/**
	* @brief Usuwa konto z listy kont uzytkownika.
	*
	* @param numerKonta Numer konta do usuniecia
	* @return true Jesli konto zostalo usuniete, false w przeciwnym razie
	*/
	bool usunKonto(const string& numerKonta)
	{
		for (auto it = kontaUzytkownika.begin(); it != kontaUzytkownika.end(); ++it)
		{
			if ((*it)->getNumerKonta() == numerKonta)
			{
				delete* it; // Usuwamy konto
				kontaUzytkownika.erase(it); // Usuwamy wskaznik z wektora
				return true; // Konto usuniete
			}
		}
		return false; // Konto nie znalezione
	}

	/**
	* @brief Usuwa karte z listy kart uzytkownika.
	*
	* @param numerKarty Numer karty do usuniecia
	* @return true Jesli karta zostala usunieta, false w przeciwnym razie
	*/
	bool usunKarte(const string& numerKarty)
	{
		for (auto it = kartyUzytkownika.begin(); it != kartyUzytkownika.end(); ++it)
		{
			if ((*it)->getNumerKarty() == numerKarty)
			{
				delete* it; // Usuwamy karte
				kartyUzytkownika.erase(it); // Usuwamy wskaznik z wektora
				return true; // Karta usunieta
			}
		}
		return false; // Karta nie znaleziona
	}

	/**
	* @brief Sprawdza, czy uzytkownik posiada konto o podanym numerze.
	*
	* @param numerKonta Numer konta do sprawdzenia
	* @return true Jesli konto istnieje, false w przeciwnym razie
	*/
	bool czyPosiadaKonto(const string& numerKonta) const
	{
		for (const auto& konto : kontaUzytkownika)
		{
			if (konto->getNumerKonta() == numerKonta)
			{
				return true; // Konto znalezione
			}
		}
		return false; // Konto nie znalezione
	}
    /**
	 * @brief Deklaracja funkcji edytujDane.
     *
     * Funkcja umozliwia edycje danych klienta, takich jak imie, nazwisko, czy PESEL,
     * zapisujac zmiany do pliku JSON.
     */
	void edytujDane(SystemBankowy* systemBankowy);

	/**
	 * @brief Wyswietla dane klienta.
	 *
	 * Funkcja wyswietla dane klienta, takie jak imie, nazwisko, PESEL, login i haslo.
	 */
	void wyswietlDane()
	{
		cout << "===== DANE KLIENTA =====" << endl;
		cout << "Imie: " << getImie() << endl;
		cout << "Nazwisko: " << getNazwisko() << endl;
		cout << "PESEL: " << getPesel() << endl;
		cout << "Login: " << getLogin() << endl;
		cout << "Haslo: " << string(haslo.length(), '*') << endl;
		cout << "========================" << endl;


		cout << "Liczba posiadanych kont: " << kontaUzytkownika.size() << endl;
		cout << "Liczba posiadanych kart: " << kartyUzytkownika.size() << endl;
		cout << "Liczba posiadanych lokat: " << lokatyUzytkownika.size() << endl;
	}

	/**
	* @brief Wyswietla wszystkie konta uzytkownika.
	*
	* Funkcja wyswietla wszystkie konta uzytkownika, ich numery, salda i typy.
	*/
	void wyswietlKonta()
	{
		cout << "===== KONTA =====" << endl;
		if (kontaUzytkownika.empty())
		{
			cout << "Brak kont." << endl;
		}
		else
		{
			for (size_t i = 0; i<kontaUzytkownika.size(); i++)
			{
				cout << "Konto" << (i + 1) << ":\n";
				kontaUzytkownika[i]->wyswietlInformacje();
				cout << "\n";
			}
		}

	}

	/**
	 * @brief Wyswietla wszystkie karty uzytkownika.
	*
	* Funkcja wyswietla wszystkie karty uzytkownika, ich numery, daty waznosci i typy.
	*/
	void wyswietlKarty()
	{
		cout << "===== KARTY =====" << endl;
		if (kartyUzytkownika.empty())
		{
			cout << "Brak kart." << endl;
		}
		else
		{
			for (size_t i = 0; i < kartyUzytkownika.size(); i++)
			{
				cout << "Karta" << (i + 1) << ":\n";
				kartyUzytkownika[i]->wyswietlInformacje();
				cout << "\n";
			}
		}

	}

	/**
	* @brief Wyswietla wszystkie lokaty uzytkownika.
	*
	* Funkcja wyswietla wszystkie lokaty uzytkownika, ich numery, salda i oprocentowanie.
	*/
	void wyswietlLokaty()
	{
		cout << "===== LOKATY =====" << endl;
		if (lokatyUzytkownika.empty())
		{
			cout << "Brak lokat." << endl;
		}
		else
		{
			for (size_t i = 0; i < lokatyUzytkownika.size(); i++)
			{
				cout << "Lokata" << (i + 1) << ":\n";
				lokatyUzytkownika[i].wyswietlInformacje();
				cout << "\n";
			}
		}

	}

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
	void to_json_Klient(json& j, const Klient& klient)
	{
		j["imie"] = klient.getImie();
		j["nazwisko"] = klient.getNazwisko();
		j["pesel"] = klient.getPesel();
		j["login"] = klient.getLogin();
		j["haslo"] = klient.getHaslo();
	}
	/**
	 * @brief Wczytuje informacje o kliencie z formatu JSON.
	 *
	 * Ta funkcja konwertuje dane w formacie JSON na obiekt Klient.
	 *
	 * @param j Obiekt JSON, z którego zostaną odczytane informacje o kliencie
	 * @param klient Obiekt Klient, do którego zostaną zapisane informacje
	 */
	void from_json_Klient(const json& j, Klient& klient)
	{
		klient.setImie(j.at("imie").get<string>());
		klient.setNazwisko(j.at("nazwisko").get<string>());
		klient.setPesel(j.at("pesel").get<string>());
		klient.setLogin(j.at("login").get<string>());
		klient.setHaslo(j.at("haslo").get<string>());
	}
	/**
	 * @brief Zwraca informacje o transakcji w formacie JSON.
	 *
	 * Ta funkcja konwertuje obiekt Transakcja na format JSON, aby ułatwić zapis do pliku.
	 *
	 * @param j Obiekt JSON, do którego zostaną zapisane informacje o transakcji
	 * @param transakcja Obiekt Transakcja, którego informacje mają zostać zapisane
	 */
	void to_json_Transakcja(json& j, const Transakcja& transakcja)
	{
		j["kwota"] = transakcja.getKwota();
		j["typ"] = transakcja.getTypTransakcji();
		j["data"] = transakcja.getDataTransakcji();
		j["nadawca"] = transakcja.getKontoNadawcy();
		j["odbiorca"] = transakcja.getKontoOdbiorcy();
	}
	/**
	 * @brief Wczytuje informacje o transakcji z formatu JSON.
	 *
	 * Ta funkcja konwertuje dane w formacie JSON na obiekt Transakcja.
	 *
	 * @param j Obiekt JSON, z którego zostaną odczytane informacje o transakcji
	 * @param transakcja Obiekt Transakcja, do którego zostaną zapisane informacje
	 */
	void from_json_Transakcja(const json& j, Transakcja& transakcja)
	{
		transakcja.setKwota(j.at("kwota").get<float>());
		transakcja.setTypTransakcji(j.at("typ").get<string>());
		transakcja.setDataTransakcji(j.at("data").get<string>());
		transakcja.setKontoNadawcy(j.at("nadawca").get<string>());
		transakcja.setKontoOdbiorcy(j.at("odbiorca").get<string>());
	}
	/**
	 * @brief Zwraca informacje o karcie w formacie JSON.
	 *
	 * Ta funkcja konwertuje obiekt Karta na format JSON, aby ułatwić zapis do pliku.
	 *
	 * @param j Obiekt JSON, do którego zostaną zapisane informacje o karcie
	 * @param karta Obiekt Karta, którego informacje mają zostać zapisane
	 */
	void to_json_Karta(json& j, const Karta& karta)
	{
		j["numer_karty"] = karta.getNumerKarty();
		j["data_waznosci"] = karta.getDataWaznosci();
		j["typ_karty"] = karta.getTypKarty();
		j["cvc"] = karta.getKodCVC();
		j["pin"] = karta.getPin();

		if (const KartaDebetowa* debetowa = dynamic_cast<const KartaDebetowa*>(&karta))
		{
			j["powiazane_konto"] = debetowa->getPowiazaneKonto();
			j["dzienny_limit"] = debetowa->getDziennyLimit();
		}

	}
	/**
	 * @brief Wczytuje informacje o karcie z formatu JSON.
	 *
	 * Ta funkcja konwertuje dane w formacie JSON na obiekt Karta.
	 *
	 * @param j Obiekt JSON, z którego zostaną odczytane informacje o karcie
	 *
	 * @return Wskaznik na obiekt Karta, lub nullptr w przypadku błędu
	 */
	Karta* from_json_Karta(const json& j)
	{
		string typ = j.at("typ_karty").get<string>();
		string numer = j.at("numer_karty").get<string>();
		string data = j.at("data_waznosci").get<string>();
		string cvc = j.at("cvc").get<string>();

		if (typ == "Debetowa")
		{
			string konto = j.at("powiazane_konto").get<string>();
			float limit = j.at("dzienny_limit").get<float>();
			return new KartaDebetowa(numer, data, cvc, konto, limit);
		}
		return nullptr;
	}
	/**
	 * @brief Zwraca informacje o lokacie w formacie JSON.
	 *
	 * Ta funkcja konwertuje obiekt Lokata na format JSON, aby ułatwić zapis do pliku.
	 *
	 * @param j Obiekt JSON, do którego zostaną zapisane informacje o lokacie
	 * @param lokata Obiekt Lokata, którego informacje mają zostać zapisane
	 */
	void to_json_Lokata(json& j, const Lokata& lokata)
	{
		j["kwota"] = lokata.getKwota();
		j["oprocentowanie"] = lokata.getOprocentowanie();
		j["data_oddania"] = lokata.getDataOddania();
		j["powiazane_konto"] = lokata.getPowiazaneKonto();
	}
	/**
	 * @brief Wczytuje informacje o lokacie z formatu JSON.
	 *
	 * Ta funkcja konwertuje dane w formacie JSON na obiekt Lokata.
	 *
	 * @param j Obiekt JSON, z którego zostaną odczytane informacje o lokacie
	 * @param lokata Obiekt Lokata, do którego zostaną zapisane informacje
	 */
	void from_json_Lokata(const json& j, Lokata& lokata)
	{
		lokata.setKwota(j.at("kwota").get<float>());
		lokata.setOprocentowanie(j.at("oprocentowanie").get<float>());
		lokata.setDataOddania(j.at("data_oddania").get<string>());
		lokata.setPowiazaneKonto(j.at("powiazane_konto").get<string>());

	}
	/**
	 * @brief Zwraca informacje o koncie w formacie JSON.
	 *
	 * Ta funkcja konwertuje obiekt Konto na format JSON, aby ułatwić zapis do pliku.
	 *
	 * @param j Obiekt JSON, do którego zostaną zapisane informacje o koncie
	 * @param konto Obiekt Konto, którego informacje mają zostać zapisane
	 */
	void to_json_Konto(json& j, const KontoGlowne& konto)
	{
		j["numer"] = konto.getNumerKonta();
		j["typ"] = konto.getTypKonta();
		j["saldo"] = konto.getSaldoKonta();


		if (const KontoOszczednosciowe* oszcz = dynamic_cast<const KontoOszczednosciowe*>(&konto))
		{
			j["oprocentowanie"] = oszcz->getOprocentowanie();
			j["data_kapitalizacji"] = oszcz->getDataOstatniejKapitalizacji();
			j["limit_wyplat"] = oszcz->getOgraniczenieWyplat();
			j["wlasciciel"] = konto.getWlasciciel();
		}
		else
		{
			j["wlasciciel"] = konto.getWlasciciel();
		}
	}
	/**
	 * @brief Wczytuje informacje o koncie z formatu JSON.
	 *
	 * Ta funkcja konwertuje dane w formacie JSON na obiekt Konto.
	 *
	 * @param j Obiekt JSON, z którego zostaną odczytane informacje o koncie
	 */
	KontoGlowne* from_json_Konto(const json& j)
	{
		string typ = j.at("typ").get<string>();
		string numer = j.at("numer").get<string>();
		float saldo = j.at("saldo").get<float>();
		string wlasciciel = j.at("wlasciciel").get<string>();

		KontoGlowne* noweKonto = nullptr;

		if (typ == "Oszczednosciowe")
		{
			float oprocentowanie = j.at("oprocentowanie").get<float>();
			string data = j.at("data_kapitalizacji").get<string>();
			int limit = j.at("limit_wyplat").get<int>();
			noweKonto = new KontoOszczednosciowe(numer, saldo, oprocentowanie, data, limit);
		}
		else
		{
			noweKonto =  new KontoGlowne(numer, typ, saldo);
		}

		if (noweKonto)
		{
			noweKonto->setWlascicielel(wlasciciel);
		}
		return noweKonto;
	}

public:

	/**
	* @brief Konstruktor klasy FileManager.
	*
	* Inicjalizuje obiekt FileManager z podaną nazwą pliku.
	*
	* @param nazwa Nazwa pliku do odczytu/zapisu
	*/
	FileManager(const string& nazwa = "dane.json")
		: nazwaPliku(nazwa) {}


	/**
	 * @brief Zapisuje dane klientów do pliku JSON.
	 * @param klienci Wektor klientów do zapisania
	 */
	void zapiszKlientow(const vector<Klient>& klienci)
	{
		json j;
		for (const auto& klient : klienci)
		{
			json klientJson;
			to_json_Klient(klientJson, klient);
			j.push_back(klientJson);
		}
		ofstream plik(nazwaPliku);
		if (plik.is_open())
		{
			plik << j.dump(4); // Zapisujemy z wcięciem 4 spacji
			plik.close();
		}
		else
		{
			cout << "Nie mozna otworzyc pliku do zapisu." << endl;
		}

	}
	/**
	 * @brief Odczytuje dane klientów z pliku JSON.
	 * @return Wektor klientów odczytanych z pliku
	 */
	vector<Klient> wczytajKlientow()
	{
		vector<Klient> klienci;
		ifstream plik(nazwaPliku);
		if (plik.is_open())
		{
			json j;
			try {
				plik >> j;
				for (const auto& klientJson : j)
				{
					Klient klient;
					from_json_Klient(klientJson, klient);
					klienci.push_back(klient);
				}
			}
				catch (const exception& e)
				{
					cout << "Blad odczytu pliku: " << e.what() << endl;
				}

			plik.close();
		}
		return klienci;

	}
	/**
	 * @brief Zapisuje historię transakcji do pliku JSON.
	 * @param transakcje Wektor transakcji do zapisania
	 */
	void zapiszTransakcje(const vector<Transakcja>& transakcje)
	{
		json j;
		for (const auto& t : transakcje)
		{
			json transakcjaJson;
			to_json_Transakcja(transakcjaJson, t);
			j.push_back(transakcjaJson);
		}

		ofstream plik("transakcje_" + nazwaPliku);
		if (plik.is_open())
		{
			plik << j.dump(4); // Zapisujemy z wcięciem 4 spacji
			plik.close();
		}
		else
		{
			cout << "Nie mozna otworzyc pliku do zapisu." << endl;
		}
	}
	/**
	 * @brief Wczytuje historię transakcji z pliku JSON.
	 * @return Wektor transakcji odczytanych z pliku
	 */
	vector<Transakcja> wczytajTransakcje()
	{
		vector<Transakcja> transakcje;
		ifstream plik("transakcje_" + nazwaPliku);

		if (plik.is_open())
		{
			json j;
			try {
				plik >> j;
				for (const auto& item : j) {
					Transakcja t;
					from_json_Transakcja(item, t);
					transakcje.push_back(t);
				}
			}
			catch (const exception& e)
			{
				cout << "Blad odczytu pliku: " << e.what() << endl;
			}
			plik.close();
		}
		return transakcje;
	}
	/**
	 * @brief Zapisuje dane kart do pliku JSON.
	 * @param karty Wektor kart do zapisania
	 */
	void zapiszKarty(const vector<Karta*>& karty)
	{
		json j;
		for (const auto& karta : karty)
		{
			json kartaJson;
			to_json_Karta(kartaJson, *karta);
			j.push_back(kartaJson);
		}
		ofstream plik("karty_" + nazwaPliku);
		if (plik.is_open())
		{
			plik << j.dump(4); // Zapisujemy z wcięciem 4 spacji
			plik.close();
		}
		else
		{
			cerr << "Nie mozna otworzyc pliku do zapisu." << endl;
		}

	}
	/**
	 * @brief Wczytuje dane kart z pliku JSON.
	 * @return Wektor kart odczytanych z pliku
	 */
	vector<Karta*> wczytajKarty()
	{
		vector<Karta*> karty;
		ifstream plik("karty_" + nazwaPliku);
		if (plik.is_open())
		{
			json j;
			try {
				plik >> j;
				for (const auto& kartaJson : j)
				{
					Karta* karta = from_json_Karta(kartaJson);
					if (karta != nullptr)
					{
						karty.push_back(karta);
					}
				}
			}
			catch (const exception& e)
			{
				cout << "Blad odczytu pliku: " << e.what() << endl;
			}
			plik.close();
		}
		return karty;

	}
	/**
	 * @brief Zapisuje dane lokat do pliku JSON.
	 * @param lokaty Wektor lokat do zapisania
	 */
	void zapiszLokaty(const vector<Lokata>& lokaty)
	{
		json j;
		for (const auto& lokata : lokaty)
		{
			json lokataJson;
			to_json_Lokata(lokataJson, lokata);
			j.push_back(lokataJson);
		}
		ofstream plik("lokaty_" + nazwaPliku);
		if (plik.is_open())
		{
			plik << j.dump(4); // Zapisujemy z wcięciem 4 spacji
			plik.close();
		}
		else
		{
			cerr << "Nie mozna otworzyc pliku do zapisu." << endl;
		}


	}
	/**
	 * @brief Wczytuje dane lokat z pliku JSON.
	 * @return Wektor lokat odczytanych z pliku
	 */
	vector<Lokata> wczytajLokaty()
	{
		vector<Lokata> lokaty;
		ifstream plik("lokaty_" + nazwaPliku);
		if (plik.is_open())
		{
			json j;
			try {
				plik >> j;
				for (const auto& lokataJson : j)
				{
					Lokata lokata;
					from_json_Lokata(lokataJson, lokata);
					lokaty.push_back(lokata);
				}
			}
			catch (const exception& e)
			{
				cout << "Blad odczytu pliku: " << e.what() << endl;
			}
			plik.close();
		}
		return lokaty;


	}
	/**
	 * @brief Zapisuje dane kont do pliku JSON.
	 * @param konta Wektor kont do zapisania
	 */
	void zapiszKonta(const vector<KontoGlowne*>& konta)
	{
		json j;
		for (const auto& konto : konta)
		{
			json kontoJson;
			to_json_Konto(kontoJson, *konto);
			j.push_back(kontoJson);
		}
		ofstream plik("konta_" + nazwaPliku);
		if (plik.is_open())
		{
			plik << j.dump(4); // Zapisujemy z wcięciem 4 spacji
			plik.close();
		}
		else
		{
			cerr << "Nie mozna otworzyc pliku do zapisu." << endl;
		}


	}
	/**
	 * @brief Wczytuje dane kont z pliku JSON.
	 * @return Wektor kont odczytanych z pliku
	 */
	vector<KontoGlowne*> wczytajKonta()
	{
		vector<KontoGlowne*> konta;
		ifstream plik("konta_" + nazwaPliku);
		if (plik.is_open())
		{
			json j;
			try {
				plik >> j;
				for (const auto& kontoJson : j)
				{
					KontoGlowne* konto = from_json_Konto(kontoJson);
					if (konto != nullptr)
					{
						konta.push_back(konto);
					}
				}
			}
			catch (const exception& e)
			{
				cout << "Blad odczytu pliku: " << e.what() << endl;
			}
			plik.close();
		}
		return konta;


	}

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
	vector<Karta*> wszystkieKarty;
	vector<Lokata> wszystkieLokaty;
	vector<KontoGlowne*> wszystkieKonta;
	Klient* zalogowanyKlient; ///< Wskaźnik na aktualnie zalogowanego klienta
	FileManager menedzerPlikow; ///< Obiekt do zarządzania plikami
public:
	/**
	 * @brief Konstruktor klasy SystemBankowy.
	 *
	 * Wczytuje dane klientów, transakcji, kart, kont i lokat z plików JSON podczas inicjalizacji.
	 */
	SystemBankowy() : zalogowanyKlient(nullptr)
	{
		klienci = menedzerPlikow.wczytajKlientow();
		transakcje = menedzerPlikow.wczytajTransakcje();
		wszystkieKarty = menedzerPlikow.wczytajKarty();
		wszystkieLokaty = menedzerPlikow.wczytajLokaty();
		wszystkieKonta = menedzerPlikow.wczytajKonta();

		for (auto& klient : klienci)
		{
			for (auto konto : wszystkieKonta)
			{
				if (konto->getWlasciciel() == klient.getPesel())
				{
					klient.dodajKonto(konto);
				}
			}

			for (auto karta : wszystkieKarty)
			{
				if (auto kartaDebetowa = dynamic_cast<KartaDebetowa*>(karta))
				{
					for (auto kontoKlienta : klient.getKontaUzytkownika())
					{
						if (kartaDebetowa->getPowiazaneKonto() == kontoKlienta->getNumerKonta())
						{
							klient.dodajKarte(karta);
							break; // Zatrzymujemy petle, gdy znajdziemy powiazanie
						}
					}
				}
			}

			for (auto lokata : wszystkieLokaty)
			{
				for (auto kontoKlienta : klient.getKontaUzytkownika())
				{
					if (lokata.getPowiazaneKonto() == kontoKlienta->getNumerKonta())
					{
						klient.dodajLokate(lokata);
						break; // Zatrzymujemy petle, gdy znajdziemy powiazanie
					}
				}
			}
		}
	}

	/**
	* @brief Destruktor klasy SystemBankowy.
	*
	* Zwalnia pamięć zajmowaną przez wszystkie obiekty.
	*/
	~SystemBankowy()
	{
		for (auto karta : wszystkieKarty)
		{
			bool czyPowiazana = false;
			for (auto& klient : klienci)
			{
				for (auto kartaKlienta : klient.getKartyUzytkownika())
				{
					if (kartaKlienta == karta)
					{
						czyPowiazana = true;
						break;
					}
				}
				if (czyPowiazana) break;
			}
			if (!czyPowiazana) delete karta; // Usuwamy karte

		}

		for (auto konto : wszystkieKonta)
		{
			bool czyPowiazane = false;
			for (auto& klient : klienci)
			{
				for (auto kontoKlienta : klient.getKontaUzytkownika())
				{
					if (kontoKlienta == konto)
					{
						czyPowiazane = true;
						break;
					}
				}
				if (czyPowiazane) break;
			}
			if (!czyPowiazane) delete konto; // Usuwamy konto
		}
	}
	/**
	 * @brief Uruchamia system bankowy.
	 *
	 * Główna metoda sterująca przepływem programu. Wyświetla menu główne
	 * i obsługuje wybory użytkownika do moment wyjścia z programu.
	 */
	void uruchom() {
		bool stop = false;
		do {
			wyswietlMenuGlowne();
			cout << "Wybierz opcje: ";

			int wybor;
			if (!(cin >> wybor)) {
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << "Niepoprawny wybor. Sprobuj ponownie." << endl;
			} else {
				cin.ignore(numeric_limits<streamsize>::max(), '\n');

				switch (wybor) {
					case 1:
						zarejestrujKlienta();
						break;
					case 2:
						if (zalogujKlienta()) {
							obslugaZalogowanegoUzytkownika();
						}
						break;
					case 0:
						cout << "Koniec programu." << endl;
						stop = true;
						break;
					default:
						cout << "Niepoprawny wybor. Sprobuj ponownie." << endl;
						break;
				}
			}

		} while (!stop);
	}

	/**
	 * @brief Usuwa wybrane konto klienta wraz z powiązanymi kartami i lokatami.
	 *
	 * Funkcja ta umożliwia zalogowanemu klientowi usunięcie jednego ze swoich kont bankowych.
	 */
	void usunKonto()
	{

		auto& konta = zalogowanyKlient->getKontaUzytkownika();
		for (size_t i = 0; i < konta.size(); ++i) {
			cout << i + 1 << ". " << konta[i]->getNumerKonta() << " (" << konta[i]->getTypKonta() << ")" << endl;
		}
		cout << "Wybierz numer konta do usuniecia: ";
		int wybor;
		cin >> wybor;

		if (wybor > 0 && wybor <= konta.size()) {
			string numer = konta[wybor - 1]->getNumerKonta();

		
			

			auto& lokaty = zalogowanyKlient->getLokatyUzytkownika();
			for (auto it = lokaty.begin(); it != lokaty.end(); ) {
				if (it->getPowiazaneKonto() == numer) {
					it = lokaty.erase(it);
				}
				else {
					++it;
				}
			}

			
			auto& karty = zalogowanyKlient->getKartyUzytkownika();
			for (auto it = karty.begin(); it != karty.end(); ) {
				KartaDebetowa* karta = dynamic_cast<KartaDebetowa*>(*it);
				if (karta && karta->getPowiazaneKonto() == numer) {
					it = karty.erase(it);
				}
				else {
					++it;
				}
			}


			
			for (auto it = wszystkieKonta.begin(); it != wszystkieKonta.end(); ++it) {
				if ((*it)->getNumerKonta() == numer) {
					wszystkieKonta.erase(it); 
					break;
				}

			}

			if (zalogowanyKlient->usunKonto(numer)) {
				cout << "Konto oraz powiązane karty i lokaty zostaly usuniete." << endl;

				menedzerPlikow.zapiszKonta(wszystkieKonta);
				menedzerPlikow.zapiszKarty(zalogowanyKlient->getKartyUzytkownika());
				menedzerPlikow.zapiszLokaty(zalogowanyKlient->getLokatyUzytkownika());
			}
			else {
				cout << "Nie znaleziono konta." << endl;
			}
		}
	}

	/**
	 * @brief Usuwa wybrana karte klienta.
	 *
	 * Funkcja ta umożliwia zalogowanemu klientowi usunięcie jednej ze swoich kart bankowych.
	 */
	void usunKarte()
	{
		auto& karty = zalogowanyKlient->getKartyUzytkownika();
		for (size_t i = 0; i < karty.size(); ++i) {
			cout << i + 1 << ". " << karty[i]->getNumerKarty() << " (" << karty[i]->getTypKarty() << ")" << endl;
		}
		cout << "Wybierz numer karty do usuniecia: ";
		int wybor;
		cin >> wybor;
		if (wybor > 0 && wybor <= karty.size()) {
			string numer = karty[wybor - 1]->getNumerKarty();
			if (zalogowanyKlient->usunKarte(numer)) {
				cout << "Karta zostala usunieta." << endl;
				menedzerPlikow.zapiszKarty(zalogowanyKlient->getKartyUzytkownika());
			}
			else {
				cout << "Nie znaleziono karty." << endl;
			}
		}
	}

	
	/**
	 * @brief Wyświetla menu główne systemu bankowego.
	 *
	 * Pokjazuje dostępne opcje dla niezalogowanego użytkownika,
	 * takiej jak rejestracja, logowanie, czy wyjście z programu.
	 */
	void wyswietlMenuGlowne()
	{
		cout << "===== SYSTEM BANKOWY =====" << endl;
		cout << "1. Rejestracja" << endl;
		cout << "2. Logowanie" << endl;
		cout << "0. Wyjscie" << endl;

	}
	/**
	 * @brief Obsługuje zalogowanego użytkownika.
	 *
	 * Wyświtla menu dla zalogowanego użytkownika i obsługuje
	 * jego wybory do momentu wylogowania lub zakończenia programu.
	 */
	void obslugaZalogowanegoUzytkownika() {
		bool stop = false;
		do {
			wyswietlMenuUzytkownika();

			int wybor;

			if (!(cin >> wybor)) {
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << "Niepoprawny wybor. Sprobuj ponownie." << endl;
			} else {
				cin.ignore(numeric_limits<streamsize>::max(), '\n');

				switch (wybor) {
					case 1:
						zalogowanyKlient->wyswietlDane();
						break;
					case 2:
						zalogowanyKlient->edytujDane(this);
						break;
					case 3:
						zalogowanyKlient->wyswietlKonta();
						break;
					case 4:
						zalogowanyKlient->wyswietlKarty();
						break;
					case 5:
						zalogowanyKlient->wyswietlLokaty();
						break;
					case 6:
						dodajKonto();
						break;
					case 7:
						dodajKarte();
						break;
					case 8:
						zalozLokate();
						break;
					case 9:
						wykonajPrzelew();
						break;
					case 10:
						wyswietlHistorieTransakcji();
						break;
					case 11:
						cout << "Uwaga! Usuniecie konta spowoduje usuniecie wszystkich powiazanych kart i lokat." << endl;
						cout << "Wpisz 1 zeby przejsc dalej, lub 0 zeby anulowac wybor" << endl;
						
					do{
						int wybor = -1;
						cin >> wybor;
					if (wybor == 1)
					{
						usunKonto();
						break;
					} else if (wybor == 0)
					{
						wyswietlMenuUzytkownika();
						break;
					}else
					{
						cout << "Niepoprawny wybor. Sprobuj ponownie." << endl;
					}
					} while (wybor != 1 && wybor != 0);
					break;

					case 12:
						usunKarte();
						break;
					case 0:
						zalogowanyKlient = nullptr; // Wylogowanie
						cout << "Wylogowano." << endl;
						stop = true;
						break;
					default:
						cout << "Niepoprawny wybor. Sprobuj ponownie." << endl;
						continue;
				}

				if (wybor != 0) {
					cout << "Nacisnij dowolny klawisz, aby kontynuowac..." << endl;
					cin.get();
				}
			}
		} while (!stop);
	}
	/**
	 * @brief Wyświetla menu dla zalogowanego użytkownika.
	 *
	 * Pokazuje dostępne opcje dla zalogowanego użytkownika, takie jak
	 * zarządzanie danymi osobowymi, kontami, kartami, lokatami, czy transakcjami.
	 */
	void wyswietlMenuUzytkownika()
	{
		cout << "===== MENU UZYTKOWNIKA =====" << endl;
		cout << "1. Wyswietl dane osobowe" << endl;
		cout << "2. Edytuj dane osobowe" << endl;
		cout << "3. Wyswietl konta" << endl;
		cout << "4. Wyswietl karty" << endl;
		cout << "5. Wyswietl lokaty" << endl;
		cout << "6. Dodaj konto" << endl;
		cout << "7. Dodaj karte" << endl;
		cout << "8. Zaloz lokate" << endl;
		cout << "9. Wykonaj przelew" << endl;
		cout << "10. Wyswietl historie transakcji" << endl;
		if (zalogowanyKlient->getKontaUzytkownika().size() > 0)
		{
			cout << "11. Usun konto" << endl;
		}
		if (zalogowanyKlient->getKartyUzytkownika().size() > 0)
		{
			cout << "12. Usun karte" << endl;
		}
		cout << "0. Wyloguj" << endl;
		cout << "Wybierz opcje: ";
	}
	/**
	 * @brief Obsługuje proces rejestracji nowego klienta.
	 *
	 * Zbiera dane od użytkownika, tworzy noy obiekt Klient
	 * i dodaje go do listy klientów banku.
	 */
	void zarejestrujKlienta()
	{
		string imie, nazwisko, pesel, login, haslo;

		cout << "===== REJESTRACJA =====" << endl;
		cout << "Podaj imie: ";
		cin >> imie;
		cout << "Podaj nazwisko: ";
		cin >> nazwisko;
		cout << "Podaj PESEL: ";
		cin >> pesel;
		cout << "Podaj login: ";
		cin >> login;
		cout << "Podaj haslo: ";
		cin >> haslo;

		if (sprawdzCzyLoginIstnieje(login))
		{
			cout << "Login jest juz zajety. Wybierz inny." << endl;
			return;
		}

		Klient nowyKlient(imie, nazwisko, pesel, login, haslo);
		klienci.push_back(nowyKlient);
		menedzerPlikow.zapiszKlientow(klienci); // Zapisujemy zmiany do pliku

		zalogowanyKlient = &klienci.back(); // Ustawiamy wskaźnik na nowego klienta
		cout << "Rejestracja zakonczona sukcesem!" << endl;
	}
	/**
	 * @brief Obsługuje proces logowania klienta.
	 *
	 * Sprawdza poprawność podanych danych logowania i
	 * ustawia wskaźnik zalogowanegoKlienta.
	 *
	 * @return true jeśli logowanie powiodło się, false w przeciwnym razie
	 */
	bool zalogujKlienta()
	{
		string login, haslo;
		cout << "===== LOGOWANIE =====" << endl;
		cout << "Podaj login: ";
		cin >> login;
		cout << "Podaj haslo: ";
		cin >> haslo;

		for (auto& klient : klienci)
		{
			if (klient.getLogin() == login && klient.getHaslo() == haslo)
			{
				zalogowanyKlient = &klient;
				cout << "Logowanie zakonczone sukcesem!" << endl;
				return true;
			}
		}

		cout << "Niepoprawny login lub haslo." << endl;
		return false;
	}

	/**
	 * @brief Zapisywanie (edytowanych) danych klientów do pliku.
	 * 
	 * 
	 */
	void zapiszDaneKlientow()
	{
		menedzerPlikow.zapiszKlientow(klienci); // Zapisujemy zmiany do pliku
		menedzerPlikow.zapiszKonta(wszystkieKonta); // Zapisujemy konta
	}
	/**
	 * @brief Dodaje nowe konto dla zalogowanego klienta.
	 *
	 * Umożliwia zalogowanemu klientowi dodanie nowego konta
	 * bankowego wybranego typu
	 */
	void dodajKonto()
	{

		string numerKonta = to_string(rand() % 1000000000 + 1); //

		cout << "===== DODAWANIE KONTO =====" << endl;
		cout << "Wybierz typ konta:" << endl;
		cout << "1. Glowne" << endl;
		cout << "2. Oszczednosciowe" << endl;

		int typ;
		while (cout << "Wybierz opcje: " && (!(cin >> typ) || !(typ == 1 || typ == 2))) {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Niepoprawny wybor. Sprobuj ponownie." << endl;
		}
		cin.ignore(numeric_limits<streamsize>::max(), '\n');

		float saldo;
		while (cout << "Podaj saldo poczatkowe: " && !(cin >> saldo)) {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Niepoprawny wybor. Sprobuj ponownie." << endl;
		}
		cin.ignore(numeric_limits<streamsize>::max(), '\n');

		if (typ == 1)
		{
			KontoGlowne* noweKonto = new KontoGlowne(numerKonta, "Glowne", saldo);
			noweKonto->setWlascicielel(zalogowanyKlient->getPesel());
			zalogowanyKlient->dodajKonto(noweKonto);
			wszystkieKonta.push_back(noweKonto);
		}
		else if (typ == 2)
		{
			float oprocentowanie;
			while (cout << "Podaj oprocentowanie: " && !(cin >> oprocentowanie)) {
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << "Niepoprawny wybor. Sprobuj ponownie." << endl;
			}
			cin.ignore(numeric_limits<streamsize>::max(), '\n');

			int limit;
			while (cout << "Podaj limit wyplat: " && !(cin >> limit)) {
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << "Niepoprawny wybor. Sprobuj ponownie." << endl;
			}
			cin.ignore(numeric_limits<streamsize>::max(), '\n');

			time_t now = time(nullptr);
			tm today;
			localtime_s(&today, &now);

			ostringstream oss;
			oss << setw(2) << setfill('0') << (today.tm_mon + 1);
			oss << setw(2) << setfill('0') << ((today.tm_year + 1900) % 100);
			string dataKapitalizacji = oss.str();

			KontoOszczednosciowe* noweKonto = new KontoOszczednosciowe(numerKonta, saldo, oprocentowanie, dataKapitalizacji, limit);
			noweKonto->setWlascicielel(zalogowanyKlient->getPesel());
			zalogowanyKlient->dodajKonto(noweKonto);
			wszystkieKonta.push_back(noweKonto);
		}
		menedzerPlikow.zapiszKonta(wszystkieKonta); // Zapisujemy zmiany do pliku
		cout << "Konto dodane pomyslnie!" << endl;
	}
	/**
	 * @brief Dodaje nową kartę dla zalogowanego klienta.
	 *
	 * Umożliwia zalogowanemu klientowi dodanie nowej karty
	 * bankowej wybranego typu
	 */
	void dodajKarte()
	{
		if (zalogowanyKlient->getKontaUzytkownika().empty())
		{
			cout << "Nie masz zadnych kont. Dodaj konto przed dodaniem karty." << endl;
			return;
		}

		string numerKarty, dataWaznosci, cvc, pin;

		cout << "===== DODAWANIE KARTY =====" << endl;
		cout << "Wybierz konto do ktorego chcesz dodac karte: " << endl;
		for (size_t i = 0; i < zalogowanyKlient->getKontaUzytkownika().size(); ++i)
		{
			cout << i + 1 << ". " << zalogowanyKlient->getKontaUzytkownika()[i]->getNumerKonta() << endl;
		}

		int wyborKonta;
		while (cout << "Wybierz opcje: " && (!(cin >> wyborKonta) || (wyborKonta < 1 || wyborKonta > zalogowanyKlient->getKontaUzytkownika().size()))) {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Niepoprawny wybor. Sprobuj ponownie." << endl;
		}
		cin.ignore(numeric_limits<streamsize>::max(), '\n');

		string powiazaneKonto = zalogowanyKlient->getKontaUzytkownika()[wyborKonta - 1]->getNumerKonta();
		cout << "Podaj numer karty: (16 cyfr) ";
		cin >> numerKarty;
		cout << "Podaj date waznosci karty: (MMRR) ";
		cin >> dataWaznosci;
		cout << "Podaj kod CVC: (3 cyfry) ";
		cin >> cvc;
		cout << "Podaj PIN: (4 cyfry) ";
		cin >> pin;

		float limit;
		while (cout << "Podaj dzienny limit transakcji: " && !(cin >> limit)) {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Niepoprawny wybor. Sprobuj ponownie." << endl;
		}
		cin.ignore(numeric_limits<streamsize>::max(), '\n');

		try
		{
			KartaDebetowa* nowaKarta = new KartaDebetowa(numerKarty, dataWaznosci, cvc,
				powiazaneKonto, limit);
			nowaKarta->setPin(pin);
			zalogowanyKlient->dodajKarte(nowaKarta);
			menedzerPlikow.zapiszKarty(zalogowanyKlient->getKartyUzytkownika()); // Zapisujemy zmiany do pliku

			cout << "Karta dodana pomyslnie!" << endl;
		} catch (const exception& e)
		{
			cout << "Blad podczas dodawania karty " << e.what() << endl;
		}
	}
	/**
	 * @brief Zakłada nową lokatę dla zalogowanego klienta.
	 *
	 * Umożliwia zalogowanemu klientowi dodanie nowej lokaty
	 * na wybraną kwotę i okres
	 */
	void zalozLokate()
	{
		if (zalogowanyKlient->getKontaUzytkownika().empty())
		{
			cout << "Nie masz zadnych kont. Dodaj konto przed dodaniem lokaty" << endl;
			return;
		}

		cout << "===== ZAKLADANIE LOKATY =====" << endl;
		cout << "Wybierz konto do ktorego chcesz dodac lokate: " << endl;
		for (size_t i = 0; i < zalogowanyKlient->getKontaUzytkownika().size(); ++i)
		{
			cout << i + 1 << ". " << zalogowanyKlient->getKontaUzytkownika()[i]->getNumerKonta() << endl;
		}

		int wyborKonta;
		while (cout << "Wybierz opcje: " && (!(cin >> wyborKonta) || (wyborKonta < 1 || wyborKonta > zalogowanyKlient->getKontaUzytkownika().size()))) {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Niepoprawny wybor. Sprobuj ponownie." << endl;
		}
		cin.ignore(numeric_limits<streamsize>::max(), '\n');

		KontoGlowne* wybraneKonto = zalogowanyKlient->getKontaUzytkownika()[wyborKonta - 1];

		float kwota;
		while (cout << "Podaj kwote lokaty: " && !(cin >> kwota)) {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Niepoprawny wybor. Sprobuj ponownie." << endl;
		}
		cin.ignore(numeric_limits<streamsize>::max(), '\n');

		float oprocentowanie;
		while (cout << "Podaj oprocentowanie (%): " && !(cin >> oprocentowanie)) {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Niepoprawny wybor. Sprobuj ponownie." << endl;
		}
		cin.ignore(numeric_limits<streamsize>::max(), '\n');

		string dataOddania;
		cout << "Podaj date oddania lokaty: (MMRR) ";
		cin >> dataOddania;

		if (wybraneKonto->getSaldoKonta() < kwota)
		{
			cout << "Nie masz wystarczających środków na koncie." << endl;
			return;
		}

		try
		{
			wybraneKonto->wyplac(kwota);
			Lokata nowaLokata(kwota, oprocentowanie, dataOddania, wybraneKonto->getNumerKonta());
			zalogowanyKlient->dodajLokate(nowaLokata);
			menedzerPlikow.zapiszLokaty(zalogowanyKlient->getLokatyUzytkownika()); // Zapisujemy zmiany do pliku

			cout << "Lokata zalozona pomyslnie!" << endl;
		}
		catch (const exception& e)
		{
			cout << "Blad podczas zakladania lokaty: " << e.what() << endl;
		}
	}
	/**
	 * @brief Wykonuje przelew z konta zalogowanego klienta.
	 *
	 * Umożliwia zalogowanemu klientowi wykonanie przelewu
	 * z jednego konta na drugie, tworząc odpowiedni rekord transakcji.
	 */
	void wykonajPrzelew()
	{
		if (zalogowanyKlient->getKontaUzytkownika().empty())
		{
			cout << "Nie masz zadnych kont. Dodaj konto przed wykonaniem przelewu." << endl;
			return;
		}

		cout << "===== WYKONYWANIE PRZELEWU =====" << endl;
		cout << "Wybierz konto z ktorego chcesz wykonac przelew: " << endl;
		for (size_t i = 0; i < zalogowanyKlient->getKontaUzytkownika().size(); ++i)
		{
			cout << i + 1 << ". " << zalogowanyKlient->getKontaUzytkownika()[i]->getNumerKonta() << endl;
		}

		int wyborKonta;
		while (cout << "Wybierz opcje: " && (!(cin >> wyborKonta) || (wyborKonta < 1 || wyborKonta > zalogowanyKlient->getKontaUzytkownika().size()))) {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Niepoprawny wybor. Sprobuj ponownie." << endl;
		}
		cin.ignore(numeric_limits<streamsize>::max(), '\n');

		KontoGlowne* wybraneKonto = zalogowanyKlient->getKontaUzytkownika()[wyborKonta - 1];

		string numerKontaDocelowego;
		cout << "Podaj numer konta docelowego: ";
		cin >> numerKontaDocelowego;

		float kwota;
		while (cout << "Podaj kwote przelewu: " && !(cin >> kwota)) {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Niepoprawny wybor. Sprobuj ponownie." << endl;
		}
		cin.ignore(numeric_limits<streamsize>::max(), '\n');

		if (wybraneKonto->wyplac(kwota)) {
			time_t now = time(nullptr);
			tm today;
			localtime_s(&today, &now);

			ostringstream ss;
			ss << put_time(&today, "%m/%Y");

			Transakcja transakcja;
			transakcja.setKwota(kwota);
			transakcja.setDataTransakcji(ss.str());
			transakcja.setTypTransakcji("przelew");
			transakcja.setKontoNadawcy(wybraneKonto->getNumerKonta());
			transakcja.setKontoOdbiorcy(numerKontaDocelowego);
			transakcje.push_back(transakcja);
			menedzerPlikow.zapiszTransakcje(transakcje); // Zapisujemy zmiany do pliku
			cout << "Przelew wykonany pomyslnie!" << endl;
		}

	}
	/**
	 * @brief Wyświetla historię transakcji dla zalogowanego klienta.
	 *
	 * Pokazuje listę wszystkich transakcji związanych z kotami
	 * zalogowanego kllienta.
	 */
	void wyswietlHistorieTransakcji()
	{
		cout << "===== HISTORIA TRANSAKCJI =====" << endl;
		bool znaleziono = false;

		for (const auto& transakcja : transakcje)
		{
			for (const auto& konto : zalogowanyKlient->getKontaUzytkownika())
			{
				if (transakcja.getKontoNadawcy() == konto->getNumerKonta() ||
					transakcja.getKontoOdbiorcy() == konto->getNumerKonta())
				{
					transakcja.wyswietlSzczegolyTransakcji();
					znaleziono = true;
					break; // Przerywamy pętlę, jeśli znaleziono transakcję
				}

			}
		}
		if (!znaleziono)
		{
			cout << "Brak transakcji do wyswietlenia." << endl;
		}
	}
	/**
	 * @brief Sprawdza, czy podany login jesy już zajęty.
	 *
	 * Weryfikuje, czy podany login nie jest już używany przez
	 * innego klienta w systemie.
	 *
	 * @param login Login do sprawdzenia
	 * @return true jeśli login istnieje, false w przeciwnym razie
	 */
	bool sprawdzCzyLoginIstnieje(const string& login)
	{
		for (const auto& klient : klienci)
		{
			if (klient.getLogin() == login)
			{
				return true;
			}
		}
		return false;
	}
};

/**
 * @brief Edytuje dane klienta.
 *
 * Funkcja umozliwia edycje danych klienta, takich jak imie, nazwisko, czy PESEL,
 * zapisujac zmiany do pliku JSON.
 */
void Klient:: edytujDane(SystemBankowy* systemBankowy) {
	cout << "===== EDYCJA DANYCH =====" << endl;
	cout << "1. Imie" << endl;
	cout << "2. Nazwisko" << endl;
	cout << "3. PESEL" << endl;
	cout << "4. Haslo" << endl;
	cout << "0. Powrot" << endl;

	int opcja;
	while (cout << "Wybierz opcje: " && (!(cin >> opcja) || (opcja < 0 || opcja > 4))) {
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "Niepoprawny wybor. Sprobuj ponownie." << endl;
	}
	cin.ignore(numeric_limits<streamsize>::max(), '\n');

	string nowaWartosc;

	switch (opcja) {
	case 1:
		cout << "Podaj nowe imie: ";
		cin >> nowaWartosc;
		setImie(nowaWartosc);
		cout << "Imie zostalo zmienione." << endl;
		break;
	case 2:
		cout << "Podaj nowe nazwisko: ";
		cin >> nowaWartosc;
		setNazwisko(nowaWartosc);
		cout << "Nazwisko zostalo zmienione." << endl;
		break;
	case 3:
		cout << "Podaj nowy PESEL: ";
		cin >> nowaWartosc;
		setPesel(nowaWartosc);
		for (auto& konto : kontaUzytkownika)
		{
			konto->setWlascicielel(nowaWartosc); // Aktualizujemy PESEL we wszystkich kontach
		}
		cout << "PESEL zostal zmieniony." << endl;
		break;
	case 4:
		cout << "Podaj nowe haslo: ";
		cin >> nowaWartosc;
		setHaslo(nowaWartosc);
		cout << "Haslo zostalo zmienione." << endl;
		break;
	case 0:
		cout << "Powrot do menu." << endl;
		break;
	default:
		cout << "Niepoprawny wybor." << endl;
		break;

	}

	if (systemBankowy)
	{
		systemBankowy->zapiszDaneKlientow();
	}
}
int main(int argc, char** argv) {

	srand(static_cast<unsigned int>(time(nullptr)));

	SystemBankowy system;
	system.uruchom();
	return 0;
}
