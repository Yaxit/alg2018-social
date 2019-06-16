
/*
 * Lo scopo di questa classe è di raccogliere un set coerente di dati
 * del tipo User e Post opportunamente legati tra di loro.
 * La classe TGraph permette volutamente di fare riferimenti tra nodi
 * non appartnenenti alla classe (eventualmente modifichiamo la cosa)
 * in modo da permettere la ricerca di relazioni tra grafi fisicamente separati
 */
#ifndef THEPROJECT_DATASET_H
#define THEPROJECT_DATASET_H

#include "TGraph.hpp"
#include "User.h"
#include "StandardUsr.h"
#include "BusinessUsr.h"
#include "GroupUsr.h"
#include "Relation.h"
#include "FunctionSet.h"
#include <fstream>
#include <stack>
#include <algorithm>
#include <iomanip>
class DataSet {
private:
    TGraph<string, User, Relation> _users;
    TGraph<string, Post, Relation> _posts; //L'id del post può anche essere un numero? Prob no.
    int _postId=0;
    bool isAnello(const string &young, const string &eld);
public:
    bool addUser(const StandardUsr &usr); //Riceve l'User e ne crea una copia dinamica da mettere nel grafo
    bool addUser(const BusinessUsr &usr);
    bool addUser(const GroupUsr &usr);
    bool addUser(const string& usr, const string &type,
                 const string &s1, const string &s2, const string &s3, const string &s4, const string &s5, const string &s6 = string(""));
    bool deleteUser(const string &username); //Rimuove l'utente dal grafo, lo distrugge e cancella le relazioni
    bool editUser(const string &usr);

    //Riceve Post, ne crea una copia dinamica e lo mette nel grafo _posts. Crea la relazione di ownership
    int addPost(const string &owner, const string &msg, const string &date);
    bool deletePost(const string &postId);//Rimuove, distrugge, e cancella tutte le relazioni (like e ownership)
    bool editPost(const string &postId);

    bool addRelation(const string &u1, const string &u2, const Relation &rel);
    bool deleteRelation(string usr,string key);

    bool addLike(const string &usr, const string &postId, const Date &date = Date());
    bool deleteLike(const string &usr, const string &postId);
    bool addDislike(const string &usr, const string &postId, const Date &date = Date());
    bool deleteDislike(const string &usr, const string &postId);

    //Controlli sul tipo di utente (include controllo se esiste)
    bool isStandard(const string &usr);
    bool isBusiness(const string &usr);
    bool isGroup(const string &usr);
    bool isStandard(User* usr);
    bool isBusiness(User* usr);
    bool isGroup(User* usr);

    //Caricamento dati da file
    bool loadUsers(const string &filename);
    bool loadRelations(const string &filename);
    bool loadPosts(const string &filename);

    bool exportUsers(const string &filename);
    bool exportRelations(const string &filename);
    bool exportPosts(const string &filename);

    //Prende uno username e il tipo di relazione da conteggiare, restituisce il conteggio, o -1 e l'utente non esiste
    int getRelationNumber(const string &usr, int relType);
    int getReactionNumber(const string &postId, int relType); //-2 se la Rel non è valida (LIKE or DISLIKE)
    list<string> getRelated(const string &username, short type); //restituisce una lista di usernames/id di nodi legato da type a username

    //FUNZIONI STATISTICHE
    unsigned int getStandardUsrsCount();
    unsigned int getBusinessUsrsCount();
    unsigned int getGroupUsrsCount();
    unsigned int getUsersCount();
    void printBiggestCompany(bool includeSubs);//Maggior numero di dipendenti
    void printBestPost();//Più like
    void printWorsePost();//più dislike
    void printUserMaxFriends();
    void printUserMaxAcquientances();
    void printByAppreciacion(float inRate, bool includeSubs);
    int averageAge(); //Solo StandardUsers
    int getUsersBornAfter(Date date);
    int getRelativesNumber(const string& usr);//Numero di parenti estesi

    bool printFamilyTree(const string &username,bool toFile = false);
    void printAllFamilyTrees(bool toFile = false);
    void printWolf(int relNum, int groupNum, bool employed, int reactionNum,int ownedPosts);

    void printDetails(const string &username);
    void overview();
};


#endif //THEPROJECT_DATASET_H
