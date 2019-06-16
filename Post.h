
#ifndef THEPROJECT_POST_H
#define THEPROJECT_POST_H

#include <iostream>
#include <list>
#include "Date.h"
using namespace std;
class Post {
private:
    string _owner, _text, _id;
    Date _date;

public:
    Post(const Post& to_copy);
    Post();

    const string &getOwner() const;
    const string &getText() const;
    const string &getId() const;
    const Date &getDate() const;

    void setOwner(const string &owner);
    void setText(const string &text);
    void setId(const string &id);
    void setDate(const Date &date);


};


#endif //THEPROJECT_POST_H
