#ifndef TST_DB_H
#define TST_DB_H
//

#include "QCoreApplication"

class tst_db : public QObject
{
    Q_OBJECT

public:
    tst_db() {}
    ~tst_db() {}

private:
    const QString filename = "..\\data\\testdb.sqlite";
    const QString testCon = "test_connection"; // "qt_sql_default_connection";
    int tableRecordCount(QString table);

private slots:
    //    void initTestCase();
    //    void cleanupTestCase();
    void init();
    void cleanup();
    void test_createSimpleTable();
    void test_createSimpleTable2();
    void test_SimpleTableAddData();
    void test_createSimpleTable_wRefInt();
    void test_createSimpleTable_wRefInt2();
    void test_addRecords_wDep();
    void test_deleteRecord_wDep();
};
#endif // TST_DB_H