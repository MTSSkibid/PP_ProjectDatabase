#include "Tabela.h"

namespace fs = std::filesystem;

unsigned int Tabela::WczytajLiczbeZakres(unsigned int min, unsigned int max) {
	unsigned int liczba;
	while (true) {
		std::cin >> liczba;

		if (std::cin.fail()) {
			std::cin.clear(); 
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cout << "Niepoprawne dane. Sprobuj ponownie.\n";
			continue;
		}
		char c;
		if (std::cin.get(c) && !std::isspace(c)) {
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cout << "Niepoprawne dane. Sprobuj ponownie.\n";
			continue;
		}

		if (liczba < min || liczba > max) {
			std::cout << "Niepoprawne dane. Sprobuj ponownie.\n";
			continue;
		}

		return liczba;
	}
}

std::string Tabela::GetNazwa()
{
	return Nazwa;
}
void Tabela::SetNazwa(std::string nazwa)
{
	Nazwa = nazwa;
}

void Tabela::WczytajTabeleZPliku(std::string NazwaPliku) {
	std::ifstream plik("Dane/"+NazwaPliku);
	for (int i = 0; i < NazwaPliku.size()-4; i++)
	{
		Nazwa += NazwaPliku[i];
	}
	if (!plik.is_open()) {
		std::cerr << "Nie mozna otworzyc pliku!" << std::endl;
	}
	std::string linia;
	
	while (std::getline(plik, linia)) {
		Rekord rekord;
		if (linia[0] == 'N') {
			rekord.WczytajKolumnyZPliku(linia);
		}
		if (linia[0] == 'R') rekord.WczytajDaneZPliku(linia);
			
		
		Rekordy.push_back(rekord);
	}
	
}

std::vector<Rekord> Tabela::GetRekordy() {
	return Rekordy;
}

void Tabela::WypiszDaneTabeli()
{
	if (Rekordy.empty() || Rekordy[0].GetKolumny().empty()) {
		std::cout << "Brak rekordow do wyswietlenia \n";
		return;
	}
	for (const auto& kolumna : Rekordy[0].GetKolumny())
	{
		std::cout << kolumna.nazwa << "   ";
	}
	
	for (auto& rekord : Rekordy)
	{
		for (auto& dana : rekord.GetDane())
		{
			std::cout << dana << "   ";
		}
		std::cout << "\n";
	}
	if (Rekordy.size() < 2) {
		std::cout << "\nZostaly wyswietlone kolumny tabeli. Nie ma podstaw do filtrowania.\n";
		return;
	}
	std::cout << "Czy chcesz filtrowac dane? \n";
	std::cout << "1.Tak \n";
	std::cout << "2.Nie \n";
	unsigned int wybor2 = WczytajLiczbeZakres(1, 2);
	if (wybor2 == 1) Filtruj();
}

void Tabela::ZmienNazweTabeli()
{
	std::string NowaNazwa{ "" };
	std::cout << "Wprowadz nowa nazwe (Jezeli nie chcesz zmieniac nazwy wprowadz znak - ): \n";
	std::cin >> NowaNazwa;

	if (NowaNazwa == "-") {
		std::cout << "Nazwa nie zostala zmieniona \n";
		return;
	}

	try {
		std::string staraSciezka = "Dane/" + Nazwa + ".txt";
		std::string nowaSciezka = "Dane/" + NowaNazwa + ".txt";
		std::filesystem::rename(staraSciezka, nowaSciezka);
		std::cout << "Plik zostal pomyslnie przemianowany.\n";
	}
	catch (const std::filesystem::filesystem_error& e) {
		std::cerr << "Blad: " << e.what() << '\n';
	}
}

void Tabela::DodajKolumne()
{
	if (Rekordy.empty())
	{
		std::ofstream zapis("Dane/" + Nazwa + ".txt", std::ios::trunc);
		zapis << "N";
		zapis.close();
	}
	std::string nazwa;
	std::cout << "Wprowadz nazwe kolumny: \n";
	std::cin >> nazwa;
	
	std::cout << "Podaj typ kolumny: \n1.Int\n2.String\n3.Float\n";
	unsigned int wybor = WczytajLiczbeZakres(1,3);
	std::set<std::string> ograniczenia;
	std::string ogr;
	std::cout << "Podaj ograniczenie (Jezeli nie chcesz zadnych ograniczen zostaw puste) lub kilka np. 13: \n";
	std::cout << "1.PRIMARY KEY, 2.SECONDARY KEY, 3.UNIQUE, 4.NOT NULL\n";
	std::getline(std::cin >> std::ws, ogr);
	for (auto& litera : ogr)
	{
		if (litera == '1') ograniczenia.insert("PRIMARY KEY");
		if (litera == '2') ograniczenia.insert("SECONDARY KEY");
		if (litera == '3') ograniczenia.insert("UNIQUE");
		if (litera == '4') ograniczenia.insert("NOT NULL");
	}
	std::string typ;
	std::string domyslnaWartosc;

	switch (wybor)
	{
	case 1: typ = "Int"; domyslnaWartosc = "0"; break;
	case 2: typ = "String"; domyslnaWartosc = ""; break;
	case 3: typ = "Float"; domyslnaWartosc = "0"; break;
	default: std::cout << "Nieprawidlowy typ\n"; return;
	}

	std::string nowaKolumna = typ + ";" + nazwa;
	for (const auto& o : ograniczenia)
	{
		nowaKolumna += ";" + o;
	}
	nowaKolumna += ";"; 

	std::ifstream plik("Dane/" + Nazwa + ".txt");
	if (!plik)
	{
		std::cerr << "Nie mozna otworzyc pliku.\n";
		return;
	}

	std::vector<std::string> linie;
	std::string linia;
	while (std::getline(plik, linia))
	{
		if (!linia.empty())
		{
			if (linia[0] == 'N')
			{
				linia += " " + nowaKolumna; 
			}
			else if (linia[0] == 'R')
			{
				linia += " " + domyslnaWartosc; 
			}
		}
		linie.push_back(linia);
	}
	plik.close();

	
	std::ofstream zapis("Dane/" + Nazwa + ".txt", std::ios::trunc);
	if (!zapis)
	{
		std::cerr << "Nie mozna zapisac pliku.\n";
		return;
	}

	for (const auto& l : linie)
	{
		zapis << l << "\n";
	}
	zapis.close();

	std::cout << "Kolumna '" << nazwa << "' dodana pomyslnie.\n";
}

void Tabela::UsunKolumne()
{
	if (Rekordy.empty()) {
		std::cout << "Brak kolumn do usuniecia!\n";
		return;
	}
	std::string nazwaKolumny;
	std::cout << "Podaj nazwe kolumny do usuniecia: ";
	std::cin >> nazwaKolumny;

	int index = -1;
	std::vector<Kolumna> kolumny = Rekordy[0].GetKolumny();
	for (size_t i = 0; i < kolumny.size(); ++i)
	{
		if (kolumny[i].nazwa == nazwaKolumny)
		{
			index = static_cast<int>(i);
			break;
		}
	}

	if (index == -1)
	{
		std::cout << "Nie znaleziono kolumny o nazwie " << nazwaKolumny << "\n";
		return;
	}

	kolumny.erase(kolumny.begin() + index);
	Rekordy[0].SetKolumny(kolumny);

	for (auto& rekord : Rekordy)
	{
		std::vector<std::string> dane = rekord.GetDane();
		if (index < static_cast<int>(dane.size()))
		{
			dane.erase(dane.begin() + index);
		}
		rekord.SetDane(dane);
	}

	std::ofstream plik("Dane/" + Nazwa + ".txt", std::ios::trunc);
	if (!plik)
	{
		std::cerr << "Nie mozna otworzyc pliku do zapisu!\n";
		return;
	}

	plik << "N ";
	for (size_t i = 0; i < kolumny.size(); ++i)
	{
		const Kolumna& kol = kolumny[i];
		std::string typStr;
		switch (kol.typ)
		{
		case TypDanych::Int: typStr = "Int"; break;
		case TypDanych::String: typStr = "String"; break;
		case TypDanych::Float: typStr = "Float"; break;
		}
		plik << typStr << ";" << kol.nazwa << ";";
		for (const auto& ogr : kol.ograniczenia)
		{
			plik << ogr << ";";
		}
		if (i != kolumny.size() - 1)
			plik << " ";
	}
	plik << "\n";

	for (auto& rekord : Rekordy)
	{
		std::vector<std::string> dane = rekord.GetDane();
		if (dane.empty())
			continue;

		plik << "R";
		for (const auto& wartosc : dane)
		{
			plik << " " << wartosc;
		}
		plik << "\n";
	}

	plik.close();

	std::cout << "Kolumna '" << nazwaKolumny << "' zostala usunieta i zapisano zmiany do pliku.\n";
}

void Tabela::DodajRekord()
{
	Rekord nowyRekord;
	std::vector<std::string> dane;
	if (Rekordy.empty()) {
		std::cout << "Nie mozna dodac rekordu, brak kolumn \n";
		return;
	}
	const auto& kolumny = Rekordy[0].GetKolumny();

	for (size_t i = 0; i < kolumny.size(); ++i)
	{
		const auto& kol = kolumny[i];
		std::string wartosc;
		bool poprawna = false;

		while (!poprawna)
		{
			std::cout << "Podaj wartosc dla kolumny \"" << kol.nazwa << "\" (typ: ";
			switch (kol.typ)
			{
			case TypDanych::Int: std::cout << "Int"; break;
			case TypDanych::Float: std::cout << "Float"; break;
			case TypDanych::String: std::cout << "String"; break;
			}
			std::cout << "): ";
			std::cin >> wartosc;

			if (kol.ograniczenia.count("NOT NULL") && wartosc.empty())
			{
				std::cout << "Ta kolumna nie moze byc pusta!\n";
				continue;
			}
			bool typPoprawny = false;

			switch (kol.typ)
			{
			case TypDanych::Int:
				typPoprawny = std::regex_match(wartosc, std::regex("^-?\\d+$"));
				break;
			case TypDanych::Float:
				typPoprawny = std::regex_match(wartosc, std::regex("^-?\\d*\\.\\d+$|^-?\\d+\\.\\d*$|^-?\\d+$"));
				break;
			case TypDanych::String:
				typPoprawny = true;
				break;
			}

			if (!typPoprawny)
			{
				std::cout << "Wartosc niezgodna z typem danych!\n";
				continue;
			}

			if (kol.ograniczenia.count("UNIQUE") || kol.ograniczenia.count("PRIMARY KEY"))
			{
				bool duplikat = false;
				for (auto& rek : Rekordy)
				{
					const auto& wartosci = rek.GetDane();
					if (wartosci.size() > i && wartosci[i] == wartosc)
					{
						duplikat = true;
						break;
					}
				}
				if (duplikat)
				{
					std::cout << "Wartosc dla \"" << kol.nazwa << "\" musi byc unikalna!\n";
					continue;
				}
			}

			poprawna = true;
		}

		dane.push_back(wartosc);
	}

	nowyRekord.SetDane(dane);

	Rekordy.push_back(nowyRekord);

	std::ofstream plik("Dane/" + Nazwa + ".txt", std::ios::app);
	if (!plik)
	{
		std::cerr << "Nie mozna otworzyc pliku do dopisania!\n";
		return;
	}

	plik << "R";
	for (const auto& wartosc : dane)
	{
		plik << " " << wartosc;
	}
	plik << "\n";
	plik.close();

	std::cout << "Rekord zostal dodany.\n";
}

void Tabela::UsunRekord()
{
	if (Rekordy.size()<2)
	{
		std::cout << "Brak rekordow do usuniecia.\n";
		return;
	}

	std::cout << "Lista rekordow:\n";
	for (size_t i = 0; i < Rekordy.size(); ++i)
	{
		if (Rekordy[i].GetDane().empty()) {
			continue; 
		}
		std::cout << i << ": ";
		for (const auto& wartosc : Rekordy[i].GetDane())
		{
			std::cout << wartosc << " ";
		}
		std::cout << "\n";
	}

	std::cout << "Podaj numer rekordu do usuniecia: ";
	unsigned int indeks = WczytajLiczbeZakres(1, Rekordy.size()-1);

	Rekordy.erase(Rekordy.begin() + indeks);
	std::cout << "Rekord zostal usuniety.\n";

	std::ofstream plik("Dane/" + Nazwa + ".txt", std::ios::trunc);
	if (!plik)
	{
		std::cerr << "Nie mozna otworzyc pliku do zapisu!\n";
		return;
	}

	plik << "N ";
	std::vector<Kolumna> kolumny = Rekordy[0].GetKolumny();
	for (size_t i = 0; i < Rekordy[0].GetKolumny().size(); ++i)
	{
		const Kolumna& kol = kolumny[i];
		std::string typStr;
		switch (kol.typ)
		{
		case TypDanych::Int: typStr = "Int"; break;
		case TypDanych::String: typStr = "String"; break;
		case TypDanych::Float: typStr = "Float"; break;
		}
		plik << typStr << ";" << kol.nazwa << ";";
		for (const auto& ogr : kol.ograniczenia)
		{
			plik << ogr << ";";
		}
		if (i != Rekordy[0].GetKolumny().size() - 1)
			plik << " ";
	}
	plik << "\n";

	for (auto& rekord : Rekordy)
	{
		if (rekord.GetDane().empty()) continue;
		plik << "R";
		for (auto& wartosc : rekord.GetDane())
		{
			plik << " " << wartosc;
		}
		plik << "\n";
	}

	plik.close();
}

void Tabela::ModyfikujRekord()
{
	if (Rekordy.size() < 2)
	{
		std::cout << "Brak rekordow do modyfikacji.\n";
		return;
	}

	std::cout << "Lista rekordow:\n";
	for (size_t i = 1; i < Rekordy.size(); ++i)
	{
		const auto& dane = Rekordy[i].GetDane();
		if (dane.empty()) continue;
		std::cout << i << ": ";
		for (const auto& wartosc : dane)
		{
			std::cout << wartosc << " ";
		}
		std::cout << "\n";
	}

	std::cout << "Podaj numer rekordu do modyfikacji: ";
	unsigned int indeks = WczytajLiczbeZakres(1, Rekordy.size() - 1);

	Rekord& rekord = Rekordy[indeks];
	std::vector<std::string> dane = rekord.GetDane();
	const auto& kolumny = Rekordy[0].GetKolumny();
	if (dane.size() < kolumny.size()) {
		dane.resize(kolumny.size());
	}

	for (size_t i = 0; i < kolumny.size(); ++i)
	{
		const Kolumna& kol = kolumny[i];

		std::cout << "Aktualna wartosc kolumny \"" << kol.nazwa << "\": ";

		if (i < dane.size()) {
			std::cout << dane[i] << "\n";
		}
		else {
			std::cout << "(brak danych)\n";
		}

		std::cout << "Nowa wartosc (wpisz `-` zeby zostawic bez zmian): ";

		std::string nowa;
		std::cin >> nowa;

		if (nowa == "-") continue;

		if (kol.ograniczenia.count("NOT NULL") && nowa.empty())
		{
			std::cout << "Ta kolumna nie moze byc pusta. Zostaje stara wartosc.\n";
			continue;
		}

		bool poprawna = false;
		switch (kol.typ)
		{
		case TypDanych::Int:
			poprawna = std::regex_match(nowa, std::regex("^-?\\d+$"));
			break;
		case TypDanych::Float:
			poprawna = std::regex_match(nowa, std::regex("^-?\\d*(\\.\\d+)?$"));
			break;
		case TypDanych::String:
			poprawna = true;
			break;
		}

		if (!poprawna)
		{
			std::cout << "Niepoprawny format dla typu " <<
				(kol.typ == TypDanych::Int ? "Int" :
					kol.typ == TypDanych::Float ? "Float" : "String") << ". Zostaje stara wartosc.\n";
			continue;
		}

		if (kol.ograniczenia.count("UNIQUE") || kol.ograniczenia.count("PRIMARY KEY"))
		{
			bool duplikat = false;
			for (size_t j = 1; j < Rekordy.size(); ++j)
			{
				if (j == indeks) continue;
				const auto& daneInne = Rekordy[j].GetDane();
				if (!daneInne.empty() && daneInne[i] == nowa)
				{
					duplikat = true;
					break;
				}
			}
			if (duplikat)
			{
				std::cout << "Wartosc musi byc unikalna (ograniczenie UNIQUE lub PRIMARY KEY). Zostaje stara wartosc.\n";
				continue;
			}
		}
		dane[i] = nowa;
	}

	rekord.SetDane(dane);

	std::ofstream plik("Dane/" + Nazwa + ".txt", std::ios::trunc);
	if (!plik)
	{
		std::cerr << "Nie mozna otworzyc pliku do zapisu!\n";
		return;
	}

	plik << "N ";
	for (size_t i = 0; i < kolumny.size(); ++i)
	{
		const Kolumna& kol = kolumny[i];
		std::string typStr;
		switch (kol.typ)
		{
		case TypDanych::Int: typStr = "Int"; break;
		case TypDanych::String: typStr = "String"; break;
		case TypDanych::Float: typStr = "Float"; break;
		}
		plik << typStr << ";" << kol.nazwa << ";";
		for (const auto& ogr : kol.ograniczenia)
		{
			plik << ogr << ";";
		}
		if (i != kolumny.size() - 1)
			plik << " ";
	}
	plik << "\n";

	for (auto& r : Rekordy)
	{
		const auto& daneR = r.GetDane();
		if (daneR.empty()) continue;
		plik << "R";
		for (const auto& wartosc : daneR)
		{
			plik << " " << wartosc;
		}
		plik << "\n";
	}

	plik.close();
	std::cout << "Rekord zostal zmodyfikowany.\n";
}


void Tabela::Filtruj()
{
	if (Rekordy.empty())
	{
		std::cout << "Brak rekordow do filtrowania.\n";
		return;
	}

	const auto& kolumny = Rekordy[0].GetKolumny();

	std::cout << "Dostepne kolumny:\n";
	for (size_t i = 0; i < kolumny.size(); ++i)
	{
		if (kolumny[i].ograniczenia.find("NOT NULL") != kolumny[i].ograniczenia.end())
		{
			std::cout << i << ": " << kolumny[i].nazwa << " ("
				<< (kolumny[i].typ == TypDanych::Int ? "Int" :
					kolumny[i].typ == TypDanych::Float ? "Float" : "String")
				<< ")\n";
		}
	}

	std::cout << "Wybierz numer kolumny do filtrowania: ";
	unsigned int kolIdx = WczytajLiczbeZakres(0,kolumny.size()-1);

	std::cout << "Wybierz sposob sortowania:\n1. Rosnaco\n2. Malejaco\n";
	unsigned int tryb = WczytajLiczbeZakres(1,2);

	if (tryb != 1 && tryb != 2)
	{
		std::cout << "Nieprawidlowy tryb sortowania.\n";
		return;
	}

	std::vector<Rekord> posortowane = Rekordy;

	const Kolumna& kolumna = kolumny[kolIdx];

	if (kolumna.typ == TypDanych::Int)
	{
		std::sort(posortowane.begin() + 1, posortowane.end(), [&](Rekord& a, Rekord& b)
			{
				int aVal = std::stoi(a.GetDane()[kolIdx]);
				int bVal = std::stoi(b.GetDane()[kolIdx]);
				return tryb == 1 ? aVal < bVal : aVal > bVal;
			});
	}
	else if (kolumna.typ == TypDanych::Float)
	{
		std::sort(posortowane.begin() + 1, posortowane.end(), [&](Rekord& a,Rekord& b)
			{
				float aVal = std::stof(a.GetDane()[kolIdx]);
				float bVal = std::stof(b.GetDane()[kolIdx]);
				return tryb == 1 ? aVal < bVal : aVal > bVal;
			});
	}
	else 
	{
		std::sort(posortowane.begin() + 1, posortowane.end(), [&](Rekord& a, Rekord& b)
			{
				return tryb == 1 ? a.GetDane()[kolIdx] < b.GetDane()[kolIdx]
					: a.GetDane()[kolIdx] > b.GetDane()[kolIdx];
			});
	}

	std::cout << "Wynik filtrowania:\n";
	for (auto& rekord : posortowane)
	{
		const auto& dane = rekord.GetDane();
		for (const auto& wartosc : dane)
		{
			std::cout << wartosc << " ";
		}
		std::cout << "\n";
	}
}
