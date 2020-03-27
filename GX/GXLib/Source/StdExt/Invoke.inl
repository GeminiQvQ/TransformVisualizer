namespace GX
{
	//-----------------------------------------------------------------------------------------------------
	// Multi-Invoke
	//-----------------------------------------------------------------------------------------------------

	template <class Container, class Callback, class... Args>
	void invoke(const Container& container, const Callback& callback, Args... args)
	{
		invoke(std::begin(container), std::end(container), callback, std::forward<Args>(args)...);
	}

	//-----------------------------------------------------------------------------------------------------

	template <class It, class Callback, class... Args>
	void invoke(It first, It end, const Callback& callback, Args... args)
	{
		for (auto it = first; it != end; ++it)
		{
			std::invoke(callback, *it, std::forward<Args>(args)...);
		}
	}

	//-----------------------------------------------------------------------------------------------------

	template <class Container, class Callback, class... Args>
	bool invoke_conditional(const Container& container, const Callback& callback, Args... args)
	{
		return invoke_conditional(std::begin(container), std::end(container), callback, std::forward<Args>(args)...);
	}

	//-----------------------------------------------------------------------------------------------------

	template <class It, class Callback, class... Args>
	bool invoke_conditional(It first, It end, const Callback& callback, Args... args)
	{
		for (auto it = first; it != end; ++it)
		{
			if (std::invoke(callback, *it, std::forward<Args>(args)...))
			{
				return true;
			}
		}

		return false;
	}
}