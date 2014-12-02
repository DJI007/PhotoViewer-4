#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include "ui_settings.h"

class SettingsDialog : public QDialog, public Ui_SettingsDlg
{
    Q_OBJECT
public:
    explicit SettingsDialog(QWidget *parent = 0);

signals:

public slots:

};

#endif // SETTINGSDIALOG_H
