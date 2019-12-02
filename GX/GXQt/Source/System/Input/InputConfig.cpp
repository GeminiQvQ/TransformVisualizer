// Local Includes
#include <GXQt/System/Input/InputConfig.h>

// Project Includes
#include <GXQt/System/Input/InputDataTypes.h>

namespace GX::QT
{
	//-----------------------------------------------------------------------------------------------------
	// Internal
	//-----------------------------------------------------------------------------------------------------

	struct InputConfig::Internal
	{
		QString						 name;
		QString						 base_config;
		bool						 cycle_enabled;
		QHash<QString, InputTrigger> bindings;

		Internal()
			: name {}
			, base_config {}
			, cycle_enabled {false}
			, bindings {}
		{
		}
	};

	//-----------------------------------------------------------------------------------------------------
	// Construction & Destruction
	//-----------------------------------------------------------------------------------------------------

	InputConfig::InputConfig()
		: m {std::make_unique<Internal>()}
	{
	}

	InputConfig::~InputConfig()
	{
	}

	//-----------------------------------------------------------------------------------------------------
	// Binding
	//-----------------------------------------------------------------------------------------------------

	void InputConfig::bind(const QString& id, const InputTrigger& trigger)
	{
		m->bindings.insert(id, trigger);
		emit signal_input_bound(id, trigger);
	}

	void InputConfig::unbind(const QString& id)
	{
		m->bindings.remove(id);
		emit signal_input_bound(id, InputTrigger{});
	}

	InputTrigger InputConfig::binding(const QString& id)
	{
		const auto it = m->bindings.find(id);
		return (it != m->bindings.end() ? *it : InputTrigger{});
	}

	//-----------------------------------------------------------------------------------------------------
	// Properties
	//-----------------------------------------------------------------------------------------------------

	void InputConfig::set_name(const QString& name)
	{
		m->name = name;
		emit signal_name_changed(name);
	}

	const QString& InputConfig::name() const
	{
		return m->name;
	}

	//-----------------------------------------------------------------------------------------------------

	void InputConfig::set_base_config(const QString& base_config)
	{
		m->base_config = base_config;
		emit signal_base_config_changed(base_config);
	}

	const QString& InputConfig::base_config() const
	{
		return m->base_config;
	}

	//-----------------------------------------------------------------------------------------------------

	void InputConfig::set_cycle_enabled(bool cycle_enabled)
	{
		m->cycle_enabled = cycle_enabled;
		emit signal_cycle_enabled_changed(cycle_enabled);
	}

	bool InputConfig::cycle_enabled() const
	{
		return m->cycle_enabled;
	}
}