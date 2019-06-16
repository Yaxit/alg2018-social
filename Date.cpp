

#include "Date.h"
#include <ctime>
#include <sstream>
Date::Date(int d, int m, int y) {
    _day = d;
    _month = m;
    _year = y;
}

Date::Date(const Date &toCopy) {
    _day = toCopy.day();
    _month = toCopy.month();
    _year = toCopy.year();

}

Date::Date() {
    time_t now = time(0);
    tm *ltm = localtime(&now);
    _day = ltm->tm_mday;
    _month = 1+ltm->tm_mon;
    _year = 1900+ltm->tm_year;
}

Date::Date(const string &str) {
    char delim;
    if(this->isParsable(str)){
        istringstream ss(str);
        ss>>_day>>delim>>_month>>delim>>_year;
        if(_year<=99)
            _year += 1900;
    } else
    {
        _day = _month = _year = 0;
    }

}
int Date::day() const {
    return _day;
}

int Date::month() const {
    return _month;
}

int Date::year() const {
    return _year;
}

int Date::yearsFrom(Date d) const {
    if(d._month > _month || (d._month == _month && d._day>=_day)){
        return d._year - _year;
    }else {
        return d._year -_year - 1;
    };
}

string Date::toStr() const {
    stringstream out;
    if(_day<10) out << "0";
    out << _day << "/";
    if(_month<10) out << "0";
    out << _month << "/" << _year;
    return out.str();
}

std::ostream &operator<<(std::ostream &stream, const Date &d) {
    stream<< d.toStr();
    return stream;
}

bool Date::operator<(const Date &d2) {

    if (_year == d2._year){ //guardo l'anno, se non sono uguali non entra oltre e va alla fine
        if (_month == d2._month){ // idem per il mese
            if (_day==d2._day) return false; // returna false in quanto date uguali
            else if (_day < d2._day) return true; //returna true poiché d2 è maggiore di d1 come impostato da operatore
            else return false; //false se il giorno di d1 è maggiore di d2
        } else if (_month < d2._month)return true;
        else return false;
    }else if (_year < d2._year) return true;
    else return false;
}

bool Date::isParsable(const string &str) {
    int day = 0;
    int month = 0;
    int year = 0;
    char delim;
    istringstream ss(str);
    if(ss>>day>>delim>>month>>delim>>year)
    {
        if (day>=1 && day<=31 && month>=1 && month<=12 && (year <= 99 || year>=1000)) //Due tipi di formattazione diversa (yy o yyyy)
        return true;
    }
    return false;
}

bool Date::setDate(const string &str) {
    int day = 0;
    int month = 0;
    int year = 0;
    char delim;
    istringstream ss(str);
    if(ss>>day>>delim>>month>>delim>>year)
    {
        if (day>=1 && day<=31 && month>=1 && month<=12 && (year <= 99 || year>=1000)) { //Due tipi di formattazione diversa (yy o yyyy)
            _day = day;
            _month = month;
            if(year<=99) year += 1900;
            _year = year;
            return true;
        }
        cerr<<"Data sbagliata"<<endl;
    }
    return false;
}







