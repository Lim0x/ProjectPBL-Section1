#include <iostream>
#include <cstdlib>
#include <fstream>
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
};







int main(int argc, char** argv) {
	return 0;
}