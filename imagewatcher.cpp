#include "imagewatcher.h"
#include <stdio.h>
#include <iostream>
#include <stack>
#include <unistd.h>
#include <QFileDialog>

QString e(QString src)
{
    QString dest;
    for (int i = 0; i < src.size(); i++)
    {
        if (src[i] == '(' || src[i] == ')' || src[i] == '\"' || src[i] == ' ')
            dest.append("\\");
        dest.append(src[i]);
    }
    return dest;
}

void writeln(QString s)
{
    std::cout << s.toUtf8().data() << std::endl;
}

void imageWatcher::checkResultDirectory()
{
    if (resultDirectory[resultDirectory.size() - 1] != '/')
        resultDirectory.append('/');
    ddd = QDir(resultDirectory);
    if (!ddd.exists())
    {
        if (ddd.mkpath(resultDirectory) == 1)
            std::cout << "No such directory, created a new one\n";
    }
        else
            std::cout << "OK, directory exists\n";
}

imageWatcher::imageWatcher(QString directory, QString result, QWidget *parent) :
    QDialog(parent),
    dir(directory)
{
    f = 0;
    resultDirectory = result;
    writeln(directory);
    writeln(resultDirectory);
    checkResultDirectory();
    setupUi(this);
    stack.push({dir, 1});
    myTransform.rotate(90);
    sc = new QGraphicsScene();
    parser();
}

void imageWatcher::parser()
{
    if (stack.empty())
    {
        name->setText("THE END. No images to watch:(");
        return;
    }
    QDir d = stack.top().first;
    int i = stack.top().second + 1;
    if (i >= d.entryList().size())
    {
        if (d.entryList().size() > 2)
            old.push({stack.top().first, stack.top().second});
        if (stack.size() == 1)
        {
            name->setText("THE END. No images to watch:(");
            return;
        }
        if (stack.size() > 1)
            stack.pop();
        parser();
        return;
    }
    QFileInfo q = d.entryInfoList().at(i);
    writeln(q.fileName());
    if (q.isDir() && q.fileName() != "." && q.fileName() != "..")
    {
        if (!d.entryInfoList().at(i - 1).isDir())
            old.push({stack.top().first, stack.top().second});
        stack.pop();
        stack.push({d, i});
        d.cd(q.fileName());
        stack.push({d, 1});
        parser();
        return;
    }
    setImage(q.absoluteFilePath());
    stack.pop();
    stack.push({d, i});
}

void imageWatcher::getDst()
{
    resultDirectory = QFileDialog::getExistingDirectory(0, "Directory to symlinks", "/home/");
    checkResultDirectory();
    writeln("result directory: " + resultDirectory);
}

void imageWatcher::getSrc()
{
    QString directory = QFileDialog::getExistingDirectory(0, "Directory to watch photos", "/home/");
    while (!stack.empty())
        stack.pop();
    stack.push({QDir(directory), 1});
    parser();
    writeln("Source directory: " + directory);
}

void imageWatcher::prev()
{
    QDir d = stack.top().first;
    int i = stack.top().second - 1;
    if (i <= 1)
    {
        if (stack.size() == 1 || old.empty())
            return;
        stack.pop();
        auto temp = stack.top();
        stack.pop();
        stack.push({temp.first, temp.second - 1});
        stack.push({old.top().first, old.top().second});
        old.pop();
        d = stack.top().first;
        i = stack.top().second;
    }
    QFileInfo q = d.entryInfoList().at(i);
    writeln(q.fileName());
    setImage(q.absoluteFilePath());
    stack.pop();
    stack.push({d, i});
}

QString getFileName(QString s)
{
    if (s.endsWith(" added"))
        s.chop(6);
    QString b = "";
    int i = s.size() - 1;
    while (i >= 0 && s[i] != '/')
        b.append(s[i--]);
    std::reverse(b.begin(), b.end());
    return b;
}

void imageWatcher::setImage(QString s)
{
    if (s != "")
        name->setText(s);
    if (ddd.entryList().contains(getFileName(s)))
        name->setText(name->text().append(" added"));
    sc->clear();
    if (f == 0)
        im.load(s);
    else
        im = im.transformed(myTransform);
    sc->addPixmap(QPixmap::fromImage(im));
    rect = sc->itemsBoundingRect();
    double w = rect.width();
    if (f % 2 || w < rect.height())
        rect.setWidth(w * 2.29229);
    image->setScene(sc);
    image->fitInView(rect);
}

void imageWatcher::keyPressEvent(QKeyEvent *e)
{
    switch (e->key())
    {
        case Qt::Key_K:
        {
            f = 0;
            parser();
            std::cout << "K";
            break;
        }
        case Qt::Key_J:
        {
            f = 0;
            prev();
            std::cout << "J";
            break;
        }
        case Qt::Key_I:
        {
            std::cout << "I";
            if (name->text().endsWith(" added"))
                break;
            QFileInfo d = stack.top().first.entryInfoList().at(stack.top().second);
            if (QFile::link(d.absoluteFilePath().toUtf8().data(), resultDirectory + d.fileName()))
                name->setText(name->text().append(" added")),
                std::cout << "added\n",
                ddd = QDir(resultDirectory);
            break;
        }
        case Qt::Key_R:
        {
            std::cout << "R";
            f++;
            setImage("");
            break;
        }
        case Qt::Key_Delete:
        {
            std::cout << "Delete";
            if (ddd.entryList().contains(getFileName(name->text())))
            {
                ddd = QDir(resultDirectory);
                QString t = name->text();
                t.chop(6);
                name->setText(t);
                QString q = resultDirectory;
                q.append(getFileName(name->text()));
                QFile(q.toUtf8().data()).remove();
            }
            break;
        }
        case Qt::Key_E:
        {
            std::cout << "R";
            f++;
            setImage("");
            f++;
            setImage("");
            f++;
            setImage("");
            break;
        }
        case Qt::Key_S:
        {
            std::cout << "S";
            if (stack.size() > 1)
            {
                old.push(stack.top());
                stack.pop();
            }
            break;
        }
        case Qt::Key_O:
        {
            std::cout << "O";
            QString temp = "xdg-open ";
            temp.append(::e(name->text()));
            if (system(temp.toUtf8().data()) == 0)
                writeln("Ok viewed");
            break;
        }
    }
}

imageWatcher::~imageWatcher()
{
}
