#include "../include/logic.h"

QString Logic::getParameter1() const
{
    return m_parameter1;
}

void Logic::setParameter1(const QString &value)
{
    m_parameter1 = value;
    emit parameterChanged1();
}

QString Logic::getParameter2() const
{
    return m_parameter2;
}

void Logic::setParameter2(const QString &value)
{
    m_parameter2 = value;
    emit parameterChanged2();
}

QString Logic::getParameter3() const
{
    return m_parameter3;
}

void Logic::setParameter3(const QString &value)
{
    m_parameter3 = value;
    emit parameterChanged3();
}

QString Logic::getParameter4() const
{
    return m_parameter4;
}

void Logic::setParameter4(const QString &value)
{
    m_parameter4 = value;
    emit parameterChanged4();
}

Logic::Logic(QObject *parent) : QObject(parent)
{
    m_parameter1 = "/home/jan/Repositories/Tambua/Data/data/dog.bmp";
    m_parameter2 = "/home/jan/Repositories/Tambua/Data/data/mobilenet_quant.tflite";
    m_parameter3 = "/home/jan/Repositories/Tambua/Data/data/labels.txt";
    m_parameter4 = "Not Classified";
}

void Logic::DoClassificationCmd()
{
    emit doClassification();
}

// #include "logic.moc"
