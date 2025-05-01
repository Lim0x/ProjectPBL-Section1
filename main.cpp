#include <iostream>
#include <cstdlib>
#include <fstream>

using namespace std;
#include "json.hpp"

using namespace std;
using json = nlohmann::json;


/**
 * @class Konto
 * @brief Reprezentuje konto.
 * 
 * 
 *  Klasa przechowuje podstawowe dane dane i funkcje związane z kontami 
 */
class Konto{
	private:
		string numerKonta; ///< Numer konta
		string typKonta; ///< Typ konta (np. Osobiste, Oszczędnościowe)
		float saldoKonta; ///< Saldo konta
		Karta karta; ///< Karta przypisana do konta (jeśli dotyczy)

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
 *  Klasa przechowuje podstawowe dane dane i funkcje związane z kartami 
 */
class Karta{
	private:
		string numerKarty; ///< Numer karty
		string dataWaznosci; ///< Data ważności karty
		string kodCVC; ///< Kod CVC karty
		string typKarty; ///< Typ karty (np. Debitowa, Kredytowa)
	public:
		///< getters i setters do atrybutow karty
		string getNumerKarty() const { return numerKarty; }	///< Zwraca numer karty
		string getDataWaznosci() const { return dataWaznosci; } ///< Zwraca dat� wa�no�ci karty
		string getKodCVC() const { return kodCVC; } ///< Zwraca kod CVC karty
		string getTypKarty() const { return typKarty; } ///< Zwraca typ karty (np. Debitowa, Kredytowa)
		void setNumerKarty(string numer) { numerKarty = numer; } ///< Ustala numer karty
		void setDataWaznosci(string data) { dataWaznosci = data; } ///< Ustala dat� wa�no�ci karty
		void setKodCVC(string kod) { kodCVC = kod; } ///< Ustala kod CVC karty		
		void setTypKarty(string typ) { typKarty = typ; } ///< Ustala typ karty (np. Debitowa, Kredytowa)
	};
/**
 * @class Lokata
 * @brief Reprezentuje Lokate.
 * 
 * 
 *  Klasa przechowuje podstawowe dane dane i funkcje związane z lokatami 
 */
class Lokata{}; //tymczasowo tutaj zeby kompilator dzialal


/**
 * @class Klient
 * @brief Reprezentuje klienta banku.
 * 
 * 
 *  Klasa przechowuje dane osobowe oraz udostępnia podstawowe funkcje związane z użytkownikiem 
 */
class Klient
{
private:
	string imie; ///< Imi� klienta
	string nazwisko; ///< Nazwisko klienta
	string pesel; ///< Numer PESEL klienta
	string login; ///< Login klienta
	string haslo; ///< Has�o klienta

	vector<Konto> kontaUzytkownika; ///< Tablica przechowująca konta użytkownika
	vector<Karta> kartyUzytkownika; ///< Tablica przechopwujaca karty użytkownika
	vector<Lokata> lokatyUzytkownika; ///< Tablica przechowująca lokaty użytkownika

	/**
	 * @brief Rejestruje nowego klienta.
	 *
	 * Funckja rejestruje nowego klienta w systemie, zapisując jego dane do pliku JSON (Imię, nazwisko, PESEL, login, hasło).
	 */
	void rejestracja();

	/**
	 * @brief Loguje klienta do systemu.
	 *
	 * Funkcja loguje klienta do systemu, sprawdzając poprawność podanych danych (login, hasło).
	 */
	void logowanie();

    /**
     * @brief Edytuje dane klienta.
     *
     * Funkcja umożliwia edycji danych klienta, takich jak imie, nazwisko, czy PESEL,
     * zapisując zmiany do pliku JSON.
     */
    void edytujDane();

	/**
	* @brief Dodaje nowe konto do listy kont użytkownika.
	*
	* Funkcja umożliwia dodanie nowego konta do listy kont uzytkownika.
	* Nowe konto jest przechowywane w wektorze kontaUzytkownika.
	*/
	void dodajKonto();

	/**
	 * @brief Dodaje nową kartę do listy kart użytkownika.
	 *
	 * Funkcja umo�liwia dodanie nowej karty do listy kart u�ytkownika.
	 * Nowa karta jest przechowywana w wektorze kartyUzytkownika.
	 */
	void dodajKarte();

	/**
	 * @brief Dodaje now� lokat� do listy lokat u�ytkownika.
	 *
	 * Funkcja umo�liwia dodanie nowej lokaty do listy lokat u�ytkownika.
	 * Nowa lokata jest przechowywana w wektorze lokatyUzytkownika.
	 */
	void dodajLokate();

	/**
	 * @brief Wy�wietla dane klienta.
	 *
	 * Funkcja wy�wietla dane klienta, takie jak imi�, nazwisko, PESEL, login i has�o.
	 */
	void wyswietlDane();
};




int main(int argc, char** argv) {
	return 0;
}