
#include "GroupUsr.h"


//Getters implementation
const string &GroupUsr::getName() const {
    return _name;
}

const string &GroupUsr::getActivity() const {
    return _activity;
}

const string &GroupUsr::getDescription() const {
    return _description;
}


//Setters implementation
void GroupUsr::setName(const string &name) {
    _name = name;
}

void GroupUsr::setActivity(const string &activity) {
    _activity = activity;
}

void GroupUsr::setDescription(const string &description) {
    _description = description;
}

GroupUsr::GroupUsr() {
    this->_userType = 'G';
}

GroupUsr::GroupUsr(const GroupUsr &to_copy) {
    //User
    this->_username = to_copy._username;
    this->_address = to_copy._address;
    this->_date = to_copy._date;
    this->_userType = to_copy._userType;
    //Special
    this->_name = to_copy._name;
    this->_activity = to_copy._activity;
    this->_description = to_copy._description;
}






