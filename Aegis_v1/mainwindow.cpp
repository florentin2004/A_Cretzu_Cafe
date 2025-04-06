#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "register.h"
#include "resizer.h"
#include <QResizeEvent>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QTimer>  // Include QTimer for delayed resizing

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout *layout = new QVBoxLayout(centralWidget);

    // Title Label
    titleLabel = new QLabel("AEGIS", this);
    titleLabel->setAlignment(Qt::AlignCenter);

    // Quote section Label
    quoteLabel = new QLabel("We work in the dark to serve the light", this);
    quoteLabel->setAlignment(Qt::AlignCenter);

    // Login & Register Buttons
    loginButton = new QPushButton("Login", this);
    registerButton = new QPushButton("Register", this);
    connect(registerButton, &QPushButton::clicked, this, &MainWindow::openRegisterForm);

    // Apply size scaling policies
    loginButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    registerButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // Vertical Layout for Buttons
    QVBoxLayout *buttonLayout = new QVBoxLayout();
    buttonLayout->addWidget(loginButton);
    buttonLayout->addWidget(registerButton);

    // Add widgets to the main layout
    layout->addWidget(titleLabel);
    layout->addWidget(quoteLabel);
    layout->addStretch(1);
    layout->addLayout(buttonLayout);

    // ** Use `Resizer::delayedResize()` for stability**
    QVector<QWidget*> widgets = {titleLabel, quoteLabel, loginButton, registerButton};
    QVector<int> fontSizes = {size().width() / 15, size().width() / 20, size().width() / 12, size().width() / 18};

    Resizer::delayedResize(this, size(), widgets, fontSizes);
}

// **🔹 Prevent Infinite Loop in Resize Event**
void MainWindow::resizeEvent(QResizeEvent* event)
{
    if (event->size() == size()) return;  // Avoid unnecessary recursive calls

    QVector<QWidget*> widgets = {titleLabel, quoteLabel, loginButton, registerButton};
    QVector<int> fontSizes = {event->size().width() / 15, event->size().width() / 20, event->size().width() / 12, event->size().width() / 18};

    Resizer::resizeFonts(event->size(), widgets, fontSizes);
    QMainWindow::resizeEvent(event);
}

// **Open Register form**
void MainWindow::openRegisterForm()
{
    Register *registerForm = new Register();
    registerForm->show();  // Show Register window
    this->close();  // Close MainWindow
}

MainWindow::~MainWindow()
{
    delete ui;
}
