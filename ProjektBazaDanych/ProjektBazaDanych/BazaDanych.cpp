#include "BazaDanych.h"


namespace fs = std::filesystem;

unsigned int BazaDanych::WczytajLiczbeZakres(unsigned int min, unsigned int max) {
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


void BazaDanych::ZaladujDaneTabel()
{
    tabele = std::vector<Tabela>();
    if (fs::is_empty("Dane")) {
        tabele = std::vector<Tabela>();
        std::cout << "Pusto";
    }
    else {
        for (const auto& entry : fs::directory_iterator("Dane")) {
            Tabela tabela;
            tabela.WczytajTabeleZPliku(entry.path().filename().string());
            tabele.push_back(tabela);
        }
    }
}

BazaDanych::BazaDanych() {
    ZaladujDaneTabel();
}

void BazaDanych::WyswietlTabele() {
    ;
    if (fs::is_empty("Dane")) {
        std::cout << "Nie ma zadnych tabel do wyswietlenia :( \n";
    }
    else {
        std::cout << "Ktora tabele chcesz wyswietlic? \n";
        int i{ 1 };
        for (auto& tabela : tabele)
        {
            std::cout <<i++<<"."+tabela.GetNazwa() << std::endl;
        }
        unsigned int wybor = WczytajLiczbeZakres(1,tabele.size());
        tabele[wybor - 1].WypiszDaneTabeli();
    }
}

void BazaDanych::DodajTabele()
{
    std::string nazwa;
    std::cout << "Wprowadz nazwe tabeli (Jezeli nie chcesz stworzyc tabeli wpisz znak - ): \n";
    std::cin >> nazwa;

    if (nazwa == "-") {
        std::cout << "Tabela nie zostala stworzona \n";
        return;
    }
    std::ofstream plik("Dane/"+ nazwa + ".txt");

    if (!plik) {
        std::cerr << "Nie mozna otworzyc pliku do zapisu!" << std::endl;
    }
    else {
        std::cout << "Pomyslnie utworzono tabele o nazwie " << nazwa << "\n";
    }
    ZaladujDaneTabel();
    
}

void BazaDanych::UsunTabele()
{
    std::cout << "Ktora tabele chcesz usunac? \n";
    int i{ 1 };
    for (auto& tabela : tabele)
    {
        std::cout << i++ << "." + tabela.GetNazwa() << std::endl;
    }
    std::cout << i << ".Powrot do menu\n";
    unsigned int wybor = WczytajLiczbeZakres(1,tabele.size()+1);
    std::string nazwaPliku;
    if (wybor == tabele.size() + 1) return;
    nazwaPliku = tabele[wybor-1].GetNazwa() + ".txt";
 
    

    if (std::filesystem::remove("Dane/"+nazwaPliku)) {
        std::cout << "Tabela '" << tabele[wybor-1].GetNazwa() << "' zostala usunieta" << std::endl;
        ZaladujDaneTabel();
    }
    else {
        std::cout << "Nie udalo sie usunac tabeli" << std::endl;
    }
}

void BazaDanych::Wyszukaj()
{
    std::cout << "Podaj fragment tekstu do wyszukania: ";
    std::string fraza;
    std::cin.ignore(); 
    std::getline(std::cin, fraza);

    bool znaleziono = false;

    for (auto& tabela : tabele)
    {
        std::vector<Rekord> dopasowane;

        for (auto& rekord : tabela.GetRekordy())
        {
            for (auto& wartosc : rekord.GetDane())
            {
                if (wartosc.find(fraza) != std::string::npos)
                {
                    dopasowane.push_back(rekord);
                    break;
                }
            }
        }

        if (!dopasowane.empty())
        {
            znaleziono = true;
            std::cout << "\nTabela: " << tabela.GetNazwa() << "\n";

            for (auto& kol : tabela.GetRekordy()[0].GetKolumny())
            {
                std::cout << kol.nazwa << "\t";
            }
            std::cout << "\n";

            for (auto& rekord : dopasowane)
            {
                for (auto& wartosc : rekord.GetDane())
                {
                    std::cout << wartosc << "\t";
                }
                std::cout << "\n";
            }
        }
    }

    if (!znaleziono)
    {
        std::cout << "Nie znaleziono zadnych dopasowan.\n";
    }
}

void BazaDanych::ModyfikujTabele() {
    std::cout << "Ktora tabele chcesz modyfikowac? \n";
    int i{ 1 };
    for (auto& tabela : tabele)
    {
        std::cout << i++ << "." + tabela.GetNazwa() << std::endl;
    }
    std::cout << i << ".Powrot do menu \n";
    unsigned int wybor = WczytajLiczbeZakres(1, tabele.size() + 1);
    if (wybor == tabele.size() + 1) return;
    std::cout << "Jaka operacje chcesz wykonac? \n";
    std::cout << "1.Zmien nazwe tabeli \n";
    std::cout << "2.Dodaj kolumne \n";
    std::cout << "3.Usun kolumne \n";
    std::cout << "4.Dodaj rekord \n";
    std::cout << "5.Usun rekord \n";
    std::cout << "6.Modyfikuj rekord \n";
    Tabela DoModyfikacji = tabele[wybor - 1];
    unsigned int wybor2 = WczytajLiczbeZakres(1, 6);
    switch(wybor2)
    {
    case 1:
        DoModyfikacji.ZmienNazweTabeli();
        ZaladujDaneTabel();
        break;
    case 2:
        DoModyfikacji.DodajKolumne();
        ZaladujDaneTabel();
        break;
    case 3:
        DoModyfikacji.UsunKolumne();
        ZaladujDaneTabel();
        break;
    case 4:
        DoModyfikacji.DodajRekord();
        ZaladujDaneTabel();
        break;
    case 5:
        DoModyfikacji.UsunRekord();
        ZaladujDaneTabel();
        break;
    case 6:
        DoModyfikacji.ModyfikujRekord();
        ZaladujDaneTabel();
        break;
    default:
        std::cout << "Nieprawidlowy numer operacji! \n";
        break;
    }
}