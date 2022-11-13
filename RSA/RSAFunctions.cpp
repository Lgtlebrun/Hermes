//
// Created by lgtle on 20/05/2020.
//

#include "RSAFunctions.h"


std::string RSAencrypt(std::string const& message, clef const& e, clef const& n){


    std::string new_message("");


    for (auto elt:message){

        new_message += std::to_string(exp_modulo(elt, e, n));
        new_message += " ";

    }

    return new_message;

}



//----------------------------------------------------------------------------------------

std::string RSAdecrypt(std::string& message, clef const& privee, clef const& e, clef const& n){

    std::string new_message("");

       size_t indice_esp;
        while((indice_esp = message.find(' ')) != (-1)){   // I.E. "Tant qu'on trouve des espaces dans le bazar...
         int elt;

         elt = stoi(message.substr(0, indice_esp));
         elt = exp_modulo(elt, privee, n);

         char c(elt);
         new_message += c;

         size_t pos(indice_esp + 1);
         message = message.substr(pos, message.size() - pos);
        }

        return new_message;
}

//----------------------------------------------------------------------------------------


jeu giveKeys(std::string const& pathPrime){

    clef privee(0);
    clef e(0);
    clef n(0), m(0);

    std::ifstream prime;
    prime.open(pathPrime);

    if(!prime.fail()){

     unsigned int p(0),q(0);
     unsigned long int gcd,x,y;    // x et y sont là pour décorer


            //do{                             // Tant que les clés choisies sont déjà prises

                while(p == q || (p*q) > 100000){          // q différent de p

                    p = (rand() % SECURE);      //On cherche une position au hasard dans la liste des premiers
                    q = (rand() % SECURE);

                    choix_premier(p, prime);
                    choix_premier(q, prime);


                }

            n = p*q;
            m = (p-1)*(q-1);

            do{

                do{
                                                    // On choisit une clef privée
                    privee = (rand() % m);
                    std::tie(gcd,x,y) = extended_gcd(privee, m);
                    while(gcd < 0){gcd += m;}

                }while (privee == p || privee == q || gcd != 1);





            // Calcul de la clef publique


                std::tie(gcd,x,y) = extended_gcd(privee,m);

                while(x < 0){x += m;}        // On pose x positif

                e = x;

                }while(e > 100000 || e < 129);

            //}while (isinkeylist(clefs_utilisees, privee) || isinkeylist(clefs_utilisees, e));


    }

    jeu sortie = {privee, e, n};
    return sortie;
}


//----------------------------------------------------------------------------------------


std::tuple<unsigned long int, unsigned long int, unsigned long int> extended_gcd(unsigned long int a, unsigned long int b)
{
    if (a == 0) {
        return std::make_tuple(b, 0, 1);
    }

    unsigned long int gcd, x, y;

    // unpack tuple  returned by function into variables
    std::tie(gcd, x, y) = extended_gcd(b % a, a);

    return std::make_tuple(gcd, (y - (b/a) * x), x);
}


//----------------------------------------------------------------------------------------


unsigned long int exp_modulo(unsigned long int x, unsigned long int y, unsigned long int z) {


    if (y == 1) return x % z;

    else if (y % 2 == 0) return exp_modulo( ((x % z) * (x % z)) % z, y / 2, z) % z;

    else return (x % z) * exp_modulo( ((x % z) * (x % z)) % z, (y - 1) / 2, z) % z;

}

//----------------------------------------------------------------------------------------


void choix_premier(unsigned int& pos, std::ifstream& prime){

            prime.seekg(0, std::ios::beg);
            prime.seekg(pos, std::ios::cur);
            prime.ignore(std::numeric_limits <std::streamsize>::max(), ',');


            prime >> std::ws >> pos;

}
