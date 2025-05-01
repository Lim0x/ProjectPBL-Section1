#include <iostream>
#include <cstdlib>
#include <fstream>

using namespace std;
#include "json.hpp"

using namespace std;
using json = nlohmann::json;

// Forward declarations
class Klient;
/**
 * @class Konto
 * @brief Reprezentuje konto.
 * 
 * 
 *  Klasa przechowuje podtsawowe dane dane i funkcje zwiazane z kontami 
 */
class Konto{
	private:
		string numerKonta; ///< Numer konta
		string typKonta; ///< Typ konta (np. Osobiste, Oszczędnościowe)
		float saldoKonta; ///< Saldo konta
		Karta karta; ///< Karta przypisana do konta (jeśli dotyczy)
		Lokata lokata; ///< Lokata przypisana do konta (jeśli dotyczy)
	public:
		string getNumerKonta() const { return numerKonta; } ///< Zwraca numer konta
		string getTypKonta() const { return typKonta; } ///< Zwraca typ konta (np. Osobiste, Oszczędnościowe)	
		float getSaldoKonta() const { return saldoKonta; } ///< Zwraca saldo konta
		void setNumerKonta(string numer) { numerKonta = numer; } ///< Ustala numer konta
		void setTypKonta(string typ) { typKonta = typ; } ///< Ustala typ konta (np. Osobiste, Oszczędnościowe)
		void setSaldoKonta(float saldo) { saldoKonta = saldo; } ///< Ustala saldo konta
	
};
/**
 * @class Karta
 * @brief Reprezentuje karte.
 * 
 * 
 *  Klasa przechowuje podtsawowe dane dane i funkcje zwiazane z kartami 
 */
class Karta{
	private:
		string numerKarty; ///< Numer karty
		string dataWaznosci; ///< Data waznosci karty
		string kodCVC; ///< Kod CVC karty
		string typKarty; ///< Typ karty (np. Debitowa, Kredytowa)
		float saldoKarty; ///< Saldo karty (jezli dotyczy)
	public:
		///< getters i setters do atrybutow karty
		string getNumerKarty() const { return numerKarty; }	///< Zwraca numer karty
		string getDataWaznosci() const { return dataWaznosci; } ///< Zwraca date waznosci karty
		string getKodCVC() const { return kodCVC; } ///< Zwraca kod CVC karty
		string getTypKarty() const { return typKarty; } ///< Zwraca typ karty (np. Debitowa, Kredytowa)
		float getSaldoKarty() const { return saldoKarty; } ///< Zwraca saldo karty
		void setNumerKarty(string numer) { numerKarty = numer; } ///< Ustala numer karty
		void setDataWaznosci(string data) { dataWaznosci = data; } ///< Ustala date waznosci karty
		void setKodCVC(string kod) { kodCVC = kod; } ///< Ustala kod CVC karty		
		void setTypKarty(string typ) { typKarty = typ; } ///< Ustala typ karty (np. Debitowa, Kredytowa)
		void setSaldoKarty(float saldo) { saldoKarty = saldo; } ///< Ustala saldo karty (jezli dotyczy)
	};

/**
 * @class Lokata
 * @brief Reprezentuje lokate.
 *
 * Klasa przechowuje kwote, oprocentowanie, date oddania oraz odnosenie do wlasciciela.
 */
class Lokata {
	double kwota; ///< Kwota
	double oprocentowanie; ///< Oprocentowanie
	string dataOddania; ///< Data oddania
	Klient* wlasciciel; ///< Wlasciciel

public:
	Lokata(double kwota, double oprocentowanie, const string& dataOddania, Klient* wlasciciel) {
		this->kwota = kwota;
		this->oprocentowanie = oprocentowanie;
		this->dataOddania = dataOddania;
		this->wlasciciel = wlasciciel;
	}

	double getKwota() const { return kwota; }
	double getOprocentowanie() const { return oprocentowanie; }
	const string& getDataOddania() const { return dataOddania; }
	Klient* getWlasciciel() const { return wlasciciel; }
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

	vector<Konto> kontaUzytkownika; ///< Tablica przechowujaca konta uzytkownika
	vector<Karta> kartyUzytkownika; ///< Tablica przechowujaca karty uzytkownika
	vector<Lokata> lokatyUzytkownika; ///< Tablica przechowujaca lokaty uzytkownika

	/**
	 * @brief Rejestruje nowego klienta.
	 *
	 * Funckja rejestruje nowego klienta w systemie, zapisujac jego dane do pliku JSON (Imie, nazwisko, PESEL, login, haslo).
	 */
	void rejestracja();

	/**
	 * @brief Loguje klienta do systemu.
	 *
	 * Funkcja loguje klienta do systemu, sprawdzajac poprawnosc podanych danych (login, haslo).
	 */
	void logowanie();

    /**
     * @brief Edytuje dane klienta.
     *
     * Funkcja umozliwia edycje danych klienta, takich jak imie, nazwisko, czy PESEL,
     * zapisujac zmiany do pliku JSON.
     */
    void edytujDane();

	/**
	* @brief Dodaje nowe konto do listy kont uzytkownika.
	*
	* Funkcja umozliwia dodanie nowego konta do listy kont uzytkownika.
	* Nowe konto jest przechowywane w wektorze kontaUzytkownika.
	*/
	void dodajKonto();

	/**
	 * @brief Dodaje nowa karte do listy kart uzytkownika.
	 *
	 * Funkcja umozliwia dodanie nowej karty do listy kart uzytkownika.
	 * Nowa karta jest przechowywana w wektorze kartyUzytkownika.
	 */
	void dodajKarte();

	/**
	 * @brief Dodaje nowa lokate do listy lokat uzytkownika.
	 *
	 * Funkcja umozliwia dodanie nowej lokaty do listy lokat uzytkownika.
	 * Nowa lokata jest przechowywana w wektorze lokatyUzytkownika.
	 */
	void dodajLokate();

	/**
	 * @brief Wyswietla dane klienta.
	 *
	 * Funkcja wyswietla dane klienta, takie jak imie, nazwisko, PESEL, login i haslo.
	 */
	void wyswietlDane();
};








int main(int argc, char** argv) {
	return 0;
}
