#include "dmanager.h"
#include "dialog.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QString currentFile = "";
    QApplication a(argc, argv);
    dmanager w;
    w.show();
    return a.exec();
}
