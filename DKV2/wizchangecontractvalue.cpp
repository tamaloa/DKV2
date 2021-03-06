#include <QDebug>
#include <QVariant>

#include <QLabel>
#include <QLineEdit>
#include <QDateEdit>
#include <QRadioButton>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QMessageBox>

#include "helper.h"
#include "appconfig.h"
#include "helperfin.h"
#include "wizchangecontractvalue.h"

wpChangeContract_IntroPage::wpChangeContract_IntroPage(QWidget* parent) : QWizardPage(parent)
{
    setTitle(qsl("Ein- / Auszahlung"));
    QRadioButton* rbDeposit = new QRadioButton(qsl("Einzahlung"));
    QRadioButton* rbPayout = new QRadioButton(qsl("Auszahlung"));
    registerField("deposit_notPayment", rbDeposit);
    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(rbDeposit);
    layout->addWidget(rbPayout);
    setLayout(layout);
}

void wpChangeContract_IntroPage::initializePage()
{
    QString subtitle =qsl("In dieser Dialogfolge kannst Du Ein- oder Auszahlungen zum Vertrag <b>%1</b> von <b>%2</b> verbuchen.");
    wizChangeContract* wiz= qobject_cast<wizChangeContract*>(wizard());
    subtitle = subtitle.arg(wiz->contractLabel, wiz->creditorName);
    setSubTitle(subtitle);
}

bool wpChangeContract_IntroPage::validatePage()
{
    wizChangeContract* wiz= qobject_cast<wizChangeContract*>(this->wizard());

    double minContract =dbConfig::readValue(MIN_AMOUNT).toDouble();
    double minPayout   =dbConfig::readValue(MIN_PAYOUT).toDouble();
    double minAmountToMakeA_Payout = minContract + minPayout +1;
    QLocale l;
    if( ! field(qsl("deposit_notPayment")).toBool() && wiz->currentAmount < minAmountToMakeA_Payout) {
        QString msg(qsl("Die kleinste Einlage beträgt %1. Die kleinste Auszahlung beträgt %2. "
                    "Daher ist im Moment keine Auszahlung möglich.<p>Du kannst einen Einzahlung machen oder "
                    "über den entsprechenden Menüpunkt den Vertrag beenden"));
        msg = msg.arg(l.toCurrencyString(minContract), l.toCurrencyString(minPayout));
        QMessageBox::information(this, qsl("Keine Auszahlung möglich"), msg);
        return false;
    }
    return true;
}

wpChangeContract_AmountPage::wpChangeContract_AmountPage(QWidget* parent) : QWizardPage(parent)
{
    QVBoxLayout*  layout = new QVBoxLayout;
    QLineEdit* le = new QLineEdit;
    registerField(qsl("amount"), le);
    le->setValidator(new QIntValidator(this));
    layout->addWidget(le);
    setLayout(layout);
}

void wpChangeContract_AmountPage::initializePage()
{
    bool deposit = field(qsl("deposit_notPayment")).toBool();
    double minPayout =dbConfig::readValue(MIN_PAYOUT).toDouble();
    double minAmount =dbConfig::readValue(MIN_AMOUNT).toDouble();
    QLocale l;
    if( deposit) {
        setTitle(qsl("Einzahlungsbetrag"));
        QString subt =qsl("Gib den eingezahlten Betrag in ganzen Euro an. Der Betrag muss größer als %1 sein.");
        subt =subt.arg(l.toCurrencyString(minPayout));
        setSubTitle(subt);

        setField(qsl("amount"), 1000.);
    } else {
        setTitle(qsl("Auszahlungsbetrag"));
        wizChangeContract* wiz= qobject_cast<wizChangeContract*>(this->wizard());
        double currentAmount = wiz->currentAmount;
        // double minPayment = 100., minRemains = 500.;
        double maxPayout = currentAmount - minAmount;
        QLocale locale;
        QString subtitle =qsl("Der Auszahlungsbetrag kann zwischen %1 und %2 liegen.");
        subtitle = subtitle.arg(locale.toCurrencyString(minPayout), locale.toCurrencyString(maxPayout));
        setSubTitle(subtitle);
        setField(qsl("amount"), minPayout);
    }
}

bool wpChangeContract_AmountPage::validatePage()
{
    bool deposit = field("deposit_notPayment").toBool();
    double amount = r2(field("amount").toDouble());
    if( amount < 100)
        return false;
    setField("amount", amount);

    if( ! deposit) {
        wizChangeContract* wiz= qobject_cast<wizChangeContract*>(this->wizard());
        double currentAmount = wiz->currentAmount;
        double amount = field("amount").toDouble();
        // double minPayout = 100., minRemains = 500.
        if( amount <100 || (currentAmount-amount) <500)
            return false;
    }
    return true;
}

wpChangeContract_DatePage::wpChangeContract_DatePage(QWidget* parent) : QWizardPage(parent)
{
    QDateEdit* de = new QDateEdit;
    de->setDisplayFormat(qsl("dd.MM.yyyy"));
    registerField(qsl("date"), de);

    QVBoxLayout*  layout = new QVBoxLayout;
    layout->addWidget(de);
    setLayout(layout);
}

void wpChangeContract_DatePage::initializePage()
{
    wizChangeContract* wiz= qobject_cast<wizChangeContract*>(this->wizard());
    QString subt=QString(qsl("Das Datum muss nach der letzten Buchung zu diesem Vertrag (%1) liegen. ")).arg(wiz->earlierstDate.toString(qsl("dd.MM.yyyy")));

    bool deposit = field(qsl("deposit_notPayment")).toBool();
    if( deposit) {
        setTitle(qsl("Datum des Geldeingangs"));
        setSubTitle(subt + qsl("<p>Gib das Datum an, an dem das Geld auf unserem Konto gutgeschrieben wurde."));
    } else {
        setTitle(qsl("Überweisungsdatum"));
        setSubTitle(subt + qsl("<p>Gib das Datum ein, zu dem die Überweisung durchgeführt wird."));
    }
    setField(qsl("date"), wiz->earlierstDate);
}

bool wpChangeContract_DatePage::validatePage()
{
    wizChangeContract* wiz= qobject_cast<wizChangeContract*>(this->wizard());
    QDate d {field(qsl("date")).toDate()};
    if( d < wiz->earlierstDate)
        return false;
    if( d.month() == 1 && d.day() == 1)
        // avoid interest bookings on the date of anual settlements.
        // it is a holiday anyways
        return false;

    return true;
}

wpChangeContract_Summary::wpChangeContract_Summary(QWidget* p) : QWizardPage(p)
{
    setTitle(qsl("Zusammenfassung"));
    QCheckBox* cb = new QCheckBox(qsl("Die Eingaben sind korrekt!"));
    registerField(qsl("confirmed"), cb);
    QVBoxLayout* layout = new QVBoxLayout;
    layout-> addWidget(cb);
    setLayout(layout);
    connect(cb, SIGNAL(stateChanged(int)), this, SLOT(onConfirmData_toggled(int)));
}
void wpChangeContract_Summary::initializePage()
{
    wizChangeContract* wiz= qobject_cast<wizChangeContract*>(this->wizard());

    QString subtitle =qsl("zum Vertrag <b>%1</b><p>von <b>%2</b>:<p>"
                      "<table width=100%><tr><td align=center>Vorheriger Wert</td><td align=center>Änderungsbetrag</td><td align=center>neuer Wert</td></tr>"
                      "<tr><td align=center>%3</td><td align=center>%4%5</td><td align=center>%6</td></tr></table>"
                      "<p>Datum: %7</b>");
    bool deposit = field(qsl("deposit_notPayment")).toBool();
    double oldValue = wiz->currentAmount, newValue =0;
    double change = field(qsl("amount")).toDouble();
    if( deposit) {
        setTitle(qsl("Zusammenfassung der Einzahlung"));
        subtitle = qsl("Einzahlung ") +subtitle;
        newValue = wiz->currentAmount + change;
    } else {
        setTitle(qsl("Zusammenfassung der Auszahlung"));
        subtitle = qsl("Auszahlung ") +subtitle;
        newValue = wiz->currentAmount - change;
    }
    QLocale locale;
    setSubTitle(subtitle.arg(wiz->contractLabel, wiz->creditorName, locale.toCurrencyString(oldValue),
                   deposit? qsl("+") : qsl("-"), locale.toCurrencyString(change),
                   locale.toCurrencyString(newValue), field(qsl("date")).toDate().toString(qsl("dd.MM.yyyy"))));
}
bool wpChangeContract_Summary::isComplete() const
{
    return field("confirmed").toBool();
}
void wpChangeContract_Summary::onConfirmData_toggled(int)
{
    completeChanged();
}


wizChangeContract::wizChangeContract(QWidget* p) : QWizard(p)
{
    addPage(new wpChangeContract_IntroPage);
    addPage(new wpChangeContract_AmountPage);
    addPage(new wpChangeContract_DatePage);
    addPage(new wpChangeContract_Summary);
    QFont f = font(); f.setPointSize(10); setFont(f);
}
