#ifndef THEPROJECT_TGRAPH_H
#define THEPROJECT_TGRAPH_H

#include <list>
#include <map>
#include <cctype>
#include <queue>
#include <iostream>
#include <functional>
#define WHITE 0
#define GREY 1
#define BLACK 2

using namespace std;

template<typename S, typename T, typename R>
class TGraph {
private:
    map<S, T *> _nodes; //Creo una mappa di nodi. Si accede con una stringa (username) a ciascun nodo
    map<S, map<S, R> > _adj;  //mappa delle relazioni
    map<S, short int> _color;
    typename map<S, T *>::iterator _currentPos;

public:
    unsigned long getNodesNumber();
    bool addNode(const S &key, T *ptr);
    bool deleteNode(const S &key);
    T *getNode(const S &key); //Mettere a posto tutti i const
    bool nodeExists(const S &key);

    void setStart(); //Imposta l'iteratore dei nodi su BEGIN
    T *getNext();// Restituisce il nodo currentPos, poi incrementa

    bool addRelation(const S &id1, const S &id2, R rel);
    bool deleteRelation(const S &id1, const S &id2);
    bool deleteRelation(const S &id);
    const R *getRelation(const S &id1, const S &id2); //Restituisce il puntatore alla relazione tra id1 e id2. nullptr se non esiste alcuna relazione.
    map<S, R> *getRelations(const S &id); //Restituisce il puntatore alla map<> con le relazioni di id
    bool relationExists(const S &id1, const S &id2);
    bool purgeFromRelations(const string &id);

    void BFSinit();
    int BFS(const S &key, bool (*relValid)(R)); //Si limita ad analizzare partendo da un nodo
    short getColor(const S &id);
    void setColor(const S &id, short color);
};

template<typename S, typename T, typename R>
bool TGraph<S, T, R>::addNode(const S &key, T *ptr) {

    if (ptr == nullptr) {
        return false;
    } else if (!nodeExists(key)) {
        _nodes[key] = ptr;
        return true;
    } else {
        char cmd;
        do {
            cerr << "ATTENZIONE! " << key << " è già inserito. Vuoi sovrascriverlo? [S/N]" << endl;
            cin >> cmd;
            cmd = (char) tolower(cmd);

        } while (cmd != 's' || cmd != 'n');
        if (cmd == 's') {
            deleteNode(key); //Delete del vecchio valore
            addNode(key, ptr); //Richiamo ricorsivamente la funzione
            return true;
        } else return false;
    }

}

template<typename S, typename T, typename R>
T *TGraph<S, T, R>::getNode(const S &key) {
    if (nodeExists(key)) return _nodes[key];
    else return nullptr;
}


template<typename S, typename T, typename R>
bool TGraph<S, T, R>::nodeExists(const S &key) {
    return (_nodes.find(key) != _nodes.end()); //Fa quello nel commento ^
}

template<typename S, typename T, typename R>
bool TGraph<S, T, R>::deleteNode(const S &key) {
    if (nodeExists(key)) {
        _nodes.erase(key);
        purgeFromRelations(key); //Elimina tutte le relazioni key->qualcuno e tutte le qualcuno->key in questo grafo
        return true;
    }
    return false;
}

template<typename S, typename T, typename R>
bool TGraph<S, T, R>::addRelation(const S &id1, const S &id2, R rel) {
    if (_adj.find(id1) != _adj.end() && _adj[id1].find(id2) != _adj[id1].end()) {
        return false; //Se la relazione tra i due User esiste già, non la modifico.
    }
    _adj[id1][id2] = rel; //Se non esiste lo crea, se esiste lo sovrascrive
    return true;
}

template<typename S, typename T, typename R>
bool TGraph<S, T, R>::relationExists(const S &id1, const S &id2) {
    if(_adj.find(id1) != _adj.end())//Se trovo le rel di id1
    {
        if(_adj[id1].find(id2) != _adj[id1].end())//Cerco quella id1->id2
        return true; //Esiste la relazione tra id1 e id2
    }
    return false;
}

template<typename S, typename T, typename R>
bool TGraph<S, T, R>::deleteRelation(const S &id1, const S &id2) {
    if (!relationExists(id1, id2)) return false;
    else {
        _adj[id1].erase(id2); //Cancello il livello più interno così da eliminare la relazione
        if (_adj[id1].size() == 0) _adj.erase(id1); //Se id1 non ha altre relazioni cancello anche lui dalla mappa.
        return true;
    }
}

template<typename S, typename T, typename R>
const R *TGraph<S, T, R>::getRelation(const S &id1, const S &id2) {
    if (!relationExists(id1, id2)) return nullptr;
    return &_adj[id1][id2];
}

template<typename S, typename T, typename R>
map<S, R> *TGraph<S, T, R>::getRelations(const S &id) {
    if(_adj.find(id) == _adj.end()) return nullptr;
    if (_adj[id].size() == 0) return nullptr;
    return &_adj[id];
}

template<typename S, typename T, typename R>
void TGraph<S, T, R>::setStart() {
    _currentPos = _nodes.begin();
    return;
}

template<typename S, typename T, typename R>
T *TGraph<S, T, R>::getNext() {
    if (_currentPos != _nodes.end()) return _currentPos++->second;
    else return nullptr;
}

template<typename S, typename T, typename R>
int TGraph<S, T, R>::BFS(const S &key, bool (*relValid)(R)) {
    queue<S> queue;
    queue.push(key);
    int count = 0;
    while (queue.size() != 0) {
        S actualNode = queue.front();
        map<S, R> *adj = this->getRelations(actualNode); //Mappa delle adiacenze del key (nodo) attuale
        if(adj == nullptr) return 1;
        typename map<S, R>::iterator it;
        for (it = adj->begin(); it != adj->end(); it++) //in ogni elemento
        {
            if (relValid(it->second) && _color[it->first] == WHITE) {

                if (nodeExists(it->first)) //Se è un riferimento esterno non mi interessa
                {
                    _color[it->first] = GREY;
                    queue.push(it->first);
                }
            }
        }
        count++;
        _color[actualNode] = BLACK;
        queue.pop();

    }
    return count;
}

template<typename S, typename T, typename R>
void TGraph<S, T, R>::BFSinit() {

    typename map<S, T*>::iterator it;
    for (it = _nodes.begin(); it != _nodes.end(); it++) {
        _color[it->first] = WHITE;
    }
}

template<typename S, typename T, typename R>
bool TGraph<S, T, R>::deleteRelation(const S &id) {
    if (_adj.find(id) != _adj.end()) {
        _adj.erase(id);
        return true;
    }
    return false;
}

template<typename S, typename T, typename R>
bool TGraph<S, T, R>::purgeFromRelations(const string &id) {
    deleteRelation(id); //Cancello tutte le relazioni da quell'utente ad altri
    for (typename map<S, map<S, R>>::iterator it = _adj.begin(); it != _adj.end(); it++) {
        deleteRelation(it->first, id);
    }

}

template<typename S, typename T, typename R>
unsigned long TGraph<S, T, R>::getNodesNumber() {
    return _nodes.size();
}

template<typename S, typename T, typename R>
short TGraph<S, T, R>::getColor(const S &id) {
    return _color[id];
}

template<typename S, typename T, typename R>
void TGraph<S, T, R>::setColor(const S &id, short color) {
    _color[id] = color;
}

#endif //THEPROJECT_TGRAPH_H
