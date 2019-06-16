
#ifndef THEPROJECT_RELATION_H
#define THEPROJECT_RELATION_H

#include "Date.h"
#define ACQUAINTANCE 0
#define FRIEND 1
#define CONSORT 2
#define PARENT 3
#define SON 4

#define SUBSIDIARY 5
#define EMPLOYEE 6

#define GROUPMEMBER 7

#define OWNER 8
#define LIKE 9
#define DISLIKE 10

typedef struct {
    short int type;
    Date date;
} Relation;


#endif //THEPROJECT_RELATION_H
