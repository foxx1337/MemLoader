#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QProgressBar>
#include <QMutex>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void startThreads();
    void stopThreads();
    void updateProgressBar(int indicator, int value);

signals:
    void workProgressed(int indicator, int value);

private:
    Ui::MainWindow *ui;
    QVector<QProgressBar *> progressBars_;

    QMutex lockStop_;
    bool canContinue;

    bool isCalculationDone();
    void setupButtonState(bool isIdle);
};

#endif // MAINWINDOW_H
