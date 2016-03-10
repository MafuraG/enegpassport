#ifndef ENERGYPASSPORTMODEL_H
#define ENERGYPASSPORTMODEL_H

#include "treemodel.h"
#include "dbctx.h"

#include <QString>
#include <QList>



class EnergyPassportModel
{
public:
    EnergyPassportModel();
    EnergyPassportModel(const QString dbname);
    ~EnergyPassportModel();
	//Общая информация
    unsigned int data_zapolnenya  = 21 ;//"Дата заполнения (число, м-ц, год)";
    unsigned int address_zdaniya = 48 ;//"Адрес здания";
    unsigned int razrabochik = 53; //"Разработчик проекта";
    unsigned int addr_tel_razrabotchika = 82; //"Адрес и телефон разработчика";
    unsigned int shriff_proekta = 7; //"Шифр проекта";
    unsigned int naznacheniye_zdaniya = 74; //"Назначение здания, серия";
    unsigned int etaj_kol_sektsii = 25; //"Этажность, количество секций";
    unsigned int kol_kvartir = 46; //"Количество квартир";
    unsigned int kol_zhitelej = 52; //"Расчетное количество жителей или служащих";
    unsigned int razmeshhenie_zastrojke = 36;//"Размещение в застройке";
    unsigned int konstruktivnoe_reshenie = 6;//"Конструктивное решение";


	//Energy passport of building
    //unsigned int energy_passport_building = "ЭНЕРГЕТИЧЕСКИЙ ПАСПОРТ ЗДАНИЯ";

    //Расчетные условия conditions for calculations
    //unsigned int calc_conditions = "Расчетные условия";
    unsigned int temp_air_internal  = 58;//"Расчетная температура внутреннего воздуха";
    unsigned int temp_air_external = 12;//"Расчетная температура наружного воздуха";
    unsigned int temp_artic = 8; //"Расчетная температура теплого чердака";
    unsigned int heating_period_days = 26; //"Продолжительность отопительного периода";
    unsigned int heating_period_temp_avg = 78;//"Средняя температура наружного воздуха за отопительный период";
    unsigned int heating_period_degree_days = 4;// "Градусо-сутки отопительного периода";
    unsigned int temp_tekh_podpolya = 66;//Расчетная температура техподполья

    //Геометрические и теплоэнергетические показатели Geometric and Heating variables
    //unsigned int geometric_thermal_indicators = "Геометрические и теплоэнергетические показатели";
    unsigned int area_all_floors = 45; //"Сумма площадей этажей здания";
    unsigned int area_living_space = 1;//"Площадь жилых помещений";
    unsigned int area_common_space = 62;//"Расчетная площадь (общественных зданий)";
    unsigned int volume_heated_space = 59;//"Отапливаемый объем здания";
    unsigned int coeff_glass_facade= 28;//"Коэффициент остекленности фасада здания";
    unsigned int coeff_compactness = 49;//"Показатель компактности здания";
    unsigned int area_boundary = 37; //"Общая площадь наружных ограждающих конструкций здания";
    unsigned int area_facade = 75;//"фасадов";
    unsigned int area_facade_keramzit = 86;//"навесной фасадной системы с основанием из керамзитобетона";
    unsigned int area_facade_beton = 87;//"навесной фасадной системы с основанием из железобетона";
    unsigned int area_wall_keramzitbeton = 88;//"трехслойной стены по кладке из керамзитобетона";
    unsigned int area_wall_monolith_beton = 89;//"трехслойной стены по монолитному железобетону";
    unsigned int area_doors = 90;//"входных дверей";
    unsigned int area_pokrity = 91;//"покрытий (совмещенных)";
    unsigned int area_roof = 92;//"эксплуатируемой кровли";
    unsigned int area_texpodpoli = 67;//"перекрытий над техподпольями";
    unsigned int area_proezd = 94;//"перекрытий над проездами и под эркерами";
    unsigned int area_windows_balcony = 95;//"окон и балконных дверей";
    unsigned int area_lift = 96; //"окон лестнично-лифтовых узлов";
    unsigned int area_facing_All = 32;//"окон по сторонам света";
    unsigned int area_facing_N = 30;//"С";
    unsigned int area_facing_NE = 55;//"СВ";
    unsigned int area_facing_E = 19;//"В";
    unsigned int area_facing_SE = 65;//"ЮВ";
    unsigned int area_facing_S = 39;//"Ю";
    unsigned int area_facing_SW = 70;//"ЮЗ";
    unsigned int area_facing_W = 24;//"З";
    unsigned int area_facing_NW = 61;//"СЗ";

    //Теплотехнические показатели Thermal Technical indicators
    //unsigned int thermal_indicators = "Теплотехнические показатели";
    unsigned int thermal_R_boundary = 11;//"Приведенное сопротивление теплопередаче наружных ограждений,   в том числе:";
    unsigned int thermal_R_facade_keramzitbeton = 29; //"навесной фасадной системы с основанием из керамзитобетона-R";
    unsigned int thermal_R_facade_beton = 22;//"навесной фасадной системы с основанием из железобетона-R";
    unsigned int thermal_R_wall_keramzit = 3;//"трехслойной стены по кладке из керамзитобетона-R";
    unsigned int thermal_R_wall_beton = 10;//"трехслойной стены по монолитному железобетону-R";
    unsigned int thermal_R_windows_balcony = 71;//"окон и балконных дверей";
    unsigned int thermal_R_lifts = 27;//"окон лестнично-лифтовых узлов";
    unsigned int thermal_R_doors =77; //входных дверей и ворот (раздельно) ;
    unsigned int thermal_R_roof = 31;//"покрытий (совмещенных";
    unsigned int thermal_R_roof_used = 51;//"эксплуатируемой кровли";
    unsigned int thermal_R_texpodpoli = 12 ;//"перекрытий над техническими подпольями";
    unsigned int thermal_R_proezd = 57;//"перекрытий над проездами и под эркерами";
    unsigned int thermal_R_walls = 114; //стен (раздельно по типу конструкции)
    unsigned int thermal_R_vitradjei = 115; //витражей
    unsigned int thermal_R_fonarei = 116; //фонарей
    unsigned int thermal_R_balkony_dverei = 117;//балконных дверей наружных переходов
    unsigned int thermal_R_cherdok_perekrity = 118;//чердачных перекрытий
    unsigned int thermal_R_cherdok_perekrity_teplyi = 119;//перекрытий "теплых" чердаков (эквивалентное)
    unsigned int thermal_R_sten_zemly = 120; //стен в земле и пола по грунту (раздельно)

    //Показатели вспомагательные other indicators
    //unsigned int other_indicators = "Показатели вспомагательные";
    unsigned int thermal_C_coeff = 44; //Приведенный трансмиссионный коэфициент теплопередачи здания
    unsigned int ventilation_cycle = 56; //"Кратность воздухообмена здания за отопительный период при удельной норме воздухообмена";
    unsigned int thermal_E_building = 50;//"Удельные бытовые тепловыделения в здании"; //thermal emissions
    unsigned int tariff_heating = 35; //"Тарифная цена тепловой энергии для проектируемого здания";


    //Удельные характеристики specific indicators
    //unsigned int specific_indicators = "Удельные характеристики";
    unsigned int specific_Thermal_Protection = 34; //"Удельная теплозащитная характеристика здания";
    unsigned int specific_Ventilation_Characteristics = 60;//"Удельная вентиляционная характеристика здания";
    unsigned int specific_Thermal_Emission = 43;//"Удельная характеристика бытовых тепловыделений здания";
    unsigned int specific_Solar_Reception = 69;//"Удельная характеристика теплопоступлений в здание от солнечной радиации";

    //Коэффициенты coefficients
    //unsigned int coefficients = "Коэффициенты";
    unsigned int coeff_auto_reg = 18;//"Коэффициент эффективности авторегулирования";
    unsigned int coeff_reduction = 2;//"Коэффициент, учитывающий снижение теплопотребления жилых зданий при наличии поквартирного учета тепловой энергии на отопление";
    unsigned int coeff_recuperation = 76;//"Коэффициент эффективности рекуператора";
    unsigned int coeff_additional = 42;//"Коэффициент учета дополнительного теплопотребления";
    unsigned int coeff_snijenie_teplopostuplenie = 83;//"Коэффициент, учитывающий снижение использования теплопоступлений в период превышения их над теплопотерями";

    //Комплексные показатели расхода тепловой энергии complex indicators
    //unsigned int complex_indicators = "Комплексные показатели расхода тепловой энергии";
    unsigned int thermal_usage_calc = 20;//"Расчетный удельный расход тепловой энергии на отопление здания";
    unsigned int thermal_usage_norminal = 16; //"Нормируемый удельный расход тепловой энергии на отопление здания";
    unsigned int thermal_usage_class = 63;//"Класс энергетической эффективности";

    //Энергетические нагрузки здания Thermal load of building
    //unsigned int thermal_usage_building = "Энергетические нагрузки здания";
    unsigned int thermal_usage_spec_heating_season = 81;//"Удельный расход тепловой энергии на отопление зданий за отопительный период";
    unsigned int thermal_usage_calc_heating_season = 41;//"Расход тепловой энергии на отопление зданий за отопительный период";
    unsigned int thermal_wastage_heating_season = 14;//"Общие теплопотери здания за отопительный период";

    //Дополнительные показатели Other indicators not directly mentioned but used
    unsigned int  n_chisov_ventilyatsi = 0; //"число часов работы вентиляции в течение недели, равное для рассматриваемого здания";
    unsigned int norm_vozdukh_pronisaemost_dver = 84; //"нормируемая воздухопроницаемость дверей";
    unsigned int norm_vozdukh_pronisaemost_okon = 38; //"нормируемая воздухопроницаемость окон";  //Таблица 11 СниП 23-02
    unsigned int vysota_zdaniya = 33; //"высота здания";
    unsigned int max_wind_velocity = 40;//"максимальная скорость  ветра за январь";
    unsigned int sontse_vert_rad_avg = 98; //средняя за отопительный период величина солнечной радиации на вертикальные поверхности при действительных условиях облачности, ориентированная по  фасадам здания
    unsigned int sontse_vert_rad_avg_N = 99;
    unsigned int sontse_vert_rad_avg_NE = 100;
    unsigned int sontse_vert_rad_avg_E = 101;
    unsigned int sontse_vert_rad_avg_SE = 102;
    unsigned int sontse_vert_rad_avg_S = 103;
    unsigned int sontse_vert_rad_avg_SW = 104;
    unsigned int sontse_vert_rad_avg_W = 105;
    unsigned int sontse_vert_rad_avg_NW = 106;
    unsigned int coeff_volume_reduction = 17;//"коэффициент снижения объема воздуха в здании, учитывающий наличие внутренних ограждающих конструкций";
    unsigned int sontse_hor_rad_avg = 107; //средняя за отопительный период величина солнечной радиации на горизонтальную поверхность при действительных условиях облачности
    unsigned int coeff_proniknovenie_sontse_okno = 109; //коэффициенты относительного проникания солнечной радиации для светопропускающих заполнений соответственно окон и зенитных фонарей, принимаемые по паспортным данным соответствующих светопропускающих изделий; при отсутствии данных следует принимать по своду правил; мансардные окна с углом наклона заполнений к горизонту 45° и более следует считать как вертикальные окна, с углом наклона менее 45° - как зенитные фонари;
    unsigned int coeff_proniknovenie_sontse_fon = 110;
    unsigned int coeff_zatenenie_okno = 112; //коэффициенты, учитывающие затенение светового проема соответственно окон и зенитных фонарей непрозрачными элементами заполнения, принимаемые по проектным данным; при отсутствии данных следует принимать по своду правил;
    unsigned int coeff_zatenenie_fon = 113;
    //Calaculated indicators

    //Показатель компактности здания
    double kompaktnost ();
	//Кратность воздухообмена здания за отопительный период при удельной норме воздухообмена
    double kratnostvozdukhobmen();
	//Удельная теплозащитная характеристика здания
    double udelnayateplozashita();
    //Удельная теплозащитная характеристика здания РАСЧЕТНАЯ
    double udelnayateplozashitaRaschet();
	//Удельная вентиляционная характеристика здания
    double udelnayaventilyatsii();
	//Удельная характеристика бытовых тепловыделений здания
    double udelnayabwitavayatenlovyidelenie();
	//Удельная характеристика теплопоступлений в здание от солнечной радиации
    double udelnayatenlopostunleniesontse();

    //Coefficients
    //Коэффициенты

    //Коэффициент, учитывающий снижение использования теплопоступлений в период превышения их над теплопотерями
    double koeffsnijenieteplopastuplenia();

    //Комплексные показатели расхода тепловой энергии

    //Расчетный удельный расход тепловой энергии на отопление здания
    double udelniraskhodteplovoienergii();

    //Энергетические нагрузки здания
    //Удельный расход тепловой энергии на отопление зданий за отопительный период
    double udelniiraskhodnaotopperiod();
    //Расход тепловой энергии на отопление зданий за отопительный период
    double raskhodnaotopperiod();
    //Общие теплопотери здания за отопительный период
    double obshieteplopoteriizaperiod();


    void raschetPakazateli();



    TreeModel *treeModel() ;
    QSqlRelationalTableModel *pakazatelModel();
    QSqlRelationalTableModel *fragmentModel();

    void saveTreeModeltoDB();
    void saveModelDatatoFile(const QString fname);
    void loadModelDataFromFile(const QString fname);


private:
    TreeModel *m_treeModel;

    Dbctx *ctx;
    double paznostDavlenie(const double k, const double y_int, const double v, const double y_ext);
    double soprativlenieVozdukhProniknovenie(const double Gn, const double delta_P_x, const double delta_P);
    double kolichestvoinfiltrvozdukh(const double delta_P_okna, const double delta_P_dver, const double A_dv, const double R_ok, const double R_dv, const double A_ok);
    double raschetGSOP();
    double koeffOtlichieVnutrVneshTemp(const double tnorm);
    double subCalcTeploZashita(const double tnorm, const double area, const double rprev);
    double totalCalcTeploZashita(QList<Entity *> fragments);

    double round(double n, unsigned d);
};

#endif // ENERGYPASSPORTMODEL_H
