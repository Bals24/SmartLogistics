#ifndef SMARTLOGISTICS_CLIENT_HPP
#define SMARTLOGISTICS_CLIENT_HPP

#include <string>

namespace sl {

class Client {
public:
    Client(std::string id,
           std::string nom,
           std::string telephone,
           std::string adresse);

    const std::string& getId() const;
    const std::string& getNom() const;
    const std::string& getTelephone() const;
    const std::string& getAdresse() const;

    void setTelephone(std::string telephone);
    void setAdresse(std::string adresse);

    void afficherInformations() const;

private:
    std::string id_;
    std::string nom_;
    std::string telephone_;
    std::string adresse_;
};

} // namespace sl

#endif // SMARTLOGISTICS_CLIENT_HPP
