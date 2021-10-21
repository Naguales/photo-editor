#include "coloritemdelegate.h"
#include "constants.h"

#include <QPainter>
#include <QApplication>

#include <cmath>

ColorItemDelegate::ColorItemDelegate(QObject* parent)
    : QStyledItemDelegate(parent)
{}

void ColorItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    auto opt = option;
    initStyleOption(&opt, index);
    opt.decorationSize.setWidth(opt.rect.width());
    auto style =  opt.widget ? opt.widget->style() : QApplication::style();
    style->drawControl(QStyle::CE_ItemViewItem, &opt, painter, opt.widget);
}
