#pragma once

// Forward Declarations
namespace GX::QT
{
	class CommandController;
}

namespace TransformVisualizer
{
	template <class ClassType, class SetFn, class GetFn, class ValueType>
	void exec_property_command(GX::QT::CommandController command_controller, ClassType& object, const SetFn& set_fn, const GetFn& get_fn, const ValueType& value);
}

// Inline Includes
#include <TransformVisualizer/Utility/CommandUtility.inl>