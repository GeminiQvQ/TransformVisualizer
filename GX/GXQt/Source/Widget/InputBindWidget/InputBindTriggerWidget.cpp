// Local Includes
#include <GXQt/Widget/InputBindWidget/InputBindTriggerWidget.h>

namespace GX::QT
{
	//-----------------------------------------------------------------------------------------------------
	// Internal
	//-----------------------------------------------------------------------------------------------------

	struct InputBindTriggerWidget::Internal
	{
		InputDataList data;
		InputDataList data_original;
		InputDataList data_current;
		bool		  editing_enabled;

		Internal()
			: data {}
			, data_original {}
			, data_current {}
			, editing_enabled {false}
		{
		}
	};

	//-----------------------------------------------------------------------------------------------------
	// Construction & Destruction
	//-----------------------------------------------------------------------------------------------------

	InputBindTriggerWidget::InputBindTriggerWidget(QWidget* parent)
		: QLineEdit {parent}
		, m {std::make_unique<Internal>()}
	{
		setReadOnly(true);
		setContextMenuPolicy(Qt::NoContextMenu);
		installEventFilter(this);

		// TEMP
		setStyleSheet("GX--QT--InputBindTriggerWidget[editing_enabled=true] { background-color: red; }");
	}

	InputBindTriggerWidget::~InputBindTriggerWidget()
	{
	}

	//-----------------------------------------------------------------------------------------------------
	// Properties
	//-----------------------------------------------------------------------------------------------------

	void InputBindTriggerWidget::set_data(const InputDataList& data)
	{
		m->data = data;
		update_text();
	}

	const InputDataList& InputBindTriggerWidget::data() const
	{
		return m->data;
	}

	//-----------------------------------------------------------------------------------------------------

	void InputBindTriggerWidget::set_editing_enabled(bool enabled)
	{
		if (enabled)
		{
			begin_editing();
		}
		else
		{
			end_editing(false);
		}
	}

	bool InputBindTriggerWidget::editing_enabled() const
	{
		return m->editing_enabled;
	}

	//-----------------------------------------------------------------------------------------------------
	// Qt Events
	//-----------------------------------------------------------------------------------------------------

	void InputBindTriggerWidget::focusInEvent(QFocusEvent* event)
	{
		QLineEdit::focusInEvent(event);
		setSelection(0, 0);
	}

	void InputBindTriggerWidget::focusOutEvent(QFocusEvent* event)
	{
		QLineEdit::focusOutEvent(event);

		if (m->editing_enabled)
		{
			end_editing(false);
		}
	}

	bool InputBindTriggerWidget::eventFilter(QObject* /*object*/, QEvent* event)
	{
		if (event->type() == QEvent::KeyPress)
		{
			const auto key_event = static_cast<QKeyEvent*>(event);

			if (m->editing_enabled)
			{
				if (key_event->key() == Qt::Key_Escape)
				{
					end_editing(false);
					return true;
				}

				if (m->data_current.isEmpty())
				{
					m->data_original = m->data;
					m->data.clear();
				}

				add_data(m->data, InputKeyboardKey{key_event->key()});
				add_data(m->data_current, InputKeyboardKey{key_event->key()});
			}
			else
			{
				if (key_event->key() == Qt::Key_Enter ||
					key_event->key() == Qt::Key_Return)
				{
					begin_editing();
					return true;
				}
			}
		}

		if (event->type() == QEvent::KeyRelease && m->editing_enabled)
		{
			const auto key_event = static_cast<QKeyEvent*>(event);
			remove_data(m->data_current, InputKeyboardKey{key_event->key()});
		}

		if (event->type() == QEvent::MouseButtonPress && m->editing_enabled)
		{
			const auto mouse_event = static_cast<QMouseEvent*>(event);

			if (m->data_current.isEmpty())
			{
				m->data_original = m->data;
				m->data.clear();
			}

			add_data(m->data, InputMouseButton{mouse_event->button()});
			add_data(m->data_current, InputMouseButton{mouse_event->button()});

			return true;
		}

		if (event->type() == QEvent::MouseButtonRelease && m->editing_enabled)
		{
			const auto mouse_event = static_cast<QMouseEvent*>(event);
			remove_data(m->data_current, InputMouseButton{mouse_event->button()});
		}

		if (event->type() == QEvent::MouseButtonDblClick && m->editing_enabled)
		{
			begin_editing();
			return true;
		}

		return false;
	}

	//-----------------------------------------------------------------------------------------------------
	// Helpers
	//-----------------------------------------------------------------------------------------------------

	void InputBindTriggerWidget::add_data(InputDataList& container, const InputData& data)
	{
		if (!container.contains(data))
		{
			container.push_back(data);
			update_text();
		}
	}

	void InputBindTriggerWidget::remove_data(InputDataList& container, const InputData& data)
	{
		const auto it = std::find(container.begin(), container.end(), data);
		if (it != container.end())
		{
			container.erase(it);
			update_text();

			if (container.isEmpty())
			{
				end_editing(true);
			}
		}
	}

	//-----------------------------------------------------------------------------------------------------

	void InputBindTriggerWidget::begin_editing()
	{
		m->data_original = m->data;
		m->editing_enabled = true;

		update_text();

		style()->unpolish(this);
		style()->polish(this);
	}

	void InputBindTriggerWidget::end_editing(bool commit)
	{
		if (!commit)
		{
			m->data = m->data_original;
		}

		m->data_original.clear();
		m->data_current.clear();
		m->editing_enabled = false;

		update_text();

		style()->unpolish(this);
		style()->polish(this);

		if (commit)
		{
			emit signal_data_changed(m->data);
		}
	}

	//-----------------------------------------------------------------------------------------------------

	void InputBindTriggerWidget::update_text()
	{
		QStringList parts;
		parts.reserve(m->data.size());

		for (const auto& data : m->data)
		{
			parts.push_back(std::visit([] (auto&& v) { return v.to_string(); }, data));
		}

		setText(parts.join(" + "));
	}
}