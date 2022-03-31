#include "moc_HelllMain.hpp"

#include <QApplication>

using namespace HELLL;

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    //app.setStyle("WindowsVista");

    HelllMain helll;
    helll.initSongFromArg(argc, argv);

    _sleep(780);

    helll.show();

    return app.exec();
}


