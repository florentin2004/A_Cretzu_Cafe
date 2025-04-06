#ifndef RESIZER_H
#define RESIZER_H

#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QVector>
#include <QTimer>  // Include QTimer for delayed execution

class Resizer
{
public:
    static void resizeFonts(const QSize &windowSize, const QVector<QWidget*>& widgets, const QVector<int>& fontSizes);
    static void delayedResize(QObject *parent, const QSize &windowSize, const QVector<QWidget*>& widgets, const QVector<int>& fontSizes);
};

#endif // RESIZER_H
