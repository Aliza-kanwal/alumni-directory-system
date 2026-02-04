#include "customlistwidget.h"
#include <QFocusEvent>

CustomListWidget::CustomListWidget(QWidget *parent) : QListWidget(parent) {}

void CustomListWidget::focusOutEvent(QFocusEvent *event) {
    QListWidget::focusOutEvent(event);
    this->hide(); // Hide the list widget when it loses focus
}
