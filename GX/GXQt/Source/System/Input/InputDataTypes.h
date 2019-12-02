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
	// Input Data
	//-----------------------------------------------------------------------------------------------------

	struct GXQT_API InputNone
	{
		QString to_string	() const;
		bool	operator == (const InputNone& rhs) const;
	};

	struct GXQT_API InputKeyboardKey
	{
		int key;

		QString to_string	() const;
		bool	operator == (const InputKeyboardKey& rhs) const;
	};

	using InputKey = InputKeyboardKey;

	struct GXQT_API InputMouseButton
	{
		int button;

		QString to_string	() const;
		bool	operator == (const InputMouseButton& rhs) const;
	};

	using InputButton = InputMouseButton;

	struct GXQT_API InputMouseMove
	{
		QString to_string	() const;
		bool	operator == (const InputMouseMove& rhs) const;
	};

	struct GXQT_API InputMouseWheel
	{
		QString to_string	() const;
		bool	operator == (const InputMouseWheel& rhs) const;
	};

	using InputData = std::variant
	<
		InputNone,
		InputKeyboardKey,
		InputMouseButton,
		InputMouseMove,
		InputMouseWheel
	>;

	using InputDataList = QVector<InputData>;

	//-----------------------------------------------------------------------------------------------------
	// Input Trigger
	//-----------------------------------------------------------------------------------------------------

	enum class InputTriggerType
	{
		OnEnabled,
		OnDisabled
	};

	struct GXQT_API InputTrigger
	{
		InputDataList		modifiers;
		InputData			trigger;
		InputTriggerType	type;

				InputTrigger	();
				InputTrigger	(InputData trigger, InputTriggerType type = InputTriggerType::OnEnabled);
				InputTrigger	(InputData modifier, InputData trigger, InputTriggerType type = InputTriggerType::OnEnabled);
				InputTrigger	(const InputDataList& modifiers, InputData trigger, InputTriggerType type = InputTriggerType::OnEnabled);

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

	using TriggerInputNone = std::monostate;
	using TriggerInputCallback = std::function<void()>;
	using MouseMoveInputCallback = std::function<void(const MouseMoveInputParameters&)>;
	using MouseWheelInputCallback = std::function<void(const MouseWheelInputParameters&)>;

	using InputCallback = std::variant
	<
		TriggerInputNone,
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

	template <> struct hash<GX::QT::InputNone>
	{
		size_t operator () (const GX::QT::InputNone& /*v*/) const { return std::hash<size_t>{}(0); }
	};

	template <> struct hash<GX::QT::InputKeyboardKey>
	{
		size_t operator () (const GX::QT::InputKeyboardKey& v) const { return 1 ^ std::hash<size_t>{}(v.key); }
	};

	template <> struct hash<GX::QT::InputMouseButton>
	{
		size_t operator () (const GX::QT::InputMouseButton& v) const { return 2 ^ std::hash<size_t>{}(v.button); }
	};

	template <> struct hash<GX::QT::InputMouseMove>
	{
		size_t operator () (const GX::QT::InputMouseMove& /*v*/) const { return 3 ^ std::hash<size_t>{}(0); }
	};

	template <> struct hash<GX::QT::InputMouseWheel>
	{
		size_t operator () (const GX::QT::InputMouseWheel& /*v*/) const { return 4 ^ std::hash<size_t>{}(0); }
	};

	template <> struct hash<GX::QT::InputDataList>
	{
		size_t operator () (const GX::QT::InputDataList& v) const
		{
			return std::accumulate(v.begin(), v.end(), size_t{0}, [] (size_t value, auto&& data)
			{
				return (value ^ std::hash<GX::QT::InputData>{}(data));
			});
		}
	};

	template <> struct hash<GX::QT::InputTrigger>
	{
		size_t operator () (const GX::QT::InputTrigger& v) const
		{
			return std::hash<GX::QT::InputDataList>{}(v.modifiers) ^ std::hash<GX::QT::InputData>{}(v.trigger);
		}
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

	inline uint qHash(const InputDataList& v)
	{
		return (uint)std::hash<InputDataList>{}(v);
	}

	inline uint qHash(const InputTrigger& v)
	{
		return (uint)std::hash<InputTrigger>{}(v);
	}
}