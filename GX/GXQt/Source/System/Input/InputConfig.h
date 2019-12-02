#pragma once

// Project Includes
#include <GXQt/API.h>

// Third-Party Includes
#include <QObject>

// Forward Declarations
namespace GX::QT
{
	struct InputTrigger;
}

namespace GX::QT
{
	class GXQT_API InputConfig : public QObject
	{
		Q_OBJECT

	public:
		// Construction & Destruction
						InputConfig			();
						~InputConfig		();

		// Binding
		void			bind				(const QString& id, const InputTrigger& trigger);
		void			unbind				(const QString& id);
		InputTrigger	binding				(const QString& id);

		// Properties
		void			set_name			(const QString& name);
		const QString&	name				() const;

		void			set_base_config		(const QString& base_config);
		const QString&	base_config			() const;

		void			set_cycle_enabled	(bool cycle_enabled);
		bool			cycle_enabled		() const;

	signals:
		// Signals
		void signal_input_bound				(const QString& id, const InputTrigger& trigger);
		void signal_name_changed			(const QString& name);
		void signal_base_config_changed		(const QString& base_config);
		void signal_cycle_enabled_changed	(bool cycle_enabled);

	private:
		struct Internal;
		const UPtr<Internal> m;
	};
}