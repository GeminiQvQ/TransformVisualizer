#pragma once

// Project Includes
#include <GXQt/API.h>

// Stdlib Includes
#include <functional>
#include <variant>

// Third-Party Includes
#include <QPoint>
#include <QVector>

// Forward Declarations
class QObject;

namespace GX::QT
{
	//-----------------------------------------------------------------------------------------------------
	// Input Triggers
	//-----------------------------------------------------------------------------------------------------

	struct GXQT_API InputKeyboardKey
	{
		int key;

		bool operator == (const InputKeyboardKey& rhs) const;
	};

	using InputKey = InputKeyboardKey;

	struct GXQT_API InputMouseButton
	{
		int button;

		bool operator == (const InputMouseButton& rhs) const;
	};

	using InputButton = InputMouseButton;

	struct GXQT_API InputMouseMove
	{
		bool operator == (const InputMouseMove& rhs) const;
	};

	struct GXQT_API InputMouseWheel
	{
		bool operator == (const InputMouseWheel& rhs) const;
	};

	using InputData = std::variant
	<
		InputKeyboardKey,
		InputMouseButton,
		InputMouseMove,
		InputMouseWheel
	>;

	enum class InputTriggerType
	{
		OnEnabled,
		OnDisabled
	};

	struct GXQT_API InputTrigger
	{
		QVector<InputData>	modifiers;
		InputData			trigger;
		InputTriggerType	type;

				InputTrigger	();
				InputTrigger	(InputData trigger, InputTriggerType type = InputTriggerType::OnEnabled);
				InputTrigger	(InputData modifier, InputData trigger, InputTriggerType type = InputTriggerType::OnEnabled);
				InputTrigger	(const QVector<InputData>& modifiers, InputData trigger, InputTriggerType type = InputTriggerType::OnEnabled);

		bool	valid			() const;
		bool	operator ==		(const InputTrigger& rhs) const;
	};

	//-----------------------------------------------------------------------------------------------------
	// Input Callback
	//-----------------------------------------------------------------------------------------------------

	struct MouseMoveInputParameters
	{
		QPointF pos;
		QPointF pos_prev;
	};

	struct MouseWheelInputParameters
	{
		QPointF pos;
		int		delta;
	};

	using TriggerInputCallback = std::function<void()>;
	using MouseMoveInputCallback = std::function<void(const MouseMoveInputParameters&)>;
	using MouseWheelInputCallback = std::function<void(const MouseWheelInputParameters&)>;

	using InputCallback = std::variant
	<
		TriggerInputCallback,
		MouseMoveInputCallback,
		MouseWheelInputCallback
	>;

	//-----------------------------------------------------------------------------------------------------
	// Input Binding
	//-----------------------------------------------------------------------------------------------------

	struct InputBinding
	{
		InputTrigger	trigger;
		InputCallback	callback;
		QObject*		receiver;
		int				poll_id;

		InputBinding();
	};
}

namespace std
{
	//-----------------------------------------------------------------------------------------------------
	// std::hash Overloads
	//-----------------------------------------------------------------------------------------------------

	template <> struct hash<GX::QT::InputKeyboardKey>
	{
		size_t operator () (const GX::QT::InputKeyboardKey& v) const { return std::hash<int>{}(v.key); }
	};

	template <> struct hash<GX::QT::InputMouseButton>
	{
		size_t operator () (const GX::QT::InputMouseButton& v) const { return std::hash<int>{}(v.button); }
	};

	template <> struct hash<GX::QT::InputMouseMove>
	{
		size_t operator () (const GX::QT::InputMouseMove& /*v*/) const { return std::hash<int>{}(0); }
	};

	template <> struct hash<GX::QT::InputMouseWheel>
	{
		size_t operator () (const GX::QT::InputMouseWheel& /*v*/) const { return std::hash<int>{}(0); }
	};
}

namespace GX::QT
{
	//-----------------------------------------------------------------------------------------------------
	// qHash Overloads
	//-----------------------------------------------------------------------------------------------------

	inline uint qHash(const InputData& v)
	{
		return (uint)std::hash<InputData>{}(v);
	}

	inline uint qHash(const QVector<InputData>& v)
	{
		return std::accumulate(v.begin(), v.end(), 0, [] (int value, auto&& data)
		{
			return (value ^ qHash(data));
		});
	}

	inline uint qHash(const InputTrigger& v)
	{
		return qHash(v.modifiers) ^ qHash(v.trigger);
	}
}