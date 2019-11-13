#ifndef VERTRAG_H
#define VERTRAG_H

#include <QString>
#include <QDate>
#include <QVariant>

#include "kreditor.h"
#include "dkdbhelper.h"

class Vertrag
{
public:
    Vertrag(int kId = -1, QString ken = "",
            double betrag =0., double wert =0., int zId =-1,
            QDate vd =QDate::currentDate(),
            bool tesa =true, bool aktiv =false,
            QDate startd =QDate(9999, 12, 31),
            QDate endd = QDate(9999, 12, 31))
            :
        id(-1), kreditorId(kId), kennung(ken),
        betrag(betrag), wert(wert), zinsId(zId),
        tesaurierend(tesa), active(aktiv),
        vertragsdatum(vd), startZinsberechnung(startd),
        laufzeitEnde(endd) {}
    // getter
    double Betrag() const { return betrag;}
    double Wert() const { return wert;}
    int KreditorId() const { return kreditorId;}
    QString Kennung() const {return kennung;}
    int ZinsId() const {return zinsId;}
    QDate Vertragsabschluss() const {return vertragsdatum;}
    QDate LaufzeitEnde() const {return laufzeitEnde;}
    bool Tesaurierend() const {return tesaurierend;}
    double Zinsfuss() const {return zinsFuss;}
    QDate StartZinsberechnung() const {return startZinsberechnung;}
    QString Vorname() const {return dkGeber.getValue("Vorname").toString();}
    QString Nachname() const {return dkGeber.getValue("Nachname").toString();}
    // setter
    void setVid(int i){ id = i;}

    // interface
    bool ausDb(int id, bool mitBelegdaten= false);
    bool verbucheNeuenVertrag();
    bool aktiviereVertrag(const QDate& aDate);
    // statics
    bool passivenVertragLoeschen();
    bool aktivenVertragLoeschen(const QDate& termin);
private:
    // helper
    bool BelegSpeichern(const int BArt, const QString& msg);
    int speichereNeuenVertrag() const;
    bool speichereBelegNeuerVertrag();
    void updateAusDb(){ausDb(id, true);}
    // data
    int id;
    int      kreditorId;
    QString kennung;
    double betrag;
    double wert;
    int zinsId;
    bool tesaurierend;
    bool active;
    QDate vertragsdatum;
    QDate startZinsberechnung;
    QDate laufzeitEnde;
    // Belegdaten
    double zinsFuss;
    Kreditor dkGeber;
    QString buchungsdatenJson;

};

#endif // VERTRAG_H