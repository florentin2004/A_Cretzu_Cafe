#include "login.h"
#include "mainwindow.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QResizeEvent>
#include <QTimer>  // Ensure QTimer is included

Login::Login(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("Login");

    // start size
    resize(600,400);

    // Create a Layout
    QVBoxLayout *layout = new QVBoxLayout(this);

    loginLabel = new QLabel("Login", this);
    loginLabel->setAlignment(Qt::AlignCenter);
    username_emailField = new QLineEdit(this);
    username_emailField->setPlaceholderText("Enter Username or Email");
    passwordField = new QLineEdit(this);
    passwordField->setPlaceholderText("Enter Password");
    passwordField->setEchoMode(QLineEdit::Password); // i use this to hide the password. So cool. in netframework was a little different


    layout->addWidget(loginLabel);
    layout->addWidget(username_emailField);
    layout->addWidget(passwordField);

    // Buttons
    backButton = new QPushButton("Back", this);

    // Connect
    connect(backButton, &QPushButton::clicked, this, &Login::BackToMainWindow);


    // Use `Resizer::delayedResize()` Instead of Direct Calls
    QVector<QWidget*> widgets = {loginLabel, username_emailField, passwordField};
    QVector<int> fontSizes = {size().width() / 14, size().width() / 18, size().width() / 18};

    Resizer::delayedResize(this, size(), widgets, fontSizes);
}

// Updated Resize Event to Prevent Infinite Loop
void Login::resizeEvent(QResizeEvent *event)
{
    if (event->size() == size()) return;  // Prevent unnecessary looping

    QVector<QWidget*> widgets = {loginLabel, username_emailField, passwordField};
    QVector<int> fontSizes = {size().width() / 14, size().width() / 18, size().width() / 18};

    Resizer::resizeFonts(event->size(), widgets, fontSizes);
    QWidget::resizeEvent(event);
}

void Login::BackToMainWindow()
{
    MainWindow *mainWindow = new MainWindow(); // Create MainWindow instance
    mainWindow->show();  // Show MainWindow
    this->close();  // Close Login form
}


Login::~Login()
{
}
