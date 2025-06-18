#pragma once
#include "Kolumna.h"
#include <vector>


class Rekord {
private:
	std::vector<Kolumna> kolumny;
	std::vector<std::string> dane;

public:
	void WczytajKolumnyZPliku(std::string NazwaPliku);
	void WczytajDaneZPliku(std::string linia);
	std::vector<Kolumna> GetKolumny();
	void SetKolumny(std::vector<Kolumna> Kolumny);
	std::vector<std::string> GetDane();
	void SetDane(std::vector<std::string> Dane);
};
