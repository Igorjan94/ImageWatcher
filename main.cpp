#include "imagewatcher.h"
#include <QApplication>
#include <iostream>

int main(int argc, char *argv[])
{
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QApplication a(argc, argv);
    if (argc <= 2)
    {
        std::cout << "Usage: directory destination\n";
<<<<<<< HEAD
        imageWatcher w("/home/igorjan/documents/фотик(01.2014-...)/", "/home/igorjan/documents/фотик(01.2014-...)/избр2");
=======
        imageWatcher w("/home/igorjan/documents/photos", "/home/igorjan/documents/photos/qwre/");
>>>>>>> 83bb776fffe2b52609ca9f1da3e0ba098a4c1216
        w.show();
        return a.exec();
    }
    int i = 0;
    if (argv[i][0] == '.' && argv[i][1] == '/')
        i++;
    imageWatcher w(argv[i], argv[i + 1]);
    w.show();
    return a.exec();
}
