
#include "Post.h"

const string &Post::getOwner() const {
    return _owner;
}

const string &Post::getText() const {
    return _text;
}

const string &Post::getId() const {
    return _id;
}

const Date &Post::getDate() const {
    return _date;
}

void Post::setOwner(const string &_owner) {
    Post::_owner = _owner;
}

void Post::setText(const string &_text) {
    Post::_text = _text;
}

void Post::setId(const string &id) {
    Post::_id = id;
}

void Post::setDate(const Date &_date) {
    Post::_date = _date;
}

Post::Post(const Post &to_copy) {
    this->_id = to_copy._id;
    this->_owner = to_copy._owner;
    this->_text = to_copy._text;
    this->_date = to_copy._date;
}

Post::Post() {
    //Do nothing
}
