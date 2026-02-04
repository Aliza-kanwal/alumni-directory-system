#include "clickabledelegate.h"
#include <QApplication>
#include <QMouseEvent>
#include <QDesktopServices>

ClickableDelegate::ClickableDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
}

void ClickableDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                              const QModelIndex &index) const
{
    QStyleOptionViewItem opt = option;
    initStyleOption(&opt, index);

    // Style as clickable link
    opt.palette.setColor(QPalette::Text, QColor(0, 0, 255));
    opt.font.setUnderline(true);

    QStyledItemDelegate::paint(painter, opt, index);
}

bool ClickableDelegate::editorEvent(QEvent *event, QAbstractItemModel *model,
                                    const QStyleOptionViewItem &option,
                                    const QModelIndex &index)
{
    if (event->type() == QEvent::MouseButtonRelease) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        if (mouseEvent->button() == Qt::LeftButton &&
            option.rect.contains(mouseEvent->pos())) {
            QString link = index.data(Qt::DisplayRole).toString();
            if (!link.isEmpty() && link.startsWith("http", Qt::CaseInsensitive)) {
                emit linkClicked(link);
                return true;
            }
        }
    }
    return QStyledItemDelegate::editorEvent(event, model, option, index);
}
