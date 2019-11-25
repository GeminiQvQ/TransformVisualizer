namespace GX::QT
{
	//-----------------------------------------------------------------------------------------------------
	// Command Execution
	//-----------------------------------------------------------------------------------------------------

	template <class T, class... Args>
	void CommandController::exec(Args&&... args)
	{
		Command* command = new T{std::forward<Args>(args)...};
		exec(command);
	}
}