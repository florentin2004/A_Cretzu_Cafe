#include "register.h"
#include "resizer.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QResizeEvent>
#include <QTimer>  // Ensure QTimer is included

Register::Register(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("Register");
    // setFixedSize(400, 300);

    // Create a Layout
    QVBoxLayout *layout = new QVBoxLayout(this);

    registerLabel = new QLabel("Create an Account", this);
    registerLabel->setAlignment(Qt::AlignCenter);
    usernameField = new QLineEdit(this);
    usernameField->setPlaceholderText("Enter Username");
    emailField = new QLineEdit(this);
    emailField->setPlaceholderText("Enter Email");
    passwordField = new QLineEdit(this);
    passwordField->setPlaceholderText("Enter Password");
    passwordField->setEchoMode(QLineEdit::Password); // i use this to hide the password. So cool. in netframework was a little different
    confirmPasswordField = new QLineEdit(this);
    confirmPasswordField->setPlaceholderText("Confirm Password");
    confirmPasswordField->setEchoMode(QLineEdit::Password);
    registerButton = new QPushButton("Register", this);

    layout->addWidget(registerLabel);
    layout->addWidget(usernameField);
    layout->addWidget(emailField);
    layout->addWidget(passwordField);
    layout->addWidget(confirmPasswordField);
    layout->addWidget(registerButton);

    // Use `Resizer::delayedResize()` Instead of Direct Calls
    QVector<QWidget*> widgets = {registerLabel, usernameField, emailField, passwordField, confirmPasswordField, registerButton};
    QVector<int> fontSizes = {size().width() / 14, size().width() / 18, size().width() / 18, size().width() / 18, size().width() / 18, size().width() / 15};

    Resizer::delayedResize(this, size(), widgets, fontSizes);
}

// Updated Resize Event to Prevent Infinite Loop
void Register::resizeEvent(QResizeEvent *event)
{
    if (event->size() == size()) return;  // Prevent unnecessary looping

    QVector<QWidget*> widgets = {registerLabel, usernameField, emailField, passwordField, confirmPasswordField, registerButton};
    QVector<int> fontSizes = {event->size().width() / 14, event->size().width() / 18, event->size().width() / 18, event->size().width() / 18, event->size().width() / 18, event->size().width() / 15};

    Resizer::resizeFonts(event->size(), widgets, fontSizes);
    QWidget::resizeEvent(event);
}

Register::~Register()
{
}
