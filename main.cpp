#include <iostream>
#include "User.h"
#include "StandardUsr.h"
#include "BusinessUsr.h"
#include "GroupUsr.h"
#include "Date.h"
#include "TGraph.hpp"
#include "Relation.h"
#include "DataSet.h"

using namespace std;
int main(int argc, char* argv[]) {

    DataSet dataSet; // classe per utilizzare i metodi del data
    string username, username2, content, ID;
    string f1, f2, f3;
    int cmd0, cmd2, cmd3, cmd4;

    if (argc != 4) {
        cerr << "Numero di argomenti passati non sufficiente. Inserire i 3 file." << endl;
        return -1;
    }

    cout << "FILE UTENTI... ";
    if (!dataSet.loadUsers(argv[1])) {
        return -2;
    } else cout << "Ok." << endl;

    cout << "FILE RELAZIONI... ";

    if (!dataSet.loadRelations(argv[2])) {
        return -3;
    } else cout << "Ok." << endl;

    cout << "FILE POST... ";
    if (!dataSet.loadPosts(argv[3])) {
        return -4;
    } else cout << "Ok." << endl << endl;
    //dataSet.overview();


    do {
        cout << endl << endl;
        cout << "**********************************"<<endl;
        cout << "MAIN MENU" << endl;//Segnare così i sottomenu completi
        cout << "1) Esporta dati." << endl;
        cout << "2) Modifica Set di Dati." << endl;
        cout << "3) Funzioni statistiche." << endl;
        cout << "4) Funzioni di ricerca." << endl;
        cout << "5) Panoramica." << endl;
        cout << "6) Visualizza utente." << endl;
        cout << "7) Esci." << endl;
        cmd0 = basicGetInt();
        cout << endl << endl;

        switch (cmd0) {
            case (1) : //ESPORTA
                cout << "ESPORTA - Inserisci i file" << endl;
                cout << "File Utenti: ";
                getline(cin,f1);
                cout << "File Relazioni: ";
                getline(cin,f2);
                cout << "File Post: ";
                getline(cin,f3);
                if (dataSet.exportUsers("export/"+f1) && dataSet.exportRelations("export/"+f2) && dataSet.exportPosts("export/"+f3)) {
                    cout << "Esportazione riuscita." << endl;
                } else cout << "Errore" << endl;
                wait();
                break;

            case (2): //MODIFICA
                do {
                    string type, s1, s2, s3, s4, s5, s6;
                    short relation;
                    int postId;
                    Relation rel;
                    Date date;

                    cout <<endl<<endl<< "**********************************"<<endl;
                    cout << "MENU DI MODIFICA" << endl;
                    cout << "1) Aggiungi Utente." << endl
                         << "2) Rimuovi Utente." << endl
                         << "3) Aggiunta relazione." << endl
                         << "4) Rimozione Relazione" << endl
                         << "5) Aggiunta post." << endl
                         << "6) Rimozione post." << endl
                         << "7) Modifica utente." << endl
                         << "8) Modifica relazione." << endl
                         << "9) Modifica post." << endl
                         <<"10) Indietro." << endl;
                    cmd2 = basicGetInt();

                    switch (cmd2) {
                        case 1://AGGIUNGI USER
                            cout << "Inserisci il nuovo utente" << endl;
                            cout << "Username" << endl;
                            getline(cin,username);
                            cout << "Tipo di utente (S,B,G)" << endl;
                            getline(cin,type);
                            if (type == "S" || type == "s") {
                                cout << "Inserisci Nome" << endl;
                                getline(cin,s1);
                                cout << "Inserisci Cognome" << endl;
                                getline(cin,s2);
                                cout << "Inserisci Sesso" << endl;
                                getline(cin,s3);
                                cout << "Inserisci Data di nascita" << endl;
                                getline(cin,s4);
                                cout << "Inserisci Indirizzo" << endl;
                                getline(cin,s5);
                            } else if (type == "G" || type == "g") {
                                cout << "Inserisci Nome" << endl;
                                getline(cin,s1);
                                cout << "Inserisci Attività" << endl;
                                getline(cin,s2);
                                cout << "Inserisci Descrizione" << endl;
                                getline(cin,s3);
                                cout << "Inserisci Data" << endl;
                                getline(cin,s4);
                                cout << "Inserisci Sede" << endl;
                                getline(cin,s5);

                            } else if (type == "B" || type == "b") {
                                cout << "Inserisci Nome" << endl;
                                getline(cin,s1);
                                cout << "Inserisci Prodotto" << endl;
                                getline(cin,s2);
                                cout << "Inserisci Fatturato" << endl;
                                getline(cin,s3);
                                cout << "Inserisci Indirizzo" << endl;
                                getline(cin,s4);
                                cout << "Inserisci Sede" << endl;
                                getline(cin,s5);
                                cout << "Inserisci Data di Fondazione" << endl;
                                getline(cin,s6);
                            }
                            if (!dataSet.addUser(username, type, s1, s2, s3, s4, s5, s6)) {
                                cerr << "Errore, utente non inserito." << endl;
                            } else {
                                cout << "Utente " << username << " inserito con successo." << endl;
                            }
                            wait();
                            break;
                        case 2://RIMUOVI USER
                            cout << "Inserisci lo username dell'utente da rimuovere" << endl;
                            getline(cin,username);
                            if (!dataSet.deleteUser(username)) {
                                cerr << "Utente non esistente." << endl;
                            } else {
                                cout << "Utente " << username << " eliminato con successo." << endl;
                            }
                            wait();
                            break;
                        case 3://ADD RELATION
                            cout << "Inserisci i due utenti in ordine" << endl;
                            getline(cin,username);
                            getline(cin,username2);

                            do {
                                cout << "Scegli fra le relazioni disponibili" << endl;
                                cout << "0) Conoscente" << endl
                                     << "1) Amico" << endl
                                     << "2) Coniuge/Convivente" << endl
                                     << "3) Genitore" << endl
                                     << "4) Figlio" << endl
                                     << "5) Consociata (Tra aziende)" << endl
                                     << "6) Impiegato (Utente->Azienda)" << endl
                                     << "7) Membro del Gruppo (Utente<->Gruppo)" << endl;
                                relation = (short)basicGetInt();
                            } while (relation > 7 || relation < 0);
                            cout << "Inserire una data per la relazione (gg/mm/yyyy) o lasciare vuoto" << endl;
                            getline(cin,s1);
                            if (date.setDate(s1))
                                rel.date = Date(s1);
                            rel.type = relation;
                            if (!dataSet.addRelation(username, username2, rel)) {
                                cerr << "Impossibile aggiungere la relazione" << endl;
                            } else {
                                cout << "Relazione aggiunta con successo" << endl;
                            }
                            wait();
                            break;
                        case 4: //DELETE RELATION
                            cout << "Inserisci i due username" << endl;
                            getline(cin,username);
                            getline(cin,username2);
                            if (!dataSet.deleteRelation(username, username2)) {
                                cerr << "Relazione non esistente" << endl;
                            } else {
                                cout << "Relazione eliminata con successo" << endl;
                            }
                            wait();
                            break;

                        case 5: //ADD POST
                            cout << "Inserire proprietario, contenuto e data del post: " << endl;
                            getline(cin,username);
                            getline(cin,content);
                            getline(cin,s1);
                            postId = dataSet.addPost(username, content, s1);
                            if (postId == -1)
                                cerr << "Utente non esistente" << endl;
                            else if (postId == -2)
                                cerr << "Formattazione errata" << endl;
                            else {
                                cout << "Questo è l'ID relativo al post: " << postId << endl;
                            }
                            wait();
                            break;

                        case 6:// DELETE POST
                            cout << "Inserire l'ID relativo al post da eliminare: " << endl;
                            getline(cin,ID);
                            if(!dataSet.deletePost(ID))
                                cout<<"Impossibile eliminare il post"<<endl;
                            else
                                cout<<"Post eliminato con successo"<<endl;
                            wait();
                            break;

                        case 7: //EDIT USERNAME
                            cout << "Hai scelto di modificare un utente. Inserire il relativo username: " << endl;
                            getline(cin,username);
                            if(!dataSet.editUser(username))
                                cout<<"Impossibile modificare l'utente"<<endl;
                            else
                                cout<<"Utente modificato con successo"<<endl;
                            wait();
                            break;
                        case 8: //EDIT RELATION
                            cout << "Inserire i due username: " << endl;
                            getline(cin,username);
                            getline(cin,username2);
                            do {
                                cout << "Scegli fra le relazioni disponibili" << endl;
                                cout << "0) Conoscente" << endl
                                     << "1) Amico" << endl
                                     << "2) Coniuge/Convivente" << endl
                                     << "3) Genitore" << endl
                                     << "4) Figlio" << endl
                                     << "5) Consociata (Tra aziende)" << endl
                                     << "6) Impiegato (Utente->Azienda)" << endl
                                     << "7) Membro del Gruppo (Utente<->Gruppo)" << endl;
                                relation = (short) basicGetInt();
                            } while (relation > 7 || relation < 0);
                            rel.type = relation;
                            if(dataSet.deleteRelation(username,username2) && dataSet.addRelation(username,username2,rel))
                                cout<<"Relazione modificata con successo"<<endl;
                            else
                                cout<<"Impossibile modificare la relazione"<<endl;
                            wait();
                            break;
                        case 9://EDIT POST
                            cout << "Inserire l'ID del post da modificare" << endl;
                            getline(cin,ID);
                            if(!dataSet.editPost(ID))
                                cout<<"Impossibile modificare post"<<endl;
                            else
                                cout<<"Post modificato con successo"<<endl;
                            wait();
                            break;
                        default:
                            if(cmd2!=10)
                                cout << "Inserire un numero valido."<<endl;
                            break;
                    }
                } while (cmd2 != 10);
                break; //END MODIFICA DATI (2)


            case (3): //METODI STATISTICI
                do {
                    Date date;
                    string str;
                    bool includeSubs;
                    list<string> relatives;

                    cout <<endl<<endl<< "**********************************"<<endl;
                    cout<< "FUNZIONI STATISTICHE"<<endl;
                    cout << " 1) Utenti totali, standard, business e gruppi." << endl
                         << " 2) Amici e parenti per ogni standard user." << endl
                         << " 3) Ricerca utenti nati dopo una certa data." << endl
                         << " 4) Numero dipendenti e consociate per ogni azienda" << endl
                         << " 5) Numero utenti all'interno di un gruppo." << endl
                         << " 6) Azienda con più dipendenti." << endl
                         << " 7) Post con più like." << endl
                         << " 8) Post con più dislike." << endl
                         << " 9) Utente con più amici." << endl
                         << "10) Utente con più conoscenze." << endl
                         << "11) Età media utenti standard." << endl
                         << "12) Indietro" << endl;
                    cmd3 = basicGetInt();
                    cout<<endl<<endl;
                    switch (cmd3) {

                        case 1:
                            cout << "Utenti totali: " << dataSet.getUsersCount() << endl;
                            cout << "Utenti standard: " << dataSet.getStandardUsrsCount() << endl;
                            cout << "Utenti business: " << dataSet.getBusinessUsrsCount() << endl;
                            cout << "Utenti gruppo: " << dataSet.getGroupUsrsCount() << endl;
                            wait();
                            break;
                        case 2:
                            cout << "Inserire l'utente di cui cercare amici e parentele: " << endl;
                            getline(cin,username);
                            if(dataSet.isStandard(username)) {
                                cout << username << " ha " << dataSet.getRelationNumber(username, FRIEND) << " amici, "
                                     << dataSet.getRelationNumber(username, ACQUAINTANCE) << " conoscenti e "
                                     << dataSet.getRelativesNumber(username) << " parenti." << endl;
                            }else{
                                cout<<"L'utente non è standard o non esiste"<<endl;
                            }
                            wait();
                            break;

                        case 3:
                            do {
                                cout << "Inserire la data in un formato valido" << endl;
                                getline(cin,str);
                            } while(!date.isParsable(str));
                            date = Date(str);
                            cout<<"Utenti nati dopo "<<str<<": "<< dataSet.getUsersBornAfter(date)<<endl;
                            wait();
                            break;

                        case 4:
                            cout << "Inserire l'username dell'azienda: " << endl;
                            getline(cin,username);
                            if(dataSet.isBusiness(username)) {
                                cout << "L'azienda ha " << dataSet.getRelationNumber(username, EMPLOYEE)
                                     << " dipendenti e " << dataSet.getRelationNumber(username, SUBSIDIARY)
                                     << " consociate." << endl;
                            } else{
                                cout<<"L'utente non è business o non esiste"<<endl;
                            }
                            wait();
                            break;

                        case 5:
                            cout << "Inserisci l'username di un gruppo" << endl;
                            getline(cin,username);
                            if(dataSet.isGroup(username))
                            {
                                cout<<"Il gruppo è composto da "<< dataSet.getRelationNumber(username, GROUPMEMBER)<<" utenti"<<endl;
                            }else{
                                cout<<"L'utente non è un gruppo o non esiste"<<endl;
                            }
                            wait();
                            break;

                        case 6:
                            cout<<"Includere le aziende consociate nel conteggio?";
                            includeSubs = basicYesNo();
                            dataSet.printBiggestCompany(includeSubs);
                            wait();
                            break;

                        case 7:
                            dataSet.printBestPost();
                            wait();
                            break;

                        case 8:
                            dataSet.printWorsePost();
                            wait();
                            break;

                        case 9:
                            dataSet.printUserMaxFriends();
                            wait();
                            break;

                        case 10:
                            dataSet.printUserMaxAcquientances();
                            wait();
                            break;
                        case 11:
                            cout << "Età media degli standard user è: " << dataSet.averageAge() << endl;
                            wait();
                            break;

                        default:
                            if(cmd3!=12)
                                cout << "Inserire un numero valido." << endl;
                            break;
                    }

                } while (cmd3 != 12);//STATS MENU
                break;

            case (4)://METODI DI RICERCA
                do {
                    bool bol1;
                    float rateo;
                    int val1,val2,val3,val4;

                    cout <<endl<<endl<< "**********************************"<<endl;
                    cout << "FUNZIONI DI RICERCA" << endl << endl;
                    cout << "1) Albero genealogico di un utente." << endl
                         << "2) Tutti gli alberi." << endl
                         << "3) Ricerca lupi solitari." << endl
                         << "4) Ricerca simpatia." << endl
                         << "5) Indietro" << endl;
                    cmd4 = basicGetInt();
                    cout<<endl<<endl;
                    switch (cmd4) {
                        case 1:
                            cout << "Inserire l'utente su cui costruire l'albero genealogico: " << endl;
                            getline(cin,username);
                            cout<<"Stampare su file?";
                            bol1 = basicYesNo();
                            if(dataSet.printFamilyTree(username,bol1));
                            else{
                                cout<<"Utente inesistente, non standard o privo di albero"<<endl;
                            }
                            wait();
                            break;

                        case 2:
                            cout<<"Stampare su file?";
                            bol1 = basicYesNo();
                            dataSet.printAllFamilyTrees(bol1);
                            wait();
                            break;

                        case 3:
                            cout<<"Definisci i parametri di un \"lupo solitario\""<<endl;
                            cout<<"Inserisci il numero massimo di Relazioni con utenti."<<endl;
                            val1=basicGetInt();
                            cout<<"Inserisci il numero massimo di Gruppi a cui appartiene."<<endl;
                            val2=basicGetInt();
                            cout<<"Deve lavorare?";
                            bol1 = basicYesNo();
                            cout<<"Numero massimo di interventi sulle news."<<endl;
                            val3=basicGetInt();
                            cout<<"Numero massimo di post scritti."<<endl;
                            val4=basicGetInt();
                            dataSet.printWolf(val1,val2,bol1,val3,val4);
                            wait();
                            break;

                        case 4:
                            cout<<"Inserisci la percentuale di apprezzamento"<<endl;
                            rateo = basicGetInt();
                            cout<<"Includere le consociate?";
                            bol1 = basicYesNo();
                            dataSet.printByAppreciacion(rateo/100, bol1);
                            wait();
                            break;

                        default:
                            if(cmd4 != 5)
                                cout << "Inserire un numero valido." << endl;
                            break;
                    }

                } while (cmd4 != 5);
                break;
            case 5:
                dataSet.overview();
                wait();
                break;
            case 6:
                cout<<"Inserisci username"<<endl;
                getline(cin,username);
                dataSet.printDetails(username);
                wait();
                break;
            default:
                if(cmd0!=7)
                    cout << "Inserire un numero valido."<<endl;
                break;
        }
    } while (cmd0 != 7);
    return 0;
}
