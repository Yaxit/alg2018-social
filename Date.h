

#ifndef LAB5_DATE_H
#define LAB5_DATE_H

#include <iostream>
#include <sstream>

using namespace std;
class Date {
private:
    int _day,_month,_year;
public:
    Date(); //Senza parametri è la data di oggi
    Date(int d, int m, int y);
    Date(const Date& toCopy);
    Date(const string& str);

    //Getter di giorno, mese, anno
    bool isParsable(const string &str);
    bool setDate(const string& str);
    int day() const;
    int month() const;
    int year() const;
    int yearsFrom ( Date d = Date () ) const ;
    string toStr() const;

    friend std :: ostream & operator << ( std :: ostream & stream , const Date & d);
    bool operator<(const Date& d2);

};


#endif //LAB5_DATE_H





