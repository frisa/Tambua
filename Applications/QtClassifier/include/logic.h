#include <QObject>
#include <QString>

class Logic : public QObject
{
    Q_OBJECT

    QString getParameter1() const;
    void setParameter1(const QString &value);

    QString getParameter2() const;
    void setParameter2(const QString &value);

    QString getParameter3() const;
    void setParameter3(const QString &value);

    QString getParameter4() const;
    void setParameter4(const QString &value);

public:
    Q_PROPERTY(QString parameter1 READ getParameter1 WRITE setParameter1 NOTIFY parameterChanged1)
    Q_PROPERTY(QString parameter2 READ getParameter2 WRITE setParameter2 NOTIFY parameterChanged2)
    Q_PROPERTY(QString parameter3 READ getParameter3 WRITE setParameter3 NOTIFY parameterChanged3)
    Q_PROPERTY(QString parameter4 READ getParameter4 WRITE setParameter4 NOTIFY parameterChanged4)

public:
    explicit Logic(QObject *parent = nullptr);
    Q_INVOKABLE void DoClassificationCmd();

signals:
    void parameterChanged1();
    void parameterChanged2();
    void parameterChanged3();
    void parameterChanged4();
    void doClassification();

private:
    QString m_parameter1;
    QString m_parameter2;
    QString m_parameter3;
    QString m_parameter4;
};
