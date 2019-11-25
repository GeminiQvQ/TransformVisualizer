// Project Includes
#include <GXQt/Utility/Command.h>
#include <GXQt/Utility/CommandController.h>

namespace TransformVisualizer
{
	template <class ClassType, class SetFn, class GetFn, class ValueType>
	class SetPropertyCommand : public GX::QT::CommandSingle
	{
	public:
		SetPropertyCommand(ClassType& object, const SetFn& set_fn, const GetFn& get_fn, const ValueType& value)
			: m_object {object}
			, m_set_fn {set_fn}
			, m_get_fn {get_fn}
			, m_value {value}
		{
		}

		virtual void exec() override
		{
			const auto value_old = (m_object.*m_get_fn)();
			(m_object.*m_set_fn)(m_value);
			m_value = value_old;
		}

	private:
		ClassType&	m_object;
		SetFn		m_set_fn;
		GetFn		m_get_fn;
		ValueType	m_value;
	};

	template <class ClassType, class SetFn, class GetFn, class ValueType>
	void exec_property_command(GX::QT::CommandController command_controller, ClassType& object, const SetFn& set_fn, const GetFn& get_fn, const ValueType& value)
	{
		command_controller.exec<SetPropertyCommand<ClassType, SetFn, GetFn, ValueType>>(object, set_fn, get_fn, value);
	}
}