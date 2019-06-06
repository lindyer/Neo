#pragma once

#include <QObject>
#include <QJsonValue>

class NeoTableHeaderModelPrivate;
class NeoTableHeaderItemPrivate;
class NeoTableHeaderItem : public QObject
{
	Q_OBJECT

	Q_PROPERTY(int index READ index WRITE setIndex NOTIFY indexChanged)
	Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
	Q_PROPERTY(int titleFontSize READ titleFontSize WRITE setTitleFontSize NOTIFY titleFontSizeChanged)
	Q_PROPERTY(int itemWidth READ itemWidth WRITE setItemWidth NOTIFY itemWidthChanged)
	Q_PROPERTY(bool sortable READ sortable WRITE setSortable NOTIFY sortableChanged)
	Q_PROPERTY(QString toolTip READ toolTip WRITE setToolTip NOTIFY toolTipChanged)
	Q_PROPERTY(bool visible READ visible WRITE setVisible NOTIFY visibleChanged)
	Q_PROPERTY(bool movable READ movable WRITE setMovable NOTIFY movableChanged)
	Q_PROPERTY(bool resizable READ resizable WRITE setResizable NOTIFY resizableChanged)
	Q_PROPERTY(int minWidth READ minWidth WRITE setMinWidth NOTIFY minWidthChanged)
	Q_PROPERTY(int titleHorizontalAlignment READ titleHorizontalAlignment WRITE setTitleHorizontalAlignment NOTIFY titleHorizontalAlignmentChanged)
	Q_PROPERTY(QJsonValue extraData READ extraData WRITE setExtraData NOTIFY extraDataChanged)
public:
	NeoTableHeaderItem(QObject* parent = nullptr);
	NeoTableHeaderItem(const QString &title,qreal itemWidth,qreal titleFontSize,QObject* parent = nullptr);
	~NeoTableHeaderItem();

signals:
	void saveChange(int index,const QString& key, const QJsonValue& value);
	void indexChanged(int index);
	void titleChanged(const QString &title);
	void itemWidthChanged(qreal itemWidth);
	void sortableChanged(bool sortable);
	void toolTipChanged(const QString& toolTip);
	void titleFontSizeChanged(qreal titleFontSize);
	void visibleChanged(bool visible);
	void movableChanged(bool movable);
	void resizableChanged(bool resizable);
	void minWidthChanged(int minWidth);
	void titleHorizontalAlignmentChanged(int horizontalAlignment);
	void extraDataChanged(const QJsonValue& extraData);

public slots:
	int index() const;
	void setIndex(int index);

	QString title() const;
	void setTitle(const QString& title);

	int itemWidth() const;
	void setItemWidth(int itemWidth);

	bool sortable() const;
	void setSortable(bool sortable);

	QString toolTip() const;
	void setToolTip(const QString& toolTip);

	int titleFontSize() const;
	void setTitleFontSize(int titleFontSize);

	bool visible() const;
	void setVisible(bool visible);

	bool movable() const;
	void setMovable(bool movable);

	bool resizable() const;
	void setResizable(bool resizable);

	int minWidth() const;
	void setMinWidth(int minWidth);

	int titleHorizontalAlignment() const;
	void setTitleHorizontalAlignment(int titleHorizontalAlignment);

	QJsonValue extraData() const;
	void setExtraData(const QJsonValue&extraData);

private:
	//NeoTableHeaderItemPrivate *

private:
	Q_DECLARE_PRIVATE(NeoTableHeaderItem)
	NeoTableHeaderItemPrivate* const d_ptr = nullptr;
	friend NeoTableHeaderModelPrivate;
};

