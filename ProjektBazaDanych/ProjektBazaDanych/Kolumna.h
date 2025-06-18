#pragma once

#include <iostream>
#include "TypDanych.h";
#include <set>

struct Kolumna {
	std::string nazwa;
	TypDanych typ;
	std::set<std::string> ograniczenia; //NOT NULL, UNIQUE, PRIMARY KEY, SECONDARY KEY

	Kolumna(std::string Nazwa, TypDanych Typ, std::set<std::string> Ograniczenia);
};