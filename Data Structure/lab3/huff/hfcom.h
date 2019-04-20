
#ifndef HFCOM_H
#define HFCOM_H

#include <QtWidgets/QMainWindow>
#include "ui_hfcom.h"
#include <qpushbutton.h>
#include <qobject.h>
#include <qstring.h>
#include <qfiledialog.h>
#include <qlineedit.h>
#include <qmessagebox.h>
#include <qstatusbar.h>
#include <qlabel.h>
#include <qicon.h>
#include "hf.h"

class hfcom : public QMainWindow
{
    Q_OBJECT

public:
    hfcom(QWidget *parent = 0);
    ~hfcom();

private:
    QPushButton *button_open1;
    QLineEdit *path1;
    QPushButton *button_compress;
    QString *compressed_filename;
    QStatusBar *ps;
    void compress();
    void open_compressed_file();

    QPushButton *button_open2;
    QLineEdit *path2;
    QPushButton *button_decompress;
    QString *decompressed_filename;
    void decompress();
    void open_decompressed_file();
};

#endif // HFCOM_H

