#include "resizer.h"

void Resizer::resizeFonts(const QSize &windowSize, const QVector<QWidget*>& widgets, const QVector<int>& fontSizes)
{
    if (widgets.size() != fontSizes.size()) return;  // Ensure both vectors match

    for (int i = 0; i < widgets.size(); ++i)
    {
        QWidget* widget = widgets[i];
        int fontSize = fontSizes[i];

        QLabel* label = dynamic_cast<QLabel*>(widget);
        QPushButton* button = dynamic_cast<QPushButton*>(widget);
        QLineEdit* inputField = dynamic_cast<QLineEdit*>(widget);

        if (label)
        {
            QFont font;
            font.setPointSize(fontSize);
            label->setFont(font);
        }
        else if (button)
        {
            QFont font;
            font.setPointSize(fontSize);
            button->setFont(font);
        }
        else if (inputField)
        {
            QFont font;
            font.setPointSize(fontSize);
            inputField->setFont(font);
        }
    }
}

// Delayed Resizing Function Using QTimer
void Resizer::delayedResize(QObject *parent, const QSize &windowSize, const QVector<QWidget*>& widgets, const QVector<int>& fontSizes)
{
    QTimer::singleShot(0, parent, [windowSize, widgets, fontSizes]() {
        Resizer::resizeFonts(windowSize, widgets, fontSizes);
    });
}
