#include "AplikacjaGui.h"


BazaDanych bazaDanych;
void AplikacjaGui::Wyswietl() {
	std::cout << "Witaj w Bazie Danych \n";
	std::cout << "Wybierz co chcesz zrobic: \n";
	std::cout << "1.Wyswietl tabele \n";
	std::cout << "2.Dodaj tabele \n";
	std::cout << "3.Usun tabele \n";
	std::cout << "4.Modyfikuj tabele \n";
	std::cout << "5.Wyszukaj w bazie danych \n";
	std::cout << "6.Zakoncz dzialanie aplikacji \n";
	ObsluzInput();
}

void AplikacjaGui::ObsluzInput() {
	unsigned int wybor;
	std::cin >> wybor;

	if (std::cin.fail()) {
		std::cin.clear(); 
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
		std::cout << "Niepoprawny input! Wprowadz operacje z listy!\n";
		std::this_thread::sleep_for(std::chrono::seconds(1));
		system("cls");
		Wyswietl();
		return;
	}
		switch (wybor)
		{
		case 1:
			bazaDanych.WyswietlTabele();
			std::cout << "\n";
			Wyswietl();
			break;
		case 2:
			bazaDanych.DodajTabele();
			std::this_thread::sleep_for(std::chrono::seconds(1));
			system("cls");
			Wyswietl();
			break;
		case 3:
			bazaDanych.UsunTabele();
			std::this_thread::sleep_for(std::chrono::seconds(1));
			system("cls");
			Wyswietl();
			break;
		case 4:
			bazaDanych.ModyfikujTabele();
			std::this_thread::sleep_for(std::chrono::seconds(1));
			system("cls");
			Wyswietl();
			break;
		case 5:
			bazaDanych.Wyszukaj();
			Wyswietl();
			break;
		case 6:
			break;
		default:
			std::cout << "Nie ma takiej operacji! \n";
			break;
		}
}

AplikacjaGui::AplikacjaGui() {
	Wyswietl();
}

