#include <iostream>
#include <cstdlib>
#include <fstream>

using namespace std;
#include "json.hpp"

using namespace std;
using json = nlohmann::json;



class Konto{};
class Karta{};
class Lokata{}; //tymczasowo tutaj zeby kompilator dzialal


/**
 * @class Klient
 * @brief Reprezentuje klienta banku.
 * 
 * 
 *  Klasa przechowuje dane osobowe oraz udostêpnia podstawowe funkcje zwi¹zane z u¿ytkownikiem 
 */
class Klient
{
private:
	string imie; ///< Imiê klienta
	string nazwisko; ///< Nazwisko klienta
	string pesel; ///< Numer PESEL klienta
	string login; ///< Login klienta
	string haslo; ///< Has³o klienta

	vector<Konto> kontaUzytkownika; ///< Tablica przechowuj¹ca konta u¿ytkownika
	vector<Karta> kartyUzytkownika; ///< Tablica przechopwuj¹ca karty u¿ytkownika
	vector<Lokata> lokatyUzytkownika; ///< Tablica przechowuj¹ca lokaty u¿ytkownika

	/**
	 * @brief Rejestruje nowego klienta.
	 *
	 * Funckja rejestruje nowego klienta w systemie, zapisuj¹c jego dane do pliku JSON (Imiê, nazwisko, PESEL, login, has³o).
	 */
	void rejestracja();

	/**
	 * @brief Loguje klienta do systemu.
	 *
	 * Funkcja loguje klienta do systemu, sprawdzaj¹c poprawnoœæ podanych danych (login, has³o).
	 */
	void logowanie();

    /**
     * @brief Edytuje dane klienta.
     *
     * Funkcja umo¿liwia edycjê danych klienta, takich jak imiê, nazwisko, czy PESEL,
     * zapisuj¹c zmiany do pliku JSON.
     */
    void edytujDane();

	/**
	* @brief Dodaje nowe konto do listy kont u¿ytkownika.
	*
	* Funkcja umo¿liwia dodanie nowego konta do listy kont u¿ytkownika.
	* Nowe konto jest przechowywane w wektorze kontaUzytkownika.
	*/
	void dodajKonto();

	/**
	 * @brief Dodaje now¹ kartê do listy kart u¿ytkownika.
	 *
	 * Funkcja umo¿liwia dodanie nowej karty do listy kart u¿ytkownika.
	 * Nowa karta jest przechowywana w wektorze kartyUzytkownika.
	 */
	void dodajKarte();

	/**
	 * @brief Dodaje now¹ lokatê do listy lokat u¿ytkownika.
	 *
	 * Funkcja umo¿liwia dodanie nowej lokaty do listy lokat u¿ytkownika.
	 * Nowa lokata jest przechowywana w wektorze lokatyUzytkownika.
	 */
	void dodajLokate();

	/**
	 * @brief Wyœwietla dane klienta.
	 *
	 * Funkcja wyœwietla dane klienta, takie jak imiê, nazwisko, PESEL, login i has³o.
	 */
	void wyswietlDane();
};








int main(int argc, char** argv) {
	return 0;
}