/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QStackedWidget *Window_logic;
    QWidget *MainMenu;
    QGridLayout *gridLayout_2;
    QWidget *left_twix;
    QVBoxLayout *verticalLayout_2;
    QLabel *title_label;
    QSpacerItem *verticalSpacer_10;
    QLabel *logo_label;
    QPushButton *LoginButton;
    QPushButton *RegisterButton;
    QPushButton *ExitButton;
    QStackedWidget *right_twix;
    QWidget *login_page;
    QHBoxLayout *horizontalLayout_2;
    QVBoxLayout *verticalLayout_3;
    QSpacerItem *verticalSpacer_9;
    QLabel *user_label;
    QLineEdit *user_lineEdit;
    QSpacerItem *verticalSpacer_6;
    QLabel *password_label;
    QLineEdit *password_lineEdit;
    QRadioButton *PasswordButton;
    QSpacerItem *verticalSpacer_7;
    QPushButton *LoginButton_2;
    QSpacerItem *verticalSpacer_8;
    QWidget *register_page;
    QWidget *exit_page;
    QHBoxLayout *horizontalLayout;
    QGridLayout *gridLayout;
    QSpacerItem *horizontalSpacer;
    QSpacerItem *verticalSpacer_2;
    QLabel *question_label;
    QSpacerItem *verticalSpacer_5;
    QSpacerItem *verticalSpacer_3;
    QPushButton *noButton;
    QPushButton *yesButton;
    QSpacerItem *verticalSpacer_4;
    QSpacerItem *horizontalSpacer_2;
    QSpacerItem *horizontalSpacer_3;
    QWidget *idle_page;
    QWidget *App;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(955, 723);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        centralwidget->setStyleSheet(QString::fromUtf8("background-color: rgb(30, 144, 255);"));
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        Window_logic = new QStackedWidget(centralwidget);
        Window_logic->setObjectName("Window_logic");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Window_logic->sizePolicy().hasHeightForWidth());
        Window_logic->setSizePolicy(sizePolicy);
        MainMenu = new QWidget();
        MainMenu->setObjectName("MainMenu");
        gridLayout_2 = new QGridLayout(MainMenu);
        gridLayout_2->setObjectName("gridLayout_2");
        left_twix = new QWidget(MainMenu);
        left_twix->setObjectName("left_twix");
        left_twix->setStyleSheet(QString::fromUtf8("background-color: rgb(30, 107, 255);\n"
"font: \"Vineta BT\";"));
        verticalLayout_2 = new QVBoxLayout(left_twix);
        verticalLayout_2->setSpacing(30);
        verticalLayout_2->setObjectName("verticalLayout_2");
        title_label = new QLabel(left_twix);
        title_label->setObjectName("title_label");
        sizePolicy.setHeightForWidth(title_label->sizePolicy().hasHeightForWidth());
        title_label->setSizePolicy(sizePolicy);
        QFont font;
        font.setFamilies({QString::fromUtf8("Vineta BT")});
        font.setPointSize(28);
        font.setBold(false);
        font.setItalic(false);
        title_label->setFont(font);

        verticalLayout_2->addWidget(title_label, 0, Qt::AlignmentFlag::AlignHCenter|Qt::AlignmentFlag::AlignTop);

        verticalSpacer_10 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_2->addItem(verticalSpacer_10);

        logo_label = new QLabel(left_twix);
        logo_label->setObjectName("logo_label");
        logo_label->setMinimumSize(QSize(50, 200));
        logo_label->setStyleSheet(QString::fromUtf8("image: url(:/img/Images/Aegis-removebg.png);"));

        verticalLayout_2->addWidget(logo_label);

        LoginButton = new QPushButton(left_twix);
        LoginButton->setObjectName("LoginButton");
        QSizePolicy sizePolicy1(QSizePolicy::Policy::Minimum, QSizePolicy::Policy::MinimumExpanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(LoginButton->sizePolicy().hasHeightForWidth());
        LoginButton->setSizePolicy(sizePolicy1);
        QFont font1;
        font1.setFamilies({QString::fromUtf8("Vineta BT")});
        font1.setPointSize(18);
        font1.setBold(false);
        font1.setItalic(false);
        LoginButton->setFont(font1);
        LoginButton->setStyleSheet(QString::fromUtf8("/* Login Button (Forest Green) */\n"
"QPushButton#LoginButton {\n"
"    background-color: #228b22; /* Forest Green */\n"
"    color: white; /* Keep text white */\n"
"    border-radius: 10px;\n"
"    padding: 8px;\n"
"}\n"
"\n"
"QPushButton#LoginButton:hover {\n"
"    background-color: #1c741c; /* Slightly darker shade on hover */\n"
"}\n"
"\n"
"QPushButton#LoginButton:pressed {\n"
"    background-color: #135813; /* Deep green when pressed */\n"
"}"));

        verticalLayout_2->addWidget(LoginButton, 0, Qt::AlignmentFlag::AlignBottom);

        RegisterButton = new QPushButton(left_twix);
        RegisterButton->setObjectName("RegisterButton");
        sizePolicy1.setHeightForWidth(RegisterButton->sizePolicy().hasHeightForWidth());
        RegisterButton->setSizePolicy(sizePolicy1);
        RegisterButton->setFont(font1);
        RegisterButton->setStyleSheet(QString::fromUtf8("/* Register Button (Orange) */\n"
"QPushButton#RegisterButton {\n"
"    background-color: #ffa500; /* Orange */\n"
"    color: white;\n"
"    border-radius: 10px;\n"
"    padding: 8px;\n"
"}\n"
"\n"
"QPushButton#RegisterButton:hover {\n"
"    background-color: #e69500; /* Darker orange */\n"
"}\n"
"\n"
"QPushButton#RegisterButton:pressed {\n"
"    background-color: #cc8400; /* Pressed effect */\n"
"}\n"
""));

        verticalLayout_2->addWidget(RegisterButton);

        ExitButton = new QPushButton(left_twix);
        ExitButton->setObjectName("ExitButton");
        sizePolicy1.setHeightForWidth(ExitButton->sizePolicy().hasHeightForWidth());
        ExitButton->setSizePolicy(sizePolicy1);
        ExitButton->setFont(font1);
        ExitButton->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: #ff6347; /* Default button color (Tomato) */\n"
"    color: white; /* Keep the text white */\n"
"    border-radius: 10px; /* Smooth edges */\n"
"    padding: 8px;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #cc523b; /* Slightly darker shade on hover */\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: #a83e29; /* Even darker when pressed */\n"
"}"));

        verticalLayout_2->addWidget(ExitButton);


        gridLayout_2->addWidget(left_twix, 0, 0, 1, 1);

        right_twix = new QStackedWidget(MainMenu);
        right_twix->setObjectName("right_twix");
        QFont font2;
        font2.setFamilies({QString::fromUtf8("Vineta BT")});
        font2.setBold(false);
        font2.setItalic(false);
        right_twix->setFont(font2);
        right_twix->setStyleSheet(QString::fromUtf8("font: \"Verdana\";"));
        login_page = new QWidget();
        login_page->setObjectName("login_page");
        login_page->setStyleSheet(QString::fromUtf8("/* Login Button (Forest Green) */\n"
"QPushButton#LoginButton_2 {\n"
"    background-color: #228b22; /* Forest Green */\n"
"    color: white; /* Keep text white */\n"
"    border-radius: 10px;\n"
"    padding: 8px;\n"
"}\n"
"\n"
"QPushButton#LoginButton_2:hover {\n"
"    background-color: #1c741c; /* Slightly darker shade on hover */\n"
"}\n"
"\n"
"QPushButton#LoginButton_2:pressed {\n"
"    background-color: #135813; /* Deep green when pressed */\n"
"}"));
        horizontalLayout_2 = new QHBoxLayout(login_page);
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName("verticalLayout_3");
        verticalSpacer_9 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_3->addItem(verticalSpacer_9);

        user_label = new QLabel(login_page);
        user_label->setObjectName("user_label");
        user_label->setFont(font1);

        verticalLayout_3->addWidget(user_label, 0, Qt::AlignmentFlag::AlignHCenter);

        user_lineEdit = new QLineEdit(login_page);
        user_lineEdit->setObjectName("user_lineEdit");
        QSizePolicy sizePolicy2(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::MinimumExpanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(user_lineEdit->sizePolicy().hasHeightForWidth());
        user_lineEdit->setSizePolicy(sizePolicy2);
        QFont font3;
        font3.setFamilies({QString::fromUtf8("Vineta BT")});
        font3.setPointSize(22);
        font3.setBold(false);
        font3.setItalic(false);
        user_lineEdit->setFont(font3);

        verticalLayout_3->addWidget(user_lineEdit);

        verticalSpacer_6 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_3->addItem(verticalSpacer_6);

        password_label = new QLabel(login_page);
        password_label->setObjectName("password_label");
        password_label->setFont(font1);

        verticalLayout_3->addWidget(password_label, 0, Qt::AlignmentFlag::AlignHCenter);

        password_lineEdit = new QLineEdit(login_page);
        password_lineEdit->setObjectName("password_lineEdit");
        sizePolicy2.setHeightForWidth(password_lineEdit->sizePolicy().hasHeightForWidth());
        password_lineEdit->setSizePolicy(sizePolicy2);
        QFont font4;
        font4.setFamilies({QString::fromUtf8("Vineta BT")});
        font4.setPointSize(20);
        font4.setBold(false);
        font4.setItalic(false);
        password_lineEdit->setFont(font4);
        password_lineEdit->setEchoMode(QLineEdit::EchoMode::Password);

        verticalLayout_3->addWidget(password_lineEdit);

        PasswordButton = new QRadioButton(login_page);
        PasswordButton->setObjectName("PasswordButton");
        QSizePolicy sizePolicy3(QSizePolicy::Policy::MinimumExpanding, QSizePolicy::Policy::MinimumExpanding);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(PasswordButton->sizePolicy().hasHeightForWidth());
        PasswordButton->setSizePolicy(sizePolicy3);
        QFont font5;
        font5.setFamilies({QString::fromUtf8("Vineta BT")});
        font5.setPointSize(16);
        font5.setBold(false);
        font5.setItalic(false);
        PasswordButton->setFont(font5);

        verticalLayout_3->addWidget(PasswordButton);

        verticalSpacer_7 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_3->addItem(verticalSpacer_7);

        LoginButton_2 = new QPushButton(login_page);
        LoginButton_2->setObjectName("LoginButton_2");
        sizePolicy3.setHeightForWidth(LoginButton_2->sizePolicy().hasHeightForWidth());
        LoginButton_2->setSizePolicy(sizePolicy3);
        LoginButton_2->setFont(font1);
        LoginButton_2->setStyleSheet(QString::fromUtf8("/* Login Button (Green) */\n"
"QPushButton#LoginButton {\n"
"    background-color: #32cd32; /* Lime Green */\n"
"    color: white;\n"
"    border-radius: 10px;\n"
"    padding: 8px;\n"
"}\n"
"\n"
"QPushButton#LoginButton:hover {\n"
"    background-color: #2ca02c; /* Darker green on hover */\n"
"}\n"
"\n"
"QPushButton#LoginButton:pressed {\n"
"    background-color: #228b22; /* Even darker when pressed */\n"
"}\n"
""));

        verticalLayout_3->addWidget(LoginButton_2);

        verticalSpacer_8 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_3->addItem(verticalSpacer_8);


        horizontalLayout_2->addLayout(verticalLayout_3);

        right_twix->addWidget(login_page);
        register_page = new QWidget();
        register_page->setObjectName("register_page");
        right_twix->addWidget(register_page);
        exit_page = new QWidget();
        exit_page->setObjectName("exit_page");
        sizePolicy.setHeightForWidth(exit_page->sizePolicy().hasHeightForWidth());
        exit_page->setSizePolicy(sizePolicy);
        exit_page->setStyleSheet(QString::fromUtf8("QPushButton#yesButton {\n"
"    background-color: #228b22; /* Forest Green */\n"
"    color: white; /* Text remains white */\n"
"    border-radius: 10px;\n"
"    padding: 8px;\n"
"}\n"
"\n"
"QPushButton#yesButton:hover {\n"
"    background-color: #1c741c; /* Slightly darker green */\n"
"}\n"
"\n"
"QPushButton#yesButton:pressed {\n"
"    background-color: #135813; /* Deep green when pressed */\n"
"}\n"
"QPushButton#noButton {\n"
"    background-color: #dc143c; /* Crimson Red */\n"
"    color: white; /* Keep text white */\n"
"    border-radius: 10px;;\n"
"    padding: 8px;\n"
"}\n"
"\n"
"QPushButton#noButton:hover {\n"
"    background-color: #b01130; /* Slightly darker red */\n"
"}\n"
"\n"
"QPushButton#noButton:pressed {\n"
"    background-color: #8a0e26; /* Deep red when pressed */\n"
"}"));
        horizontalLayout = new QHBoxLayout(exit_page);
        horizontalLayout->setObjectName("horizontalLayout");
        gridLayout = new QGridLayout();
        gridLayout->setObjectName("gridLayout");
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        gridLayout->addItem(horizontalSpacer, 2, 2, 1, 1);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        gridLayout->addItem(verticalSpacer_2, 1, 1, 1, 1);

        question_label = new QLabel(exit_page);
        question_label->setObjectName("question_label");
        QFont font6;
        font6.setFamilies({QString::fromUtf8("Vineta BT")});
        font6.setPointSize(24);
        font6.setBold(false);
        font6.setItalic(false);
        question_label->setFont(font6);

        gridLayout->addWidget(question_label, 0, 1, 1, 3, Qt::AlignmentFlag::AlignHCenter|Qt::AlignmentFlag::AlignTop);

        verticalSpacer_5 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        gridLayout->addItem(verticalSpacer_5, 3, 3, 1, 1);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        gridLayout->addItem(verticalSpacer_3, 1, 3, 1, 1);

        noButton = new QPushButton(exit_page);
        noButton->setObjectName("noButton");
        QFont font7;
        font7.setFamilies({QString::fromUtf8("Vineta BT")});
        font7.setPointSize(48);
        font7.setBold(false);
        font7.setItalic(false);
        noButton->setFont(font7);

        gridLayout->addWidget(noButton, 2, 3, 1, 1, Qt::AlignmentFlag::AlignVCenter);

        yesButton = new QPushButton(exit_page);
        yesButton->setObjectName("yesButton");
        QFont font8;
        font8.setFamilies({QString::fromUtf8("Vineta BT")});
        font8.setPointSize(14);
        font8.setBold(false);
        font8.setItalic(false);
        yesButton->setFont(font8);
        yesButton->setStyleSheet(QString::fromUtf8(""));

        gridLayout->addWidget(yesButton, 2, 1, 1, 1);

        verticalSpacer_4 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        gridLayout->addItem(verticalSpacer_4, 3, 1, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        gridLayout->addItem(horizontalSpacer_2, 2, 0, 1, 1);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        gridLayout->addItem(horizontalSpacer_3, 2, 4, 1, 1);


        horizontalLayout->addLayout(gridLayout);

        right_twix->addWidget(exit_page);
        idle_page = new QWidget();
        idle_page->setObjectName("idle_page");
        right_twix->addWidget(idle_page);

        gridLayout_2->addWidget(right_twix, 0, 1, 1, 1);

        Window_logic->addWidget(MainMenu);
        App = new QWidget();
        App->setObjectName("App");
        Window_logic->addWidget(App);

        verticalLayout->addWidget(Window_logic);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 955, 25));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        right_twix->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        title_label->setText(QCoreApplication::translate("MainWindow", "Aegis", nullptr));
        logo_label->setText(QString());
        LoginButton->setText(QCoreApplication::translate("MainWindow", "Login", nullptr));
        RegisterButton->setText(QCoreApplication::translate("MainWindow", "Register", nullptr));
        ExitButton->setText(QCoreApplication::translate("MainWindow", "Exit", nullptr));
        user_label->setText(QCoreApplication::translate("MainWindow", "Username/Email", nullptr));
        password_label->setText(QCoreApplication::translate("MainWindow", "Password", nullptr));
        PasswordButton->setText(QCoreApplication::translate("MainWindow", "See Password", nullptr));
        LoginButton_2->setText(QCoreApplication::translate("MainWindow", "Login", nullptr));
        question_label->setText(QCoreApplication::translate("MainWindow", "Are you sure ?", nullptr));
        noButton->setText(QCoreApplication::translate("MainWindow", "No", nullptr));
        yesButton->setText(QCoreApplication::translate("MainWindow", "Yes", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
