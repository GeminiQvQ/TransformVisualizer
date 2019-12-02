// Local Includes
#include <GXQt/System/Input/InputConfigContainer.h>

namespace GX::QT
{
	//-----------------------------------------------------------------------------------------------------
	// Internal
	//-----------------------------------------------------------------------------------------------------

	struct InputConfigContainer::Internal
	{
		SPtr<InputConfig> current;
	};

	//-----------------------------------------------------------------------------------------------------
	// Construction & Destruction
	//-----------------------------------------------------------------------------------------------------

	InputConfigContainer::InputConfigContainer()
		: ListPtrContainerSearchable {*static_cast<ListPtrContainerSearchCallback*>(this)}
		, m {std::make_unique<Internal>()}
	{
		add_handler(*this);
	}

	InputConfigContainer::~InputConfigContainer()
	{
	}

	//-----------------------------------------------------------------------------------------------------
	// Configurations
	//-----------------------------------------------------------------------------------------------------

	void InputConfigContainer::set_current(SPtr<InputConfig> config)
	{
		m->current = config;
		emit signal_config_changed(config);
	}

	SPtr<InputConfig> InputConfigContainer::current()
	{
		return m->current;
	}

	SCPtr<InputConfig> InputConfigContainer::current() const
	{
		return m->current;
	}

	//-----------------------------------------------------------------------------------------------------
	// ListPtrContainerEventHandler Interface
	//-----------------------------------------------------------------------------------------------------

	void InputConfigContainer::on_entry_added(SPtr<InputConfig> config)
	{
		emit signal_config_added(config);
	}

	void InputConfigContainer::on_entry_removed(SPtr<InputConfig> config)
	{
		emit signal_config_removed(config);
	}

	//-----------------------------------------------------------------------------------------------------
	// ListPtrContainerSearchCallback Interface
	//-----------------------------------------------------------------------------------------------------

	bool InputConfigContainer::equals(const InputConfig& config, const QString& name) const
	{
		return (config.name() == name);
	}
}