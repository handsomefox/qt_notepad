#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QApplication>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QPrintDialog>
#include <QPrinter>
#include <QStyleFactory>
#include <QTextStream>
#include <QtPlugin>

void ApplyDarkTheme()
{
    qApp->setStyle(QStyleFactory::create("Fusion"));

    QPalette darkPalette;
    darkPalette.setColor(QPalette::Window, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::WindowText, Qt::white);
    darkPalette.setColor(QPalette::Base, QColor(25, 25, 25));
    darkPalette.setColor(QPalette::AlternateBase, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
    darkPalette.setColor(QPalette::ToolTipText, Qt::white);
    darkPalette.setColor(QPalette::Text, Qt::white);
    darkPalette.setColor(QPalette::Button, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::ButtonText, Qt::white);
    darkPalette.setColor(QPalette::BrightText, Qt::red);
    darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::HighlightedText, Qt::black);

    qApp->setPalette(darkPalette);
    qApp->setStyleSheet("QToolTip { color: #ffffff; background-color: #2a82da; border: 1px solid white; }");
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Notepad");
    this->setCentralWidget(ui->textEdit);
    ApplyDarkTheme();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionExit_triggered()
{
    QApplication::quit();
}

void MainWindow::on_actionPrint_triggered()
{
    QPrinter printer;
    printer.setPrinterName("Printer");
    QPrintDialog printer_dialog(&printer, this);

    if (printer_dialog.exec() == QDialog::Rejected)
    {
        QMessageBox::warning(this, "Warning", "Cannot access printer");
        return;
    }

    ui->textEdit->print(&printer);
}

void MainWindow::on_actionOpen_triggered()
{
    QString file_path = QFileDialog::getOpenFileName((this), "Open file");
    QFile file(file_path);

    current_file = file_path;

    if (!file.open(QIODevice::ReadOnly | QFile::Text))
    {
        QMessageBox::warning(this, "Warning", "Couldn't open the file: " + file.errorString());
        return;
    }

    setWindowTitle(file_path);

    QTextStream input_stream(&file);
    QString text = input_stream.readAll();
    ui->textEdit->setText(text);
    file.close();
}

void MainWindow::on_actionNew_triggered()
{
    if (!ui->textEdit->toPlainText().isEmpty())
    {
        if (current_file.isEmpty())
        {
            auto reply = QMessageBox::question(this, "Warning", "File has unsaved changes\n\nSave?", QMessageBox::Yes | QMessageBox::No);
            if (reply == QMessageBox::Yes)
            {
                on_actionSave_as_triggered();
            }
            return;
        }
        else
        {
            QFile file(current_file);
            if (!file.open(QFile::ReadOnly | QFile::Text))
            {
                QMessageBox::warning(this, "Warning", "Couldn't save the file: " + file.errorString());
                return;
            }
            QTextStream input_stream(&file);
            QString file_text = input_stream.readAll();

            if (file_text == ui->textEdit->toPlainText())
            {
                return;
            }
            else
            {
                auto reply = QMessageBox::question(this, "Warning", "File has unsaved changes\n\nSave?", QMessageBox::Yes | QMessageBox::No);
                if (reply == QMessageBox::Yes)
                {
                    on_actionSave_triggered();
                }
            }
        }
    }

    current_file.clear();
    ui->textEdit->setText(QString());
}

void MainWindow::on_actionSave_triggered()
{
    if (current_file.isEmpty())
    {
        on_actionSave_as_triggered();
        return;
    }
    QFile file(current_file);

    if (!file.open(QFile::WriteOnly | QFile::Text))
    {
        QMessageBox::warning(this, "Warning", "Couldn't save the file: " + file.errorString());
        return;
    }

    QTextStream output_stream(&file);
    output_stream << ui->textEdit->toPlainText();
    file.close();
}

void MainWindow::on_actionSave_as_triggered()
{
    QString file_path = QFileDialog::getSaveFileName((this), "Save as");
    QFile file(file_path);

    current_file = file_path;

    if (!file.open(QFile::WriteOnly | QFile::Text))
    {
        QMessageBox::warning(this, "Warning", "Couldn't save the file: " + file.errorString());
        return;
    }
    setWindowTitle(file_path);

    QTextStream output_stream(&file);
    output_stream << ui->textEdit->toPlainText();
    file.close();
}

void MainWindow::on_actionCopy_triggered()
{
    ui->textEdit->copy();
}

void MainWindow::on_actionPaste_triggered()
{
    ui->textEdit->paste();
}

void MainWindow::on_actionCut_triggered()
{
    ui->textEdit->cut();
}

void MainWindow::on_actionUndo_triggered()
{
    ui->textEdit->undo();
}

void MainWindow::on_actionRedo_triggered()
{
    ui->textEdit->copy();
}
