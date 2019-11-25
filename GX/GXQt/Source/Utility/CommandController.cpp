// Local Includes
#include <GXQt/Utility/CommandController.h>

// Third-Party Includes
#include <QVariant>

namespace GX::QT
{
	//-----------------------------------------------------------------------------------------------------
	// Construction & Destruction
	//-----------------------------------------------------------------------------------------------------

	CommandController::CommandController()
		: m_command_stack {nullptr}
	{
	}

	CommandController::CommandController(CommandStack& command_stack)
		: m_command_stack {&command_stack}
	{
	}

	//-----------------------------------------------------------------------------------------------------
	// Command Execution
	//-----------------------------------------------------------------------------------------------------

	void CommandController::exec(Command*& command)
	{
		if (m_command_stack != nullptr && is_enabled())
		{
			m_command_stack->push(command);
		}
		else
		{
			command->redo();
			delete command;
			command = nullptr;
		}
	}

	//-----------------------------------------------------------------------------------------------------
	// Properties
	//-----------------------------------------------------------------------------------------------------

	void CommandController::set_enabled(bool enabled)
	{
		if (m_command_stack != nullptr)
		{
			m_command_stack->setProperty("Enabled", enabled);
		}
	}

	bool CommandController::is_enabled() const
	{
		if (m_command_stack != nullptr)
		{
			const auto enabled_value = m_command_stack->property("Enabled");
			return (enabled_value.isNull() || enabled_value.toBool());
		}
		else
		{
			return true;
		}
	}

	//-----------------------------------------------------------------------------------------------------
	// Utility}
	//-----------------------------------------------------------------------------------------------------

	void CommandController::begin()
	{
		if (m_command_stack != nullptr && is_enabled())
		{
			m_command_stack->beginMacro(QString());
		}
	}
	
	void CommandController::end()
	{
		if (m_command_stack != nullptr && is_enabled())
		{
			m_command_stack->endMacro();
		}
	}
}