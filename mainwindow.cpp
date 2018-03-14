#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>
#include <QDebug>
#include <QTime>
#include <QDateTime>
#include <QDate>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QCoreApplication::processEvents(QEventLoop::AllEvents);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_listWidget_itemClicked(QListWidgetItem *item)
{

    QDateTime currentDateTime = QDateTime::currentDateTime();
    qint64 timerTime = ui->listWidget->currentItem()->data(Qt::UserRole+1).toInt();

    if (ui->listWidget->selectedItems().count() >0) {


            QDateTime endTime = ui->listWidget->currentItem()->data(Qt::UserRole+2).toDateTime();

            if (timerList.at(ui->listWidget->currentItem()->data(Qt::UserRole).toInt())->isActive()) {
            if (endTime.msecsTo(currentDateTime) < timerTime) {
                qint64 asdf = timerTime-(endTime.msecsTo(currentDateTime));
                qDebug() << timerTime-(endTime.msecsTo(currentDateTime));
                setTimeLabel(asdf,ui->listWidget->currentItem()->data(Qt::DisplayRole).toString()+":"+QString::number(timerTime-(endTime.msecsTo(currentDateTime))));
            } else {
                qint64 asdf = timerTime-(endTime.msecsTo(currentDateTime));
                timerList.at(ui->listWidget->currentItem()->data(Qt::UserRole).toInt())->stop();
                qDebug() << "stopped";

                setTimeLabel(0,ui->listWidget->currentItem()->data(Qt::DisplayRole).toString()+":"+"stopped");
            }
            } else {
                qint64 asdf = timerTime-(endTime.msecsTo(currentDateTime));
                setTimeLabel(asdf,ui->listWidget->currentItem()->data(Qt::DisplayRole).toString()+":"+"inactive");
            }

    }

    ui->lineEdit->setText(item->data(Qt::DisplayRole).toString());
}

//!! SET TIME LABEL
void MainWindow::setTimeLabel(qint64 mseconds, QString title) {



//ui->timerlabel->setText(QDateTime::fromTime_t(mseconds/1000).toUTC().toString("hh:mm:ss"));
//ui->timerlabel->setText(QDateTime::fromTime_t(mseconds/1000).toUTC().toString("hh")+"--"+QDateTime::fromTime_t(mseconds/1000).toUTC().toString("mm")+"-"+QDateTime::fromTime_t(mseconds/1000).toUTC().toString("ss"));
    ui->a_h->setValue(QDateTime::fromTime_t(mseconds/1000).toUTC().toString("hh").toInt());
    ui->a_m->setValue(QDateTime::fromTime_t(mseconds/1000).toUTC().toString("mm").toInt());
    ui->a_s->setValue(QDateTime::fromTime_t(mseconds/1000).toUTC().toString("ss").toInt());
    ui->lineEdit->setText(title);

}

//!! TIMEOUT
void MainWindow::alarmTimerTimeout(QTimer *newTimer, QString title) {

    QDateTime currentDateTime = QDateTime::currentDateTime();
    qint64 timerTime = ui->listWidget->currentItem()->data(Qt::UserRole+1).toInt();

    if (ui->listWidget->selectedItems().count() >0) {
        if (newTimer->timerId() == timerList.at(ui->listWidget->currentItem()->data(Qt::UserRole).toInt())->timerId()) {

            QDateTime endTime = ui->listWidget->currentItem()->data(Qt::UserRole+2).toDateTime();

            if (endTime.msecsTo(currentDateTime)+1000 < timerTime) {
                qint64 asdf = timerTime-(endTime.msecsTo(currentDateTime));

                setTimeLabel(asdf,ui->listWidget->currentItem()->data(Qt::DisplayRole).toString());
            } else {
                newTimer->stop();
                /*QMessageBox msgBox;
                msgBox.setText("Stopped.");
                msgBox.exec();*/
                qDebug() << "stopped";
                setTimeLabel(0,ui->listWidget->currentItem()->data(Qt::DisplayRole).toString()+":"+"stopped");
            }
        }
    }
}



//!! NEU
void MainWindow::on_neu_clicked()
{
    QTimer *newTimer = new QTimer(this);
    newTimer->setInterval(100);

    int lastID = timerList.count();
    timerList.insert(lastID,newTimer);

    int timerMsecs = ((ui->s->value())*1000)+
                     ((ui->m->value())*60*1000)+
                     ((ui->h->value())*60*60*1000);

    QDateTime currentDateTime = QDateTime::currentDateTimeUtc();
    ulong unixtimeMSecs = (currentDateTime.toTime_t()*60*1000)+timerMsecs;
    //uint endzeit = unixtimeMSecs + timerMsecs;

    //qDebug() << "unixtime" << unixtimeMSecs << "unixtimeMSecsEnde:" << endzeit << "zwischenzeit" << endzeit-unixtimeMSecs << "timermsecs" << timerMsecs;

    qint64 currentDateMsecs = QDateTime::currentMSecsSinceEpoch();
    qDebug() << "currentDateMsecs" << currentDateMsecs << "endtime" << currentDateMsecs + timerMsecs << "diff" << (currentDateMsecs+timerMsecs)-currentDateMsecs;


    qDebug() << "init:" << timerMsecs << unixtimeMSecs;
    QListWidgetItem *newTimerItem = new QListWidgetItem();
    newTimerItem->setData(Qt::DisplayRole,ui->lineEdit->text());
    newTimerItem->setData(Qt::UserRole,lastID);                     //!! USER ROLE: ID IN TIMERLIST
    newTimerItem->setData(Qt::UserRole+1,timerMsecs);               //!! USER ROLE+1: INITIAL TIMER MSECS
    newTimerItem->setData(Qt::UserRole+2, (currentDateMsecs+timerMsecs)); //!! USER ROLE+2: INITIAL ENDTIME
    ui->listWidget->addItem(newTimerItem);
    QString titleString = newTimerItem->data(Qt::DisplayRole).toString();



    //! CONNECTION COUNTDOWN
    connect(newTimer,&QTimer::timeout,[=]() {

        alarmTimerTimeout(newTimer, titleString);

    });
    //! -------------

    //newTimer->start();
}

//!! PAUSE
void MainWindow::on_pause_clicked()
{



    if (ui->listWidget->selectedItems().count() >0) {

        QDateTime currentDateTime = QDateTime::currentDateTime();
        qint64 timerTime = ui->listWidget->currentItem()->data(Qt::UserRole+1).toInt();

        if (timerList.at(ui->listWidget->currentItem()->data(Qt::UserRole).toInt())->isActive() == false) {
            ui->pause->setText("pause timer");
            int timerMsecs = (ui->s->value()*1000)+
                             (ui->m->value()*60*1000)+
                             (ui->h->value()*60*60*1000);

            qint64 currentDateMsecs = QDateTime::currentMSecsSinceEpoch();
            QDateTime endTime = QDateTime::currentDateTime();
                      endTime.addMSecs(timerMsecs);
            qDebug() << "currentDateMsecs" << currentDateMsecs << "endtime" << currentDateMsecs + timerMsecs << "diff" << (currentDateMsecs+timerMsecs)-currentDateMsecs;

            ui->listWidget->currentItem()->setData(Qt::UserRole+2, endTime);
            if (((currentDateMsecs+timerMsecs)-currentDateMsecs) != ui->listWidget->currentItem()->data(Qt::UserRole+1).toInt()) {
                timerList.at(ui->listWidget->currentItem()->data(Qt::UserRole).toInt())->blockSignals(false);
                timerList.at(ui->listWidget->currentItem()->data(Qt::UserRole).toInt())->start();
            }
        } else {
            ui->pause->setText("resume Timer");
            timerList.at(ui->listWidget->currentItem()->data(Qt::UserRole).toInt())->stop();
            timerList.at(ui->listWidget->currentItem()->data(Qt::UserRole).toInt())->blockSignals(true);

            QDateTime endTime = ui->listWidget->currentItem()->data(Qt::UserRole+2).toDateTime();
            qDebug() << "pause:"<< timerTime-(endTime.msecsTo(currentDateTime));

            timerTime = timerTime-(endTime.msecsTo(currentDateTime));
            ui->listWidget->currentItem()->setData(Qt::UserRole+1,timerTime);
            endTime.addMSecs(timerTime);
            ui->listWidget->currentItem()->setData(Qt::UserRole+2,endTime);

        }

    }


}

//! RESUME
void MainWindow::on_resume_clicked()
{



}

//!! START
void MainWindow::on_start_clicked()
{
    if (ui->listWidget->selectedItems().count() >0) {

        if (timerList.at(ui->listWidget->currentItem()->data(Qt::UserRole).toInt())->isActive() == false) {
            int timerMsecs = (ui->s->value()*1000)+
                             (ui->m->value()*60*1000)+
                             (ui->h->value()*60*60*1000);


            qint64 currentDateMsecs = QDateTime::currentMSecsSinceEpoch();
            QDateTime endTime = QDateTime::currentDateTime();
                      endTime.addMSecs(timerMsecs);
            qDebug() << "currentDateMsecs" << currentDateMsecs << "endtime" << currentDateMsecs + timerMsecs << "diff" << (currentDateMsecs+timerMsecs)-currentDateMsecs;

            ui->listWidget->currentItem()->setData(Qt::UserRole+2, endTime);
            timerList.at(ui->listWidget->currentItem()->data(Qt::UserRole).toInt())->blockSignals(false);
            timerList.at(ui->listWidget->currentItem()->data(Qt::UserRole).toInt())->start();
        }
    }
}

//!! STOP
void MainWindow::on_stop_clicked()
{
    if (ui->listWidget->selectedItems().count() >0) {
        timerList.at(ui->listWidget->currentItem()->data(Qt::UserRole).toInt())->stop();
        setTimeLabel(0,ui->listWidget->currentItem()->data(Qt::DisplayRole).toString());
    }
}

//!! ENTFERNEN
void MainWindow::on_entfernen_clicked()
{
    if (ui->listWidget->selectedItems().count() >0) {
        timerList.at(ui->listWidget->currentItem()->data(Qt::UserRole).toInt())->stop();
        delete timerList.at(ui->listWidget->currentItem()->data(Qt::UserRole).toInt());
        delete ui->listWidget->currentItem();
    }
}

void MainWindow::on_test_clicked()
{
    QMessageBox msgBox;
    msgBox.setText("Stopped.");
    msgBox.exec();
}
