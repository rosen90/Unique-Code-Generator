#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QTextStream>
#include <QFile>
#include <QFileDialog>
#include <algorithm>

QString globalMsg = "";
bool success = false;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle(tr("Unique Code Generator - Free to use"));

    ui->progressBar->setMinimum(0);
    ui->progressBar->setMaximum(100);
    ui->progressBar->setValue(0);
    ui->codeCount->setValidator( new QIntValidator(0, 1000000, this) );
    ui->saveFile->setVisible(false);
}

void MainWindow::Init()
{
    codeCount = ui->codeCount->text().toInt();
    uniqueCode.clear();
    allCodes.clear();
    ui->progressBar->setMinimum(0);
    ui->progressBar->setMaximum(100);
    ui->progressBar->setValue(0);

}

char MainWindow::Ch(int x)
{
    if(x < 10) return char(x+48);
    if(x < 36) return char(x+55);
    if(x < 62) return char(x+61);
    return '?';
}

QString MainWindow::GenerateUniqueCode()
{
    srand(rand()); srand(rand());

    QString code = "";

    for(int i = 0; i < 6; i++)
    {
        code += Ch(rand()%62);
    }

    return code;
}

void MainWindow::FillUniqueCodes()
{
    for(unsigned int i = 0; i < codeCount; i++)
    {
        allCodes.push_back(GenerateUniqueCode().toUpper());
    }
}

void MainWindow::CreateFile()
{

    QString filePath = QFileDialog::getSaveFileName(this,
            tr("Save File Dialog"), tr("Generated_Codes.txt"),
            tr("Text file (*.txt);;All Files (*)"));

    if(filePath.isEmpty())
        return;

    QFile outputFile(filePath);

    outputFile.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream File(&outputFile);


    if(!allCodes.empty())
    {
        for(unsigned int i = 0; i < codeCount; i++)
        {
            if(i == allCodes.size() - 1)
            {
                File << allCodes[i];
            }
            else
            {
                File << allCodes[i] << "\n";
            }
        }

        success = true;

        outputFile.close();
    }
    else
    {
        success = false;
    }
}

bool MainWindow::CheckUniqueness()
{
    std::sort   ( allCodes.begin(), allCodes.end());
    std::unique ( allCodes.begin(), allCodes.end());
    std::random_shuffle ( allCodes.begin(), allCodes.end());

    if(allCodes.size() < codeCount)
    {
        for(unsigned int i = 0; i < (codeCount - allCodes.size()); i++)
        {
            allCodes.push_back(GenerateUniqueCode().toUpper());
        }

        CheckUniqueness();
    }

    return allCodes.size() == codeCount ? true : false;
}

void MainWindow::on_pushButton_clicked()
{
    Init();
    ui->console->clear();
    ui->ListAllCodes->clear();

    if( ui->codeCount->text().isEmpty() || codeCount <= 0)
    {
        QMessageBox::information(this,"Error", "Please enter how many codes you want to be generated!");
    }
    else
    {
        FillUniqueCodes();

        if(CheckUniqueness())
        {
            for(unsigned int i = 0; i < allCodes.size(); i++)
            {
                ui->ListAllCodes->append(allCodes[i]);

                int progress = (static_cast<double>(i+1)/static_cast<double>(allCodes.size()))*100.00;

                 ui->progressBar->setValue(progress);
            }

            ui->ListAllCodes->append("");

            globalMsg = "Generating Codes...[Success]\n";
            ui->console->append(globalMsg);
        }

        globalMsg = QString::number(allCodes.size()) + " unique codes were generated.\n";
        ui->console->append(globalMsg);

        QTextCursor c =  ui->ListAllCodes->textCursor();
        c.movePosition(QTextCursor::End);
        ui->ListAllCodes->setTextCursor(c);
        ui->ListAllCodes->ensureCursorVisible();

        ui->saveFile->setVisible(true);
    }
}

void MainWindow::on_saveFile_clicked()
{
    CreateFile();

    if(success)
    {
        globalMsg = "Creating output file...[sucsess]\n";
        ui->console->append(globalMsg);
    }
    else
    {
        globalMsg = "Creating output file...[failed]\n";
        ui->console->append(globalMsg);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_action_About_triggered()
{
    QMessageBox::information(this, "About", "Author: R. Karadinev \n\nGenerate countless unique codes for the needs of the JENVIS Research project");
}
