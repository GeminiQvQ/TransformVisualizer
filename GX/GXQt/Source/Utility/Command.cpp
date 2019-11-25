// Local Includes
#include <GXQt/Utility/Command.h>

namespace GX::QT
{
	//-----------------------------------------------------------------------------------------------------
	// Construction & Destruction
	//-----------------------------------------------------------------------------------------------------

	CommandSingle::CommandSingle(Command* parent)
		: Command {parent}
	{
	}

	CommandSingle::CommandSingle(const QString& text, Command* parent)
		: Command {text, parent}
	{
	}

	//-----------------------------------------------------------------------------------------------------
	// Command Interface
	//-----------------------------------------------------------------------------------------------------

	void CommandSingle::undo()
	{
		exec();
	}

	void CommandSingle::redo()
	{
		exec();
	}
}