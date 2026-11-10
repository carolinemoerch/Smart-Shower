#pragma once
#include <string>

class User{
public:
    User ();
    User(int Id, std::string UserName);
    
    int getId() const;
    std::string getUserName() const;

private:
    int Id_;
    std::string UserName_;

};
