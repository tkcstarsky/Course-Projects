#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include<iomanip>
#include<fstream>
#include<string>
#include <QMainWindow>
#include"dijkstra.h"
#include <QMainWindow>
#include <qpushbutton.h>
#include <qobject.h>
#include <qstring.h>
#include <qfiledialog.h>
#include <qlineedit.h>
#include <qmessagebox.h>
#include <qstatusbar.h>
#include <qlabel.h>
#include <qicon.h>
#include <QtWidgets/QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QPushButton *button_open1;
    QLineEdit *path1;
    QPushButton *button_compress;
    QString *compressed_filename;
    void Dijkstra();
    void open_compressed_file();
};

#endif // MAINWINDOW_H
