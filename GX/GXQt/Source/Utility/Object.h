#pragma once

// Forward Declarations
class QObject;

namespace GX::QT
{
	bool object_or_child_has_focus	(const QObject* object);
	bool is_ancestor_of				(const QObject* object, const QObject* child);
}