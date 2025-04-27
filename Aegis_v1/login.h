#ifndef LOGIN_H
#define LOGIN_H

#include <QMainWindow>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include "resizer.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Login; }
QT_END_NAMESPACE

class Login : public QWidget
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login();
protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    Ui::Login *ui;
    QLabel *titleLabel;
    QLabel *quoteLabel;
    QLabel *loginLabel;
    QLineEdit *username_emailField;
    QLineEdit *passwordField;
    QPushButton *loginButton;
    QPushButton *backButton;

    void BackToMainWindow();
};


#endif // LOGIN_H
