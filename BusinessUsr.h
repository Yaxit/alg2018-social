
#ifndef THEPROJECT_BUSINESSUSR_H
#define THEPROJECT_BUSINESSUSR_H

#include "User.h"

class BusinessUsr : public User{
private:
    string _name, _headqtr, _product;
    int _incomes;

public:
    BusinessUsr();
    BusinessUsr(const BusinessUsr &to_copy);
    //Getters
    const string &getName() const;
    const string &getProduct() const;
    const string &getHeadqtr() const;
    int getIncomes() const;

    //Setters
    void setName(const string &name);
    void setProduct(const string& prod);
    void setHeadqtr(const string& headqtr);  //perché non gli piace?
    void setIncomes(int incomes);

};


#endif //THEPROJECT_BUSINESSUSR_H