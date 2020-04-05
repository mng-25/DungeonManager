#include "dmanager.h"
#include "ui_dmanager.h"
#include "dialog.h"
#include <QWidget>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlDriver>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <iostream>
#include <stdio.h>
#include <QFileDialog>
#include <QDebug>
#include <QVariant>
#include "globals.h"

//Dialog dialog;
//dialog.setModal(true);
//dialog.exec();

dmanager::dmanager(QWidget *parent) :QMainWindow(parent), ui(new Ui::dmanager)
{
    ui->setupUi(this);
    connect(ui->sa_campaign, &QAction::triggered,this, &dmanager::setSaveFileName);
    connect(ui->o_campaign, &QAction::triggered,this, &dmanager::setOpenFileName);
    connect(ui->n_campaign, &QAction::triggered,this, &dmanager::setNewFileName);
    /*const QString DRIVER("QSQLITE");
    if(QSqlDatabase::isDriverAvailable(DRIVER)){
        QSqlDatabase database = QSqlDatabase::addDatabase(DRIVER);
        database.setDatabaseName(":memory:");
        database.open();
        QSqlQuery query;
        query.exec("CREATE TABLE CampaignData "
                   "(Param TEXT PRIMARY KEY, Value TEXT)");
        query.exec("CREATE TABLE GameData "
                   "(Character TEXT PRIMARY KEY, Value TEXT)");
    }*/
}
dmanager::~dmanager()
{
    delete ui;
    QSqlDatabase::database().commit();
    QSqlDatabase::database().close();
}
void dmanager::setNewFileName()
{
    QString selectedFilter;
    QString newFileName = QFileDialog::getSaveFileName(this,tr("Save Campaign As - DManager"));
    currentFile = newFileName;
    const QString DRIVER("QSQLITE");
    if(QSqlDatabase::isDriverAvailable(DRIVER)){
        QSqlDatabase database = QSqlDatabase::addDatabase(DRIVER);
        database.setDatabaseName(currentFile);
        database.open();
        QSqlQuery query;
        query.exec("CREATE TABLE CampaignData "
                   "(Param TEXT PRIMARY KEY, Value TEXT)");
        query.exec("CREATE TABLE GameData "
                   "(Character TEXT PRIMARY KEY, Value TEXT)");
    }
}
void dmanager::setOpenFileName()
{
    QString selectedFilter;
    QString openfileName = QFileDialog::getOpenFileName(this,tr("Open Campaign - DManager"));
    const QString DRIVER("QSQLITE");
    if(QSqlDatabase::isDriverAvailable(DRIVER)){
        QSqlDatabase db = QSqlDatabase::addDatabase(DRIVER);
        db.setDatabaseName(openfileName);
    }
    //if (!fileName.isEmpty())
    //    openFileNameLabel->setText(fileName);
}
void dmanager::setSaveFileName()
{
        QString selectedFilter;
        QString newfileName = QFileDialog::getSaveFileName(this,tr("Save Campaign As - DManager"));
        const QString DRIVER("QSQLITE");
        if(QSqlDatabase::isDriverAvailable(DRIVER)){
            QSqlDatabase db = QSqlDatabase::addDatabase(DRIVER);
            db.setDatabaseName(newfileName);
        }
}
void dmanager::on_campaignName_editingFinished()
{
    QSqlDatabase::database();
    QSqlQuery setCampaignName;
    setCampaignName.prepare("INSERT INTO CampaignData (Param,Value) "
                            "VALUES ('CampaignName',:camName) "
                            "ON CONFLICT (Param) DO UPDATE SET Value=excluded.Value" );
    setCampaignName.bindValue(":camName",ui->campaignName->text());
    if(!setCampaignName.exec()){
        qWarning() << "Error setting campaign name: " << setCampaignName.lastError().text();
    }
}
void dmanager::on_addCharButton_clicked()
{
    QSqlDatabase::database();
    new QListWidgetItem(tr("New Character"), ui->charList);
    QSqlQuery newChar;
    if(!newChar.exec("INSERT into GameData(Character) VALUES('New Character')"))
    {
        qWarning() << "Error adding character: " << newChar.lastError().text();
    }
    //ui->charList->setCurrentItem(QListWidgetItem *"NewCharacter");
}
void dmanager::on_charName_editingFinished()
{

    QString oldChar = ui->charList->currentItem()->text();
    if(debugmode == 1)
    {
        qWarning() << "Old character name is: " << oldChar;
    }
    ui->charList->currentItem()->setText(ui->charName->text());
    currentChar = ui->charList->currentItem()->text();
    if(debugmode == 1)
    {
        qWarning() << "Current character name is: " << currentChar;
    }
    QSqlDatabase::database();
    QSqlQuery setName;
    setName.prepare("UPDATE GameData SET Character = :newName WHERE Character = :oldName" );
    setName.bindValue(":oldName",oldChar);
    setName.bindValue(":newName",ui->charName->text());
    if(!setName.exec()){
        qWarning() << "Error setting character name: " << setName.lastError().text();
    }
}

void dmanager::on_charList_itemSelectionChanged()
{
    currentChar = ui->charList->currentItem()->text();
    if(debugmode == 1)
    {
        qWarning() << "Current character is: " << currentChar;
    }
    ui->charName->setText(currentChar);
}
