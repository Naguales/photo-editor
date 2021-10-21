#pragma once

#include <QStyledItemDelegate>

class ColorItemDelegate : public QStyledItemDelegate
{
	Q_OBJECT

public:
    ColorItemDelegate(QObject* parent = nullptr);
    ~ColorItemDelegate() = default;

	void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const Q_DECL_OVERRIDE;
};
