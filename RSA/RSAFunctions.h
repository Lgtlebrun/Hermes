//
// Created by lgtle on 20/05/2020.
//

#pragma once

#ifndef SERVEURCHAT_RSAFUNCTIONS_H
#define SERVEURCHAT_RSAFUNCTIONS_H

#include <string>
#include <iostream>
#include <fstream>
#include <tuple>
#include <limits>

int const       SECURE(500);

typedef unsigned long int clef;

struct jeu {clef privee; clef e; clef n;};

//Crypt
std::string RSAencrypt(std::string const& message, clef const& e, clef const& n);
std::string RSAdecrypt(std::string& message, clef const& privee, clef const& e, clef const& n);

//Math
std::tuple<unsigned long int, unsigned long int, unsigned long int> extended_gcd(unsigned long int a, unsigned long int b);
unsigned long int exp_modulo(unsigned long int x, unsigned long int n, unsigned long int m);

//Utilitaires
void choix_premier(unsigned int& pos, std::ifstream& prime);
jeu giveKeys(std::string const& pathPrime);

#endif //SERVEURCHAT_RSAFUNCTIONS_H
