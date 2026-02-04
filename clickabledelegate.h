#ifndef CLICKABLEDELEGATE_H
#define CLICKABLEDELEGATE_H

#include <QStyledItemDelegate>
#include <QPainter>
#include <QMouseEvent>
#include <QDesktopServices>
#include <QApplication>

class ClickableDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit ClickableDelegate(QObject *parent = nullptr);

protected:
    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const override;
    bool editorEvent(QEvent *event, QAbstractItemModel *model,
                     const QStyleOptionViewItem &option, const QModelIndex &index) override;

signals:
    void linkClicked(const QString &link);
};

#endif // CLICKABLEDELEGATE_H
