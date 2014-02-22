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
    QDir dir;
    QString resultDirectory;
    std::stack<std::pair<QDir, int>> stack, old;
    QGraphicsScene* sc;
    QImage im;
    QRectF rect;
    QTransform myTransform;
    void setImage(QString s, bool f);
    void parser();
    void prev();
};

#endif // IMAGEWATCHER_H
