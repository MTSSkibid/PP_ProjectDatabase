#pragma once
#include "Tabela.h"
#include <vector>
#include <filesystem>
#include <fstream>
#include <cstdlib>
#include <chrono>
#include <thread>
#include <limits>
#include <regex>
class BazaDanych {
private:
	std::vector<Tabela> tabele;

public:
	void WyswietlTabele();
	void DodajTabele();
	void UsunTabele();
	void ZaladujDaneTabel();
	void ModyfikujTabele();
	void Wyszukaj();
	unsigned int WczytajLiczbeZakres(unsigned int min, unsigned int max);
	BazaDanych();
};