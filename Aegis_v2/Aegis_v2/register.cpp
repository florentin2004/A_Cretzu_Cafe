#include "register.h"
#include "resizer.h"
#include "mainmenu.h"
#include "Network.h"

#include <QPalette>
#include <QPixmap>
#include <QResizeEvent>
#include <QApplication>
#include <QMessageBox>
#include <QFormLayout>

// Constructor
Register::Register(QWidget *parent) : QWidget(parent)
{
    resize(800, 600);

    setupUI();
    setBackground(":/img/Images/background.jpg");

    setWindowIcon(QIcon(":/img/Images/Aegis.ico"));

    Resizer::delayedResize(this, size(), getResizableWidgets(), 24, 14);
}

// Setting up the UI
void Register::setupUI()
{
    // Title
    titleLabel = new QLabel("Register", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    QFont titleFont;
    titleFont.setPointSize(24);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);

    // Labels
    emailLabel = new QLabel("Email:", this);
    usernameLabel = new QLabel("Username:", this);
    passwordLabel = new QLabel("Password:", this);
    confirmPasswordLabel = new QLabel("Confirm Password:", this);

    emailLabel->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
    usernameLabel->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
    passwordLabel->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
    confirmPasswordLabel->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);

    // Input Fields
    emailInput = new QLineEdit(this);
    usernameInput = new QLineEdit(this);
    passwordInput = new QLineEdit(this);
    confirmPasswordInput = new QLineEdit(this);

    emailInput->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    usernameInput->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    passwordInput->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    confirmPasswordInput->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // Mask password fields
    passwordInput->setEchoMode(QLineEdit::Password);
    confirmPasswordInput->setEchoMode(QLineEdit::Password);

    // Terms & Conditions
    termsCheckBox = new QCheckBox("I agree to the Terms and Conditions", this);

    // Buttons
    registerButton = new QPushButton("Register", this);
    backButton = new QPushButton("Back", this);

    registerButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    backButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    QFormLayout *formLayout = new QFormLayout();
    formLayout->addRow(emailLabel, emailInput);
    formLayout->addRow(usernameLabel, usernameInput);
    formLayout->addRow(passwordLabel, passwordInput);
    formLayout->addRow(confirmPasswordLabel, confirmPasswordInput);

    // Main layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    // Push form downward dynamically
    mainLayout->addWidget(titleLabel, 0, Qt::AlignHCenter);
    mainLayout->addStretch();
    mainLayout->addLayout(formLayout); // Proper alignment
    mainLayout->setAlignment(formLayout, Qt::AlignCenter); // Center input fields
    mainLayout->addSpacing(10);
    mainLayout->addWidget(termsCheckBox, 0, Qt::AlignHCenter);
    mainLayout->addStretch(); // Ensure elements move lower when resizing

    // Button layout (fixed at bottom)
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(registerButton);
    buttonLayout->addWidget(backButton);
    buttonLayout->setAlignment(Qt::AlignCenter);
    mainLayout->addLayout(buttonLayout);

    setLayout(mainLayout);

    // Button connections
    connect(backButton, &QPushButton::clicked, this, &Register::goBackToMainMenu);
    connect(registerButton, &QPushButton::clicked, this, &Register::handleRegister);
}

// Background Setup
void Register::setBackground(const QString &path)
{
    QPalette palette;
    palette.setBrush(QPalette::Window,
                     QBrush(QPixmap(path).scaled(size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));
    setAutoFillBackground(true);
    setPalette(palette);
}

// Resizable Widgets
QVector<QWidget*> Register::getResizableWidgets() const
{
    return { titleLabel, emailLabel, usernameLabel, passwordLabel, confirmPasswordLabel, emailInput, usernameInput, passwordInput, confirmPasswordInput, termsCheckBox, registerButton, backButton };
}

// Resize Event
void Register::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);

    int baseTitleSize = 24;
    int baseButtonSize = 14;

    Resizer::delayedResize(this, size(), getResizableWidgets(), baseTitleSize, baseButtonSize);

    // Ensure background is properly scaled on resize
    setBackground(":/img/Images/background.jpg");
}

// Navigate Back to MainMenu
void Register::goBackToMainMenu()
{
    MainMenu *mm = new MainMenu(nullptr);
    mm->show();
    this->close();
}

// Handle Registration
void Register::handleRegister()
{
    // Validate fields
    if (emailInput->text().isEmpty() || usernameInput->text().isEmpty() ||
        passwordInput->text().isEmpty() || confirmPasswordInput->text().isEmpty())
    {
        QMessageBox::warning(this, "Error", "All fields must be filled!");
        return;
    }

    if (passwordInput->text() != confirmPasswordInput->text())
    {
        QMessageBox::warning(this, "Error", "Passwords do not match!");
        return;
    }

    if (!termsCheckBox->isChecked())
    {
        QMessageBox::warning(this, "Error", "You must agree to the terms!");
        return;
    }

    // Create network client
    Network *client = new Network();
    client->connectToServer();

    // Send actual input data
    QString registrationData = "REGISTER:" + usernameInput->text() + ":" + passwordInput->text() + ":" + emailInput->text();
    client->sendMessage(registrationData);
}
