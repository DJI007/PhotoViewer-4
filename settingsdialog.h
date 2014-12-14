#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>

#include "ui_settings.h"

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SettingsDialog(QWidget *parent = 0);

    void saveSettings ();

signals:

public slots:

private slots:
    void on_lstIcons_itemSelectionChanged();

    void on_pbDSelectDatabaseFile_clicked();

    void on_leDDatabaseFile_editingFinished();

private:
    Ui::SettingsDialog *ui;

    void loadSettings ();
    bool setDBFileName(QString fileName);
};

#endif // SETTINGSDIALOG_H
