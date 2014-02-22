#include "imagewatcher.h"
#include <stdio.h>
#include <iostream>
#include <stack>
#include <unistd.h>

QString e(QString src)
{
    QString dest;
    for (int i = 0; i < src.size(); i++)
    {
        if (src[i] == '(' || src[i] == ')' || src[i] == '\"')
            dest.append("\\");
        dest.append(src[i]);
    }
    return dest;
}

imageWatcher::imageWatcher(QString directory, QString result, QWidget *parent) :
    QDialog(parent),
    dir(directory)
{
    resultDirectory = e(result);
    directory = e(directory);
    std::cout << directory.toUtf8().data() << std::endl;
    if (resultDirectory[resultDirectory.size() - 1] != '/')
        resultDirectory.append('/');
    QDir ddd(resultDirectory);
    if (!ddd.exists())
        ddd.mkpath(resultDirectory);
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
        if (stack.size() > 1)
            stack.pop();
        if (stack.size() == 1)
        {
            name->setText("THE END. No images to watch:(");
            return;
        }
        parser();
        return;
    }
    QFileInfo q = d.entryInfoList().at(i);
    std::cout << " " << q.fileName().toUtf8().data() << std::endl;
    if (q.isDir() && q.fileName() != "." && q.fileName() != "..")
    {
        if (!d.entryInfoList().at(i - 1).isDir())
            old.push({stack.top().first, stack.top().second + 1});
        stack.pop();
        stack.push({d, i});
        d.cd(q.fileName());
        stack.push({d, 1});
        parser();
        return;
    }
    setImage(q.absoluteFilePath(), false);
    stack.pop();
    stack.push({d, i});
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
        stack.push({old.top().first, old.top().second});
        old.pop();
        d = stack.top().first;
        i = stack.top().second - 1;
    }
    QFileInfo q = d.entryInfoList().at(i);
    std::cout << " " << q.fileName().toUtf8().data() << std::endl;
    setImage(q.absoluteFilePath(), false);
    stack.pop();
    stack.push({d, i});
}

void imageWatcher::setImage(QString s, bool f)
{
    if (!f)
        im.load(s),
        name->setText(s);
    else
        im = im.transformed(myTransform);
    sc->clear();
    sc->addPixmap(QPixmap::fromImage(im));
    rect = sc->itemsBoundingRect();
    const double eps = 1;
    double w = rect.width(),
           h = rect.height();
    if (!f)
        rect.setWidth(w * eps),
        rect.setHeight(h * eps);
    image->setScene(sc);
    image->fitInView(rect);
}

void imageWatcher::keyPressEvent(QKeyEvent *e)
{
    switch (e->key())
    {
        case Qt::Key_K:
        {
            parser();
            std::cout << "K";
            break;
        }
        case Qt::Key_J:
        {
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
            ///TODO: make crossplatform
            QString s = "ln --symbolic ";
            s.append(d.absoluteFilePath().toUtf8().data());
            s = ::e(s);
            s.append(" ");
            s.append(resultDirectory);
            s.append(d.fileName());
            std::cout << s.toUtf8().data() << std::endl;
            if (system(s.toUtf8().data()) == 0)
                name->setText(name->text().append(" added")),
                std::cout << "added\n";
            break;
        }
        case Qt::Key_R:
        {
            std::cout << "R";
            setImage("", true);
            break;
        }
        case Qt::Key_E:
        {
            std::cout << "R";
            setImage("", true);
            setImage("", true);
            setImage("", true);
            break;
        }
        case Qt::Key_S:
        {
            std::cout << "S";
            if (stack.size() > 1)
                stack.pop();
            break;
        }
    }
}

imageWatcher::~imageWatcher()
{
}
