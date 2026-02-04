#include "searchresultdialog.h"
#include "ui_searchresultdialog.h"
#include <QPixmap>
#include <QDebug>
#include <QGraphicsOpacityEffect>

SearchResultDialog::SearchResultDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SearchResultDialog)
{
    ui->setupUi(this);

    // Make line edits read-only
    ui->namelineEdit->setReadOnly(true);
    ui->fieldlineEdit->setReadOnly(true);
    ui->yearlineEdit->setReadOnly(true);
    ui->linkedInlineEdit->setReadOnly(true);
    ui->lineEdit_5->setReadOnly(true);

    // Load logo image
    QPixmap image(":/images/nedlogo.jpg");
    if (image.isNull()) {
        qDebug() << "Error: Image not loaded! Check path:" << ":/images/nedlogo.jpg";
    } else {
        ui->label_10->setPixmap(image);
        qDebug() << "Image loaded successfully.";
    }
}

SearchResultDialog::~SearchResultDialog()
{
    delete ui;
}

void SearchResultDialog::setSearchResult(const QString &name,
                                         const QString &graduationYear,
                                         const QString &linkedIn,
                                         const QString &field,
                                         const QString &degree)
{
    ui->namelineEdit->setText(name);
    ui->fieldlineEdit->setText(field);
    ui->yearlineEdit->setText(graduationYear);
    ui->linkedInlineEdit->setText(linkedIn);
    ui->lineEdit_5->setText(degree);

    // Set tooltips
    ui->namelineEdit->setToolTip(name);
    ui->fieldlineEdit->setToolTip(field);
    ui->yearlineEdit->setToolTip(graduationYear);
    ui->linkedInlineEdit->setToolTip(linkedIn);
    ui->lineEdit_5->setToolTip(degree);
}

void SearchResultDialog::setPictureTransparency(qreal opacity)
{
    if (!ui->label_10->pixmap().isNull()) {
        QGraphicsOpacityEffect *effect = new QGraphicsOpacityEffect(this);
        effect->setOpacity(opacity);
        ui->label_10->setGraphicsEffect(effect);
    }
}
