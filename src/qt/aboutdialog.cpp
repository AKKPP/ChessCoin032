#include "aboutdialog.h"
#include "ui_aboutdialog.h"
#include "clientmodel.h"

#include "version.h"

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);

#if (defined (_WIN32) || defined (_WIN64))
	setMinimumSize(600, 300);
	setMaximumSize(600, 300);
#elif (defined (LINUX) || defined (__linux__))
    setMinimumSize(600, 360);
    setMaximumSize(600, 360);
#else
    setMinimumSize(600, 390);
    setMaximumSize(600, 390);
#endif
}

void AboutDialog::setModel(ClientModel *model)
{
    if(model)
    {
        ui->versionLabel->setText(model->formatFullVersion());
    }
}

AboutDialog::~AboutDialog()
{
    delete ui;
}

void AboutDialog::on_buttonBox_accepted()
{
    close();
}

