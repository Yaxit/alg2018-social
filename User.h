
#ifndef THEPROJECT_USER_H
#define THEPROJECT_USER_H

#include "Date.h"
#include "Post.h"
class User {
protected:
    char _userType;
    string _username, _address;
    Date _date;
    //list<Post> _posts;
public:
    User();
    User(const User &to_copy);
    //Getters:
    const string &getUsername() const;
    const string &getAddress() const;
    Date getDate() const;
    const char getType() const;

    //Setters:
    void setUsername(const string& username);
    void setAddress(const string& address);
    void setDate(const Date& date); //Passaggio
    void setType(char type);
};


#endif //THEPROJECT_USER_H