
#ifndef THEPROJECT_GROUPUSR_H
#define THEPROJECT_GROUPUSR_H

#include "User.h"

class GroupUsr : public User{
private:
    string _name, _activity, _description;

public:
    GroupUsr();
    GroupUsr(const GroupUsr &to_copy);
    //Getters
    const string &getName() const;
    const string &getActivity() const;
    const string &getDescription() const;

    //Setters
    void setName(const string &name);
    void setActivity(const string &activity);
    void setDescription(const string &description);
};


#endif //THEPROJECT_GROUPUSR_H







