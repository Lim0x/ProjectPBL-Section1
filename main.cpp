#include <iostream>
#include <cstdlib>
#include <fstream>

using namespace std;
#include "json.hpp"

using namespace std;
using json = nlohmann::json;



class Konto{};
class Karta{
	private:
		string numerKarty; ///< Numer karty
		string dataWaznosci; ///< Data wa�no�ci karty
		string kodCVC; ///< Kod CVC karty
		string typKarty; ///< Typ karty (np. Debitowa, Kredytowa)
		float saldoKarty; ///< Saldo karty (je�li dotyczy)
	public:
		///< getters i setters do atrybutow karty
		string getNumerKarty() const { return numerKarty; }	///< Zwraca numer karty
		string getDataWaznosci() const { return dataWaznosci; } ///< Zwraca dat� wa�no�ci karty
		string getKodCVC() const { return kodCVC; } ///< Zwraca kod CVC karty
		string getTypKarty() const { return typKarty; } ///< Zwraca typ karty (np. Debitowa, Kredytowa)
		float getSaldoKarty() const { return saldoKarty; } ///< Zwraca saldo karty
		void setNumerKarty(string numer) { numerKarty = numer; } ///< Ustala numer karty
		void setDataWaznosci(string data) { dataWaznosci = data; } ///< Ustala dat� wa�no�ci karty
		void setKodCVC(string kod) { kodCVC = kod; } ///< Ustala kod CVC karty		
		void setTypKarty(string typ) { typKarty = typ; } ///< Ustala typ karty (np. Debitowa, Kredytowa)
		void setSaldoKarty(float saldo) { saldoKarty = saldo; } ///< Ustala saldo karty (je�li dotyczy)
	};
class Lokata{}; //tymczasowo tutaj zeby kompilator dzialal


/**
 * @class Klient
 * @brief Reprezentuje klienta banku.
 * 
 * 
 *  Klasa przechowuje dane osobowe oraz udost�pnia podstawowe funkcje zwi�zane z u�ytkownikiem 
 */
class Klient
{
private:
	string imie; ///< Imi� klienta
	string nazwisko; ///< Nazwisko klienta
	string pesel; ///< Numer PESEL klienta
	string login; ///< Login klienta
	string haslo; ///< Has�o klienta

	vector<Konto> kontaUzytkownika; ///< Tablica przechowuj�ca konta u�ytkownika
	vector<Karta> kartyUzytkownika; ///< Tablica przechopwuj�ca karty u�ytkownika
	vector<Lokata> lokatyUzytkownika; ///< Tablica przechowuj�ca lokaty u�ytkownika

	/**
	 * @brief Rejestruje nowego klienta.
	 *
	 * Funckja rejestruje nowego klienta w systemie, zapisuj�c jego dane do pliku JSON (Imi�, nazwisko, PESEL, login, has�o).
	 */
	void rejestracja();

	/**
	 * @brief Loguje klienta do systemu.
	 *
	 * Funkcja loguje klienta do systemu, sprawdzaj�c poprawno�� podanych danych (login, has�o).
	 */
	void logowanie();

    /**
     * @brief Edytuje dane klienta.
     *
     * Funkcja umo�liwia edycj� danych klienta, takich jak imi�, nazwisko, czy PESEL,
     * zapisuj�c zmiany do pliku JSON.
     */
    void edytujDane();

	/**
	* @brief Dodaje nowe konto do listy kont u�ytkownika.
	*
	* Funkcja umo�liwia dodanie nowego konta do listy kont u�ytkownika.
	* Nowe konto jest przechowywane w wektorze kontaUzytkownika.
	*/
	void dodajKonto();

	/**
	 * @brief Dodaje now� kart� do listy kart u�ytkownika.
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