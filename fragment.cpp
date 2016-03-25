#include "fragment.h"

Fragment::Fragment()
{
    m_section = "найменование фрагмента";
    m_tnorm = 0.00;
    m_area = 0.00;
    m_resistance = 0.00;
}

Fragment::~Fragment()
{

}

double Fragment::tnorm() const
{
    return m_tnorm;
}

void Fragment::setTnorm(double tnorm)
{
    m_tnorm = tnorm;
}
double Fragment::area() const
{
    return m_area;
}

void Fragment::setArea(double area)
{
    m_area = area;
}

double Fragment::resistance() const
{
    return m_resistance;
}

void Fragment::setResistance(double resistance)
{
    m_resistance = resistance;
}

QString Fragment::section() const
{
    return m_section;
}

void Fragment::setSection(const QString &section)
{
    m_section = section;
}

QVariant Fragment::data(const int col) const
{
    if (col >= 0 && col < Fragment::ColumnCount){
        switch(col){
            case(0):{
            return QVariant(section());
        }
        case(1):{
            return QVariant(tnorm());
        }
        case(2):{
            return QVariant(area());
        }
        case(3):{
            return QVariant(resistance());
        }
        default:{
            return QVariant();
        }
        }
    }

    return QVariant();
}

bool Fragment::setData(int col, QVariant val)
{
    if (col >= 0 && col < Fragment::ColumnCount){
        switch(col){
            case(0):{
                setSection(val.toString());
                break;
            }
            case(1):{
                setTnorm(val.toDouble());
                break;
            }
            case(2):{
                setArea(val.toDouble());
                break;
            }
            case(3):{
                setResistance(val.toDouble());
                break;
            }
            default:{
                return false;
            }
        }

        return true;
    }

    return false;

}

QVariant Fragment::header(int col)
{
    if (col >= 0 && col < Fragment::ColumnCount){
        switch(col){
            case(0):{
            return QVariant(Fragment::D_Section);
        }
        case(1):{
            return QVariant(Fragment::D_Tnorm);
        }
        case(2):{
            return QVariant(Fragment::D_Area);
        }
        case(3):{
            return QVariant(Fragment::D_Resistance);
        }
        default:{
            return QVariant();
        }
        }
    }

    return QVariant();
}


QString Fragment::Section = "section";
QString Fragment::Tnorm = "tnorm";
QString Fragment::Area = "area";
QString Fragment::EntityName = "fragment";
QString Fragment::Resistance = "resistance";
double Fragment::ColumnCount = 4;

QString Fragment::D_Section = "Наименование фрагмента";
QString Fragment::D_Tnorm = "tпом";
QString Fragment::D_Area = "Aф,I, м2";
QString Fragment::D_EntityName = "fragment";
QString Fragment::D_Resistance = "Rпро,i, (м2*0С)/Вт";




