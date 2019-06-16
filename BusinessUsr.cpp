
#include "BusinessUsr.h"


const string &BusinessUsr::getName() const {
    return _name;
}

const string &BusinessUsr::getProduct() const {
    return _product;
}

const string &BusinessUsr::getHeadqtr() const {
    return _headqtr;
}

int BusinessUsr::getIncomes() const {
    return _incomes;
}


void BusinessUsr::setName(const string &name) {
    _name = name;
}

void BusinessUsr::setProduct(const string &prod) {
    _product = prod;
}

void BusinessUsr::setHeadqtr(const string &headqtr) {
    _headqtr = headqtr;
}

void BusinessUsr::setIncomes(int incomes) {
    _incomes = incomes;
}

BusinessUsr::BusinessUsr(const BusinessUsr &to_copy) {
    //User
    this->_username = to_copy._username;
    this->_address = to_copy._address;
    this->_date = to_copy._date;
    this->_userType = to_copy._userType;
    //Speciali
    this->_headqtr = to_copy._headqtr;
    this->_incomes = to_copy._incomes;
    this->_product = to_copy._product;
    this->_name = to_copy._name;
}

BusinessUsr::BusinessUsr() {
    this->_userType = 'B';
}



