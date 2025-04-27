#include "mainmenu.h"
#include "resizer.h"
#include "register.h"
#include "login.h"
#include <QPalette>
#include <QResizeEvent>
#include <QApplication>

MainMenu::MainMenu(QWidget *parent) : QWidget(parent)
{
    resize(800, 600);

    setupUI();
    setBackground(":/img/Images/background.jpg");

    setWindowIcon(QIcon(":/img/Images/Aegis.ico"));


    Resizer::delayedResize(this, size(), getResizableWidgets(), 24, 14);
}

// resize logic - START

void MainMenu::setupUI()
{
    // Title
    titleLabel = new QLabel("Aegis", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    QFont titleFont;
    titleFont.setPointSize(24);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);

    // Image
    imageLabel = new QLabel(this);
    originalImage.load(":/img/Images/Aegis-removebg.png");
    imageLabel->setPixmap(originalImage.scaled(300, 300, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    imageLabel->setAlignment(Qt::AlignCenter);

    // Buttons
    loginButton = new QPushButton("Login", this);
    registerButton = new QPushButton("Register", this);
    exitButton = new QPushButton("Exit", this);

    loginButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    registerButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    exitButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    // Layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(titleLabel);
    mainLayout->addStretch();
    mainLayout->addWidget(imageLabel);
    mainLayout->addStretch();
    mainLayout->addWidget(loginButton);
    mainLayout->addWidget(registerButton);
    mainLayout->addWidget(exitButton);

    mainLayout->setAlignment(titleLabel, Qt::AlignHCenter);
    mainLayout->setAlignment(imageLabel, Qt::AlignHCenter);
    mainLayout->setAlignment(loginButton, Qt::AlignHCenter);
    mainLayout->setAlignment(registerButton, Qt::AlignHCenter);
    mainLayout->setAlignment(exitButton, Qt::AlignHCenter);

    setLayout(mainLayout);

    // Buttons Logic
    connect(exitButton, &QPushButton::clicked, this, &MainMenu::exitApplication);
    connect(registerButton, &QPushButton::clicked, this, &MainMenu::registerLogic);
    connect(loginButton, &QPushButton::clicked, this, &MainMenu::loginLogic);
}

void MainMenu::setBackground(const QString &path)
{
    QPalette palette;
    palette.setBrush(QPalette::Window,
                     QBrush(QPixmap(path).scaled(size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));
    setAutoFillBackground(true);
    setPalette(palette);
}

QVector<QWidget*> MainMenu::getResizableWidgets() const
{
    return { titleLabel, loginButton, registerButton, exitButton };
}


void MainMenu::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);

    // Adjustable Values here
    int baseTitleSize = 24;
    int baseButtonSize = 14;

    Resizer::delayedResize(this, size(), getResizableWidgets(), baseTitleSize, baseButtonSize);

    // Resize image *only* if the size has changed significantly
    static QSize lastSize; // Store the last size to avoid unnecessary updates
    if (event->size() != lastSize) {
        lastSize = event->size();

        if (!originalImage.isNull()) {
            int imgWidth = this->width() * 0.7;
            int imgHeight = this->height() * 0.7;
            imageLabel->setPixmap(originalImage.scaled(
                imgWidth, imgHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        }

        // Refresh background
        setBackground(":/img/Images/background.jpg");
    }
}

// resize logic - END

void MainMenu::exitApplication()
{
    QApplication::quit(); // This properly closes the entire app
}

void MainMenu::registerLogic()
{
    Register *r = new Register(nullptr);
    r->show();
    this->close();
}

void MainMenu::loginLogic()
{
    Login *l = new Login(nullptr);
    l->show();
    this->close();
}
