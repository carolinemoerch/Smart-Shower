#include "User.h"

User::User(){}
User::User(int Id, std::string UserName)
    : Id_(Id), UserName_(UserName)
{}

int User::getId() const { return Id_; }
std::string User::getUserName() const { return UserName_; }