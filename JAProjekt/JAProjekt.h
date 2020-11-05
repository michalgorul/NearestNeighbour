#pragma once
#include <fstream>
#include <iostream>
#include <QtWidgets/QWidget>
#include "ui_JAProjekt.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QDir>
#include <QPixmap>
#include <string>
#include <Windows.h>
#include <algorithm>
#include <QTextCodec>

#define Q(txt) QString::fromWCharArray(L##txt)

class JAProjekt : public QWidget
{
    Q_OBJECT

public:
    /// <summary>QT constructor</summary>
    JAProjekt(QWidget *parent = Q_NULLPTR);

    /// <summary>Function turns on MakeButton in GUI</summary>
    void isReadyToScale();

    /// <summary>Function updates paths to both source and output files</summary>
    void updatePaths();
	
    /// <summary>Function sets atributes scaleX, scaleY and threads taken from GUI</summary>
    void getScaleAndThreads();

private:
    Ui::JAProjektClass ui;

    QString fileInPath;
    QString fileOutPath;
    std::string fileInPathString;
    std::string fileOutPathString;

    bool ifFileInChoosen = false;
    bool ifFileOutChoosen = false;
    unsigned int scaleX = 0;
    unsigned int scaleY = 0;
    unsigned int threads = 1;

    QString time;       //in here ill store execution time ready to display in GUI
	
	// filter in dialog window while choosing file
    const QString filter = "BitMap (*.bmp)";

public slots:
    /// <summary>Function responsible for handling actions after clicking FileInButton in GUI</summary>
    void JAProjekt::onFileInButtonClicked();

    /// <summary>Function responsible for handling actions after clicking FileOutButton in GUI</summary>
    void JAProjekt::onFileOutButtonClicked();

    /// <summary>Function responsible for handling actions after clicking MakeButton in GUI</summary>
    void JAProjekt::onMakeButtonClicked();
};

/// <summary>Function gives us n last characters of string</summary>
/// <param name="input">Processed string</param>
/// <param name="n">Number of characters we want to return</param>
/// <returns>N last characters in string</returns>
std::string lastN(std::string input, int n);

