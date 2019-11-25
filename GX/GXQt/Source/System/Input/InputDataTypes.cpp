// Local Includes
#include <GXQt/System/Input/InputDataTypes.h>

namespace GX::QT
{
	//-----------------------------------------------------------------------------------------------------
	// Input Triggers
	//-----------------------------------------------------------------------------------------------------

	bool InputKeyboardKey::operator==(const InputKeyboardKey& rhs) const
	{
		return (key == rhs.key);
	}

	//-----------------------------------------------------------------------------------------------------

	bool InputMouseButton::operator==(const InputMouseButton& rhs) const
	{
		return (button == rhs.button);
	}

	//-----------------------------------------------------------------------------------------------------

	bool InputMouseMove::operator==(const InputMouseMove& /*rhs*/) const
	{
		return true;
	}

	//-----------------------------------------------------------------------------------------------------

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

	InputTrigger::InputTrigger(const QVector<InputData>& modifiers, InputData trigger, InputTriggerType type)
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