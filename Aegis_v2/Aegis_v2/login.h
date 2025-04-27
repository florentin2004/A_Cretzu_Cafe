#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>

class MainMenu; // Forward declaration to prevent circular dependencies

class Login : public QWidget
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    QLabel *titleLabel;
    QLabel *usernameLabel;
    QLabel *passwordLabel;
    QLabel *messageLabel;

    QLineEdit *usernameInput;
    QLineEdit *passwordInput;

    QPushButton *loginButton;
    QPushButton *registerButton;
    QPushButton *backButton;

    void setupUI();
    void setBackground(const QString &path);
    QVector<QWidget*> getResizableWidgets() const;

private slots:
    void handleLogin();
    void goBackToMainMenu();
    void openRegisterWindow();
};

#endif // LOGIN_H
