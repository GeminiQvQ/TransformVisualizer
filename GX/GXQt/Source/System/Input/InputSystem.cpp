// Local Includes
#include <GXQt/System/Input/InputSystem.h>

// Project Includes
#include <GXLib/StdExt/VariantIndex.h>
#include <GXQt/Utility/Object.h>

// Third-Party Includes
#include <QApplication>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QPoint>
#include <QSet>
#include <QVector>
#include <QWheelEvent>
#include <QWidget>

namespace GX::QT
{
	//-----------------------------------------------------------------------------------------------------
	// Internal
	//-----------------------------------------------------------------------------------------------------

	struct InputSystem::Internal
	{
		QVector<InputBinding>	bindings;
		QSet<int>				bindings_enabled;
		QSet<int>				key_state;
		QSet<int>				button_state;
		QPointF					cursor_pos;
		QPointF					cursor_pos_prev;
		int						wheel_delta;
		QVector<int>			handled_events;

		Internal()
			: wheel_delta {0}
		{
		}
	};

	//-----------------------------------------------------------------------------------------------------
	// Construction & Destruction
	//-----------------------------------------------------------------------------------------------------

	InputSystem::InputSystem()
		: m {std::make_unique<Internal>()}
	{
		qApp->installEventFilter(this);
		startTimer(1);
	}

	InputSystem::~InputSystem()
	{
		qApp->removeEventFilter(this);
	}

	//-----------------------------------------------------------------------------------------------------
	// Binding
	//-----------------------------------------------------------------------------------------------------

	void InputSystem::bind(const InputTrigger& trigger, int poll_id)
	{
		bind(trigger, nullptr, poll_id);
	}

	void InputSystem::bind(const InputTrigger& trigger, const InputCallback& callback)
	{
		bind(trigger, nullptr, callback);
	}

	void InputSystem::bind(const InputTrigger& trigger, QObject* receiver, int poll_id)
	{
		InputBinding binding;
		binding.trigger = trigger;
		binding.receiver = receiver;
		binding.poll_id = poll_id;
		m->bindings.push_back(binding);
	}

	void InputSystem::bind(const InputTrigger& trigger, QObject* receiver, const InputCallback& callback)
	{
		InputBinding binding;
		binding.trigger = trigger;
		binding.receiver = receiver;
		binding.callback = callback;
		m->bindings.push_back(binding);
	}

	//-----------------------------------------------------------------------------------------------------
	// Qt Events
	//-----------------------------------------------------------------------------------------------------

	void InputSystem::timerEvent(QTimerEvent* /*event*/)
	{
		m->handled_events.clear();
	}

	bool InputSystem::eventFilter(QObject* /*object*/, QEvent* event)
	{
		int event_hash = ::qHash(event) ^ ::qHash((uint)event->type());
		if (m->handled_events.contains(event_hash))
		{
			return false;
		}
		m->handled_events.push_back(event_hash);

		if (event->type() == QEvent::KeyPress)
		{
			const auto key_event = static_cast<QKeyEvent*>(event);

			m->key_state.remove(key_event->key());
			const auto modifiers = get_modifiers();
			m->key_state.insert(key_event->key());

			if (enable_binding(modifiers, InputKeyboardKey{key_event->key()}))
			{
				event->accept();
				return true;
			}
		}

		if (event->type() == QEvent::KeyRelease)
		{
			const auto key_event = static_cast<QKeyEvent*>(event);

			m->key_state.remove(key_event->key());

			disable_binding(InputKeyboardKey{key_event->key()});
		}

		if (event->type() == QEvent::MouseButtonPress)
		{
			const auto mouse_event = static_cast<QMouseEvent*>(event);

			const auto widget = qApp->widgetAt(mouse_event->globalPos());
			if (widget != nullptr && !widget->hasFocus())
			{
				return false;
			}

			m->button_state.remove(mouse_event->button());
			const auto modifiers = get_modifiers();
			m->button_state.insert(mouse_event->button());

			if (enable_binding(modifiers, InputMouseButton{mouse_event->button()}))
			{
				event->accept();
				return true;
			}
		}

		if (event->type() == QEvent::MouseButtonRelease)
		{
			const auto mouse_event = static_cast<QMouseEvent*>(event);

			m->button_state.remove(mouse_event->button());

			disable_binding(InputMouseButton{mouse_event->button()});
		}

		if (event->type() == QEvent::MouseMove)
		{
			const auto mouse_event = static_cast<QMouseEvent*>(event);

			if (mouse_event->screenPos() != m->cursor_pos)
			{
				m->cursor_pos_prev = m->cursor_pos;
				m->cursor_pos = mouse_event->screenPos();
			}

			enable_binding(get_modifiers(), InputMouseMove{});
		}

		if (event->type() == QEvent::Wheel)
		{
			const auto wheel_event = static_cast<QWheelEvent*>(event);

			m->wheel_delta = wheel_event->delta();

			enable_binding(get_modifiers(), InputMouseWheel{});
		}

		if (event->type() == QEvent::FocusIn ||
			event->type() == QEvent::FocusOut)
		{
			m->key_state.clear();
			m->button_state.clear();
		}

		return false;
	}

	//-----------------------------------------------------------------------------------------------------
	// Helpers
	//-----------------------------------------------------------------------------------------------------

	bool InputSystem::enable_binding(const QVector<InputData>& modifiers, const InputData& trigger)
	{
		QVector<const InputBinding*> candidates;

		for (const auto& binding : m->bindings)
		{
			const bool match = (qHash(binding.trigger.modifiers) == qHash(modifiers) && binding.trigger.trigger == trigger && binding.trigger.type == InputTriggerType::OnEnabled);

			if (match && (binding.receiver == nullptr || object_or_child_has_focus(binding.receiver)))
			{
				candidates.push_back(&binding);
			}
		}

		if (candidates.isEmpty())
		{
			return false;
		}

		QVector<const InputBinding*> candidates_final;

		std::copy_if(candidates.begin(), candidates.end(), std::back_inserter(candidates_final), [&candidates] (auto&& candidate_a)
		{
			return std::all_of(candidates.begin(), candidates.end(), [&candidate_a] (auto&& candidate_b)
			{
				return (candidate_a->receiver == candidate_b->receiver || !is_ancestor_of(candidate_a->receiver, candidate_b->receiver));
			});
		});

		if (candidates_final.size() != 1)
		{
			return false;
		}

		const auto& binding = *candidates_final[0];

		if (binding.receiver != nullptr && binding.receiver->isWidgetType())
		{
			const auto modal_widget = qApp->activeModalWidget();

			if (modal_widget != nullptr && binding.receiver != modal_widget)
			{
				return false;
			}
		}

		if (binding.poll_id >= 0)
		{
			m->bindings_enabled.insert(binding.poll_id);
		}
		
		if (is_variant<TriggerInputCallback>(binding.callback))
		{
			if (binding.trigger.type == InputTriggerType::OnEnabled)
			{
				std::get<TriggerInputCallback>(binding.callback)();
			}
		}

		if (is_variant<MouseMoveInputCallback>(binding.callback))
		{
			MouseMoveInputParameters parameters;
			parameters.pos = m->cursor_pos;
			parameters.pos_prev = m->cursor_pos_prev;

			if (binding.receiver != nullptr && binding.receiver->isWidgetType())
			{
				const QPointF offset = static_cast<QWidget*>(binding.receiver)->mapFromGlobal(QPoint{});
				parameters.pos += offset;
				parameters.pos_prev += offset;
			}

			std::get<MouseMoveInputCallback>(binding.callback)(parameters);
		}

		if (is_variant<MouseWheelInputCallback>(binding.callback))
		{
			MouseWheelInputParameters parameters;
			parameters.pos = m->cursor_pos;
			parameters.delta = m->wheel_delta;

			if (binding.receiver != nullptr && binding.receiver->isWidgetType())
			{
				const QPointF offset = static_cast<QWidget*>(binding.receiver)->mapFromGlobal(QPoint{});
				parameters.pos += offset;
			}

			std::get<MouseWheelInputCallback>(binding.callback)(parameters);
		}

		return true;
	}

	void InputSystem::disable_binding(const InputData& data)
	{
		for (const auto& binding : m->bindings)
		{
			if (binding.poll_id != -1 && binding.trigger.modifiers.contains(data) || binding.trigger.trigger == data)
			{
				if (is_variant<TriggerInputCallback>(binding.callback))
				{
					if (binding.trigger.type == InputTriggerType::OnDisabled)
					{
						std::get<TriggerInputCallback>(binding.callback)();
					}
				}

				m->bindings_enabled.remove(binding.poll_id);
			}
		}
	}

	QVector<InputData> InputSystem::get_modifiers() const
	{
		QVector<InputData> modifiers;
		modifiers.reserve(m->key_state.size() + m->button_state.size());
		
		for (auto key : m->key_state)
		{
			modifiers.push_back(InputKeyboardKey{key});
		}

		for (auto button : m->button_state)
		{
			modifiers.push_back(InputMouseButton{button});
		}

		return modifiers;
	}
}