#include "Kolumna.h"

Kolumna::Kolumna(std::string Nazwa, TypDanych Typ, std::set<std::string> Ograniczenia) {
	nazwa = Nazwa;
	typ = Typ;
	ograniczenia = Ograniczenia;
}