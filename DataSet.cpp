
#include "DataSet.h"
#include "Relation.h"

bool DataSet::addUser(const StandardUsr &usr) {
    if(!_users.nodeExists(usr.getUsername())) {
        _users.addNode(usr.getUsername(), new StandardUsr(usr));
        return true;
    } else
    {
        char cmd;
        do {
            cerr << "ATTENZIONE! " << usr.getUsername() << " è già inserito. Vuoi sovrascriverlo? [S/N]" << endl;
            cin>>cmd;
            cmd = (char)tolower(cmd);

        }while(!(cmd == 's' || cmd == 'n'));
        if(cmd=='s')
        {
            deleteUser(usr.getUsername()); //Safe-Delete del vecchio valore
            addUser(usr); //Richiamo ricorsivamente la funzione
            return true;
        } else return false;
    }
}
bool DataSet::addUser(const BusinessUsr &usr) {
    if(!_users.nodeExists(usr.getUsername())) {
        _users.addNode(usr.getUsername(), new BusinessUsr(usr));
        return true;
    } else
    {
        char cmd;
        do {
            cerr << "ATTENZIONE! " << usr.getUsername() << " è già inserito. Vuoi sovrascriverlo? [S/N]" << endl;
            cin>>cmd;
            cmd = (char)tolower(cmd);

        }while(!(cmd == 's' || cmd == 'n'));
        if(cmd=='s')
        {
            deleteUser(usr.getUsername()); //Safe-Delete del vecchio valore
            addUser(usr); //Richiamo ricorsivamente la funzione
            return true;
        } else return false;
    }
}
bool DataSet::addUser(const GroupUsr &usr) {
    if(!_users.nodeExists(usr.getUsername())) {
        _users.addNode(usr.getUsername(), new GroupUsr(usr));
        return true;
    } else
    {
        char cmd;
        do {
            cerr << "ATTENZIONE! " << usr.getUsername() << " è già inserito. Vuoi sovrascriverlo? [S/N]" << endl;
            cin>>cmd;
            cmd = (char)tolower(cmd);

        }while(!(cmd == 's' || cmd == 'n'));
        if(cmd=='s')
        {
            deleteUser(usr.getUsername()); //Safe-Delete del vecchio valore
            addUser(usr); //Richiamo ricorsivamente la funzione
            return true;
        } else return false;
    }
}
bool DataSet::deleteUser(const string &username) {
    if(!_users.nodeExists(username)) return false;

    map<string, Relation>* rels = _users.getRelations(username);
    for(map<string, Relation>::iterator it = rels->begin(); it != rels->end();it=rels->begin()) {

        if (it->second.type == OWNER) {
            deletePost(it->first);
        } else {
            deleteRelation(username, it->first); //A seconda del tipo di Relation tra i due capisce cosa fare.
        }
    }
    delete _users.getNode(username); //Libero la memoria dedicata al nostro utente
    _users.deleteNode(username); //Cancello la entry dell'utente dai nodi del grafo.
    return true;
}

bool DataSet::deletePost(const string &postId) {
    if(!_posts.nodeExists(postId)) return false;

    map<string, Relation>* rels = _posts.getRelations(postId); //Estraggo le relazioni del post specifico
    for(map<string, Relation>::iterator it = rels->begin(); it != rels->end(); it=rels->begin()) //Elimino rel Usr->Post e Post->Usr
    {
        string first = it->first;
        _posts.deleteRelation(postId, first);
        _users.deleteRelation(first, postId);

    }
    delete _posts.getNode(postId); //Distruggi
    _posts.deleteNode(postId); //Esegue anche un purge di tutte le eventuali relazioni interne a _posts
    return true;
}

int DataSet::addPost(const string &owner, const string &msg, const string &date) {
    if(!_users.nodeExists(owner)) return -1; //Owner non esiste
    Date dat;
    if(owner.empty() || owner.find(',') != string::npos ||
            msg.empty() || !dat.setDate(date))
        return -2; //Errore di formattazione
    Post post;
    Relation rel;
    post.setOwner(owner);
    post.setText(msg);
    post.setDate(Date(date));
    post.setId(to_string(_postId));
    rel.type = OWNER;
    rel.date = Date(date);

    _posts.addNode(post.getId(), new Post(post)); //Crea una copia dinamica e la aggiunge

    _posts.addRelation(to_string(_postId), owner, rel);
    _users.addRelation(owner, to_string(_postId), rel);

    return _postId++;
}

bool DataSet::addRelation(const string &u1, const string &u2, const Relation &rel) {
    if(_users.relationExists(u1, u2) || _users.relationExists(u2,u1) ||
            !_users.nodeExists(u1) || !_users.nodeExists(u2)) return false;
    if(u1==u2) return false;
    User* us1;
    User* us2;
    us1 = _users.getNode(u1);
    us2 = _users.getNode(u2);

    if(rel.type == ACQUAINTANCE && isStandard(us1) && isStandard(us2))
    {
        const Relation *ptr =_users.getRelation(u2,u1); //Controllo che lui non mi conosca, nel caso diventano amici.
        if(ptr != nullptr)
            if(ptr->type == ACQUAINTANCE)
            {
                Relation modified;
                modified.type = FRIEND;
                _users.deleteRelation(u2,u1);
                _users.addRelation(u1,u2,modified);
                _users.addRelation(u2,u1,modified);
            }
        _users.addRelation(u1,u2, rel);
    }else if((rel.type == FRIEND || rel.type == CONSORT) && isStandard(us1) && isStandard(us2))
    {
        _users.addRelation(u1,u2,rel);
        _users.addRelation(u2,u1,rel);
    }else if (rel.type == EMPLOYEE && ( (isStandard(us1) && isBusiness(us2)) || (isStandard(us2) && isBusiness(us1))) ) {
        _users.addRelation(u1,u2,rel);
        _users.addRelation(u2,u1,rel);
    }else if (rel.type == SUBSIDIARY && isBusiness(us1) && isBusiness(us2)) {
        _users.addRelation(u1,u2,rel);
        _users.addRelation(u2,u1,rel);
    }else if (rel.type == GROUPMEMBER && ( (isStandard(us1)&&isGroup(us2))||(isStandard(us2)&&isGroup(us1)) ) ) {
        _users.addRelation(u1,u2,rel);
        _users.addRelation(u2,u1,rel);
    }else if(rel.type == PARENT && isStandard(us1) && isStandard(us2))
    {
        //Controllo anello
        if(isAnello(u1,u2))
            return false;
        Relation backRel = rel;
        backRel.type = SON;
        _users.addRelation(u1,u2,rel);
        _users.addRelation(u2,u1, backRel);
    }else if(rel.type == SON && isStandard(us1) && isStandard(us2))
    {
        if(isAnello(u2,u1))
            return false;
        Relation backRel = rel;
        backRel.type = PARENT;
        _users.addRelation(u1,u2,rel);
        _users.addRelation(u2,u1,backRel);
    } else return false;//Il tipo di relazione non esiste o i tipi di utente non sono compatibili
    return true;
}

bool DataSet::addLike(const string &usr, const string &postId, const Date &date) {
    if((_posts.relationExists(postId,usr) && _users.relationExists(usr, postId) )|| !_users.nodeExists(usr)) return false;
    else
    {
        Relation rel;
        rel.type = LIKE;
        rel.date = date;

        _users.addRelation(usr, postId, rel);
        _posts.addRelation(postId, usr, rel);
        return true;
    }
}

bool DataSet::addDislike(const string &usr, const string &postId, const Date &date) {
    if((_posts.relationExists(postId,usr) && _users.relationExists(usr, postId) )|| !_users.nodeExists(usr)) return false;
    else
    {
        Relation rel;
        rel.type = DISLIKE;
        rel.date = date;

        _users.addRelation(usr, postId, rel);
        _posts.addRelation(postId, usr, rel);
        return true;
    }
}

bool DataSet::deleteRelation(string usr, string key) {
    const Relation *rel =_users.getRelation(usr, key); //Mi ritorna il puntatore della Relation, o nullptr
    if(rel == nullptr) return false;

    if(rel->type == ACQUAINTANCE) //Non simmetrica
    {
        _users.deleteRelation(usr, key);
    } else if(rel->type == FRIEND || rel->type == SON || rel->type == PARENT || rel->type == CONSORT
              || rel->type == EMPLOYEE || rel->type == SUBSIDIARY || rel->type == GROUPMEMBER)
    {
        _users.deleteRelation(usr, key);
        _users.deleteRelation(key, usr);
    } else if(rel->type == LIKE || rel->type == DISLIKE)
    {
        _users.deleteRelation(usr, key);
        _posts.deleteRelation(key, usr);
    }
    return true;
}

bool DataSet::editUser(const string &usr) {
    if(!_users.nodeExists(usr)) return false;
    string address, date;
    User* usrPtr = _users.getNode(usr);
    cout<<"Indirizzo: ";
    getline(cin,address);
    cout<<"Data: ";
    getline(cin,date);
    if (!address.empty()) usrPtr->setAddress(address);
    if (!date.empty()) usrPtr->setDate(Date(date));
    if(isStandard(usrPtr->getUsername())) {
        StandardUsr *stdPtr = (StandardUsr *) usrPtr;
        string name, surname, gender;
        cout << "Nome: ";
        getline(cin,name);
        cout << "Cognome: ";
        getline(cin,surname);
        cout << "Sesso: ";
        getline(cin,gender);

        if (!name.empty()) stdPtr->setName(name);
        if (!surname.empty()) stdPtr->setSurname(surname);
        if(!gender.empty())
            if (tolower(gender[0]) == 'm' || tolower(gender[0]) == 'f') stdPtr->setGender(toupper(gender[0]));
    }
    else if(isBusiness(usrPtr->getUsername())) {
        BusinessUsr *bssPtr = (BusinessUsr *) usrPtr;
        string name, headqtr, product, incomes;
        cout<<"Nome :";
        getline(cin,name);
        cout<<"Headquarters :";
        getline(cin,headqtr);
        cout<<"Product: ";
        getline(cin,product);
        cout<<"Fatturato :";
        getline(cin,incomes);

        if(!name.empty()) bssPtr->setName(name);
        if(!headqtr.empty()) bssPtr->setHeadqtr(headqtr);
        if(!product.empty()) bssPtr->setProduct(product);
        if(!incomes.empty()) bssPtr->setIncomes(stoi(incomes));
    } else if (isGroup(usrPtr->getUsername())){
        GroupUsr* grpPtr = (GroupUsr*) usrPtr;
        string name, activity, desc;
        cout<<"Nome: ";
        getline(cin,name);
        cout<<"Attività: ";
        getline(cin,activity);
        cout<<"Descrizione :";
        getline(cin,desc);

        if(!name.empty()) grpPtr->setName(name);
        if(!activity.empty()) grpPtr->setActivity(activity);
        if(!desc.empty()) grpPtr->setDescription(desc);
    }
    return true;
}

bool DataSet::deleteLike(const string &usr, const string &postId) {
    if(!_users.nodeExists(usr) || !_posts.nodeExists(postId) || !_users.relationExists(usr, postId)) return false;
    _users.deleteRelation(usr, postId);
    _posts.deleteRelation(postId, usr);
    return true;
}

bool DataSet::deleteDislike(const string &usr, const string &postId) {
    if(!_users.nodeExists(usr) || !_posts.nodeExists(postId) || !_users.relationExists(usr, postId)) return false;
    _users.deleteRelation(usr, postId);
    _posts.deleteRelation(postId, usr);
    return true;
}

unsigned int DataSet::getStandardUsrsCount() {
    unsigned int count = 0;
    _users.setStart();
    for(User* usr = _users.getNext();usr != nullptr; usr = _users.getNext())
    {
        if(isStandard(usr)) count++;
    }
    return count;
}
unsigned int DataSet::getBusinessUsrsCount() {
    unsigned int count = 0;
    _users.setStart();
    for(User* usr = _users.getNext();usr != nullptr; usr = _users.getNext())
    {
        if(isBusiness(usr)) count++;
    }
    return count;
}
unsigned int DataSet::getGroupUsrsCount(){
    unsigned int count = 0;
    _users.setStart();
    for(User* usr = _users.getNext();usr != nullptr; usr = _users.getNext())
    {
        if(isGroup(usr)) count++;
    }
    return count;
}

unsigned int DataSet::getUsersCount() {
    return (unsigned int)_users.getNodesNumber();
}

void DataSet::printBiggestCompany(bool includeSubs) {
    list<BusinessUsr*> list;
    int maxNumber = 0;
    _users.setStart();
    for(User* usr = _users.getNext(); usr != nullptr; usr = _users.getNext()) {
        if (usr->getType() == 'B') { //Lavoro solo se ho un BusinessUser
            int localCount = 0; //E' il contatore di ogni azienda
            localCount += getRelationNumber(usr->getUsername(), EMPLOYEE); //aggiungo i lavoratori presso l'azienda
            if (includeSubs) //Se devo contare anche le SUBS
            {
                map<string, Relation> *rels = _users.getRelations(
                        usr->getUsername()); //Estraggo le relazioni di quella di partenza
                for (map<string, Relation>::iterator it = rels->begin(); it != rels->end(); it++) {
                    if (it->second.type == SUBSIDIARY) //Per ogni SUBSIDIARY che trovo
                    {
                        localCount += getRelationNumber(it->first, EMPLOYEE); //Aggiungo anche i suoi impiegati
                    }
                }
            }
            //Finito il conteggio, controllo con il max fino ad ora
            if (localCount > maxNumber) {//Ho un nuovo massimo
                maxNumber = localCount;
                list.clear();
                list.push_back((BusinessUsr *) usr);
            } else if (localCount == maxNumber) {//Ho un uguale al massimo
                list.push_back((BusinessUsr *) usr);
            }
        }
    }
    //Print delle aziende
    cout<<"Aziende con maggior numero di dipendenti ("<<maxNumber;
    if(includeSubs) cout<<" incluse consociate";
    cout<<")"<<endl;
    for(auto it = list.begin(); it != list.end();it++)
    {
        cout<<(*it)->getName()<<"\t("<<(*it)->getUsername()<<")\t";
    }
}

int DataSet::getRelationNumber(const string &usr, int relType) {
    if(!_users.nodeExists(usr)) return -1;
    map<string, Relation>* ptr =_users.getRelations(usr);
    if(ptr == nullptr) return 0;
    int count = 0;
    for(map<string,Relation>::iterator it = ptr->begin(); it!= ptr->end();it++)
    {
        if(it->second.type == relType) count++;
    }
    return count;

}

void DataSet::printBestPost() {
    list<Post*> list;
    int max = 0;
    _posts.setStart();
    for(Post* ptr = _posts.getNext(); ptr != nullptr; ptr = _posts.getNext())
    {
        int postCount = getReactionNumber(ptr->getId(), LIKE);
        if(postCount>max)
        {
            max = postCount;
            list.clear();
            list.push_back(ptr);
        }else if(postCount == max)
        {
            list.push_back(ptr);
        }
    }
    //Print dei migliori post
    cout<<"Post con più like ("<<max<<")"<<endl;
    for(auto it = list.begin(); it != list.end();it++)
    {
        string out1 = "(" + (*it)->getId() + ")\t";
        cout<<setw(5)<<right<<out1<<(*it)->getDate()<<"\t"<<(*it)->getOwner()<<":\""<<(*it)->getText()<<"\""<<endl;
    }
}

int DataSet::getReactionNumber(const string &postId, int relType) {
    if(!_posts.nodeExists(postId)) return -1;
    if(relType != DISLIKE && relType != LIKE) return -2;
    map<string, Relation>* ptr =_posts.getRelations(postId);
    int count = 0;
    for(map<string,Relation>::iterator it = ptr->begin(); it!= ptr->end();it++)
    {
        if(it->second.type == relType) count++;
    }
    return count;
}

void DataSet::printWorsePost() {
    list<Post*> list;
    int max = 0;
    _posts.setStart();
    for(Post* ptr = _posts.getNext(); ptr != nullptr; ptr = _posts.getNext())
    {
        int postCount = getReactionNumber(ptr->getId(), DISLIKE);
        if(postCount>max)
        {
            max = postCount;
            list.clear();
            list.push_back(ptr);
        }else if(postCount == max)
        {
            list.push_back(ptr);
        }
    }
    cout<<"Post con più dislike ("<<max<<")"<<endl;
    for(auto it = list.begin(); it != list.end();it++)
    {
        string out1 = "(" + (*it)->getId() + ")\t";
        cout<<setw(5)<<right<<out1<<(*it)->getDate()<<"\t"<<(*it)->getOwner()<<":\""<<(*it)->getText()<<"\""<<endl;

    }
}

void DataSet::printUserMaxFriends() {
    list<StandardUsr*> list;
    int max = 0;
    _users.setStart();
    for(User* ptr = _users.getNext(); ptr != nullptr; ptr = _users.getNext())
    {
        if(ptr->getType() == 'S') {

            int friendsCount = getRelationNumber( ptr->getUsername(), FRIEND);
            if (friendsCount > max) {
                max = friendsCount;
                list.clear();
                list.push_back((StandardUsr*)ptr);
            } else if (friendsCount == max) {
                list.push_back((StandardUsr*)ptr);
            }
        }
    }
    cout<<"Utenti con più amici ("<<max<<")"<<endl;
    for(auto it = list.begin(); it != list.end();it++)
    {
        cout<<(*it)->getName()<<" "<<(*it)->getSurname()<<" ("<<(*it)->getUsername()<<")"<<endl;
    }
}

void DataSet::printUserMaxAcquientances() {
    list<StandardUsr*> list;
    int max = 0;
    _users.setStart();
    for(User* ptr = _users.getNext(); ptr != nullptr; ptr = _users.getNext())
    {
        if(isStandard(ptr)) {
            int friendsCount = getRelationNumber( ptr->getUsername(), ACQUAINTANCE);
            if (friendsCount > max) {
                max = friendsCount;
                list.clear();
                list.push_back((StandardUsr*)ptr);
            } else if (friendsCount == max) {
                list.push_back((StandardUsr*)ptr);
            }
        }
    }
    cout<<"Utenti con più conoscenti ("<<max<<")"<<endl;
    for(auto it = list.begin(); it != list.end();it++)
    {
        cout<<(*it)->getName()<<" "<<(*it)->getSurname()<<" ("<<(*it)->getUsername()<<")"<<endl;
    }
}

int DataSet::averageAge() {
    int ageSum = 0;
    int stdUsrsNum = 0;
    _users.setStart();
    for(User* usr = _users.getNext(); usr != nullptr; usr = _users.getNext())
    {
        if(isStandard(usr))
        {
            ageSum += usr->getDate().yearsFrom(Date());
            stdUsrsNum++;
        }
    }
    return ageSum/stdUsrsNum;
}

bool DataSet::loadUsers(const string &filename){
    int i=1;
    bool correct;
    string line, username, type, infos, info;
    string out[8];
    ifstream file;
    file.open(filename);

    if (!file.is_open()) {
        cerr << "Errore apertura file utenti" << endl;
        return false;
    }
    while (!file.eof()) {
        if(file.peek() == ifstream::traits_type::eof())
            return true;
        correct = true;
        getline(file, line);
        while (line.empty())
        {
            if(file.eof()) return true;
            getline(file, line);
        }
        stringstream ss(line);
        getline(ss, out[0], ',');
        getline(ss, out[1], ',');
        getline(ss, infos);
        out[1][0] = toupper(out[1][0]);
        if(infos.empty()) return false;
        if (infos[infos.length() - 1] == '\r') infos.pop_back(); //Win
        if (infos[0] != '{' || infos[infos.length() - 1] != '}') correct = false;
        infos = infos.substr(1, infos.length() - 2);
        ss.str(infos);
        ss.clear();

        if (!correct) {
            cerr << "Errore alla linea: " << i << endl;
            return false;
        }

        if (out[1][0] == 'S') { //Da type la prima lettera
            string key[5] = {"nome:", "cognome:", "sesso:", "nato:", "indirizzo:"};
            for (int i = 0; i < 5; i++) {
                getline(ss, info, ',');
                if ((info.substr(0, key[i].size()) == key[i]))out[i + 2] = info.substr(key[i].size());
                else correct = false;
            }

        } else if (out[1][0] == 'B') { //Da type la prima lettera
            string key[6] = {"nome:", "prodotto:", "fatturato:", "indirizzo:", "sede:", "fondazione:"};
            for (int i = 0; i < 6; i++) {
                getline(ss, info, ',');
                if ((info.substr(0, key[i].size()) == key[i]))out[i + 2] = info.substr(key[i].size());
                else correct = false;
            }

        } else {
            string key[5] = {"nome:", "attivita:", "descrizione:", "data:", "sede:"};
            for (int i = 0; i < 5; i++) {
                getline(ss, info, ',');
                if ((info.substr(0, key[i].size()) == key[i])) out[i + 2] = info.substr(key[i].size());
                else correct = false;
            }
        }

        if (!correct) {
            cerr << "Errore alla linea: " << i << endl;
            return false;
        }
        if (!addUser(out[0], out[1], out[2], out[3], out[4], out[5], out[6], out[7])) {
            cerr << "Impossibile aggiungere l'utente '" << out[0] << "'";
            return false;
        }
        i++;
    }
    file.close();
    return true;
}

bool DataSet::loadRelations(const string &filename) {
    ifstream file;
    file.open(filename);
    string line;
    int count = 0;

    if (!file.is_open()) {
        cerr << "Errore apertura file relazioni." << endl;
        return false;
    };
    while (!file.eof()) {
        if (file.peek() == ifstream::traits_type::eof())
            return true;
        count++;
        string id1, id2, relation;
        Relation rel;
        getline(file, line);

        while (line.empty())
        {
            if(file.eof()) return true;
            getline(file, line);
            count++;
        }
        stringstream ss(line);
        getline(ss, id1, ',');
        getline(ss, id2, ',');
        getline(ss, relation);
        if (relation[relation.length() - 1] == '\r') relation = relation.substr(0, relation.length() - 1);

        if (id1.empty() || id2.empty() || relation.empty()) {
            cerr << "Linea " << count << ". Errore di formattazione." << endl;
            return false;
        }
        if (relation == "ACQUAINTANCE") rel.type = ACQUAINTANCE;
        else if (relation == "FRIEND") rel.type = FRIEND;
        else if (relation == "CONSORT") rel.type = CONSORT;
        else if (relation == "PARENT") rel.type = PARENT;
        else if (relation == "SON") rel.type = SON;
        else if (relation == "SUBSIDIARY") rel.type = SUBSIDIARY;
        else if (relation == "EMPLOYEE") rel.type = EMPLOYEE;
        else if (relation == "GROUPMEMBER") rel.type = GROUPMEMBER;
        else {
            cerr << "Linea " << count << ". Errore di formattazione." << endl;
            return false;
        }
        if (!addRelation(id1, id2, rel)) {
            cerr << "Linea " << count << ". Impossibile aggiungere la relazione." << endl;
            return false;
        }
    }
    return true;
}

bool DataSet::exportUsers(const string &filename) {
    ofstream file;
    file.open(filename);
    if(!file.is_open())
    {
        cerr<<"Errore nell'apertura del file"<<endl;
        return false;
    }
    _users.setStart();
    for(User* usr =_users.getNext(); usr!= nullptr; usr = _users.getNext())
    {
        file<<usr->getUsername()<<','<<usr->getType()<<",{";
        if(usr->getType() == 'S')
        {
            StandardUsr* ptr = (StandardUsr*) usr;
            file<<"nome:"<<ptr->getName()<<",cognome:"<<ptr->getSurname()<<",sesso:"<<ptr->getGender()<<",nato:"
                <<ptr->getDate()<<",indirizzo:"<<ptr->getAddress()<<'}'<<endl;
        }else if(usr->getType() == 'B')
        {
            BusinessUsr* ptr = (BusinessUsr*) usr;
            file<<"nome:"<<ptr->getName()<<",prodotto:"<<ptr->getProduct()<<",fatturato:"<<ptr->getIncomes()
                <<",indirizzo:"<<ptr->getAddress()<<",sede:"<<ptr->getHeadqtr()<<",fondazione:"<<ptr->getDate()<<'}'<<endl;
        } else if(usr->getType() == 'G')
        {
            GroupUsr* ptr = (GroupUsr*) usr;
            file<<"nome:"<<ptr->getName()<<",attivita:"<<ptr->getActivity()<<",descrizione:"<<ptr->getDescription()
                <<",data:"<<ptr->getDate()<<",sede:"<<ptr->getAddress()<<'}'<<endl;
        }
    }
    file.close();
    return true;
}

bool DataSet::exportRelations(const string &filename) {
    ofstream file;
    file.open(filename);
    if(!file.is_open())
    {
        cerr<<"Impossibile aprire "<<filename<<endl;
        return false;
    }
    _users.BFSinit(); //Segno tutti come da passare
    _users.setStart();
    for(User* usr = _users.getNext(); usr!=nullptr; usr = _users.getNext()) //Passo uno ad uno tutti gli user
    {
        string tmp = usr->getUsername();
        map<string, Relation> *rels = _users.getRelations(usr->getUsername()); //Per ognuno recupero le relazioni
        if (rels != nullptr) {
            for (map<string, Relation>::iterator it = rels->begin(); it != rels->end();it++) {
                if (it->second.type ==
                    ACQUAINTANCE) {  //Se è una conoscenza non mi importa se dall'altro sono già passato
                    file << usr->getUsername() << ',' << it->first << ',' << relToStr(it->second.type) << endl;
                } else if (_users.getColor(it->first) != BLACK) //altrimenti controllo di non esserci ancora passato
                {
                    if (it->second.type == FRIEND || it->second.type == CONSORT ||
                        //Controllo le varie relazioni da salvare (tra utenti)
                        it->second.type == PARENT || it->second.type == SON ||
                        it->second.type == SUBSIDIARY || it->second.type == EMPLOYEE ||
                        it->second.type == GROUPMEMBER) {
                        file << usr->getUsername() << ',' << it->first << ',' << relToStr(it->second.type)<<endl; //Scrivo sul file la relazione
                    }
                }
            }
        }
        _users.setColor(usr->getUsername(), BLACK);
    }
    return true;

}

bool DataSet::loadPosts(const string &filename) {
    int i = 1;
    int id;
    bool correct;
    string line,likers,dislikers;
    ifstream file;

    file.open(filename);

    if (!file.is_open()) {
        cerr << "Errore apertura file posts" << endl;
        return false;
    }
    while(!file.eof()){
        if (file.peek() == ifstream::traits_type::eof())
            return true;

        Date date;
        correct = true;
        string owner,data,token[2];
        string key[2] = {":ekilsid",":ekil"};
        getline(file,line);
        while (line.empty())
        {
            if(file.eof()) return true;
            getline(file, line);
        }
        if(!line.empty())
            if(line[line.length()-1] == '\r') line.pop_back();
        stringstream ss(line);
        getline(ss,owner,','); //Prendo il primo campo (owner)
        getline(ss,line);
        reverse(line.begin(),line.end());
        ss.clear();
        ss.str(line);//Svuoto lo stream
        for(int i=0;i<2;i++)
        {
            string garbage;
            getline(ss,garbage,'}');
            if(!garbage.empty())
                correct = false;
            getline(ss,token[i],'{');
            reverse(token[i].begin(),token[i].end());
            getline(ss,garbage,',');
            if(garbage != key[i])
                correct = false;
        }
        getline(ss,data,',');
        reverse(data.begin(),data.end());
        getline(ss,line);
        reverse(line.begin(),line.end()); //Messaggio
        if(!date.setDate(data)) correct = false;
        if(line.empty()) correct = false;
        if(!correct){
            cerr<<"Errore di formattazione linea: "<<i<<endl;
            return false;
        }
        id = addPost(owner,line,date.toStr()); //Il numero del post inserito.
        if (id == -1){
            cerr << "Impossibile aggiungere post alla linea: " << i << endl;
            return false;
        }
        ss.clear();
        ss.str(token[1]);
        while (getline(ss,line,',')) {
            if(!addLike(line,to_string(id),date)) {
                cout << "Errore aggiunta like linea: " << i << endl;
                return false;
            }
        }
        ss.clear();
        ss.str(token[0]);
        while (getline(ss,line,',')) {
            if(!addDislike(line,to_string(id),date)) {
                cout << "Errore aggiunta dislike linea: " << i << endl;
                return false;
            }
        }
        i++;
    }
    return true;
}

bool DataSet::exportPosts(const string &filename) {
    ofstream file;
    file.open(filename);
    if(!file.is_open())
    {
        cerr<<"Impossibile aprire "<<filename<<endl;
        return false;
    }
    _posts.setStart();
    for(Post* post = _posts.getNext(); post != nullptr; post = _posts.getNext())
    {
        list<string> likers,dislikers;
        const string &text = post->getText();
        string ownerId;
        map<string, Relation>* rels = _posts.getRelations(post->getId());
        for(map<string, Relation>::iterator it = rels->begin(); it != rels->end(); it++)
        {
            if(it->second.type == LIKE) likers.push_back(it->first);
            else if(it->second.type == DISLIKE) dislikers.push_back(it->first);
            else if(it->second.type == OWNER) ownerId = it->first;
        }
        file<<ownerId<<','<<text<<','<<post->getDate()<<",like:{";
        for(list<string>::iterator it = likers.begin(); it != likers.end();) //Non molto bello anyway
        {
            file<<*it;
            if(++it != likers.end()) file<<',';
        }
        file<<"},dislike:{";
        for(list<string>::iterator it = dislikers.begin(); it != dislikers.end();)
        {
            file<<*it;
            if(++it != dislikers.end()) file<<',';

        }
        file<<'}'<<endl;
    }
    file.close();
    return true;
}

bool DataSet::printFamilyTree(const string &username, bool toFile) {
    if(!isStandard(username)) return false;
    int relCount = 0;
    stringstream out;
    queue<string> queue1, queue2;
    map<string, int> genMap, genMap2; //associo ad ogni utente la sua generazione relativa a username
    string youngest; //Username della generazione più recente
    string eldest; //Username del capostipite
    int minGen = 0; //Inizializzo generazione più in basso
    int maxGen = 0; //Inizializzo generazione più in alto
    queue1.push(username);
    genMap[username] = 0; //Il punto di partenza è la generazione 0, le generazioni sono riferite a questo
    while (!queue1.empty())
    {
        list<string> parents = getRelated(queue1.front(), SON); //Ottengo i genitori dell'elemento in coda
        for(auto it = parents.begin(); it!=parents.end();it++) //Li scorro tutti
        {
            queue1.push(*it);//Ognuno lo aggiungo alla coda
            genMap[*it] = genMap[queue1.front()]+1;//E gli associo il suo valore di generazione relativo
        }
        queue1.pop();//Passo al prossimo in coda
    }

    for(auto it = genMap.begin(); it!=genMap.end();it++)
    {
        if(it->second >= maxGen)
        {
            maxGen = it-> second;
            eldest = it->first;
        }
    }

    if (maxGen<2) {
        queue2.push(eldest);
        while (!queue2.empty())
        {
            list<string> sons = getRelated(queue2.front(),PARENT);
            for(auto it = sons.begin(); it!=sons.end();it++)
            {
                queue2.push(*it);
                genMap2[*it] = genMap2[queue2.front()]-1;
            }
            queue2.pop();
        }

        for(auto it = genMap2.begin(); it!=genMap2.end();it++)
        {
            if(it->second <= minGen)
            {
                minGen = it-> second;
                youngest = it->first;
            }
        }
        if (maxGen-minGen < 2) { // Non stampo alberi "corti" (voglio almeno un figlio che sia a sua volta padre)
            return false;
        }
    }

    list<string> toPrint;
    toPrint.push_front(eldest);
    int num = maxGen;
    while (!toPrint.empty())
    {
        StandardUsr* ptr;
        /* Stampo un numero di spaziature in funzione della distanza
         * in termini generazionali tra il capostipite (maxGen) e l'utente
         * che sto lavorando */
        for(int i = genMap[toPrint.front()]; i<maxGen;i++) {
            out << "\t\t";
        }
        //Se la distanza tra il precedente e quello che sto analizzando è <= 1
        //ho a che fare con i suoi figli, e stampo un braccino
        if(num-genMap[toPrint.front()] <=1)
            out<<"└─────> ";
        ptr = (StandardUsr*)_users.getNode(toPrint.front());
        out<<ptr->getName()<<" "<<ptr->getSurname();//Stampo il nome dell'utente attuale
        relCount++;
        _users.setColor(toPrint.front(),BLACK);//Mi segno che ci sono passato
        list<string> consorts = getRelated(toPrint.front(), CONSORT);
        for(auto it = consorts.begin(); it!=consorts.end();it++)
        {
            ptr =(StandardUsr*) _users.getNode(*it);
            out<<" + ("<<ptr->getName()<<" "<<ptr->getSurname()<<")";
            relCount++;
        }
        out<<endl;

        list<string> sons = getRelated(toPrint.front(), PARENT);
        if(!sons.empty()) {

            for (auto it = sons.begin(); it != sons.end(); it++) {
                list<string>::iterator padre = toPrint.begin();
                genMap[*it] = genMap[toPrint.front()]-1;
                toPrint.insert(++padre,*it);
            }

        }
        num = genMap[toPrint.front()];
        toPrint.pop_front();
    }
    if(toFile)
    {
        ofstream file("alberi/"+username+".txt");
        file<<out.rdbuf();
        file.close();
    } else
    {
        out<<endl<<"---------------------------------------------------------------------------------------"<<endl<<endl;
        cout<<out.rdbuf();
    }
    return true;
}

list<string> DataSet::getRelated(const string &username, short type) {
    list<string> out;
    map<string, Relation>* usr = _users.getRelations(username);
    if(usr == nullptr) return out;
    for(auto it=usr->begin();it!=usr->end();it++)
    {
        if(it->second.type == type) out.push_back(it->first);
    }
    return out;
}

bool DataSet::addUser(const string &usr, const string &type, const string &s1, const string &s2, const string &s3,
                      const string &s4, const string &s5, const string &s6) {
    if(_users.nodeExists(usr)) return false;
    if(usr.empty() || type.empty()) return false;
    Date date;
    if(toupper(type[0]) == 'S')
    {
        if(s1.empty() || s1.find(',') != string::npos ||        //Nome
           s2.empty() || s2.find(',') != string::npos ||        //Cognome
           (toupper(s3[0]) != 'M' && toupper(s3[0]) != 'F') ||  //Genere
           !date.setDate(s4) ||                            //Nato (Data)
           s5.empty() || s5.find(',') != string::npos)          //Indirizzo
        {
            return false;
        }
        string name = s1;
        string surname = s2;
        name[0] = toupper(name[0]);
        surname[0] = toupper(surname[0]);
        StandardUsr user;
        user.setUsername(usr);
        user.setType(toupper(type[0]));
        user.setName(name);
        user.setSurname(surname);
        user.setGender(toupper(s3[0]));
        user.setDate(date);
        user.setAddress(s5);
        if(!addUser(user)) return false;
    }else if(toupper(type[0]) == 'B')
    {
        if(s1.empty() || s1.find(',') != string::npos || //Nome
           s2.empty() || s2.find(',') != string::npos || //Prodotto
           s3.empty() ||                                 //Fatturato
           s4.empty() || s4.find(',') != string::npos || //Indirizzo
           s5.empty() || s5.find(',') != string::npos || //Sede
           !date.setDate(s6) )                      //Fondazione
        {
            return false;
        }
        BusinessUsr user;
        user.setUsername(usr);
        user.setType(toupper(type[0]));
        user.setName(s1);
        user.setProduct(s2);
        user.setIncomes(atoi(s3.c_str()));
        user.setAddress(s4);
        user.setHeadqtr(s5);
        user.setDate(date);
        if(!addUser(user)) return false;
    } else if(toupper(type[0]) == 'G')
    {
        if(s1.empty() || s1.find(',') != string::npos || //Nome
           s2.empty() || s2.find(',') != string::npos || //Attivita
           s3.empty() || s3.find(',') != string::npos || //Descrizione
           !date.setDate(s4) ||                          //Data
           s5.empty() || s5.find(',') != string::npos)   //Sede (Indirizzo)
        {
            return false;
        }
        GroupUsr user;
        user.setUsername(usr);
        user.setType(toupper(type[0]));
        user.setName(s1);
        user.setActivity(s2);
        user.setDescription(s3);
        user.setDate(date);
        user.setAddress(s5);

        if(!addUser(user)) return false;
    }else return false; //Il tipo era diverso, non posso inserire
    return true;
}

void DataSet::printAllFamilyTrees(bool toFile) {
    _users.BFSinit();
    _users.setStart();
    for(User* usr = _users.getNext(); usr!=nullptr; usr = _users.getNext())
    {
        string tmp = usr->getUsername();
        if(isStandard(usr->getUsername()) && _users.getColor(usr->getUsername()) == WHITE)
        {
            printFamilyTree(usr->getUsername(),toFile);
        }
    }
}

int DataSet::getUsersBornAfter(Date date) {
    int count = 0;
    _users.setStart();
    for(User* usr = _users.getNext(); usr != nullptr; usr = _users.getNext())
    {
        if(isStandard(usr) && date<usr->getDate()) count++;
    }
    return count;
}

bool DataSet::isStandard(const string &usr) {
    if(_users.getNode(usr) != nullptr)
        return _users.getNode(usr)->getType() == 'S';
    return false;
}

bool DataSet::isGroup(const string &usr) {
    if(_users.getNode(usr) != nullptr)
        return _users.getNode(usr)->getType() == 'G';
    return false;
}

bool DataSet::isBusiness(const string &usr) {
    if(_users.getNode(usr) != nullptr)
        return _users.getNode(usr)->getType() == 'B';
    return false;
}

bool DataSet::isStandard(User *usr) {
    if(usr != nullptr)
        return usr->getType() == 'S';
    return false;
}

bool DataSet::isBusiness(User *usr) {
    if(usr != nullptr)
        return usr->getType() == 'B';
    return false;
}

bool DataSet::isGroup(User *usr) {
    if(usr != nullptr)
        return usr->getType() == 'G';
    return false;
}

void DataSet::printByAppreciacion(float inRate, bool includeSubs) {
    multimap<float, BusinessUsr*> ordered;
    _users.setStart();
    for(User* usr = _users.getNext(); usr!=nullptr; usr = _users.getNext())//Passo tutti gli utenti
    {
        int likes=0;
        int dislikes=0;
        list<string> posts;

        if(isBusiness(usr->getUsername()))//Mi interessando solo i Business
        {
            posts = getRelated(usr->getUsername(), OWNER); //Prendo l'id di tutti i post dell'azienda;
            for(list<string>::iterator it = posts.begin();it!=posts.end();it++) //Per ogni post aggiorno i contatori dell'azienda
            {
                likes += getReactionNumber(*it, LIKE);
                dislikes += getReactionNumber(*it,DISLIKE);
            }
            if(includeSubs) //Se anche subs
            {
                list<string> subs = getRelated(usr->getUsername(), SUBSIDIARY); //Prendo tutte le subs
                for(list<string>::iterator it = subs.begin(); it != subs.end();it++)//Per ogni azienda sub
                {
                    likes += getReactionNumber(*it,LIKE); //Aggiungo all'azienda principale anche i suoi likes
                    dislikes += getReactionNumber(*it,DISLIKE);
                }
            }
            float den = likes+dislikes;
            float rate;
            if(den == 0)
                rate=0;
            else
                rate = (float)likes/den;
            if(rate > inRate) //Se il inRate supera quello minimo
            {   //Salvo l'utente nella mappa. Usando come key il suo inRate. La mappa si ordina secondo la key

                ordered.insert(pair<float, BusinessUsr*>(rate, (BusinessUsr*)usr));
            }
        }
    }
    cout<<"Aziende con apprezzamento superiore a "<<inRate*100<<"%";
    if(includeSubs) cout<<" (consociate incluse)";
    cout<<endl;
    for(auto it = ordered.begin(); it!=ordered.end();it++)
    {   cout<<setprecision(1)<<fixed;
        ;
        cout<<it->first*100<<"%\t"<<it->second->getName()<<" ("<<it->second->getUsername()<<")"<<endl;
    }
}

void DataSet::printDetails(const string &username) {
    User* usr = _users.getNode(username);
    if(!_users.nodeExists(username) || usr == nullptr){
        cout<<"Utente non registrato"<<endl;
        return;
    }
    bool seePosts;
    if(isStandard(usr)) {
        StandardUsr *usrPtr = (StandardUsr *) usr;
        cout << "STD\t" << usrPtr->getName() << " " << usrPtr->getSurname() << " (" << usrPtr->getUsername() << ')'
             << endl << "Nato: " << usrPtr->getDate() << endl << "Sesso: " << usrPtr->getGender() << endl
             << "Indirizzo: " << usrPtr->getAddress() << endl << "Post scritti: "
             << getRelationNumber(usr->getUsername(), OWNER) << endl << "Like e Dislike: "
             << getRelationNumber(usr->getUsername(), LIKE) << ',' << getRelationNumber(usr->getUsername(), DISLIKE)
             << endl;
    } else if(isBusiness(usr)) {
        BusinessUsr *usrPtr = (BusinessUsr *) usr;
        cout << "BSS\t" << usrPtr->getName() << " (" << usrPtr->getUsername() << ")" << endl << "Prodotto: "
             << usrPtr->getProduct() << endl << "Fatturato: " << usrPtr->getIncomes() << endl << "Indirizzo: "
             << usrPtr->getAddress() << endl << "Sede: " << usrPtr->getHeadqtr() << endl << "Fondazione: "
             << usrPtr->getDate() << endl << "Dipendenti: " << getRelationNumber(usr->getUsername(), EMPLOYEE) << endl
             << "Consociate: " << getRelationNumber(usr->getUsername(), SUBSIDIARY) << endl;
    }else if(isGroup(usr)) {
        GroupUsr *usrPtr = (GroupUsr *) usr;
        cout << "GRP\t" << usrPtr->getName() << " (" << usrPtr->getUsername() << ")" << endl << "Attività: "
             << usrPtr->getActivity() << endl << "Descrizione: " << usrPtr->getDescription() << endl << "Sede: "
             << usrPtr->getAddress() << endl << "Fondazione: " << usrPtr->getDate() << endl << "Partecipanti: "
             << getRelationNumber(usr->getUsername(), GROUPMEMBER) << endl;
    }
    cout<<"Desideri vedere i post?";
    seePosts = basicYesNo();
    if(seePosts)
    {
        list<string> posts = getRelated(username, OWNER);
        if(posts.empty()) cout<<"Nessun post"<<endl;
        for(list<string>::iterator it = posts.begin();it!=posts.end();it++)
        {
            Post* ptr = _posts.getNode(*it);
            cout<<"---- POST ID: "<<ptr->getId()<<" SCRITTO: "<<ptr->getDate()<<" ----"<<endl;
            cout<<ptr->getText()<<endl;
        }
    }
}

void DataSet::overview() {
    cout << "Utenti registrati: " << getUsersCount() << endl << "--> di cui Semplici: " << getStandardUsrsCount()
         << endl << "--> di cui Business: " << getBusinessUsrsCount() << endl << "--> di cui Gruppi: "
         << getGroupUsrsCount() << endl;
    cout << "Post Inseriti: " << _posts.getNodesNumber()<<endl<<endl;
}

bool DataSet::editPost(const string &postId) {
    string msg,date;
    Post* ptr;
    Date data;
    ptr = _posts.getNode(postId);
    if(ptr == nullptr)
        return false;
    cout<<"Inserisci il nuovo messaggio"<<endl;
    getline(cin, msg);
    cout<<"Inserisci una nuova data"<<endl;
    cin>>date; cin.ignore();
    ptr->setText(msg);
    if(data.setDate(date))
        ptr->setDate(data);
    return true;
}

int DataSet::getRelativesNumber(const string &usr) {
    if(!_users.nodeExists(usr)) return -1;
    _users.BFSinit();
    return _users.BFS(usr, isRelative)-1;
}

void DataSet::printWolf(int relNum, int groupNum, bool employed, int reactionNum, int ownedPosts) {
    _users.setStart();
    for(User* usr = _users.getNext();usr!= nullptr; usr=_users.getNext())
    {
        if(isStandard(usr))
        {
            int uRelNum,uGroupNum,uReactionNum,uOwnedPosts;
            bool uEmployed;
            uRelNum = getRelationNumber(usr->getUsername(), FRIEND) + getRelationNumber(usr->getUsername(), ACQUAINTANCE);
            uGroupNum = getRelationNumber(usr->getUsername(), GROUPMEMBER);
            uReactionNum = getRelationNumber(usr->getUsername(), LIKE) + getRelationNumber(usr->getUsername(), DISLIKE);
            uOwnedPosts = getRelationNumber(usr->getUsername(), OWNER);
            uEmployed = (bool) getRelationNumber(usr->getUsername(), EMPLOYEE);

            if(uRelNum<=relNum && uGroupNum<=groupNum && uReactionNum<=reactionNum && uOwnedPosts<=ownedPosts && !(uEmployed && employed))
            {
                StandardUsr* ptr = (StandardUsr*)usr;
                cout<<ptr->getName()<<" "<<ptr->getSurname()<< " ("<<ptr->getUsername()<<")"<<endl;
            }


        }
    }

}

bool DataSet::isAnello(const string &young, const string &eld) {
    queue<string> coda;
    coda.push(young);
    while (!coda.empty()) {
        map<string, Relation> *rels = _users.getRelations(coda.front());
        if (rels == nullptr) return false;
        for (auto it = rels->begin(); it != rels->end(); it++) {
            if (it->second.type == SON) {
                if (it->first == eld) {
                    return true;
                }
                coda.push(it->first);
            }
        }
        coda.pop();
    }
    return false;
}
