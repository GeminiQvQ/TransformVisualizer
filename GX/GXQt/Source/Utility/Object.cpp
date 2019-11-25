// Local Includes
#include <GXQt/Utility/Object.h>

// Third-Party Includes
#include <QWidget>

namespace GX::QT
{
	bool object_or_child_has_focus(const QObject* object)
	{
		if (object->isWidgetType() && static_cast<const QWidget*>(object)->hasFocus())
		{
			return true;
		}

		for (auto&& child : object->children())
		{
			if (object_or_child_has_focus(child))
			{
				return true;
			}
		}

		return false;
	}

	bool is_ancestor_of(const QObject* object, const QObject* child)
	{
		return (object != nullptr && child != nullptr && (object == child->parent() || is_ancestor_of(object, child->parent())));
	}
}