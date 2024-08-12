#include <QApplication>

#include "main_window.h"

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    Main_Window w;

    w.show();

    return a.exec();
}
