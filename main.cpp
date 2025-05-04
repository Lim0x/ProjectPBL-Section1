#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <vector>
#include "json.hpp"

using namespace std;
using json = nlohmann::json;


/**
 * @class Karta
 * @brief Reprezentuje karte.
 *
 *
 *  Klasa przechowuje podtsawowe dane dane i funkcje zwiazane z kartami
 */
class Karta {
private:
	string numerKarty; ///< Numer karty
	string dataWaznosci; ///< Data waznosci karty
	string kodCVC; ///< Kod CVC karty
	string typKarty; ///< Typ karty (np. Debitowa, Kredytowa)
	float saldoKarty; ///< Saldo karty (jezli dotyczy)
public:
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
	string getDataWaznosci() const { return dataWaznosci; }
	/**
	 * @brief Zwraca kod CVC karty.
	 *
	 * @return Kod CVC karty
	 */
	string getKodCVC() const { return kodCVC; }
	/**
	 * @brief Zwraca typ karty.
	 *
	 * @return Typ karty (np. Debitowa, Kredytowa)
	 */
	string getTypKarty() const { return typKarty; }
	/**
	 * @brief Zwraca saldo karty.
	 *
	 * @return Saldo karty (jezli dotyczy)
	 */
	float getSaldoKarty() const { return saldoKarty; }
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
	void setDataWaznosci(string data) { dataWaznosci = data; }
	/**
	 * @brief Ustala kod CVC karty.
	 *
	 * @param kod Kod CVC karty
	 */
	void setKodCVC(string kod) { kodCVC = kod; }
	/**
	 * @brief Ustala typ karty.
	 *
	 * @param typ Typ karty (np. Debitowa, Kredytowa)
	 */
	void setTypKarty(string typ) { typKarty = typ; }
	/**
	 * @brief Ustala saldo karty.
	 *
	 * @param saldo Saldo karty
	 */
	void setSaldoKarty(float saldo) { saldoKarty = saldo; }
};


/**
 * @class Lokata
 * @brief Reprezentuje lokate.
 *
 * Klasa przechowuje kwote, oprocentowanie, date oddania oraz odnosenie do wlasciciela.
 */
class Lokata {
	float kwota; ///< Kwota lokaty
	float oprocentowanie; ///< Oprocentowanie lokaty
	string dataOddania; ///< Data oddania lokaty

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
	Lokata(float kwota, float oprocentowanie, const string& dataOddania) {
		this->kwota = kwota;
		this->oprocentowanie = oprocentowanie;
		this->dataOddania = dataOddania;
	}

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
	const string& getDataOddania() const { return dataOddania; }
};

/**
 * @class Konto
 * @brief Reprezentuje konto.
 * 
 * 
 *  Klasa przechowuje podtsawowe dane dane i funkcje zwiazane z kontami 
 */
class Konto {
	private:
		string numerKonta; ///< Numer konta
		string typKonta; ///< Typ konta (np. Osobiste, Oszczędnościowe)
		float saldoKonta; ///< Saldo konta

	public:
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
		void setSaldoKonta(float saldo) { saldoKonta = saldo; }
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

public:

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
