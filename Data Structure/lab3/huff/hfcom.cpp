#include "hfcom.h"

hfcom::hfcom(QWidget *parent) : QMainWindow(parent)
{
    this->resize(400, 247);
    this->setMinimumSize(450, 278);
    this->setMaximumSize(450, 278);
    this->setWindowIcon(QIcon("hflog.ico"));

    ps = new QStatusBar(this);
    ps->setGeometry(0,241,450,37);
    ps->showMessage(QString::fromLocal8Bit("ps:不支持中文路径和文件名"));

    button_open1 = new QPushButton(QString::fromLocal8Bit("打开文件"), this);
    button_open1->setGeometry(20, 40, 80, 37);
    path1 = new QLineEdit(this);
    path1->setReadOnly(true);
    path1->setGeometry(110, 40, 200, 37);
    button_compress = new QPushButton(QString::fromLocal8Bit("压缩"), this);
    button_compress->setGeometry(350, 40, 80, 37);


    button_open2 = new QPushButton(QString::fromLocal8Bit("打开文件"), this);
    button_open2->setGeometry(20, 140, 80, 37);
    path2 = new QLineEdit(this);
    path2->setReadOnly(true);
    path2->setGeometry(110, 140, 200, 37);
    button_decompress = new QPushButton(QString::fromLocal8Bit("解压"), this);
    button_decompress->setGeometry(350, 140, 80, 37);



    QObject::connect(button_compress, &QPushButton::clicked, this, &hfcom::compress);
    QObject::connect(button_decompress, &QPushButton::clicked, this, &hfcom::decompress);
    QObject::connect(button_open1, &QPushButton::clicked, this, &hfcom::open_compressed_file);
    QObject::connect(button_open2, &QPushButton::clicked, this, &hfcom::open_decompressed_file);
    //ui.setupUi(this);
}

hfcom::~hfcom()
{
}

void hfcom::compress()
{
    if( Compress((compressed_filename->toStdString()).c_str()) == true)
        QMessageBox::information(this, tr("Information"), QString::fromLocal8Bit("压缩成功"));
    else
        QMessageBox::information(this, tr("Information"), QString::fromLocal8Bit("压缩失败"));
}
void hfcom::decompress()
{
    if( Decompress((decompressed_filename->toStdString()).c_str()) == true )
        QMessageBox::information(this, tr("Information"), QString::fromLocal8Bit("解压成功"));
    else
        QMessageBox::information(this, tr("Information"), QString::fromLocal8Bit("解压失败"));
}
void hfcom::open_compressed_file()
{
    compressed_filename = new QString(
            QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("打开要压缩的文件"), "", tr("Allfile(*.*)"))
        );
    path1->setText(*compressed_filename);
}
void hfcom::open_decompressed_file()
{
    decompressed_filename = new QString(
            QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("打开要解压的文件"), "", tr("hffile(*.huff)"))
        );
    path2->setText(*decompressed_filename);
}

