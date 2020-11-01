/********************************************************************************
** Form generated from reading UI file 'JAProjekt.ui'
**
** Created by: Qt User Interface Compiler version 5.15.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_JAPROJEKT_H
#define UI_JAPROJEKT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_JAProjektClass
{
public:
    QPushButton *exitButton;
    QPushButton *fileInButton;
    QLabel *imageLabel;
    QPushButton *makeButton;
    QLabel *label_1;
    QLabel *label_2;
    QLineEdit *lineEditX;
    QLineEdit *lineEditY;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label;
    QSpinBox *threadsSpinBox;
    QLabel *label_6;
    QRadioButton *radioButtonAsm;
    QRadioButton *radioButtonCpp;
    QLabel *label_5;
    QLabel *label_7;
    QLabel *label_8;
    QLineEdit *fileInlineEdit;
    QPushButton *fileOutButton;
    QLineEdit *fileOutlineEdit;
    QLineEdit *lineEditTime;
    QLabel *label_9;

    void setupUi(QWidget *JAProjektClass)
    {
        if (JAProjektClass->objectName().isEmpty())
            JAProjektClass->setObjectName(QString::fromUtf8("JAProjektClass"));
        JAProjektClass->resize(700, 792);
        exitButton = new QPushButton(JAProjektClass);
        exitButton->setObjectName(QString::fromUtf8("exitButton"));
        exitButton->setGeometry(QRect(50, 740, 581, 28));
        fileInButton = new QPushButton(JAProjektClass);
        fileInButton->setObjectName(QString::fromUtf8("fileInButton"));
        fileInButton->setGeometry(QRect(110, 130, 171, 31));
        imageLabel = new QLabel(JAProjektClass);
        imageLabel->setObjectName(QString::fromUtf8("imageLabel"));
        imageLabel->setGeometry(QRect(20, 410, 651, 291));
        imageLabel->setScaledContents(true);
        makeButton = new QPushButton(JAProjektClass);
        makeButton->setObjectName(QString::fromUtf8("makeButton"));
        makeButton->setEnabled(false);
        makeButton->setGeometry(QRect(230, 360, 231, 28));
        label_1 = new QLabel(JAProjektClass);
        label_1->setObjectName(QString::fromUtf8("label_1"));
        label_1->setGeometry(QRect(300, 240, 55, 16));
        label_2 = new QLabel(JAProjektClass);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(300, 270, 55, 16));
        lineEditX = new QLineEdit(JAProjektClass);
        lineEditX->setObjectName(QString::fromUtf8("lineEditX"));
        lineEditX->setGeometry(QRect(350, 240, 61, 22));
        lineEditY = new QLineEdit(JAProjektClass);
        lineEditY->setObjectName(QString::fromUtf8("lineEditY"));
        lineEditY->setGeometry(QRect(350, 270, 61, 22));
        label_3 = new QLabel(JAProjektClass);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(413, 243, 55, 16));
        label_4 = new QLabel(JAProjektClass);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(413, 272, 55, 16));
        label = new QLabel(JAProjektClass);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(190, 710, 101, 20));
        threadsSpinBox = new QSpinBox(JAProjektClass);
        threadsSpinBox->setObjectName(QString::fromUtf8("threadsSpinBox"));
        threadsSpinBox->setGeometry(QRect(360, 330, 42, 22));
        threadsSpinBox->setMinimum(1);
        threadsSpinBox->setMaximum(64);
        label_6 = new QLabel(JAProjektClass);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(270, 330, 91, 16));
        radioButtonAsm = new QRadioButton(JAProjektClass);
        radioButtonAsm->setObjectName(QString::fromUtf8("radioButtonAsm"));
        radioButtonAsm->setGeometry(QRect(370, 300, 95, 20));
        radioButtonAsm->setChecked(true);
        radioButtonCpp = new QRadioButton(JAProjektClass);
        radioButtonCpp->setObjectName(QString::fromUtf8("radioButtonCpp"));
        radioButtonCpp->setGeometry(QRect(240, 300, 95, 20));
        radioButtonCpp->setChecked(false);
        label_5 = new QLabel(JAProjektClass);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(100, 10, 501, 41));
        QFont font;
        font.setFamily(QString::fromUtf8("Arial"));
        font.setPointSize(18);
        font.setBold(true);
        font.setWeight(75);
        label_5->setFont(font);
        label_5->setLayoutDirection(Qt::LeftToRight);
        label_5->setAutoFillBackground(false);
        label_7 = new QLabel(JAProjektClass);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(100, 60, 511, 41));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Arial Black"));
        font1.setPointSize(16);
        font1.setBold(true);
        font1.setWeight(75);
        label_7->setFont(font1);
        label_8 = new QLabel(JAProjektClass);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(590, 770, 111, 16));
        fileInlineEdit = new QLineEdit(JAProjektClass);
        fileInlineEdit->setObjectName(QString::fromUtf8("fileInlineEdit"));
        fileInlineEdit->setGeometry(QRect(292, 130, 300, 31));
        fileInlineEdit->setReadOnly(true);
        fileOutButton = new QPushButton(JAProjektClass);
        fileOutButton->setObjectName(QString::fromUtf8("fileOutButton"));
        fileOutButton->setGeometry(QRect(110, 167, 171, 31));
        fileOutlineEdit = new QLineEdit(JAProjektClass);
        fileOutlineEdit->setObjectName(QString::fromUtf8("fileOutlineEdit"));
        fileOutlineEdit->setEnabled(true);
        fileOutlineEdit->setGeometry(QRect(292, 167, 300, 31));
        fileOutlineEdit->setReadOnly(false);
        lineEditTime = new QLineEdit(JAProjektClass);
        lineEditTime->setObjectName(QString::fromUtf8("lineEditTime"));
        lineEditTime->setGeometry(QRect(300, 710, 221, 23));
        lineEditTime->setReadOnly(true);
        label_9 = new QLabel(JAProjektClass);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setGeometry(QRect(95, 200, 511, 20));

        retranslateUi(JAProjektClass);
        QObject::connect(exitButton, SIGNAL(clicked()), JAProjektClass, SLOT(close()));

        QMetaObject::connectSlotsByName(JAProjektClass);
    } // setupUi

    void retranslateUi(QWidget *JAProjektClass)
    {
        JAProjektClass->setWindowTitle(QCoreApplication::translate("JAProjektClass", "JAProjekt", nullptr));
        exitButton->setText(QCoreApplication::translate("JAProjektClass", "Zamknij", nullptr));
        fileInButton->setText(QCoreApplication::translate("JAProjektClass", "\305\232cie\305\274ka pliku wej\305\233ciowego", nullptr));
        imageLabel->setText(QString());
        makeButton->setText(QCoreApplication::translate("JAProjektClass", "Skaluj", nullptr));
        label_1->setText(QCoreApplication::translate("JAProjektClass", "Skala X:", nullptr));
        label_2->setText(QCoreApplication::translate("JAProjektClass", "Skala Y:", nullptr));
        lineEditX->setText(QCoreApplication::translate("JAProjektClass", "100", nullptr));
        lineEditY->setText(QCoreApplication::translate("JAProjektClass", "100", nullptr));
        label_3->setText(QCoreApplication::translate("JAProjektClass", "%", nullptr));
        label_4->setText(QCoreApplication::translate("JAProjektClass", "%", nullptr));
        label->setText(QCoreApplication::translate("JAProjektClass", "Czas wykonania:", nullptr));
        label_6->setText(QCoreApplication::translate("JAProjektClass", "Ilo\305\233\304\207 w\304\205tk\303\263w:", nullptr));
        radioButtonAsm->setText(QCoreApplication::translate("JAProjektClass", "Assembler", nullptr));
        radioButtonCpp->setText(QCoreApplication::translate("JAProjektClass", "C++", nullptr));
        label_5->setText(QCoreApplication::translate("JAProjektClass", "Projekt z J\304\231zyk\303\263w Asemblerowych", nullptr));
        label_7->setText(QCoreApplication::translate("JAProjektClass", "Algorytm najbli\305\274szego s\304\205siedztwa", nullptr));
        label_8->setText(QCoreApplication::translate("JAProjektClass", "Micha\305\202 G\303\263ral gr. 2  ", nullptr));
        fileOutButton->setText(QCoreApplication::translate("JAProjektClass", "\305\232cie\305\274ka pliku wynikowego", nullptr));
        label_9->setText(QCoreApplication::translate("JAProjektClass", "Nale\305\274y wybra\304\207 plik z rozszerzeniem .bmp. Program obs\305\202uguje jedynie bitmapy 24 bitowe ", nullptr));
    } // retranslateUi

};

namespace Ui {
    class JAProjektClass: public Ui_JAProjektClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_JAPROJEKT_H
