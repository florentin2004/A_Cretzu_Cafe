#include "login.h"
#include "mainmenu.h"
#include "register.h"
#include "resizer.h"

#include <QPalette>
#include <QPixmap>
#include <QResizeEvent>
#include <QApplication>
#include <QMessageBox>
#include <QFormLayout>

Login::Login(QWidget *parent) : QWidget(parent)
{
    resize(800, 600); // Fixed initial size
    setupUI();
    setBackground(":/img/Images/background.jpg");

    setWindowIcon(QIcon(":/img/Images/Aegis.ico"));
}

// Setting up UI
void Login::setupUI()
{
    // Title
    titleLabel = new QLabel("Login", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    QFont titleFont;
    titleFont.setPointSize(24);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);

    // Labels
    usernameLabel = new QLabel("Username or Email:", this);
    passwordLabel = new QLabel("Password:", this);

    usernameLabel->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
    passwordLabel->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);

    // Input Fields
    usernameInput = new QLineEdit(this);
    passwordInput = new QLineEdit(this);

    usernameInput->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    passwordInput->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    passwordInput->setEchoMode(QLineEdit::Password);

    // Message Label
    messageLabel = new QLabel("Don't have an account? Make one!", this);
    messageLabel->setAlignment(Qt::AlignCenter);

    // Buttons
    loginButton = new QPushButton("Login", this);
    registerButton = new QPushButton("Register", this);
    backButton = new QPushButton("Back", this);

    loginButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    registerButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    backButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    // Layout for inputs
    QFormLayout *formLayout = new QFormLayout();
    formLayout->addRow(usernameLabel, usernameInput);
    formLayout->addRow(passwordLabel, passwordInput);

    // Main Layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(titleLabel, 0, Qt::AlignHCenter);
    mainLayout->addStretch();
    mainLayout->addLayout(formLayout);
    mainLayout->setAlignment(formLayout, Qt::AlignCenter);
    mainLayout->addSpacing(10);
    mainLayout->addWidget(messageLabel);
    mainLayout->addStretch();

    // Button Layout
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(loginButton);
    buttonLayout->addWidget(registerButton);
    buttonLayout->addWidget(backButton);
    buttonLayout->setAlignment(Qt::AlignCenter);
    mainLayout->addLayout(buttonLayout);

    setLayout(mainLayout);

    // Button Connections
    connect(loginButton, &QPushButton::clicked, this, &Login::handleLogin);
    connect(registerButton, &QPushButton::clicked, this, &Login::openRegisterWindow);
    connect(backButton, &QPushButton::clicked, this, &Login::goBackToMainMenu);
}

// Background Setup
void Login::setBackground(const QString &path)
{
    QPalette palette;
    palette.setBrush(QPalette::Window,
                     QBrush(QPixmap(path).scaled(size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));
    setAutoFillBackground(true);
    setPalette(palette);
}

// Resizable Widgets
QVector<QWidget*> Login::getResizableWidgets() const
{
    return { titleLabel, usernameLabel, passwordLabel, usernameInput, passwordInput, messageLabel, loginButton, registerButton, backButton };
}

// Resize Event
void Login::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);

    int baseTitleSize = 24;
    int baseButtonSize = 14;

    Resizer::delayedResize(this, size(), getResizableWidgets(), baseTitleSize, baseButtonSize);

    setBackground(":/img/Images/background.jpg");
}

// Navigate Back to MainMenu
void Login::goBackToMainMenu()
{
    MainMenu *mm = new MainMenu(nullptr);
    mm->show();
    this->close();
}

// Open Register Window
void Login::openRegisterWindow()
{
    Register *r = new Register(nullptr);
    r->show();
    this->close();
}

// Handle Login (Example Logic)
void Login::handleLogin()
{
    if (usernameInput->text().isEmpty() || passwordInput->text().isEmpty())
    {
        QMessageBox::warning(this, "Error", "Both fields must be filled!");
        return;
    }

    // Example Authentication (Replace with actual logic)
    if (usernameInput->text() == "test" && passwordInput->text() == "password")
    {
        QMessageBox::information(this, "Success", "Login successful!");
        goBackToMainMenu();
    }
    else
    {
        QMessageBox::warning(this, "Error", "Invalid username or password!");
    }
}
