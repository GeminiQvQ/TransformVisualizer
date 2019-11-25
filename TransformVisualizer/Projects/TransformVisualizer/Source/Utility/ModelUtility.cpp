// Local Includes
#include <TransformVisualizer/Utility/ModelUtility.h>

// Third-Party Includes
#include <QColor>
#include <QFont>
#include <QModelIndex>
#include <QVariant>

namespace TransformVisualizer
{
	QVariant create_section(const QString& name, const QModelIndex& index, int role)
	{
		if (index.column() == 0 && role == Qt::DisplayRole)
		{
			return " ~ " + name;
		}

		if (role == Qt::FontRole)
		{
			QFont font;
			font.setBold(true);
			return font;
		}

		if (role == Qt::BackgroundRole)
		{
			return QColor{90, 105, 120};
		}

		return QVariant{};
	}
}