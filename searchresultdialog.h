#ifndef SEARCHRESULTDIALOG_H
#define SEARCHRESULTDIALOG_H

#include <QDialog>
#include <QGraphicsOpacityEffect>

namespace Ui {
class SearchResultDialog;
}

class SearchResultDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SearchResultDialog(QWidget *parent = nullptr);
    ~SearchResultDialog();

    void setSearchResult(const QString &name,
                         const QString &graduationYear,
                         const QString &linkedIn,
                         const QString &field,
                         const QString &degree);
    void setPictureTransparency(qreal opacity);

private:
    Ui::SearchResultDialog *ui;
};

#endif // SEARCHRESULTDIALOG_H
