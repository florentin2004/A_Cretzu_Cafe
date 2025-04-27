#ifndef MAINMENU_H
#define MAINMENU_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QVector>
#include <QPixmap>

class MainMenu : public QWidget
{
    Q_OBJECT

public:
    explicit MainMenu(QWidget *parent = nullptr);

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    QLabel *titleLabel;
    QLabel *imageLabel;
    QPushButton *loginButton;
    QPushButton *registerButton;
    QPushButton *exitButton;

    QPixmap originalImage;

    void setupUI();
    void setBackground(const QString &path);
    QVector<QWidget*> getResizableWidgets() const;
    QSize previousSize;
    void resizeFonts();

private slots:
    void exitApplication();
    void registerLogic();
    void loginLogic();

};

#endif // MAINMENU_H
