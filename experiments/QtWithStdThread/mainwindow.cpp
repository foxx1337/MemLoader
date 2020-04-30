#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMutexLocker>

#include <vector>
#include <functional>
#include <thread>
#include <chrono>

using std::vector;
using std::thread;

typedef std::function<bool()> continuation_proc;
typedef std::function<void(int, int)> progress_proc;

void thread_setup(int num_threads,
    continuation_proc can_continue,
    progress_proc progress_callback)
{
    vector<thread> workers;

    for (int i = 0; i < num_threads; i++)
    {
        workers.emplace_back(thread(
            [i, can_continue, progress_callback]()
            {
                for (int step = 1; can_continue() && step <= 100; step++)
                {
                    std::this_thread::sleep_for(std::chrono::milliseconds(100));
                    progress_callback(i, step);
                }
            }
        ));
    }

    for (auto &worker : workers)
    {
        worker.detach();
    }
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    progressBars_ << ui->progressBar0 << ui-> progressBar1
                  << ui->progressBar2 << ui->progressBar3;

    // QueuedConnection is important to ensure corss-thread signal passing.
    connect(this, &MainWindow::workProgressed, this, &MainWindow::updateProgressBar);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateProgressBar(int indicator, int value)
{
    if (indicator < 0 || indicator >= progressBars_.length())
    {
        return;
    }

    progressBars_[indicator]->setValue(value);

    if (isCalculationDone())
    {
        setupButtonState(true);
    }
}

void MainWindow::setupButtonState(bool isIdle)
{
    ui->pushButtonStartThreads->setEnabled(isIdle);
    ui->pushButtonStopThreads->setEnabled(!isIdle);
}

bool MainWindow::isCalculationDone()
{
    for (auto elem : progressBars_) {
        if (elem->value() < 100) {
            return false;
        }
    }

    return true;
}

void MainWindow::startThreads()
{
    canContinue = true;
    setupButtonState(false);

    thread_setup(4,
        [this]() -> bool {
            QMutexLocker locker(&lockStop_);
            return canContinue;
        },
        [this](int threadId, int progress) {
            emit workProgressed(threadId, progress);
        }
    );
}

void MainWindow::stopThreads()
{
    setupButtonState(true);
    QMutexLocker locker(&lockStop_);
    canContinue = false;
}
