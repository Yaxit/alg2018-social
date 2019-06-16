

#include "FunctionSet.h"
#include "DataSet.h"
bool isRelative(Relation rel)
{
    return rel.type==CONSORT || rel.type==PARENT || rel.type==SON;
}

int basicGetInt()
{
    int num;
    string tmp;
    while (!(cin>>num))
    {
        //cin.ignore(INT16_MAX,'\n');
        cin.clear();
        getline(cin,tmp);
    }
    cin.ignore();
    return num;
}
bool basicYesNo()
{
    char cmd;
    do {
        cout<<"[S/N]"<<endl;
        cin>>cmd;
        cmd = (char)tolower(cmd);

    }while(!(cmd == 's' || cmd == 'n'));
    return cmd == 's';
}

string relToStr(short relType)
{
    switch (relType)
    {
        case 0:
            return "ACQUAINTANCE";
        case 1:
            return "FRIEND";
        case 2:
            return "CONSORT";
        case 3:
            return "PARENT";
        case 4:
            return "SON";
        case 5:
            return "SUBSIDIARY";
        case 6:
            return "EMPLOYEE";
        case 7:
            return "GROUPMEMBER";
        case 8:
            return "OWNER";
        case 9:
            return "LIKE";
        case 10:
            return "DISLIKE";
        default:
            return "";
    }
}

void wait()
{
    cin.get();
    cin.ignore();
}