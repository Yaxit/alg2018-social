//
// Created by andri on 29/05/2018.
//

#ifndef THEPROJECT_STANDARDUSR_H
#define THEPROJECT_STANDARDUSR_H

#include "User.h"

class StandardUsr : public User {
private:
    string _name, _surname;
    char _gender;

public:
    StandardUsr();
    StandardUsr(const StandardUsr &to_copy);

    //Getters:
    const string &getName() const;
    const string &getSurname() const;
    char getGender() const;

    //Setters:
    void setName(const string& name);
    void setSurname(const string& surname);
    void setGender(char gender);

};


#endif //THEPROJECT_STANDARDUSR_H