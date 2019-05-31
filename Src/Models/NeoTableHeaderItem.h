#pragma once

#include <QObject>

struct NeoTableHeaderItemPrivate;
class NeoTableHeaderItem : public QObject
{
	Q_OBJECT

	Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
	Q_PROPERTY(QString subTitle READ subTitle WRITE setSubTitle NOTIFY subTitleChanged)
	Q_PROPERTY(qreal itemWidth READ itemWidth WRITE setItemWidth NOTIFY itemWidthChanged)
	Q_PROPERTY(bool sortable READ sortable WRITE setSortable NOTIFY sortableChanged)
	Q_PROPERTY(QString toolTip READ toolTip WRITE setToolTip NOTIFY toolTipChanged)
	Q_PROPERTY(qreal titleFontSize READ titleFontSize WRITE setTitleFontSize NOTIFY titleFontSizeChanged)
	Q_PROPERTY(qreal subTitleFontSize READ subTitleFontSize WRITE setSubTitleFontSize NOTIFY subTitleFontSizeChanged)
	Q_PROPERTY(bool visible READ visible WRITE setVisible NOTIFY visibleChanged)
	Q_PROPERTY(bool movable READ movable WRITE setMovable NOTIFY movableChanged)
	Q_PROPERTY(bool resizable READ resizable WRITE setResizable NOTIFY resizableChanged)
	Q_PROPERTY(int minWidth READ minWidth WRITE setMinWidth NOTIFY minWidthChanged)
public:
	NeoTableHeaderItem(QObject* parent = nullptr);
	NeoTableHeaderItem(const QString &title,qreal itemWidth,qreal titleFontSize,QObject* parent = nullptr);
	~NeoTableHeaderItem();

signals:
	void titleChanged(const QString &title);
	void subTitleChanged(const QString& subTitle);
	void itemWidthChanged(qreal itemWidth);
	void sortableChanged(bool sortable);
	void toolTipChanged(const QString& toolTip);
	void titleFontSizeChanged(qreal titleFontSize);
	void subTitleFontSizeChanged(qreal subTitleFontSize);
	void visibleChanged(bool visible);
	void movableChanged(bool movable);
	void resizableChanged(bool resizable);
	void minWidthChanged(int minWidth);

public slots:
	QString title() const;
	void setTitle(const QString& title);

	QString subTitle() const;
	void setSubTitle(const QString& subTitle);

	qreal itemWidth() const;
	void setItemWidth(qreal itemWidth);

	bool sortable() const;
	void setSortable(bool sortable);

	QString toolTip() const;
	void setToolTip(const QString& toolTip);

	qreal titleFontSize() const;
	void setTitleFontSize(qreal titleFontSize);

	qreal subTitleFontSize() const;
	void setSubTitleFontSize(qreal subTitleFontSize);

	bool visible() const;
	void setVisible(bool visible);

	bool movable() const;
	void setMovable(bool movable);

	bool resizable() const;
	void setResizable(bool resizable);

	int minWidth() const;
	void setMinWidth(int minWidth);

private:
	Q_DECLARE_PRIVATE(NeoTableHeaderItem)
	NeoTableHeaderItemPrivate* const d_ptr = nullptr;
};

