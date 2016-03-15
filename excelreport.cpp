#include "excelreport.h"

#include <xlsxdocument.h>

ExcelReport::ExcelReport()
{

}

ExcelReport::~ExcelReport()
{

}

void ExcelReport::linkAddrrToVal(QXlsx::Worksheet *ws, QHash<QString, QString> &mapAddr)
{
    QString part = "Val-";
    QString key;
    uint count = 1;
    for(int i = 1 ; i < 120; i++){
        key = part + QString(count);
        for (int j ; j < 10 ; j++){
            QVariant val = ws->read(i,j);
            if (key == val.toString()){
                QXlsx::CellReference c(i,j);
                mapAddr[key] = c.toString();
                count++;
            }
        }
    }
}

