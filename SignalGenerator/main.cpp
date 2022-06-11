#include "signalgenerator.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SignalGenerator w;
    w.show();
    return a.exec();
}
