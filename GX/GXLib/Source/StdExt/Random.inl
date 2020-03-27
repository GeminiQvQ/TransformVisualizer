namespace GX
{
	template <class Engine>
	typename Engine::result_type random(Engine& engine)
	{
		return engine();
	}

	template <class Engine>
	typename Engine::result_type random(Engine& engine, typename Engine::result_type min, typename Engine::result_type max)
	{
		return (min + (engine() % (max - min)));
	}
}