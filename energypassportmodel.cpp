#include "energypassportmodel.h"
#include "excelreport.h"
#include "treeitem.h"
#include <QFile>
#include <QTextStream>
#include <QLocale>
#include <xlsxdocument.h>
#include <xlsxworkbook.h>
#include <QStringBuilder>


EnergyPassportModel::EnergyPassportModel()
{

}

EnergyPassportModel::EnergyPassportModel(const QString dbname)
{
    ctx = new Dbctx("QSQLITE",dbname);

    QStringList headers;
       headers << Pakazatel::D_Name
               << Pakazatel::D_Unit
               << Pakazatel::D_NomValue
               << Pakazatel::D_CalcValue
               << Pakazatel::D_FactValue
               << Pakazatel::D_Calculated
               << Pakazatel::D_Numeration
               << Pakazatel::ID;


    //QFile file(":/menu_structure.txt");
    //file.open(QIODevice::ReadOnly);
    QList<Entity *> plist;
    QStringList filter;
    ctx->getPakazateli(plist,filter);
    m_treeModel = new TreeModel(headers,plist);
    //file.close();

    m_fragmentModel = new FragmentTableModel();
    ctx->initPakazatelModel();
}

EnergyPassportModel::~EnergyPassportModel()
{
    delete ctx;
}

double EnergyPassportModel::kompaktnost()
{
    if (m_treeModel == nullptr) return 0;

    double area = m_treeModel->getCalcValueByID(area_boundary);
    double volume = m_treeModel->getCalcValueByID(volume_heated_space);

    return area / volume;
}

double EnergyPassportModel::soprativlenieVozdukhProniknovenie(const double Gn, const double delta_P_x, const double delta_P)
{
    QString s; //placeholder
    double R =  soprativlenieVozdukhProniknovenie(Gn,delta_P_x,delta_P,&s);
    return R;
}

double EnergyPassportModel::soprativlenieVozdukhProniknovenie(const double Gn, const double delta_P_x, const double delta_P,QString *f)
{
    double R =  (1/Gn) * (pow((delta_P_x /delta_P),(2/3.0))) ;
    if (f != nullptr){
        QLocale russian;
        QString formula = "(1 / %0) * (%1/%2)^ 2/3";
        QString ss = formula.arg(
                    russian.toString(Gn,'f',2),
                    russian.toString(delta_P_x,'f',2),
                    russian.toString(delta_P,'f',2)
                    );
        (*f) = ss;
    }
    return R;
}

double EnergyPassportModel::udelniivesVozdukha(const double t){
    double y = 3463/(273 + t);

    return y;
}

double EnergyPassportModel::raznostDavlenie(const double k, QString *f)
{
    Pakazatel * p_skopost_veter = m_treeModel->getIndicatorByID(max_wind_velocity);
    Pakazatel * p_temp_int = m_treeModel->getIndicatorByID(temp_air_internal);
    Pakazatel * p_temp_ext = m_treeModel->getIndicatorByID(temp_air_external);
    Pakazatel * p_vysota_zdanya = m_treeModel->getIndicatorByID(vysota_zdaniya);

    double temp_int = p_temp_int->calcValue();
    double temp_ext = p_temp_ext->calcValue();

    double v = p_skopost_veter->calcValue();
    double y_ext = udelniivesVozdukha(temp_ext);
    double y_int = udelniivesVozdukha(temp_int);
    double H = p_vysota_zdanya->calcValue();

    double delta_P = k * H *(y_ext - y_int) +  0.03 * y_ext* v*v;

    if (f != nullptr){
        QLocale russian(QLocale::Russian);
        QString formula = "%0 * %1 * (%2 - %3) + 0.03 * (%4) * (%5)^2 ";
        QString ss = formula.arg(russian.toString(k,'f',2),
                                 russian.toString(H,'f',2),
                                 russian.toString(y_ext,'f',2),
                                 russian.toString(y_int,'f',2),
                                 russian.toString(y_ext,'f',2),
                                 russian.toString(v,'f',2));
        (*f) = ss;
    }

    delete p_skopost_veter;
    delete p_temp_int;
    delete p_temp_ext;
    delete p_vysota_zdanya;

    return delta_P;
}

double EnergyPassportModel::kolichestvoinfiltrvozdukh( QString *f )
{
    double Gn_ok = m_treeModel->getCalcValueByID(norm_vozdukh_pronisaemost_okon);
    double p_ok = raznostDavlenie(0.28);
    double R_ok = soprativlenieVozdukhProniknovenie(Gn_ok,p_ok,10,f);

    double Gn_dv = m_treeModel->getCalcValueByID(norm_vozdukh_pronisaemost_dver);
    double p_dv = raznostDavlenie(0.55);
    double R_dv = soprativlenieVozdukhProniknovenie(Gn_dv,p_dv,10,f);

    //A_ok and A_dv
    double A_dv = m_treeModel->getCalcValueByID(area_doors);
    double A_ok =  m_treeModel->getCalcValueByID(area_windows_balcony);


    double G_inf = ( A_ok / R_ok )* (pow((p_ok/10.0),0.666666667)) + (A_dv/R_dv) * sqrt(p_dv/10);

    if (f != nullptr){
        //Gинф = (Aок / Rок) · (DPок / 10)2/3 + (Aдв / Rдв · (DPдв / 10)1/2)
        QLocale russian;
        QString formula = "(%0/%1 ) * (%2/10)^ (2/3) + (%3/%4 * ( %5/10 ) ^ (1/2)";
        QString ss = formula.arg(
                    russian.toString(A_ok,'f',2),
                    russian.toString(R_ok,'f',2),
                    russian.toString(p_ok,'f',2),
                    russian.toString(A_dv,'f',2),
                    russian.toString(R_dv,'f',2),
                    russian.toString(p_dv,'f',2));
        (*f) = ss;
    }
    return G_inf;
}

double EnergyPassportModel::lventilyatsiV1(){
    //Для Жилых зданий
    Pakazatel * p_area_living_space = m_treeModel->getIndicatorByID(area_living_space);
    double area_living = p_area_living_space->calcValue();
    double l_vent = 3 * area_living;

    delete p_area_living_space;
    return l_vent;
}

double EnergyPassportModel::lventilyatsiV2(){
    //b)других жилых зданий

    double lvent1 = lVent1();
    double lvent2 = lVent2();

    double lvent = lvent1;
    if (lvent < lvent2) lvent = lvent2;    
    return lvent;
}

double EnergyPassportModel::lVent1(){
    double H_etazha = m_treeModel->getCalcValueByID(vysota_etazha);
    double area_living = m_treeModel->getCalcValueByID(area_living_space);

    double lvent1 = 0.35 * H_etazha * area_living;
    return lvent1;
}

double EnergyPassportModel::lVent2(){
    double zhitelej = m_treeModel->getCalcValueByID(kol_zhitelej);
    double lvent2 = 30 * zhitelej;
    return lvent2;
}

double EnergyPassportModel::lventilyatsi(EnergyPassportModel::TipZdaniya z_type){

    double Ap = m_treeModel->getCalcValueByID(area_living_space);
    double lvent = 0;

    switch(z_type){
        case EnergyPassportModel::type1 : lvent = 4 * Ap;
        case EnergyPassportModel::type2 : lvent = 5 * Ap;
        case EnergyPassportModel::type3 : lvent = 7 * Ap;
        case EnergyPassportModel::type4 : lvent = 10 * Ap;
        case EnergyPassportModel::type5 : lvent = lventilyatsiV1();
        case EnergyPassportModel::type6 : lvent = lventilyatsiV2();
    }    
    return lvent;
}

double EnergyPassportModel::rhoVozdukh(QString *f){     
    double temp_ext_avg = m_treeModel->getCalcValueByID(heating_period_temp_avg);;
    double rho_vozdukh = 353/(273 + temp_ext_avg);

    if (f != nullptr){
        (*f) = "";
        (*f) = "353 / (273 + " + QString::number(temp_ext_avg) + ")";
    }    
    return rho_vozdukh;
}

double EnergyPassportModel::kratnostvozdukhobmen()
{
    if (m_treeModel == nullptr) return 0;

    double n_v1 = kratnostvozdukhobmen_nV1();
    double n_v2 = kratnostvozdukhobmen_nV2() ;
    double n_v3 = kratnostvozdukhobmen_nV3();
    double n = 0;

    switch(z_type){
        case EnergyPassportModel::type1 :
        case EnergyPassportModel::type2 :
        case EnergyPassportModel::type3 :
    case EnergyPassportModel::type4 : {
        n = n_v2 + n_v3;
        break;
    }
    case EnergyPassportModel::type5 : {
        n = n_v1 + n_v3;
        break;
    }
    case EnergyPassportModel::type6 : {
        n = n_v1 + n_v2 + n_v3;
        break;
    }
    }
    return n;
}

double EnergyPassportModel::kratnostvozdukhobmen_nV1(){
    double V_otop = m_treeModel->getCalcValueByID(volume_heated_space);
    double B_v = m_treeModel->getCalcValueByID(coeff_volume_reduction);

    double lvent = lventilyatsi(m_tzdaniya);
    double n_v1 = lvent/ (B_v * V_otop );

    return n_v1;
}

double EnergyPassportModel::kratnostvozdukhobmen_nV2(){
    double V_otop = m_treeModel->getCalcValueByID(volume_heated_space);
    double B_v = m_treeModel->getCalcValueByID(coeff_volume_reduction);
    double rho_vozdukh = rhoVozdukh();
    double n_vent = 60;
    double n_info = 168;
    double G_inf = kolichestvoinfiltrvozdukh();

    double lvent = lventilyatsi(m_tzdaniya);
    double n_v2 = (((lvent * n_vent)/168) + (G_inf *0.8* n_info)/(168 * rho_vozdukh))/(B_v * V_otop) ;

    return n_v2;
}

double EnergyPassportModel::kratnostvozdukhobmen_nV3(){
    double V_otop = m_treeModel->getCalcValueByID(volume_heated_space);
    double B_v = m_treeModel->getCalcValueByID(coeff_volume_reduction);
    double rho_vozdukh = rhoVozdukh();
    double n_info = 168;
    double G_inf = kolichestvoinfiltrvozdukh();

    double n_v3 = ((G_inf * n_info)/(168 * rho_vozdukh))/(B_v * V_otop);

    return n_v3;
}
double EnergyPassportModel::udelnayateplozashita(){

    double V_otop = m_treeModel->getCalcValueByID(volume_heated_space);
    QList<Fragment *> frags = m_fragmentModel->getFragmentList();
    double total = totalCalcTeploZashita(frags);

    double res = (1/V_otop) * total;

    return res;
}

double EnergyPassportModel::udelnayateplozashitaRaschet()
{   
    double V_otop = m_treeModel->getCalcValueByID(volume_heated_space);
    double GSOP = raschetGSOP();

    double res ;

    if (V_otop > 960){
        res = (0.16 + (10/sqrt(V_otop)))/((0.00013*GSOP) + 0.61);
    }else{
        res = ((4.74)/(((0.00013*GSOP) + 0.61))) * (1 /(pow(V_otop,1/3.0)));
    }

    return res;
}

double EnergyPassportModel::udelnayaventilyatsii()
{
    double k_eff = m_treeModel->getCalcValueByID(coeff_recuperation);
    double B_v = m_treeModel->getCalcValueByID(coeff_volume_reduction);
    double n = kratnostvozdukhobmen();

    double K_vent = 0.28 * B_v * n * (1 - k_eff);

    return K_vent;
}

double EnergyPassportModel::udelnayabwitavayatenlovyidelenie()
{
    if (m_treeModel == nullptr) return 0;

    double q_byit = m_treeModel->getCalcValueByID(thermal_E_building);
    double a_jiloi = m_treeModel->getCalcValueByID(area_living_space);
    double V_otop = m_treeModel->getCalcValueByID(volume_heated_space);
    double t_int = m_treeModel->getCalcValueByID(temp_air_internal);
    double t_ext = m_treeModel->getCalcValueByID(heating_period_temp_avg);

    double K_byit = (q_byit * a_jiloi)/(V_otop * (t_int - t_ext));   

    return K_byit;
}

inline double EnergyPassportModel::round(double n , unsigned d){
    return floor( n * pow(10.  ,d) + 0.5) / pow(10.,d);
}
EnergyPassportModel::TipZdaniya EnergyPassportModel::tzdaniya() const
{
    return m_tzdaniya;
}

void EnergyPassportModel::setTzdaniya(const EnergyPassportModel::TipZdaniya &tzdaniya)
{
    m_tzdaniya = tzdaniya;
}

void EnergyPassportModel::newFragment()
{
    m_fragmentModel->newFragment();
}

void EnergyPassportModel::removeFragment(int row)
{
    if (row >= 0 && row < m_fragmentModel->rowCount() ){
        m_fragmentModel->removeRow(row);
    }
}


double EnergyPassportModel::raschetGSOP()
{

    double temp_int = m_treeModel->getCalcValueByID(temp_air_internal);
    double temp_ext_avg = m_treeModel->getCalcValueByID(heating_period_temp_avg);
    double otop_period = m_treeModel->getCalcValueByID(heating_period_days);
    double GSOP = (temp_int - temp_ext_avg)* otop_period;    

    return round(GSOP,0);
}

double EnergyPassportModel::koeffOtlichieVnutrVneshTemp(const double tnorm)
{
    double temp_int = m_treeModel->getCalcValueByID(temp_air_internal);
    double temp_ext_avg = m_treeModel->getCalcValueByID(heating_period_temp_avg);

    double res = (tnorm - temp_ext_avg)/(temp_int - temp_ext_avg);    
    return res;
}

double EnergyPassportModel::subCalcTeploZashita(const double tnorm,const double area, const double rprev)
{
    double n_ij = koeffOtlichieVnutrVneshTemp(tnorm);
    double res = n_ij * (area/rprev);

    return res;
}

double EnergyPassportModel::totalCalcTeploZashita(QList<Fragment *> fragments)
{
    double res = 0.0,sub_res;
    double tnorm,area,rprev;
    Fragment *f;
    for(int i=0;i < fragments.count() ; i++){
        f = fragments[i];
        tnorm = f->tnorm();
        area = f->area();
        rprev = f->resistance();

        sub_res = subCalcTeploZashita(tnorm,area,rprev);
        res += sub_res;
    }

    return res;
}

void EnergyPassportModel::saveTreeModeltoDB()
{
    QList<TreeItem*> items;    
    m_treeModel->getIndicators(items);

    ctx->startTransaction();
    for(int i = 0; i < items.count();i++){
        Pakazatel *c = m_treeModel->getIndicatorByID(items[i]->data(7).toInt());
        if (c == nullptr) continue;
        ctx->insertPakazatel(c);
        delete c;
    }
    ctx->endTransaction();
}

void EnergyPassportModel::saveModelDatatoFile(const QString fname)
{
    QList<TreeItem*> list;
    m_treeModel->getIndicators(list);
    QLocale russian(QLocale::Russian);

    QFile file(fname);
    if (file.open(QIODevice::ReadWrite)){
        QTextStream out(&file);        
        for (int i = 0 ; i < list.count(); i++){
            TreeItem *t = list[i];
            //t not null(exists) and is a leaf on tree (no child elements)
            if (t != nullptr){
                //find out if leaf is calculated
                QVariant val = t->data(7);
                QString p = "--";
                QVariant cal = t->data(3) ;
                if (t->parent() != nullptr) p = t->parent()->data(0).toString();
                out <<Pakazatel::D_EntityName<<";"<< val.toInt() <<";"<<russian.toString(cal.toDouble())<<"\n";
            }
        }

        for (int i = 0; i < m_fragmentModel->rowCount();i++){
            out << Fragment::D_EntityName<<";";
            Fragment *f = m_fragmentModel->getFragment(i) ;//= fmodel->data(i,j);
            if (f == nullptr) continue;
           for (int j= 0; j < m_fragmentModel->columnCount(); j++){
              QVariant item = f->data(j);
               switch(j){                
                case 0:{
                   //section
                   out<<item.toString()<<";";
                   break;
               }
                default:{
                   //Tnorm / Area / Resistance
                   out<<russian.toString(item.toDouble())<<";";
                   break;
               }
               }
           }
           out<<"\n";
        }


    }
}

void EnergyPassportModel::loadModelDataFromFile(const QString fname)
{
    QList<TreeItem*> list;
    m_treeModel->getIndicators(list);
    //RegEx to check if string is composed of digits
    QLocale russian(QLocale::Russian);

    QFile file(fname);
    if (file.open(QIODevice::ReadOnly)){
        QTextStream in(&file);
        QString line;
        //remove all present rows in fragment
        m_fragmentModel->removeRows(0,m_fragmentModel->rowCount());
        while (!in.atEnd()){
            line = in.readLine();
            QStringList parts = line.split(QString(";"));
            if (parts[0] == Pakazatel::D_EntityName){
                Pakazatel p;
                QString calcval = parts[2];
                QString id = parts[1];
                bool isnum;
                double dcalcval = russian.toDouble(calcval,&isnum);
                if (isnum){
                    p.setCalcValue(dcalcval);
                    //p.setCalculated(false);
                    p.setId(id.toInt());
                    m_treeModel->setIndicatorByID(&p);
                }
                else continue;                
            }
            if (parts[0] == Fragment::D_EntityName){
                int last = m_fragmentModel->rowCount();
                m_fragmentModel->insertRows(last,1);
                Fragment *f = m_fragmentModel->getFragment(last);
                QString section = parts[1];
                double tnorm  = russian.toDouble(parts[2]);
                double area = russian.toDouble(parts[3]);
                double resistance = russian.toDouble(parts[4]);
                f->setSection(section);
                f->setArea(area);
                f->setResistance(resistance);
                f->setTnorm(tnorm);
            }
        }
    }
}

double EnergyPassportModel::Q_year_rad(){

    double t_1ok = m_treeModel->getCalcValueByID(coeff_proniknovenie_sontse_okno);
    double t_2ok = m_treeModel->getCalcValueByID(coeff_zatenenie_okno);
    //double t_1fon = 0;
    //double t_2fon = 0;
    double A_ok1 = m_treeModel->getCalcValueByID(area_facing_N);
    double A_ok2 = m_treeModel->getCalcValueByID(area_facing_NE);
    double A_ok3 = m_treeModel->getCalcValueByID(area_facing_E);
    double A_ok4 = m_treeModel->getCalcValueByID(area_facing_SE);
    double A_ok5 = m_treeModel->getCalcValueByID(area_facing_S);
    double A_ok6 = m_treeModel->getCalcValueByID(area_facing_SW);
    double A_ok7 = m_treeModel->getCalcValueByID(area_facing_W);
    double A_ok8 = m_treeModel->getCalcValueByID(area_facing_NW);



    double I_1 = m_treeModel->getCalcValueByID(sontse_vert_rad_avg_N);
    double I_2 = m_treeModel->getCalcValueByID(sontse_vert_rad_avg_NE);
    double I_3 = m_treeModel->getCalcValueByID(sontse_vert_rad_avg_E);
    double I_4 = m_treeModel->getCalcValueByID(sontse_vert_rad_avg_SE);
    double I_5 = m_treeModel->getCalcValueByID(sontse_vert_rad_avg_S);
    double I_6 = m_treeModel->getCalcValueByID(sontse_vert_rad_avg_SW);
    double I_7 = m_treeModel->getCalcValueByID(sontse_vert_rad_avg_W);
    double I_8 = m_treeModel->getCalcValueByID(sontse_vert_rad_avg_NW);

    double Q_rad = t_1ok * t_2ok * (A_ok1 * I_1 + A_ok2 * I_2 + A_ok3 * I_3 + A_ok4* I_4 +
                                  A_ok5 * I_5 + A_ok6 * I_6 + A_ok7 * I_7 + A_ok8* I_8) ;
    //+ t_1fon * t_2fon *A_fon*I_gor
    return Q_rad;
}


double EnergyPassportModel::udelnayatenlopostunleniesontse()
{
    if (m_treeModel == nullptr) return 0;    

    double V_otop = m_treeModel->getCalcValueByID(volume_heated_space);

    double Q_rad = Q_year_rad() ;

    double GSOP = raschetGSOP();
    double K_rad = (11.6 * Q_rad)/(V_otop * GSOP);    

    return K_rad;
}

double EnergyPassportModel::koeffsnijenieteplopastuplenia()
{
    //ОКРУГЛ(0,7+0,000025*('Эн паспорт 1'!E31-1000);2)
    double GSOP = raschetGSOP();
    double res = (0.7 + (0.000025 * (GSOP - 1000)));
    return res;
}

double EnergyPassportModel::udelniraskhodteplovoienergii()
{
    double K_ob = udelnayateplozashita();
    double K_vent = udelnayaventilyatsii();
    double K_bwit = udelnayabwitavayatenlovyidelenie();
    double K_rad = udelnayatenlopostunleniesontse();
    double n = koeffsnijenieteplopastuplenia();
    double Z = m_treeModel->getCalcValueByID(coeff_auto_reg);
    double X = m_treeModel->getCalcValueByID(coeff_reduction);
    double bh = m_treeModel->getCalcValueByID(coeff_additional);

    //qрот=[Kоб + Квент - (Кбыт + Крад) * n * z] * (1 - x) * bh
    double q_rot = (K_ob + K_vent - ((K_bwit + K_rad) * n * Z)) * ((1 - X) * bh);

    return q_rot;

}

double EnergyPassportModel::udelniiraskhodzaotopperiod()
{
    //q=Qгодот/Аот

    double A_ot =  m_treeModel->getCalcValueByID(area_all_floors);
    double q_godrot = raskhodzaotopperiod();
    double q = q_godrot / A_ot ;

    return q ;
}

double EnergyPassportModel::raskhodzaotopperiod()
{
    //Qгодот = 0,024*ГСОП*Vот*qрот    

    double GSOP = raschetGSOP();
    double V_otop = m_treeModel->getCalcValueByID(volume_heated_space);
    double q_rot = udelniraskhodteplovoienergii();

    double Q_godot = (0.024 * GSOP * V_otop * q_rot)/10 ;

    return Q_godot;

}

double EnergyPassportModel::obshieteplopoteriizaperiod()
{
    //Qгодобщ = 0,024*ГСОП*Vот*(Коб+Квент)    

    double GSOP = raschetGSOP();
    double V_otop = m_treeModel->getCalcValueByID(volume_heated_space);
    double K_ob = udelnayateplozashita();
    double K_vent = udelnayaventilyatsii();

    double Q_god_obsh = (0.024 * GSOP * V_otop * (K_ob + K_vent))/10;

    return Q_god_obsh;

}

void EnergyPassportModel::raschetPakazateli()
{
    //TO coeff osteklenie not known
    //Add it after asking
    Pakazatel p;
    p.setCalculated(true);

    //GSOP
    p.setId(heating_period_degree_days);
    p.setCalcValue(raschetGSOP());
    m_treeModel->setIndicatorByID(&p);


    //koeff kompaknost
    p.setId(coeff_compactness);
    p.setCalcValue(kompaktnost());
    m_treeModel->setIndicatorByID(&p);

    //kratnoist vozdukhoobmen
    p.setId(ventilation_cycle);
    p.setCalcValue(kratnostvozdukhobmen());
    m_treeModel->setIndicatorByID(&p);

    //udelnaya teplozashita
    p.setId(specific_Thermal_Protection);
    p.setCalcValue(udelnayateplozashita());
    m_treeModel->setIndicatorByID(&p);

    //udelnaya ventilyatsionaya charactistics
    p.setId(specific_Ventilation_Characteristics);
    p.setCalcValue(udelnayaventilyatsii());
    m_treeModel->setIndicatorByID(&p);

    //udelnaya bwitovaya teplovidelenie
    p.setId(specific_Thermal_Emission);
    p.setCalcValue(udelnayabwitavayatenlovyidelenie());
    m_treeModel->setIndicatorByID(&p);

    //udelnaya teplopactuplenie
    p.setId(specific_Solar_Reception);
    p.setCalcValue(udelnayatenlopostunleniesontse());
    m_treeModel->setIndicatorByID(&p);

    //coeff snijenie isplolzovanie teplopotreblenie
    p.setId(coeff_snijenie_teplopostuplenie);
    p.setCalcValue(koeffsnijenieteplopastuplenia());
    m_treeModel->setIndicatorByID(&p);

    //raschet udelnii raskhod thermal energy
    p.setId(thermal_usage_calc);
    p.setCalcValue(udelniraskhodteplovoienergii());
    m_treeModel->setIndicatorByID(&p);

    //udelnii raskhod thermal energy for period
    p.setId(thermal_usage_spec_heating_season);
    p.setCalcValue(udelniiraskhodzaotopperiod());
    m_treeModel->setIndicatorByID(&p);

    //raskhod teplovoi energii za otopitel'nii period
    p.setId(thermal_usage_calc_heating_season);
    p.setCalcValue(raskhodzaotopperiod());
    m_treeModel->setIndicatorByID(&p);

    //obshie teplopaterya zdaniya
    p.setId(thermal_wastage_heating_season);
    p.setCalcValue(obshieteplopoteriizaperiod());
    m_treeModel->setIndicatorByID(&p);

}

void EnergyPassportModel::writeXlsReport(const QString template_, const QString output_){

    QHash<QString,QString> addr;
    QXlsx::Document doc_t(template_);
    QXlsx::Workbook *wb = doc_t.workbook();
    QXlsx::Worksheet *ws =(QXlsx::Worksheet *) wb->sheet(0);

    ExcelReport util;

    util.linkAddrrToVal(ws,addr);

    Pakazatel *p;
    double val;
    QString f;

    //writing values to xlsx report
    //val-1 Bv
    p = m_treeModel->getIndicatorByID(coeff_volume_reduction);
    ws->write(addr[util.B_v],p->calcValue());
    delete p;

    //val-2 val-3
    //function return formula
    val = rhoVozdukh(&f);
    ws->write(addr[util.rho_vent_f],f);
    ws->write(addr[util.rho_vent],val);

    //val-4
    p = m_treeModel->getIndicatorByID(volume_heated_space);
    ws->write(addr[util.V_otop],p->calcValue());
    delete p;

    //val-5
    val = lventilyatsi(m_tzdaniya);
    ws->write(addr[util.L_vent],val);

    //val-6
    val = 168;
    ws->write(addr[util.n_inf],val);

    //val-7
    p = m_treeModel->getIndicatorByID(area_windows_balcony);
    ws->write(addr[util.A_ok],p->calcValue());
    delete p;

    //val-8
    p = m_treeModel->getIndicatorByID(area_doors);
    ws->write(addr[util.A_dv],p->calcValue());
    delete p;

    //val-9 val-10
    val = raznostDavlenie(0.55,&f);
    ws->write(addr[util.Delta_P_dv_f],f);
    ws->write(addr[util.Delta_P_dv],val);

    //val-11 val-12
    val = raznostDavlenie(0.28,&f);
    ws->write(addr[util.Delta_P_ok_f],f);
    ws->write(addr[util.Delta_P_ok],val);

    //val-13
    p =  m_treeModel->getIndicatorByID(temp_air_external);
    val = udelniivesVozdukha(p->calcValue());
    ws->write(addr[util.gamma_naruj],val);
    delete p;

    //val-14
    p = m_treeModel->getIndicatorByID(temp_air_internal);
    val = udelniivesVozdukha(p->calcValue());
    ws->write(addr[util.gamma_vnutr],val);
    delete p;

    //val-15
    p = m_treeModel->getIndicatorByID(vysota_zdaniya);
    ws->write(addr[util.H],p->calcValue());

    //val-16
    p = m_treeModel->getIndicatorByID(max_wind_velocity);
    ws->write(addr[util.v],p->calcValue());

    //val - 17 val- 18 okno
    p = m_treeModel->getIndicatorByID(norm_vozdukh_pronisaemost_okon);
    double Gn_ok = p->calcValue();
    double p_ok = raznostDavlenie(0.28);
    val = soprativlenieVozdukhProniknovenie(Gn_ok,p_ok,10,&f);
    ws->write(addr[util.R_tr_ok_f],f);
    ws->write(addr[util.R_tr_ok],val);
    delete p;

    //val-19 val-20 dver
    p = m_treeModel->getIndicatorByID(norm_vozdukh_pronisaemost_dver);
    double Gn_dv = p->calcValue();
    double p_dv = raznostDavlenie(0.55);
    val = soprativlenieVozdukhProniknovenie(Gn_dv,p_dv,10,&f);
    ws->write(addr[util.R_tr_dv_f],f);
    ws->write(addr[util.R_tr_dv],val);
    delete p;

    //val 21
    val = Gn_ok;
    ws->write(addr[util.Gn_dv_bal],val);

    //val 22
    val = Gn_dv;
    ws->write(addr[util.Gn_dv_naruj],val);

    //val 23 val 24
    val = kolichestvoinfiltrvozdukh(&f);
    ws->write(addr[util.G_inf_f],f);
    ws->write(addr[util.G_inf],val);

    //val 25
    val = kratnostvozdukhobmen_nV1();
    ws->write(addr[util.n_vent1],val);

    //val 26
    val = lVent1();
    ws->write(addr[util.l_vent1],val);

    //val 27
    val = lVent2();
    ws->write(addr[util.l_vent2],val);

    //val 28
    val = lventilyatsi(m_tzdaniya);
    ws->write(addr[util.l_vent_chosen],val);

    //val 29
    val = kratnostvozdukhobmen_nV2();
    ws->write(addr[util.n_vent2],val);

    //val 30
    val = kratnostvozdukhobmen_nV3();
    ws->write(addr[util.n_vent3],val);

    //val 31
    val = kratnostvozdukhobmen();
    ws->write(addr[util.n_vent],val);

    //val 32
    val = udelnayaventilyatsii();
    ws->write(addr[util.K_vent],val);

    //val 33
    val = koeffsnijenieteplopastuplenia();
    ws->write(addr[util.coeff_v],val);

    //val 34
    val = m_treeModel->getCalcValueByID(coeff_auto_reg);
    ws->write(addr[util.coeff_sigma],val);

    //val 35
    val = m_treeModel->getCalcValueByID(coeff_additional);
    ws->write(addr[util.coeff_Bh],val);

    //val 36
    val = udelnayabwitavayatenlovyidelenie();
    ws->write(addr[util.K_bwit],val);

    //val 37
    val = udelnayatenlopostunleniesontse();
    ws->write(addr[util.K_rad],val);

    //val 38
    val = m_treeModel->getCalcValueByID(coeff_proniknovenie_sontse_okno);
    ws->write(addr[util.t1_ok],val);

    //val 39
    val = m_treeModel->getCalcValueByID(coeff_proniknovenie_sontse_fon);
    ws->write(addr[util.t1_fon],val);

    //val 40
    val = m_treeModel->getCalcValueByID(coeff_zatenenie_okno);
    ws->write(addr[util.t2_ok],val);

    //val 41
    val = m_treeModel->getCalcValueByID(coeff_zatenenie_fon);
    ws->write(addr[util.t2_fon],val);

    //val 42 - 49
    val = m_treeModel->getCalcValueByID(area_facing_N);
    ws->write(addr[util.A_ok1],val);

    val = m_treeModel->getCalcValueByID(area_facing_NE);
    ws->write(addr[util.A_ok2],val);

    val = m_treeModel->getCalcValueByID(area_facing_E);
    ws->write(addr[util.A_ok3],val);

    val = m_treeModel->getCalcValueByID(area_facing_SE);
    ws->write(addr[util.A_ok4],val);

    val = m_treeModel->getCalcValueByID(area_facing_S);
    ws->write(addr[util.A_ok5],val);

    val = m_treeModel->getCalcValueByID(area_facing_SW);
    ws->write(addr[util.A_ok6],val);

    val = m_treeModel->getCalcValueByID(area_facing_W);
    ws->write(addr[util.A_ok7],val);

    val = m_treeModel->getCalcValueByID(area_facing_NW);
    ws->write(addr[util.A_ok8],val);

    //val 50 - 57
    val = m_treeModel->getCalcValueByID(sontse_vert_rad_avg_N);
    ws->write(addr[util.I1],val);

    val = m_treeModel->getCalcValueByID(sontse_vert_rad_avg_NE);
    ws->write(addr[util.I2],val);

    val = m_treeModel->getCalcValueByID(sontse_vert_rad_avg_E);
    ws->write(addr[util.I3],val);

    val = m_treeModel->getCalcValueByID(sontse_vert_rad_avg_SE);
    ws->write(addr[util.I4],val);

    val = m_treeModel->getCalcValueByID(sontse_vert_rad_avg_S);
    ws->write(addr[util.I5],val);

    val = m_treeModel->getCalcValueByID(sontse_vert_rad_avg_SW);
    ws->write(addr[util.I6],val);

    val = m_treeModel->getCalcValueByID(sontse_vert_rad_avg_W);
    ws->write(addr[util.I7],val);

    val = m_treeModel->getCalcValueByID(sontse_vert_rad_avg_NW);
    ws->write(addr[util.I8],val);

    //val 58
    val = Q_year_rad();
    ws->write(addr[util.Q_year_rad],val);

    //val 59
    val = udelniraskhodteplovoienergii();
    ws->write(addr[util.q_raskh_otop],val);

    //val 60
    val = raskhodzaotopperiod();
    ws->write(addr[util.Q_year_otop],val);

    //val 61
    val = obshieteplopoteriizaperiod();
    ws->write(addr[util.Q_year_obshei],val);

    //val 62
    val = udelniiraskhodzaotopperiod();
    ws->write(addr[util.Q_year_otop_area],val);

    doc_t.saveAs(output_);

}

TreeModel *EnergyPassportModel::treeModel()
{
    return m_treeModel;
}

QSqlRelationalTableModel *EnergyPassportModel::pakazatelModel()
{
    return ctx->getPakazatelModel();
}

FragmentTableModel *EnergyPassportModel::fragmentModel()
{
    return m_fragmentModel;
}
















