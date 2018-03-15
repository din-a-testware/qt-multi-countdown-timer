#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include <QDateTime>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QVector<QTimer *> timerList;

private slots:

    void setTimeLabel(qint64 mseconds,QString title,QDateTime timeoutDate);

    void on_listWidget_itemClicked(QListWidgetItem *item);

    void on_pause_clicked();

    void alarmTimerTimeout(QTimer *newTimer,QString title,QListWidgetItem *currentListItem);

    void on_resume_clicked();

    void on_start_clicked();

    void on_stop_clicked();

    void on_neu_clicked();

    void on_entfernen_clicked();

    void on_test_clicked();

    void on_neu_2_clicked();

    void on_start_2_clicked();

    void on_listWidget_2_itemClicked(QListWidgetItem *item);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
