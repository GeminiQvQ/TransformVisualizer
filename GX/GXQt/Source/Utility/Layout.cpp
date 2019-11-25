// Local Includes
#include <GXQt/Utility/Layout.h>

// Third-Party Includes
#include <QGridLayout>

namespace GX::QT
{
	int num_row_items(const QGridLayout& layout, int col)
	{
		int i = 0;
		for (; i < layout.rowCount(); ++i)
		{
			if (layout.itemAtPosition(i, col) == nullptr)
			{
				break;
			}
		}

		return i;
	}

	int num_col_items(const QGridLayout& layout, int row)
	{
		int i = 0;
		for (; i < layout.columnCount(); ++i)
		{
			if (layout.itemAtPosition(row, i) == nullptr)
			{
				break;
			}
		}

		return i;
	}
}