// Local Includes
#include <GXQt/System/Input/InputDataTypes.h>

namespace GX::QT
{
	//-----------------------------------------------------------------------------------------------------
	// Input Triggers
	//-----------------------------------------------------------------------------------------------------

	QString InputNone::to_string() const
	{
		return QString{};
	}

	bool InputNone::operator==(const InputNone& /*rhs*/) const
	{
		return true;
	}

	//-----------------------------------------------------------------------------------------------------

	QString InputKeyboardKey::to_string() const
	{
		static const QHash<int, QString> s_map
		{
			{ Qt::Key_Shift,	"Shift" },
			{ Qt::Key_Control,	"Ctrl"	},
			{ Qt::Key_Alt,		"Alt"	},
			{ Qt::Key_Meta,		"Win"	}
		};

		const auto it = s_map.find(key);
		return (it != s_map.end() ? *it : QKeySequence{key}.toString());
	}

	bool InputKeyboardKey::operator==(const InputKeyboardKey& rhs) const
	{
		return (key == rhs.key);
	}

	//-----------------------------------------------------------------------------------------------------

	QString InputMouseButton::to_string() const
	{
		static const QHash<int, QString> s_map
		{
			{ Qt::LeftButton,		"LeftButton"	},
			{ Qt::RightButton,		"RightButton"	},
			{ Qt::MiddleButton,		"MiddleButton"	},
			{ Qt::BackButton,		"BackButton"	},
			{ Qt::ForwardButton,	"ForwardButton" },
			{ Qt::TaskButton,		"TaskButton"	},
			{ Qt::LeftButton,		"LeftButton"	},
			{ Qt::ExtraButton4,		"ExtraButton4"	},
			{ Qt::ExtraButton5,		"ExtraButton5"	},
			{ Qt::ExtraButton6,		"ExtraButton6"	},
			{ Qt::ExtraButton7,		"ExtraButton7"	},
			{ Qt::ExtraButton8,		"ExtraButton8"	},
			{ Qt::ExtraButton9,		"ExtraButton9"	},
			{ Qt::ExtraButton10,	"ExtraButton10"	},
			{ Qt::ExtraButton11,	"ExtraButton11"	},
			{ Qt::ExtraButton12,	"ExtraButton12"	},
			{ Qt::ExtraButton13,	"ExtraButton13"	},
			{ Qt::ExtraButton14,	"ExtraButton14"	},
			{ Qt::ExtraButton15,	"ExtraButton15"	},
			{ Qt::ExtraButton16,	"ExtraButton16"	},
			{ Qt::ExtraButton17,	"ExtraButton17"	},
			{ Qt::ExtraButton18,	"ExtraButton18"	},
			{ Qt::ExtraButton19,	"ExtraButton19"	},
			{ Qt::ExtraButton20,	"ExtraButton20"	},
			{ Qt::ExtraButton21,	"ExtraButton21"	},
			{ Qt::ExtraButton22,	"ExtraButton22"	},
			{ Qt::ExtraButton23,	"ExtraButton23"	},
			{ Qt::ExtraButton24,	"ExtraButton24"	}
		};

		const auto it = s_map.find(button);
		return (it != s_map.end() ? *it : QString{});
	}

	bool InputMouseButton::operator==(const InputMouseButton& rhs) const
	{
		return (button == rhs.button);
	}

	//-----------------------------------------------------------------------------------------------------

	QString InputMouseMove::to_string() const
	{
		return "MouseMove";
	}

	bool InputMouseMove::operator==(const InputMouseMove& /*rhs*/) const
	{
		return true;
	}

	//-----------------------------------------------------------------------------------------------------

	QString InputMouseWheel::to_string() const
	{
		return "MouseWheel";
	}

	bool InputMouseWheel::operator==(const InputMouseWheel& /*rhs*/) const
	{
		return true;
	}

	//-----------------------------------------------------------------------------------------------------

	InputTrigger::InputTrigger()
		: modifiers {}
		, trigger {}
		, type {InputTriggerType::OnEnabled}
	{
	}

	InputTrigger::InputTrigger(InputData trigger, InputTriggerType type)
		: modifiers {}
		, trigger {trigger}
		, type {type}
	{
	}

	InputTrigger::InputTrigger(InputData modifier, InputData trigger, InputTriggerType type)
		: modifiers {modifier}
		, trigger {trigger}
		, type {type}
	{
	}

	InputTrigger::InputTrigger(const InputDataList& modifiers, InputData trigger, InputTriggerType type)
		: modifiers {modifiers}
		, trigger {trigger}
		, type {type}
	{
	}

	bool InputTrigger::valid() const
	{
		return (trigger.index() != std::variant_npos);
	}

	bool InputTrigger::operator==(const InputTrigger& rhs) const
	{
		const bool modifiers_equal = std::all_of(modifiers.begin(), modifiers.end(), [&rhs] (auto&& modifier)
		{
			return rhs.modifiers.contains(modifier);
		});

		return (modifiers.size() == rhs.modifiers.size() && modifiers_equal && trigger == rhs.trigger && type == rhs.type);
	}

	//-----------------------------------------------------------------------------------------------------
	// Input Binding
	//-----------------------------------------------------------------------------------------------------

	InputBinding::InputBinding()
		: trigger {}
		, callback {}
		, receiver {}
		, poll_id {-1}
	{
	}
}