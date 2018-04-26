#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vector>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_saveFile_clicked();

    void on_action_About_triggered();

private:
    Ui::MainWindow *ui;

    unsigned long codeCount;
    std::vector<char> uniqueCode;
    std::vector<QString> allCodes;

    void Init();
    char Ch(int x);
    QString GenerateUniqueCode();
    void FillUniqueCodes();
    void CreateFile();
    bool CheckUniqueness();

};

#endif // MAINWINDOW_H
