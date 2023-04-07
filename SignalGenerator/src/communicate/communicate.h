
#ifndef COMMUNICATE_H
#define COMMUNICATE_H


#include <QObject>
#include <QTimer>

class Communicate : public QObject
{
    Q_OBJECT
public:
    explicit Communicate(QObject* parent = nullptr);
public:
    void SetTimerInterval(int interval);
signals:

};

#endif // COMMUNICATE_H
