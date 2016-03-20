#include "excelreport.h"
#include <QDebug>
#include <xlsxdocument.h>

ExcelReport::ExcelReport()
{

}

ExcelReport::~ExcelReport()
{

}

void ExcelReport::linkAddrrToVal(QXlsx::Worksheet *ws, QHash<QString, QString> &mapAddr)
{
    QString part = "val-";
    QString key;
    uint count = 1;
    for(int i = 1 ; i < 120; i++){
        key = part + QString::number(count);
        for (int j = 1; j < 15 ; j++){
            QVariant val = ws->read(i,j);
            if (key == val.toString()){
                QXlsx::CellReference c(i,j);
                QString addr = c.toString();
                qDebug() << "Key = " << key << "; addr = " << addr;
                mapAddr[key] = addr;
                count++;
                key = part + QString::number(count);
            }
        }
    }
}

