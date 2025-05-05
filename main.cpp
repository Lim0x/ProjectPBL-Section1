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
 *  Klasa przechowuje podstawowe dane i funkcje zwiazane z kartami
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
	 * @brief Konstruktor klasy Karta.
	 *
	 * Inicjalizuje obiekt Karta z podanymi parametrami.
	 *
	 * @param numer Numer karty
	 * @param data Data waznosci karty
	 * @param kod Kod CVC karty
	 * @param typ Typ karty (np. Debitowa, Kredytowa)
	 * @param saldo Saldo karty (jesli dotyczy)
	 */
	Karta(string numer, string data, string kod, string typ, float saldo) {
		this->numerKarty = numer;
		this->dataWaznosci = data;
		this->kodCVC = kod;
		this->typKarty = typ;
		this->saldoKarty = saldo;
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
	 * @return Saldo karty (jesli dotyczy)
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
 * Klasa przechowuje kwote, oprocentowanie oraz date oddania.
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
	 * @brief Konstruktor klasy Konto.
	 *
	 * Inicjalizuje obiekt Konto z podanymi parametrami.
	 *
	 * @param numer Numer konta
	 * @param typ Typ konta (np. Osobiste, Oszczędnościowe)
	 * @param saldo Saldo konta
	 */
	Konto(string numer, string typ, float saldo) {
			this->numerKonta = numer;
			this->typKonta = typ;
			this->saldoKonta = saldo;
	}
	/**
	 * @brief Konstruktor domyslny klasy Konto.
	 *
	 * Inicjalizuje obiekt Konto z domyslnymi wartosciami.
	 */
	Konto() = default;
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
	* @brief Zwraca bieżącą datę w formacie "DD-MM-YYYY".
	*
	* @return Bieżąca data w formacie "DD-MM-YYYY"
	*/
	string getAktualnaData();
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
	void szczegolyTransakcji();
	/**
	 * @brief Wyświetla szczegóły transakcji.
	 *
	 * Funckja wyświetla szczegóły transakcji, takie jak kwota, typ transakcji, data transakcji oraz numery kont nadawcy i odbiorcy.
	 */
	void wyswietlSzczegolyTransakcji();
	
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
	 * @brief Rejestruje nowego klienta.
	 *
	 * Funkcja rejestruje nowego klienta w systemie, zapisujac jego dane do pliku JSON (Imie, nazwisko, PESEL, login, haslo).
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
	/**
	 * @brief Zwraca informacje o kliencie w formacie JSON.
	 *
	 * Ta funkcja konwertuje obiekt Klient na format JSON, aby ułatwić zapis do pliku.
	 *
	 * @param j Obiekt JSON, do którego zostaną zapisane informacje o kliencie
	 * @param klient Obiekt Klient, którego informacje mają zostać zapisane
	 */
	friend void to_json_Klient(json& j, const Klient& klient);
	/**
	 * @brief Wczytuje informacje o kliencie z formatu JSON.
	 *
	 * Ta funkcja konwertuje dane w formacie JSON na obiekt Klient.
	 *
	 * @param j Obiekt JSON, z którego zostaną odczytane informacje o kliencie
	 * @param klient Obiekt Klient, do którego zostaną zapisane informacje
	 */
	friend void from_json_Klient(const json& j, Klient& klient);
	/**
	 * @brief Zwraca informacje o transakcji w formacie JSON.
	 *
	 * Ta funkcja konwertuje obiekt Transakcja na format JSON, aby ułatwić zapis do pliku.
	 *
	 * @param j Obiekt JSON, do którego zostaną zapisane informacje o transakcji
	 * @param transakcja Obiekt Transakcja, którego informacje mają zostać zapisane
	 */
	friend void to_json_Transakcja(json& j, const Transakcja& transakcja);
	/**
	 * @brief Wczytuje informacje o transakcji z formatu JSON.
	 *
	 * Ta funkcja konwertuje dane w formacie JSON na obiekt Transakcja.
	 *
	 * @param j Obiekt JSON, z którego zostaną odczytane informacje o transakcji
	 * @param transakcja Obiekt Transakcja, do którego zostaną zapisane informacje
	 */
	friend void from_json_Transakcja(const json& j, Transakcja& transakcja);
	/**
	 * @brief Zwraca informacje o karcie w formacie JSON.
	 *
	 * Ta funkcja konwertuje obiekt Karta na format JSON, aby ułatwić zapis do pliku.
	 *
	 * @param j Obiekt JSON, do którego zostaną zapisane informacje o karcie
	 * @param karta Obiekt Karta, którego informacje mają zostać zapisane
	 */
	friend void to_json_Karta(json& j, const Karta& karta);
	/**
	 * @brief Wczytuje informacje o karcie z formatu JSON.
	 *
	 * Ta funkcja konwertuje dane w formacie JSON na obiekt Karta.
	 *
	 * @param j Obiekt JSON, z którego zostaną odczytane informacje o karcie
	 * @param karta Obiekt Karta, do którego zostaną zapisane informacje
	 */
	friend void from_json_Karta(const json& j, Karta& karta);
	/**
	 * @brief Zwraca informacje o lokacie w formacie JSON.
	 *
	 * Ta funkcja konwertuje obiekt Lokata na format JSON, aby ułatwić zapis do pliku.
	 *
	 * @param j Obiekt JSON, do którego zostaną zapisane informacje o lokacie
	 * @param lokata Obiekt Lokata, którego informacje mają zostać zapisane
	 */
	friend void to_json_Lokata(json& j, const Lokata& lokata);
	/**
	 * @brief Wczytuje informacje o lokacie z formatu JSON.
	 *
	 * Ta funkcja konwertuje dane w formacie JSON na obiekt Lokata.
	 *
	 * @param j Obiekt JSON, z którego zostaną odczytane informacje o lokacie
	 * @param lokata Obiekt Lokata, do którego zostaną zapisane informacje
	 */
	friend void from_json_Lokata(const json& j, Lokata& lokata);
	/**
	 * @brief Zwraca informacje o koncie w formacie JSON.
	 *
	 * Ta funkcja konwertuje obiekt Konto na format JSON, aby ułatwić zapis do pliku.
	 *
	 * @param j Obiekt JSON, do którego zostaną zapisane informacje o koncie
	 * @param konto Obiekt Konto, którego informacje mają zostać zapisane
	 */
	friend void to_json_Konto(json& j, const Konto& konto);
	/**
	 * @brief Wczytuje informacje o koncie z formatu JSON.
	 *
	 * Ta funkcja konwertuje dane w formacie JSON na obiekt Konto.
	 *
	 * @param j Obiekt JSON, z którego zostaną odczytane informacje o koncie
	 * @param konto Obiekt Konto, do którego zostaną zapisane informacje
	 */
	friend void from_json_Konto(const json& j, Konto& konto);

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
	void zapiszKonta(const vector<Konto*>& konta);
	/**
	 * @brief Wczytuje dane kont z pliku JSON.
	 * @return Wektor kont odczytanych z pliku
	 */
	vector<Konto*> wczytajKonta();

};







int main(int argc, char** argv) {
	return 0;
}
