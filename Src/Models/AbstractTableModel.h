#pragma once
#include <QAbstractTableModel>
#include "TableHeaderItem.h"

namespace neo {
	

	class TableHeaderModel;
	class AbstractTableModelPrivate;
	class AbstractTableModel : public QAbstractTableModel {
		Q_OBJECT

	public:
		//for qml
		AbstractTableModel(QObject* parent = nullptr);
		//init ctor
		AbstractTableModel(const QString& jsonFilePath, const QString& keyPath, QObject* parent = nullptr);

		~AbstractTableModel();
		QHash<int, QByteArray> roleNames() const override;
		int columnCount(const QModelIndex& parent) const override final;


	public slots:
		//根据内容大小调整
		int resizeToContents(int column, const QFont& font);

		neo::TableHeaderModel* headerModel() const;

		void move(int from, int to);

		void refresh();

	public:
		/**
		 * \brief
		 * \param functor QVariant: data(), 返回值计算内容宽度
		 */
		void setContentMetrics(std::function<int(const QVariant&)>&& functor);

	protected:
		virtual QVariant fieldProvider(const QString& field) const = 0;

	private:
		Q_DECLARE_PRIVATE(AbstractTableModel)
		AbstractTableModelPrivate* const d_ptr = nullptr;
	};


}
