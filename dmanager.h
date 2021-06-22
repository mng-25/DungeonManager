#ifndef DMANAGER_H
#define DMANAGER_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class dmanager; }
QT_END_NAMESPACE

class dmanager : public QMainWindow
{
    Q_OBJECT

public:
    dmanager(QWidget *parent = nullptr);
    ~dmanager();

private slots:

    void on_addCharButton_clicked();
    void setNewFileName();
    void setSaveFileName();
    void setOpenFileName();
    void export_JSON();

    void updateFields();
    void openDB(QString, bool); //true triggers a new DB
    void debugMsg(QString,QString,int);     //Message, message, debug level


    void on_campaignName_editingFinished();
    void on_charList_itemSelectionChanged();

    void on_charName_editingFinished();
    void on_playerName_editingFinished();
    void on_race_editingFinished();
    void on_class_subclass_editingFinished();


    /*       Stat Change Handling       */
    void on_strBaseSpin_valueChanged(int arg1);
    void on_strMod_valueChanged(int arg1);
    void on_dexBaseSpin_valueChanged(int arg1);
    void on_dexMod_valueChanged(int arg1);
    void on_conBaseSpin_valueChanged(int arg1);
    void on_conMod_valueChanged(int arg1);
    void on_intBaseSpin_valueChanged(int arg1);
    void on_intMod_valueChanged(int arg1);
    void on_wisBaseSpin_valueChanged(int arg1);
    void on_wisMod_valueChanged(int arg1);
    void on_chaBaseSpin_valueChanged(int arg1);
    void on_chaMod_valueChanged(int arg1);
    void profToggle();

    void on_lvl_spin_valueChanged(int arg1);

    void on_spellsText_textChanged();
    
    void on_tabWidget_currentChanged(int index);

    void on_inventoryText_textChanged();

    void on_abilitiesText_textChanged();

    void on_loreText_textChanged();

    void on_notesText_textChanged();

    void on_killCharButton_clicked();

signals:

private:
    Ui::dmanager *ui;
};
#endif // DMANAGER_H
