#pragma once

// Forward Declarations
namespace GX::QT
{
	class CommandController;
	class InputSystem;
}

namespace TransformVisualizer
{
	class State;
	class TransformContainer;
	struct Settings;
}

namespace TransformVisualizer
{
	class KernelInterface
	{
	public:
		virtual GX::QT::InputSystem&		input_system		() = 0;
		virtual const GX::QT::InputSystem&	input_system		() const = 0;

		virtual Settings&					settings			() = 0;
		virtual const Settings&				settings			() const = 0;

		virtual State&						state				() = 0;
		virtual const State&				state				() const = 0;

		virtual TransformContainer&			transform_container	() = 0;
		virtual const TransformContainer&	transform_container	() const = 0;

		virtual GX::QT::CommandController	command_controller	() = 0;
	};
}