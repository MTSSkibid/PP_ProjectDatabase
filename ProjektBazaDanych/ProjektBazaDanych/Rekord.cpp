#include "Rekord.h"
#include <fstream>
#include <sstream>
#include <stdexcept>

void Rekord::WczytajKolumnyZPliku(std::string linia) {
    linia = linia.substr(1);
    if (!linia.empty() && linia[0] == ' ') linia = linia.substr(1);

    std::vector<std::string> elementy;
    std::string segment;
    std::istringstream iss(linia);
    while (std::getline(iss, segment, ';')) {
        segment.erase(0, segment.find_first_not_of(" \t"));
        segment.erase(segment.find_last_not_of(" \t") + 1);
        if (!segment.empty()) elementy.push_back(segment);
    }

    static const std::set<std::string> dozwoloneTypy = { "Int", "String", "Float" };
    static const std::set<std::string> dozwoloneOgraniczenia = {
        "PRIMARY KEY", "UNIQUE", "NOT NULL", "SECONDARY KEY"
    };

    size_t i = 0;
    while (i + 1 < elementy.size()) {
        std::string typStr = elementy[i++];
        std::string nazwa = elementy[i++];

        if (dozwoloneTypy.find(typStr) == dozwoloneTypy.end()) {
            throw std::runtime_error("Nieznany typ danych: " + typStr);
        }

        TypDanych typ;
        if (typStr == "Int") typ = TypDanych::Int;
        else if (typStr == "String") typ = TypDanych::String;
        else if (typStr == "Float") typ = TypDanych::Float;

        std::set<std::string> ograniczenia;
        while (i < elementy.size() && dozwoloneTypy.find(elementy[i]) == dozwoloneTypy.end()) {
            if (dozwoloneOgraniczenia.find(elementy[i]) != dozwoloneOgraniczenia.end()) {
                ograniczenia.insert(elementy[i]);
            }
            else {
                throw std::runtime_error("Nieznane ograniczenie: " + elementy[i]);
            }
            i++;
        }

        kolumny.emplace_back(nazwa, typ, ograniczenia);
    }
}



void Rekord::WczytajDaneZPliku(std::string linia) {
			std::istringstream iss(linia.substr(1));
			std::string wartosc;
			while (iss >> wartosc) {
				dane.push_back(wartosc);
			}
}


void Rekord::SetDane(std::vector<std::string> Dane) {
    dane = Dane;
}
void Rekord::SetKolumny(std::vector<Kolumna> Kolumny)
{
    kolumny = Kolumny;
}
std::vector<std::string> Rekord::GetDane()
{
    return dane;
}
std::vector<Kolumna> Rekord::GetKolumny()
{
    return kolumny;
}