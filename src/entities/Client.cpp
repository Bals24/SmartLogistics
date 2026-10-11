#include "entities/Client.hpp"
#include <iostream>

namespace sl {

Client::Client(std::string id,
               std::string nom,
               std::string telephone,
               std::string adresse)
    : id_(std::move(id)),
      nom_(std::move(nom)),
      telephone_(std::move(telephone)),
      adresse_(std::move(adresse)) {}

const std::string& Client::getId() const { return id_; }
const std::string& Client::getNom() const { return nom_; }
const std::string& Client::getTelephone() const { return telephone_; }
const std::string& Client::getAdresse() const { return adresse_; }

void Client::setTelephone(std::string telephone) { telephone_ = std::move(telephone); }
void Client::setAdresse(std::string adresse) { adresse_ = std::move(adresse); }

void Client::afficherInformations() const {
    std::cout << "------------------------------\n";
    std::cout << "CLIENT #" << id_ << "\n";
    std::cout << "Nom       : " << nom_ << "\n";
    std::cout << "Telephone : " << telephone_ << "\n";
    std::cout << "Adresse   : " << adresse_ << "\n";
    std::cout << "------------------------------\n";
}

} // namespace sl
