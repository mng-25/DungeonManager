﻿#include "globals.h"
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
    connect(ui->exportJSON, &QAction::triggered,this, &dmanager::export_JSON);

    connect(ui->athl_chk, &QCheckBox::toggled,this, &dmanager::profToggle);
    connect(ui->acro_chk, &QCheckBox::toggled,this, &dmanager::profToggle);
    connect(ui->slha_chk, &QCheckBox::toggled,this, &dmanager::profToggle);
    connect(ui->stlh_chk, &QCheckBox::toggled,this, &dmanager::profToggle);
    connect(ui->arca_chk, &QCheckBox::toggled,this, &dmanager::profToggle);
    connect(ui->hist_chk, &QCheckBox::toggled,this, &dmanager::profToggle);
    connect(ui->inve_chk, &QCheckBox::toggled,this, &dmanager::profToggle);
    connect(ui->natr_chk, &QCheckBox::toggled,this, &dmanager::profToggle);
    connect(ui->rlgn_chk, &QCheckBox::toggled,this, &dmanager::profToggle);
    connect(ui->anha_chk, &QCheckBox::toggled,this, &dmanager::profToggle);
    connect(ui->insi_chk, &QCheckBox::toggled,this, &dmanager::profToggle);
    connect(ui->medi_chk, &QCheckBox::toggled,this, &dmanager::profToggle);
    connect(ui->prcp_chk, &QCheckBox::toggled,this, &dmanager::profToggle);
    connect(ui->srvl_chk, &QCheckBox::toggled,this, &dmanager::profToggle);
    connect(ui->dcep_chk, &QCheckBox::toggled,this, &dmanager::profToggle);
    connect(ui->intm_chk, &QCheckBox::toggled,this, &dmanager::profToggle);
    connect(ui->perf_chk, &QCheckBox::toggled,this, &dmanager::profToggle);
    connect(ui->pers_chk, &QCheckBox::toggled,this, &dmanager::profToggle);

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
void dmanager::export_JSON()
{
    debugMsg("Export triggered"," NOT YET IMPLEMENTED :)",1);
}
/*      General purpose slots      */
void dmanager::updateFields()
{
    currentChar = ui->charList->currentItem()->text();
    ui->charName->setText(currentChar);
    QSqlQuery update;
    update.prepare("SELECT Player, Class_Subclass, Race FROM GameData WHERE Character = :charName");
    update.bindValue(":charName",currentChar);
    if(!update.exec())
    {
        debugMsg("Error updating: " , update.lastError().text(),2);
    }
    while(update.next())
    {
        ui->playerName->setText(update.value(0).toString());
        ui->class_subclass->setText(update.value(1).toString());
        ui->race->setText(update.value(2).toString());
    }
    switch (ui->tabWidget->currentIndex())
    {
        case 0: //stats_and_saves tab active
            QSqlQuery update;
            update.prepare("SELECT StrBase,DexBase,ConBase,IntBase,WisBase,CharBase,StrMod,DexMod,ConMod,IntMod,WisMod,CharMod,Profs "
                           "FROM GameData WHERE Character = :charName");
            update.bindValue(":charName",currentChar);
            if(!update.exec())
            {
                debugMsg("Error updating stats: " , update.lastError().text(),2);
            }
            while(update.next())
            {
                ui->strBaseSpin->setValue(update.value(0).toInt());
                ui->dexBaseSpin->setValue(update.value(1).toInt());
                ui->conBaseSpin->setValue(update.value(2).toInt());
                ui->intBaseSpin->setValue(update.value(3).toInt());
                ui->wisBaseSpin->setValue(update.value(4).toInt());
                ui->charBaseSpin->setValue(update.value(5).toInt());
                ui->strMod->setValue(update.value(6).toInt());
                ui->dexMod->setValue(update.value(7).toInt());
                ui->conMod->setValue(update.value(8).toInt());
                ui->intMod->setValue(update.value(9).toInt());
                ui->wisMod->setValue(update.value(10).toInt());
                ui->charMod->setValue(update.value(11).toInt());
                /*       Update proficiencies       */
                QString profs = update.value(12).toString();
                QStringList profsList = profs.split(',');
                ui->athl_chk->setCheckState(Qt::Unchecked);
                ui->acro_chk->setCheckState(Qt::Unchecked);
                ui->slha_chk->setCheckState(Qt::Unchecked);
                ui->stlh_chk->setCheckState(Qt::Unchecked);
                ui->arca_chk->setCheckState(Qt::Unchecked);
                ui->hist_chk->setCheckState(Qt::Unchecked);
                ui->inve_chk->setCheckState(Qt::Unchecked);
                ui->natr_chk->setCheckState(Qt::Unchecked);
                ui->rlgn_chk->setCheckState(Qt::Unchecked);
                ui->anha_chk->setCheckState(Qt::Unchecked);
                ui->insi_chk->setCheckState(Qt::Unchecked);
                ui->medi_chk->setCheckState(Qt::Unchecked);
                ui->prcp_chk->setCheckState(Qt::Unchecked);
                ui->srvl_chk->setCheckState(Qt::Unchecked);
                ui->dcep_chk->setCheckState(Qt::Unchecked);
                ui->intm_chk->setCheckState(Qt::Unchecked);
                ui->perf_chk->setCheckState(Qt::Unchecked);
                ui->pers_chk->setCheckState(Qt::Unchecked);
                for(int i = 0; i < profsList.count(); i++)  //Switch case not possible with strings, makes this messier than I'd like
                {
                    if(profsList[i] == "athl")
                    {
                        debugMsg("Proficiency in ", "athletics",1);
                        ui->athl_chk->setCheckState(Qt::Checked);
                    }
                    else if(profsList[i] == "acro")
                    {
                        debugMsg("Proficiency in ", "acrobatics",1);
                        ui->acro_chk->setCheckState(Qt::Checked);
                    }
                    else if(profsList[i] == "slha")
                    {
                        debugMsg("Proficiency in ", "sleight of hand",1);
                        ui->slha_chk->setCheckState(Qt::Checked);
                    }
                    else if(profsList[i] == "stlh")
                    {
                        debugMsg("Proficiency in ", "stealth",1);
                        ui->stlh_chk->setCheckState(Qt::Checked);
                    }
                    else if(profsList[i] == "arca")
                    {
                        debugMsg("Proficiency in ", "arcana",1);
                        ui->arca_chk->setCheckState(Qt::Checked);
                    }
                    else if(profsList[i] == "hist")
                    {
                        debugMsg("Proficiency in ", "history",1);
                        ui->hist_chk->setCheckState(Qt::Checked);
                    }
                    else if(profsList[i] == "inve")
                    {
                        debugMsg("Proficiency in ", "investigation",1);
                        ui->inve_chk->setCheckState(Qt::Checked);
                    }
                    else if(profsList[i] == "natr")
                    {
                        debugMsg("Proficiency in ", "nature",1);
                        ui->natr_chk->setCheckState(Qt::Checked);
                    }
                    else if(profsList[i] == "rlgn")
                    {
                        debugMsg("Proficiency in ", "religion",1);
                        ui->rlgn_chk->setCheckState(Qt::Checked);
                    }
                    else if(profsList[i] == "anha")
                    {
                        debugMsg("Proficiency in ", "animal handling",1);
                        ui->anha_chk->setCheckState(Qt::Checked);
                    }
                    else if(profsList[i] == "insi")
                    {
                        debugMsg("Proficiency in ", "insight",1);
                        ui->insi_chk->setCheckState(Qt::Checked);
                    }
                    else if(profsList[i] == "medi")
                    {
                        debugMsg("Proficiency in ", "medicine",1);
                        ui->medi_chk->setCheckState(Qt::Checked);
                    }
                    else if(profsList[i] == "prcp")
                    {
                        debugMsg("Proficiency in ", "perception",1);
                        ui->prcp_chk->setCheckState(Qt::Checked);
                    }
                    else if(profsList[i] == "srvl")
                    {
                        debugMsg("Proficiency in ", "survival",1);
                        ui->srvl_chk->setCheckState(Qt::Checked);
                    }
                    else if(profsList[i] == "dcep")
                    {
                        debugMsg("Proficiency in ", "deception",1);
                        ui->dcep_chk->setCheckState(Qt::Checked);
                    }
                    else if(profsList[i] == "intm")
                    {
                        debugMsg("Proficiency in ", "intimidation",1);
                        ui->intm_chk->setCheckState(Qt::Checked);
                    }
                    else if(profsList[i] == "perf")
                    {
                        debugMsg("Proficiency in ", "performance",1);
                        ui->perf_chk->setCheckState(Qt::Checked);
                    }
                    else if(profsList[i] == "pers")
                    {
                        debugMsg("Proficiency in ", "persuasion",1);
                        ui->pers_chk->setCheckState(Qt::Checked);
                    }

                }

            }
        //case 1: //other tab active
            //do stuff
           // ;
        //case 2: //other tab active
            //do stuff
           // ;
        //case 3: //other tab active
            //do stuff
           // ;

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
                       "(Character TEXT PRIMARY KEY, RowID INTEGER, Player TEXT, "
                       "Class_Subclass TEXT, Race TEXT, "
                       "StrBase INTEGER, DexBase INTEGER, ConBase INTEGER, IntBase INTEGER, WisBase INTEGER, CharBase INTEGER,"
                       "StrMod INTEGER, DexMod INTEGER, ConMod INTEGER, IntMod INTEGER, WisMod INTEGER, CharMod INTEGER, "
                       "Profs TEXT)");
            debugMsg("Created database file: ", filename,1);
        }
        else    //opening existing db
        {
            if(!query.exec("SELECT Character, RowID FROM GameData ORDER BY RowID ")) // fetch campaign metadata
            {
                debugMsg("Error opening database file: " , query.lastError().text(),2);
            }
            while(query.next())
            {
                ui->charList->addItem(query.value(0).toString());
                debugMsg(query.value(0).toString(),query.value(1).toString(),1);
                ui->charList->setCurrentRow(0);
                QApplication::processEvents();
            }
            query.exec("CREATE TABLE CampaignData "
                       "(Param TEXT PRIMARY KEY, Value TEXT)");
            query.exec("CREATE TABLE GameData "
                       "(Character TEXT PRIMARY KEY, RowID INTEGER, Player TEXT, "
                       "Class_Subclass TEXT, Race TEXT, "
                       "StrBase INTEGER, DexBase INTEGER, ConBase INTEGER, IntBase INTEGER, WisBase INTEGER, CharBase INTEGER,"
                       "StrMod INTEGER, DexMod INTEGER, ConMod INTEGER, IntMod INTEGER, WisMod INTEGER, CharMod INTEGER, "
                       "Profs TEXT)");
            query.prepare("SELECT Value FROM CampaignData WHERE Param = 'CampaignName'"); // fetch campaign metadata
            if(!query.exec())
            {
                debugMsg("Error selecting campaign metadata: " , query.lastError().text(),2);
            }
            while(query.next())
            {
                debugMsg("Fetched CampaignName: ", query.value(0).toString(),1);
                ui->campaignName->setText(query.value(0).toString());   // add fields as necessary
            }
            debugMsg("Opened database file: ", filename,2);
        }
    }

}
void dmanager::debugMsg(QString message, QString error, int level)
{
    /*      Debugging Levels - maybe crash logging, etc here
     *          2 - qWarning
     *          1 - qDebug + statusBar
     *          0 - statusBar
     */
    if(debugmode == 1)
    {
        switch (level)
        {
        case 0:
            ui->statusbar->showMessage(message + error);
            break;
        case 1:
            qDebug() << message << error;
            ui->statusbar->showMessage(message + error);
            break;
        case 2:
            qWarning() << message << error;
            break;

        }
    }
}
/*      Data manipulation slots      */
void dmanager::on_charList_itemSelectionChanged()
{
   updateFields();
   debugMsg("Current character is: ",currentChar,1);
}
void dmanager::on_addCharButton_clicked()
{
    QSqlDatabase::database();
    ui->charList->addItem("New Character");
    debugMsg("New char row: ", QString::number(ui->charList->currentRow()),1);
    QSqlQuery newChar;
    newChar.prepare("INSERT into GameData (Character, RowID) VALUES ('New Character', :rowID)");
    newChar.bindValue(":rowID", ui->charList->currentRow());
    if(!newChar.exec())
    {
        debugMsg("Error adding character: " , newChar.lastError().text(),2);
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
        debugMsg("Error setting campaign name: " , setCampaignName.lastError().text(),2);
    }
    else
    {
        debugMsg("Campaign name is: ", ui->campaignName->text(),1);
    }
}
void dmanager::on_charName_editingFinished()
{
    QString oldChar = ui->charList->currentItem()->text();
    if(oldChar != ui->charName->text())
    {
        debugMsg("Old character name is: ", oldChar,1);
        ui->charList->currentItem()->setText(ui->charName->text());
        currentChar = ui->charList->currentItem()->text();
        QSqlDatabase::database();
        QSqlQuery setName;
        setName.prepare("UPDATE GameData SET Character = :newName WHERE Character = :oldName" );
        setName.bindValue(":oldName",oldChar);
        setName.bindValue(":newName",ui->charName->text());
        if(!setName.exec()){
            debugMsg("Error setting character name: " , setName.lastError().text(),2);
        }
        else
        {
            debugMsg("Current character name is: ",currentChar,1);
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
            debugMsg("Error setting player name: " , setPlayer.lastError().text(),2);
        }
        else
        {
            debugMsg("Player name is: ",ui->playerName->text(),1);
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
            debugMsg("Error setting race: " , setRace.lastError().text(),2);
        }
        else
        {
            debugMsg("Character race is: ",ui->race->text(),1);
        }
    }
}
void dmanager::on_class_subclass_editingFinished()
{
    if(true)    // possibly filter double inputs, in fututre
    {
        QSqlDatabase::database();
        QSqlQuery setClass;
        setClass.prepare("UPDATE GameData SET Class_Subclass = :class WHERE Character = :charName" );
        setClass.bindValue(":charName",currentChar);
        setClass.bindValue(":class",ui->class_subclass->text());
        if(!setClass.exec()){
            debugMsg("Error setting class: " , setClass.lastError().text(),2);
        }
        else
        {
            debugMsg("Character class is: ",ui->class_subclass->text(),1);
        }
    }
}
        /*      Stats & Profs       */
        /* MNG TODO: Convert individual
         * slots into general slots
         * and connect to signals, once
         * I get that working       */
void dmanager::on_strBaseSpin_valueChanged(int arg1)
{
    if(true)    // possibly filter double inputs, in fututre
    {
        QSqlDatabase::database();
        QSqlQuery setStat;
        setStat.prepare("UPDATE GameData SET StrBase = :stat WHERE Character = :charName" );
        setStat.bindValue(":charName",currentChar);
        setStat.bindValue(":stat",arg1);
        if(!setStat.exec()){
            debugMsg("Error setting strength: " , setStat.lastError().text(),2);
        }
        else
        {
            debugMsg("Character strength is: ",QString::number(arg1),1);
        }
    }
}
void dmanager::on_strMod_valueChanged(int arg1)
{
   if(true)    // possibly filter double inputs, in fututre
    {
        QSqlDatabase::database();
        QSqlQuery setStat;
        setStat.prepare("UPDATE GameData SET StrMod = :stat WHERE Character = :charName" );
        setStat.bindValue(":charName",currentChar);
        setStat.bindValue(":stat",arg1);
        if(!setStat.exec()){
            debugMsg("Error setting strength: " , setStat.lastError().text(),2);
        }
        else
        {
            debugMsg("Character strength mod is: ",QString::number(arg1),1);
        }
    }
}
void dmanager::on_dexBaseSpin_valueChanged(int arg1)
{
    if(true)    // possibly filter double inputs, in fututre
    {
        QSqlDatabase::database();
        QSqlQuery setStat;
        setStat.prepare("UPDATE GameData SET DexBase = :stat WHERE Character = :charName" );
        setStat.bindValue(":charName",currentChar);
        setStat.bindValue(":stat",arg1);
        if(!setStat.exec()){
            debugMsg("Error setting dexterity: " , setStat.lastError().text(),2);
        }
        else
        {
            debugMsg("Character dexterity is: ",QString::number(arg1),1);
        }
    }
}
void dmanager::on_dexMod_valueChanged(int arg1)
{
    if(true)    // possibly filter double inputs, in fututre
    {
        QSqlDatabase::database();
        QSqlQuery setStat;
        setStat.prepare("UPDATE GameData SET DexMod = :stat WHERE Character = :charName" );
        setStat.bindValue(":charName",currentChar);
        setStat.bindValue(":stat",arg1);
        if(!setStat.exec()){
            debugMsg("Error setting dexterity: " , setStat.lastError().text(),2);
        }
        else
        {
            debugMsg("Character dexterity mod is: ",QString::number(arg1),1);
        }
    }
}
void dmanager::on_conBaseSpin_valueChanged(int arg1)
{
    if(true)    // possibly filter double inputs, in fututre
    {
        QSqlDatabase::database();
        QSqlQuery setStat;
        setStat.prepare("UPDATE GameData SET ConBase = :stat WHERE Character = :charName" );
        setStat.bindValue(":charName",currentChar);
        setStat.bindValue(":stat",arg1);
        if(!setStat.exec()){
            debugMsg("Error setting constitution: " , setStat.lastError().text(),2);
        }
        else
        {
            debugMsg("Character constitution is: ",QString::number(arg1),1);
        }
    }
}
void dmanager::on_conMod_valueChanged(int arg1)
{
    if(true)    // possibly filter double inputs, in fututre
    {
        QSqlDatabase::database();
        QSqlQuery setStat;
        setStat.prepare("UPDATE GameData SET ConMod = :stat WHERE Character = :charName" );
        setStat.bindValue(":charName",currentChar);
        setStat.bindValue(":stat",arg1);
        if(!setStat.exec()){
            debugMsg("Error setting constitution: " , setStat.lastError().text(),2);
        }
        else
        {
            debugMsg("Character constitution mod is: ",QString::number(arg1),1);
        }
    }
}
void dmanager::on_intBaseSpin_valueChanged(int arg1)
{
    if(true)    // possibly filter double inputs, in fututre
    {
        QSqlDatabase::database();
        QSqlQuery setStat;
        setStat.prepare("UPDATE GameData SET IntBase = :stat WHERE Character = :charName" );
        setStat.bindValue(":charName",currentChar);
        setStat.bindValue(":stat",arg1);
        if(!setStat.exec()){
            debugMsg("Error setting intelligence: " , setStat.lastError().text(),2);
        }
        else
        {
            debugMsg("Character intelligence is: ",QString::number(arg1),1);
        }
    }
}
void dmanager::on_intMod_valueChanged(int arg1)
{
    if(true)    // possibly filter double inputs, in fututre
    {
        QSqlDatabase::database();
        QSqlQuery setStat;
        setStat.prepare("UPDATE GameData SET IntMod = :stat WHERE Character = :charName" );
        setStat.bindValue(":charName",currentChar);
        setStat.bindValue(":stat",arg1);
        if(!setStat.exec()){
            debugMsg("Error setting intelligence: " , setStat.lastError().text(),2);
        }
        else
        {
            debugMsg("Character intelligence mod is: ",QString::number(arg1),1);
        }
    }
}
void dmanager::on_wisBaseSpin_valueChanged(int arg1)
{
    if(true)    // possibly filter double inputs, in fututre
    {
        QSqlDatabase::database();
        QSqlQuery setStat;
        setStat.prepare("UPDATE GameData SET WisBase = :stat WHERE Character = :charName" );
        setStat.bindValue(":charName",currentChar);
        setStat.bindValue(":stat",arg1);
        if(!setStat.exec()){
            debugMsg("Error setting wisdom: " , setStat.lastError().text(),2);
        }
        else
        {
            debugMsg("Character wisdom is: ",QString::number(arg1),1);
        }
    }
}
void dmanager::on_wisMod_valueChanged(int arg1)
{
    if(true)    // possibly filter double inputs, in fututre
    {
        QSqlDatabase::database();
        QSqlQuery setStat;
        setStat.prepare("UPDATE GameData SET WisMod = :stat WHERE Character = :charName" );
        setStat.bindValue(":charName",currentChar);
        setStat.bindValue(":stat",arg1);
        if(!setStat.exec()){
            debugMsg("Error setting wisdom: " , setStat.lastError().text(),2);
        }
        else
        {
            debugMsg("Character wisdom mod is: ",QString::number(arg1),1);
        }
    }
}
void dmanager::on_charBaseSpin_valueChanged(int arg1)
{
    if(true)    // possibly filter double inputs, in fututre
    {
        QSqlDatabase::database();
        QSqlQuery setStat;
        setStat.prepare("UPDATE GameData SET CharBase = :stat WHERE Character = :charName" );
        setStat.bindValue(":charName",currentChar);
        setStat.bindValue(":stat",arg1);
        if(!setStat.exec()){
           debugMsg("Error setting charisma: " , setStat.lastError().text(),2);
        }
        else
        {
            debugMsg("Character charisma is: ",QString::number(arg1),1);
        }
    }
}
void dmanager::on_charMod_valueChanged(int arg1)
{
    if(true)    // possibly filter double inputs, in fututre
    {
        QSqlDatabase::database();
        QSqlQuery setStat;
        setStat.prepare("UPDATE GameData SET CharMod = :stat WHERE Character = :charName" );
        setStat.bindValue(":charName",currentChar);
        setStat.bindValue(":stat",arg1);
        if(!setStat.exec()){
            debugMsg("Error setting charisma: " , setStat.lastError().text(),2);
        }
        else
        {
            debugMsg("Character charisma mod is: ",QString::number(arg1),1);
        }
    }
}
void dmanager::profToggle()
{
    debugMsg(sender()->objectName(), " toggled", 1);
    QString profsList;
    if(true)        //for folding purposes
    {
        if(ui->acro_chk->isChecked()==true)
        {
            profsList += "athl,";
        }
        if(ui->athl_chk->isChecked()==true)
        {
            profsList += "acro,";
        }
        if(ui->slha_chk->isChecked()==true)
        {
            profsList += "slha,";
        }
        if(ui->stlh_chk->isChecked()==true)
        {
            profsList += "stlh,";
        }
        if(ui->arca_chk->isChecked()==true)
        {
            profsList += "arca,";
        }
        if(ui->hist_chk->isChecked()==true)
        {
            profsList += "hist,";
        }
        if(ui->inve_chk->isChecked()==true)
        {
            profsList += "inve,";
        }
        if(ui->natr_chk->isChecked()==true)
        {
            profsList += "natr,";
        }
        if(ui->rlgn_chk->isChecked()==true)
        {
            profsList += "rlgn,";
        }
        if(ui->anha_chk->isChecked()==true)
        {
            profsList += "anha,";
        }
        if(ui->insi_chk->isChecked()==true)
        {
            profsList += "insi,";
        }
        if(ui->medi_chk->isChecked()==true)
        {
            profsList += "medi,";
        }
        if(ui->prcp_chk->isChecked()==true)
        {
            profsList += "prcp,";
        }
        if(ui->srvl_chk->isChecked()==true)
        {
            profsList += "srvl,";
        }
        if(ui->dcep_chk->isChecked()==true)
        {
            profsList += "dcep,";
        }
        if(ui->intm_chk->isChecked()==true)
        {
            profsList += "intm,";
        }
        if(ui->perf_chk->isChecked()==true)
        {
            profsList += "perf,";
        }
        if(ui->pers_chk->isChecked()==true)
        {
            profsList += "pers,";
        }
    }
    QSqlDatabase::database();
    QSqlQuery setProf;
    setProf.prepare("UPDATE GameData SET Profs = :prof WHERE Character = :charName" );
    setProf.bindValue(":charName",currentChar);
    setProf.bindValue(":prof",profsList);
    if(!setProf.exec()){
        debugMsg("Error setting skills: " , setProf.lastError().text(),2);
    }
    else
    {
        debugMsg(currentChar + " skills: ", profsList,1);
    }
}
