#include "globals.h"
#include "dmanager.h"
#include "ui_dmanager.h"
#include "dialog.h"
#include <QWidget>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlDriver>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QFileDialog>
#include <QDebug>
#include <QVariant>


/*      Constructor & Destructor        */
dmanager::dmanager(QWidget *parent) :QMainWindow(parent), ui(new Ui::dmanager)
{
    ui->setupUi(this);
    connect(ui->sa_campaign, &QAction::triggered,this, &dmanager::setSaveFileName);
    connect(ui->o_campaign, &QAction::triggered,this, &dmanager::setOpenFileName);
    connect(ui->n_campaign, &QAction::triggered,this, &dmanager::setNewFileName);
    openDB(":memory:",true);
}
dmanager::~dmanager()
{
    delete ui;
    QSqlDatabase::database().commit();
    QSqlDatabase::database().close();
}
/*      Menubar slots       */
void dmanager::setNewFileName()
{
    QSqlDatabase::database().close();
    QString newFileName = QFileDialog::getSaveFileName(this,tr("Save Campaign As - DManager"));
    currentFile = newFileName;
    openDB(newFileName,true);
}
void dmanager::setOpenFileName()
{
    QString openFileName = QFileDialog::getOpenFileName(this,tr("Open Campaign - DManager"));
    currentFile = openFileName;
    /*if(a database is currently connected)
    {
        close database
    }*/
    openDB(openFileName, false);
    updateFields();
}
void dmanager::setSaveFileName()
{
        QString saveFileName = QFileDialog::getSaveFileName(this,tr("Save Campaign As - DManager"));
}
/*      General purpose slots      */
void dmanager::updateFields()
{
    currentChar = ui->charList->currentItem()->text();
    ui->charName->setText(currentChar);
    QSqlQuery update;
    update.prepare("SELECT Player,Class1,Race FROM GameData WHERE Character = :charName");
    update.bindValue(":charName",currentChar);
    if(!update.exec())
    {
        qWarning() << "Error updating: " << update.lastError().text();
    }
    while(update.next())
    {
        ui->playerName->setText(update.value(0).toString());
        ui->class_subclass->setText(update.value(1).toString());
        ui->race->setText(update.value(2).toString());
    }
}
void dmanager::openDB(QString filename, bool newdb)  //true to wipe existing data
{
    const QString DRIVER("QSQLITE");
    if(QSqlDatabase::isDriverAvailable(DRIVER)){
        QSqlDatabase database = QSqlDatabase::addDatabase(DRIVER);
        database.setDatabaseName(filename);
        database.open();
        QSqlQuery query;
        if(newdb == true)
        {
            query.exec("DROP TABLE IF EXISTS CampaignData");
            query.exec("DROP TABLE IF EXISTS GameData");
            query.exec("CREATE TABLE CampaignData "
                       "(Param TEXT PRIMARY KEY, Value TEXT)");
            query.exec("CREATE TABLE GameData "
                       "(Character TEXT PRIMARY KEY, RowID INTEGER, Player TEXT, Class1 TEXT, Race TEXT)");
        }
        else    //opening existing db
        {
            //do stuff here
        }
    }
    debugMsg("Database file: ", filename);
}
void dmanager::debugMsg(QString message, QString error)
{
    //standard debug messaging template
    if(debugmode == 1)
    {
        qDebug() << message << error;
        ui->statusbar->showMessage(message + error);
    }
}
/*      Data manipulation slots      */
void dmanager::on_charList_itemSelectionChanged()
{
   updateFields();
   debugMsg("Current character is: ",currentChar);
}
void dmanager::on_addCharButton_clicked()
{
    QSqlDatabase::database();
    new QListWidgetItem(tr("New Character"), ui->charList);
    debugMsg("New char row: ", QString::number(ui->charList->currentRow()));
    QSqlQuery newChar;
    newChar.prepare("INSERT into GameData (Character, RowID) VALUES ('New Character', :rowID)");
    newChar.bindValue(":rowID", ui->charList->currentRow());
    if(!newChar.exec())
    {
        qWarning() << "Error adding character: " << newChar.lastError().text();
    }
    //ui->charList->setCurrentRow(ui->charList->currentRow());    //Maybe this will select
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
    else
    {
        debugMsg("Campaign name is: ", ui->campaignName->text());
    }
}
void dmanager::on_charName_editingFinished()
{
    QString oldChar = ui->charList->currentItem()->text();
    if(oldChar != ui->charName->text())
    {
        debugMsg("Old character name is: ", oldChar);
        ui->charList->currentItem()->setText(ui->charName->text());
        currentChar = ui->charList->currentItem()->text();
        QSqlDatabase::database();
        QSqlQuery setName;
        setName.prepare("UPDATE GameData SET Character = :newName WHERE Character = :oldName" );
        setName.bindValue(":oldName",oldChar);
        setName.bindValue(":newName",ui->charName->text());
        if(!setName.exec()){
            qWarning() << "Error setting character name: " << setName.lastError().text();
        }
        else
        {
            debugMsg("Current character name is: ",currentChar);
        }
    }
}
void dmanager::on_playerName_editingFinished()
{
    if(true)    // possibly filter double inputs, in fututre
    {
        QSqlDatabase::database();
        QSqlQuery setPlayer;
        setPlayer.prepare("UPDATE GameData SET Player = :playerName WHERE Character = :charName" );
        setPlayer.bindValue(":charName",currentChar);
        setPlayer.bindValue(":playerName",ui->playerName->text());
        if(!setPlayer.exec()){
            qWarning() << "Error setting player name: " << setPlayer.lastError().text();
        }
        else
        {
            debugMsg("Player name is: ",ui->playerName->text());
        }
    }
}
void dmanager::on_race_editingFinished()
{
    if(true)    // possibly filter double inputs, in fututre
    {
        QSqlDatabase::database();
        QSqlQuery setRace;
        setRace.prepare("UPDATE GameData SET Race = :race WHERE Character = :charName" );
        setRace.bindValue(":charName",currentChar);
        setRace.bindValue(":race",ui->race->text());
        if(!setRace.exec()){
            qWarning() << "Error setting race: " << setRace.lastError().text();
        }
        else
        {
            debugMsg("Character race is: ",ui->race->text());
        }
    }
}
void dmanager::on_class_subclass_editingFinished()
{
    if(true)    // possibly filter double inputs, in fututre
    {
        QSqlDatabase::database();
        QSqlQuery setClass;
        setClass.prepare("UPDATE GameData SET Class1 = :class WHERE Character = :charName" );
        setClass.bindValue(":charName",currentChar);
        setClass.bindValue(":class",ui->class_subclass->text());
        if(!setClass.exec()){
            qWarning() << "Error setting class: " << setClass.lastError().text();
        }
        else
        {
            debugMsg("Character class is: ",ui->class_subclass->text());
        }
    }
}
