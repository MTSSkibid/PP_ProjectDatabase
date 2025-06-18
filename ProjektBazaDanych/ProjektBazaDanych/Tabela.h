#pragma once

#include "Rekord.h"
#include <vector>
#include <filesystem>
#include <fstream>
#include <cstdlib>
#include <chrono>
#include <thread>
#include <limits>
#include <regex>
class Tabela {
private:
	std::string Nazwa;
	std::vector<Rekord> Rekordy;

public:
	void DodajRekord();
	void DodajKolumne();
	void UsunKolumne();
	void UsunRekord();
	void ModyfikujRekord();
	void Filtruj();
	void WczytajTabeleZPliku(std::string NazwaPliku);
	void WypiszDaneTabeli();
	void ZmienNazweTabeli();
	unsigned int WczytajLiczbeZakres(unsigned int min, unsigned int max);
	std::string GetNazwa();
	void SetNazwa(std::string nazwa);
	std::vector<Rekord> GetRekordy();
};