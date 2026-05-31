#include <QApplication>
#include <QInputDialog>
#include <QMessageBox>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    quint16 port = 0;
    if (argc > 1) {
        bool ok = false;
        const int argPort = QString::fromLocal8Bit(argv[1]).toInt(&ok);
        if (ok && argPort > 0 && argPort < 65535) {
            port = static_cast<quint16>(argPort);
        }
    }

    if (port == 0) {
        bool ok = false;
        const int inputPort = QInputDialog::getInt(nullptr, "Порт узла",
                                                   "Введите порт для запуска:",
                                                   5001, 1024, 65535, 1, &ok);
        if (!ok) {
            QMessageBox::warning(nullptr, "Ошибка", "Порт не выбран");
            return 0;
        }
        port = static_cast<quint16>(inputPort);
    }

    MainWindow window(port);
    window.show();

    return app.exec();
}
