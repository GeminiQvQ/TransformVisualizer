#pragma once

// Project Includes
#include <GXQt/Utility/Command.h>
#include <GXQt/API.h>

namespace GX::QT
{
	class GXQT_API CommandController
	{
	public:
		// Construction & Destruction
				CommandController	();
				CommandController	(CommandStack& command_stack);

		// Command Execution
		template <class T, class... Args>
		void	exec				(Args&&... args);
		void	exec				(Command*& command);

		// Properties
		void	set_enabled			(bool enabled);
		bool	is_enabled			() const;

		// Utility
		void	begin				();
		void	end					();

	private:
		CommandStack* m_command_stack;
	};
}

// Inline Includes
#include <GXQt/Utility/CommandController.inl>