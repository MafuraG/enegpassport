#ifndef EXCELREPORT_H
#define EXCELREPORT_H

#include <QString>
#include <QHash>
#include <xlsxdocument.h>



class ExcelReport
{
public:
    ExcelReport();
    ~ExcelReport();

   QString B_v = "val-1";
   QString rho_vent_f = "val-2";
   QString rho_vent = "val-3";
   QString V_otop = "val-4";
   QString L_vent = "val-5";
   QString n_inf = "val-6";
   QString A_ok = "val-7";
   QString A_dv = "val-8";
   QString Delta_P_ok_f = "val-9";
   QString Delta_P_ok = "val-10";
   QString Delta_P_dv_f = "val-11";
   QString Delta_P_dv = "val-12";
   QString gamma_naruj = "val-13";
   QString gamma_vnutr = "val-14";
   QString H = "val-15";
   QString v = "val-16";
   QString R_tr_ok_f = "val-17";
   QString R_tr_ok = "val-18";
   QString R_tr_dv_f = "val-19";
   QString R_tr_dv = "val-20";
   QString Gn_dv_bal = "val-21";
   QString Gn_dv_naruj = "val-22";
   QString G_inf_f = "val-23";
   QString G_inf = "val-24";
   QString n_vent1= "val-25";
   QString l_vent1 = "val-26";
   QString l_vent2 = "val-27";
   QString l_vent_chosen = "val-28";
   QString n_vent2 = "val-29";
   QString n_vent3 = "val-30";
   QString n_vent = "val-31";
   QString K_vent = "val-32";
   QString coeff_v = "val-33";
   QString coeff_sigma = "val-34";
   QString coeff_Bh = "val-35";
   QString K_bwit = "val-36";
   QString K_rad = "val-37";
   QString t1_ok = "val-38";
   QString t1_fon = "val-39";
   QString t2_ok= "val-40";
   QString t2_fon = "val-41";
   QString A_ok1 = "val-42";
   QString A_ok2 = "val-43";
   QString A_ok3 = "val-44";
   QString A_ok4 = "val-45";
   QString A_ok5 = "val-46";
   QString A_ok6 = "val-47";
   QString A_ok7 = "val-48";
   QString A_ok8 = "val-49";
   QString I1 = "val-50";
   QString I2 = "val-51";
   QString I3 = "val-52";
   QString I4 = "val-53";
   QString I5 = "val-54";
   QString I6 = "val-55";
   QString I7 = "val-56";
   QString I8 = "val-57";

   QString Q_year_rad = "val-58";
   QString q_raskh_otop = "val-59";
   QString Q_year_otop = "val-60";
   QString Q_year_otop_area = "val-62";

   void linkAddrrToVal(QXlsx::Worksheet *ws, QHash<QString,QString> &mapAddr );

};

#endif // EXCELREPORT_H
