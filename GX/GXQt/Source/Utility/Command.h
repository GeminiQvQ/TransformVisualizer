#pragma once

// Project Includes
#include <GXQt/API.h>

// Third-Party Includes
#include <QUndoCommand>
#include <QUndoStack>

namespace GX::QT
{
	//-----------------------------------------------------------------------------------------------------
	// Types
	//-----------------------------------------------------------------------------------------------------

	using Command		= QUndoCommand;
	using CommandStack	= QUndoStack;

	//-----------------------------------------------------------------------------------------------------
	// CommandSingle
	//-----------------------------------------------------------------------------------------------------

	class GXQT_API CommandSingle : public Command
	{
	public:
		// Construction & Destruction
						CommandSingle	(Command* parent = nullptr);
						CommandSingle	(const QString& text, Command* parent = nullptr);

		// CommandSingle Interface
		virtual void	exec			() = 0;

		// Command Interface
		virtual void	undo			() override;
		virtual void	redo			() override;
	};
}