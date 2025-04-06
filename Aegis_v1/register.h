#ifndef REGISTER_H
#define REGISTER_H

#include <QMainWindow>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include "resizer.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Register; }
QT_END_NAMESPACE

class Register : public QWidget
{
    Q_OBJECT

public:
    explicit Register(QWidget *parent = nullptr);
    ~Register();
protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    Ui::Register *ui;
    QLabel *titleLabel;
    QLabel *quoteLabel;
    QLabel *registerLabel;
    QLineEdit *usernameField;
    QLineEdit *emailField;
    QLineEdit *passwordField;
    QLineEdit *confirmPasswordField;
    QPushButton *registerButton;
};

#endif // REGISTER_H
