
#include "User.h"


//Getters implementation
const string &User::getUsername() const {
    return _username;
}

const string &User::getAddress() const {
    return _address;
}

Date User::getDate() const {
    return _date;
}


//Setters implementation
void User::setUsername(const string &username) {
    _username = username;
}

void User::setAddress(const string &address) {
    _address = address;
}

void User::setDate(const Date &date) {
    _date = date;
}

User::User(const User &to_copy) {
    this->_date = to_copy._date;
    this->_username = to_copy._username;
    this->_address = to_copy._address;

}

User::User() {
    //Do nothing
}

const char User::getType() const {
    return _userType;
}

void User::setType(char type) {
    _userType=type;
}
