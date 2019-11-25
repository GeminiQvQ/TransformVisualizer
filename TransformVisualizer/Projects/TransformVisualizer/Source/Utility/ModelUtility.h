#pragma once

// Forward Declarations
class QModelIndex;
class QString;
class QVariant;

namespace TransformVisualizer
{
	QVariant create_section(const QString& name, const QModelIndex& index, int role);
}