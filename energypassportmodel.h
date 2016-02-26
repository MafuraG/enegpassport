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
	QString data_zapolnenya  = "Дата заполнения (число, м-ц, год)";	
    QString address_zdaniya = "Адрес здания";
	QString razrabochik = "Разработчик проекта";
	QString addr_tel_razrabotchika = "Адрес и телефон разработчика";
	QString shriff_proekta = "Шифр проекта";	
	QString naznacheniye_zdaniya = "Назначение здания, серия";
	QString etaj_kol_sektsii = "Этажность, количество секций";
	QString kol_kvartir = "Количество квартир";
	QString kol_zhitelej = "Расчетное количество жителей или служащих";	
	QString razmeshhenie_zastrojke = "Размещение в застройке";
	QString konstruktivnoe_reshenie = "Конструктивное решение";
    QString vysota_zdaniya = "высота здания";
    QString max_wind_velocity = "максимальная скорость  ветра за январь";


	//Energy passport of building
    QString energy_passport_building = "ЭНЕРГЕТИЧЕСКИЙ ПАСПОРТ ЗДАНИЯ";
    //conditions for calculations
    QString calc_conditions = "Расчетные условия";
    QString temp_air_internal  = "Расчетная температура внутреннего воздуха";
    QString temp_air_external = "Расчетная температура наружного воздуха";
    QString temp_artic = "Расчетная температура теплого чердака";
    QString heating_period_days = "Продолжительность отопительного периода";
    QString heating_period_temp_avg = "Средняя температура наружного воздуха за отопительный период";
    QString heating_period_degree_days = "Градусо-сутки отопительного периода";

    //Geometric and Heating variables
    QString geometric_thermal_indicators = "Геометрические и теплоэнергетические показатели";
    QString area_all_floors = "Сумма площадей этажей здания";
    QString area_living_space = "Площадь жилых помещений";
    QString area_common_space = "Расчетная площадь (общественных зданий)";
    QString volume_heated_space = "Отапливаемый объем здания";
    QString coeff_glass_facade= "Коэффициент остекленности фасада здания";
    QString coeff_compactness = "Показатель компактности здания";
    QString area_boundary = "Общая площадь наружных ограждающих конструкций здания";
    QString area_facade = "фасадов";
    QString area_facade_keramzit = "навесной фасадной системы с основанием из керамзитобетона";
    QString area_facade_beton = "навесной фасадной системы с основанием из железобетона";
    QString area_wall_keramzitbeton = "трехслойной стены по кладке из керамзитобетона";
    QString area_wall_monolith_beton = "трехслойной стены по монолитному железобетону";
    QString area_doors = "входных дверей";
    QString area_pokrity = "покрытий (совмещенных)";
    QString area_roof = "эксплуатируемой кровли";
    QString area_texpodpoli = "перекрытий над техподпольями";
    QString area_proezd = "перекрытий над проездами и под эркерами";
    QString area_windows_balcony = "окон и балконных дверей";
    QString area_lift = "окон лестнично-лифтовых узлов";
    QString area_facing_All = "окон по сторонам света";
    QString area_facing_N = "С";
    QString area_facing_NE = "СВ";
    QString area_facing_E = "В";
    QString area_facing_SE = "ЮВ";
    QString area_facing_S = "Ю";
    QString area_facing_SW = "ЮЗ";
    QString area_facing_W = "З";
    QString area_facing_NW = "СЗ";

    //Thermal Technical indicators
    QString thermal_indicators = "Теплотехнические показатели";
    QString thermal_R_boundary = "Приведенное сопротивление теплопередаче наружных ограждений,   в том числе:";
    QString thermal_R_facade_keramzitbeton = "навесной фасадной системы с основанием из керамзитобетона";
    QString thermal_R_facade_beton = "навесной фасадной системы с основанием из железобетона";
    QString thermal_R_wall_keramzit = "трехслойной стены по кладке из керамзитобетона";
    QString thermal_R_wall_beton = "трехслойной стены по монолитному железобетону";
    QString thermal_R_windows_balcony = "окон и балконных дверей";
    QString thermal_R_lifts = "окон лестнично-лифтовых узлов";
    QString thermal_R_doors = "входных дверей";
    QString thermal_R_roof = "покрытий (совмещенных)";
    QString thermal_R_roof_used = "эксплуатируемой кровли";
    QString thermal_R_texpodpoli = "перекрытий над техническими подпольями";
    QString thermal_R_proezd ="перекрытий над проездами и под эркерами";

    //other indicators
    QString other_indicators = "Показатели вспомагательные";
    QString thermal_C_coeff = "Приведенный трмнсмиссионный коэфициент теплопередачи здания"; //thermal conductence
    QString ventilation_cycle = "Кратность воздухообмена здания за отопительный период при удельной норме воздухообмена";
    QString thermal_E_building ="Удельные бытовые тепловыделения в здании"; //thermal emissions
    QString tariff_heating = "Тарифная цена тепловой энергии для проектируемого здания";


    //specific indicators
    QString specific_indicators = "Удельные характеристики";
    QString specific_Thermal_Protection = "Удельная теплозащитная характеристика здания";
    QString specific_Ventilation_Characteristics = "Удельная вентиляционная характеристика здания";
    QString specific_Thermal_Emission = "Удельная характеристика бытовых тепловыделений здания";
    QString specific_Solar_Reception = "Удельная характеристика теплопоступлений в здание от солнечной радиации";

    //coefficients
    QString coefficients = "Коэффициенты";
    QString coeff_auto_reg = "Коэффициент эффективности авторегулирования";
    QString coeff_reduction = "Коэффициент, учитывающий снижение теплопотребления жилых зданий при наличии поквартирного учета тепловой энергии на отопление";
    QString coeff_recuperation = "Коэффициент эффективности рекуператора";
    //QString coeff_effectiveness = "Коэффициент эффективности рекуператора";
    QString coeff_additional = "Коэффициент учета дополнительного теплопотребления";
	QString coeff_volume_reduction = "коэффициент снижения объема воздуха в здании, учитывающий наличие внутренних ограждающих конструкций";

    //complex indicators
    QString complex_indicators = "Комплексные показатели расхода тепловой энергии";
    QString thermal_usage_calc = "Расчетный удельный расход тепловой энергии на отопление здания";
    QString thermal_usage_norminal ="Нормируемый удельный расход тепловой энергии на отопление здания";
    QString thermal_usage_class = "Класс энергетической эффективности";

    //Thermal load of building
    QString thermal_usage_building = "Энергетические нагрузки здания";
    QString thermal_usage_spec_heating_season = "Удельный расход тепловой энергии на отопление зданий за отопительный период";
    QString thermal_usage_calc_heating_season = "Расход тепловой энергии на отопление зданий за отопительный период";
    QString thermal_wastage_heating_season = "Общие теплопотери здания за отопительный период";

    //Other indicators not directly mentioned but used
    QString  n_chisov_ventilyatsi = "число часов работы вентиляции в течение недели, равное для рассматриваемого здания";
	QString norm_vozdukh_pronisaemost_dver = "нормируемая воздухопроницаемость дверей";
	QString norm_vozdukh_pronisaemost_okon = "нормируемая воздухопроницаемость окон";  //Таблица 11 СниП 23-02
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
    double udelnayatenlovyidelenie();
	//Удельная характеристика теплопоступлений в здание от солнечной радиации
    double udelnayatenlopostunleniesontse();

    TreeModel *treeModel() ;
    QSqlRelationalTableModel *pakazatelModel();
    QSqlRelationalTableModel *fragmentModel();

    double saveTreeModeltoDB();


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

};

#endif // ENERGYPASSPORTMODEL_H
