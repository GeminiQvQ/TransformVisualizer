// Local Includes
#include <GXQt/Utility/WidgetState/WidgetStateManager.h>

// Project Includes
#include <GXQt/Utility/WidgetState/WidgetState.h>

// Third-Party Includes
#include <QApplication>
#include <QHash>
#include <QJsonObject>
#include <QWidget>

namespace GX::QT
{
	//-----------------------------------------------------------------------------------------------------
	// Internal
	//-----------------------------------------------------------------------------------------------------

	struct WidgetStateManager::Internal
	{
		struct Entry
		{
			QJsonObject state;
			bool		restored;
			bool		dirty;

			Entry()
				: state {}
				, restored {false}
				, dirty {false}
			{
			}

			Entry(const QJsonObject& state, bool restored)
				: state {state}
				, restored {restored}
				, dirty {false}
			{
			}
		};

		QHash<QString, Entry> entries;
	};

	//-----------------------------------------------------------------------------------------------------
	// Construction & Destruction
	//-----------------------------------------------------------------------------------------------------

	WidgetStateManager::WidgetStateManager()
		: m {std::make_unique<Internal>()}
	{
		qApp->installEventFilter(this);
	}

	WidgetStateManager::~WidgetStateManager()
	{
		qApp->removeEventFilter(this);
	}

	//-----------------------------------------------------------------------------------------------------
	// Save & Load
	//-----------------------------------------------------------------------------------------------------

	void WidgetStateManager::save(QJsonObject& json)
	{
		for (auto it = m->entries.begin(); it != m->entries.end(); ++it)
		{
			json.insert(it.key(), it->state);
		}
	}

	void WidgetStateManager::load(const QJsonObject& json)
	{
		m->entries.clear();

		for (auto it = json.begin(); it != json.end(); ++it)
		{
			m->entries.insert(it.key(), Internal::Entry{it->toObject(), false});
		}
	}

	//-----------------------------------------------------------------------------------------------------
	// Qt Events
	//-----------------------------------------------------------------------------------------------------

	bool WidgetStateManager::eventFilter(QObject* object, QEvent* event)
	{
		if (!object->isWidgetType())
		{
			return false;
		}

		auto widget = static_cast<QWidget*>(object);
		auto id = widget->property("WidgetStateID").toString();

		if (id.isEmpty())
		{
			return false;
		}

		auto entry_it = m->entries.find(id);
		if (entry_it == m->entries.end())
		{
			entry_it = m->entries.insert(id, Internal::Entry{QJsonObject{}, true});
		}
		
		if (event->type() == QEvent::Show)
		{
			if (!entry_it->restored)
			{
				load_widget_state(*widget, entry_it->state);
				entry_it->restored = true;
			}

			entry_it->dirty = true;
		}

		if (event->type() == QEvent::Hide)
		{
			if (entry_it->dirty)
			{
				save_widget_state(*widget, entry_it->state);
				entry_it->dirty = false;
			}
		}

		if (event->type() == QEvent::Destroy)
		{
			entry_it->restored = false;
		}

		return false;
	}
}