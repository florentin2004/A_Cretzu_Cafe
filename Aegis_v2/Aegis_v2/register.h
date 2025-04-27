#ifndef REGISTER_H
#define REGISTER_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QCheckBox>

class MainMenu;

class Register : public QWidget
{
    Q_OBJECT

public:
    explicit Register(QWidget *parent = nullptr);

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    QLabel *titleLabel;
    QLabel *emailLabel;
    QLabel *usernameLabel;
    QLabel *passwordLabel;
    QLabel *confirmPasswordLabel;

    QLineEdit *emailInput;
    QLineEdit *usernameInput;
    QLineEdit *passwordInput;
    QLineEdit *confirmPasswordInput;

    QCheckBox *termsCheckBox;

    QPushButton *registerButton;
    QPushButton *backButton;

    void setupUI();
    void setBackground(const QString &path);
    QVector<QWidget*> getResizableWidgets() const;

private slots:
    void goBackToMainMenu();
    void handleRegister();
};

#endif // REGISTER_H
