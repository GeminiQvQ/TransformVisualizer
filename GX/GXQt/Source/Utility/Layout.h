#pragma once

// Forward Declarations
class QGridLayout;

namespace GX::QT
{
	int num_row_items(const QGridLayout& layout, int col);
	int num_col_items(const QGridLayout& layout, int row);
}