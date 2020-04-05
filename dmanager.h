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
    void on_campaignName_editingFinished();
    void on_charName_editingFinished();
    void on_charList_itemSelectionChanged();

signals:

private:
    Ui::dmanager *ui;
};
#endif // DMANAGER_H
