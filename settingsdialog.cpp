#include "settingsdialog.h"

#include <QStandardItemModel>
#include <QDebug>
#include <QFileInfo>
#include <QFileDialog>
#include <QMessageBox>

#include "settingshelper.h"
#include "digikammetadata.h"

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);

    ui->lstIcons->setCurrentRow(0);

    loadSettings ();
}

void SettingsDialog::on_lstIcons_itemSelectionChanged()
{
    ui->swPages->setCurrentIndex(ui->lstIcons->currentIndex().row());
}

void SettingsDialog::saveSettings()
{
    // General -> Main view
    SettingsHelper::instance().setShowToolbar(ui->cbMVShowToolbar->isChecked());
    SettingsHelper::instance().setShowLocationInformation(ui->cbMVShowLocation->isChecked());
    SettingsHelper::instance().setShowRating(ui->cbMVShowRating->isChecked());
    SettingsHelper::instance().setShowFileInformation(ui->cbMVShowFileInfo->isChecked());

    // General -> Presentation
    SettingsHelper::instance().setPresentationShowLocationInformation(ui->cbPShowLocation->isChecked());
    SettingsHelper::instance().setPresentationShowRating(ui->cbPShowRating->isChecked());
    SettingsHelper::instance().setPresentationShowFileInformation(ui->cbPShowFileInfo->isChecked());
    SettingsHelper::instance().setPresentationInterval(ui->sbPPictureInterval->value());

    // Digikam -> Digikam configuration
    SettingsHelper::instance().setIntegrateDigikam(ui->cbDIntegrateDigikam->isChecked());
    SettingsHelper::instance().setDigikamDBFile(ui->leDDatabaseFile->text());
}

void SettingsDialog::loadSettings()
{
    // General -> Main view
    ui->cbMVShowToolbar->setChecked(SettingsHelper::instance().showToolbar());
    ui->cbMVShowLocation->setChecked(SettingsHelper::instance().showLocationInformation());
    ui->cbMVShowRating->setChecked(SettingsHelper::instance().showRating());
    ui->cbMVShowFileInfo->setChecked(SettingsHelper::instance().showFileInformation());

    // General -> Presentation
    ui->cbPShowLocation->setChecked(SettingsHelper::instance().presentationShowLocationInformation());
    ui->cbPShowRating->setChecked(SettingsHelper::instance().presentationShowRating());
    ui->cbPShowFileInfo->setChecked(SettingsHelper::instance().presentationShowFileInformation());
    ui->sbPPictureInterval->setValue(SettingsHelper::instance().presentationInterval());

    // Digikam -> Digikam configuration
    ui->cbDIntegrateDigikam->setChecked(SettingsHelper::instance().integrateDigikam());
    ui->leDDatabaseFile->setText(SettingsHelper::instance().digikamDBFile());
}

void SettingsDialog::on_pbDSelectDatabaseFile_clicked()
{
    QString selected;
    QFileInfo initPath;

    initPath.setFile(SettingsHelper::instance().digikamDBFile());

    selected = QFileDialog::getOpenFileName(this,
                                            tr("Open Digikam database file"),
                                            initPath.absolutePath(),
                                            tr("Digikam database files (digikam*.db);; All files (*)"));

    if (!selected.isNull()) {
        setDBFileName(selected);
    }
}

bool SettingsDialog::setDBFileName(QString fileName)
{
    if (DigikamMetadata::isValidDBFile(fileName)) {
        ui->leDDatabaseFile->setText(fileName);

        return true;
    }
    else {
        QMessageBox::critical (this,
                               tr("Photo Viewer"),
                               tr("Invalid database file"));

        return false;
    }
}

void SettingsDialog::on_leDDatabaseFile_editingFinished()
{
    if (!setDBFileName(ui->leDDatabaseFile->text())) {
        ui->leDDatabaseFile->setFocus();
        ui->leDDatabaseFile->setText(SettingsHelper::instance().digikamDBFile());
    }
}
