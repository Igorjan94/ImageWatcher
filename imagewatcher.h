#ifndef IMAGEWATCHER_H
#define IMAGEWATCHER_H

#include <QDialog>
#include <QtGui>
#include "ui_imagewatcher.h"
#include <stack>

class imageWatcher : public QDialog, private Ui::imageWatcher
{
    Q_OBJECT

public:
    explicit imageWatcher(QString directory, QString result, QWidget *parent = 0);
    ~imageWatcher();
    void keyPressEvent(QKeyEvent *);

private:
    QDir dir, ddd;
    QString resultDirectory;
    std::stack<std::pair<QDir, int>> stack, old;
    QGraphicsScene* sc;
    QImage im;
    QRectF rect;
    QTransform myTransform;
    void setImage(QString s);
    void parser();
    void prev();
    int f;
};

#endif // IMAGEWATCHER_H
