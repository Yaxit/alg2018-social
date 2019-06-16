
#include "StandardUsr.h"


//Getters implementation
const string &StandardUsr::getName() const{
    return _name;
}

const string &StandardUsr::getSurname() const{
    return _surname;
}

char StandardUsr::getGender() const{
    return _gender;
}

//Setters implementation
void StandardUsr::setName(const string &name) {
    _name = name;
}

void StandardUsr::setSurname(const string &surname) {
    _surname = surname;
}

void StandardUsr::setGender(char gender) {
    _gender = gender;
}

StandardUsr::StandardUsr(const StandardUsr &to_copy) {
    this->_username = to_copy._username;
    this->_address = to_copy._address;
    this->_date = to_copy._date;
    this->_userType = to_copy._userType;
    //Special
    this->_name = to_copy._name;
    this->_surname = to_copy._surname;
    this->_gender = to_copy._gender;

}

StandardUsr::StandardUsr() {
    this->_userType = 'S';
}
