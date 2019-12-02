#pragma once

// Project Includes
#include <GXQt/Container/ListPtrContainer.h>
#include <GXQt/System/Input/InputConfig.h>
#include <GXQt/API.h>

// Third-Party Includes
#include <QObject>
#include <QString>

namespace GX::QT
{
	class GXQT_API InputConfigContainer : public QObject
										, public ListPtrContainerSearchable<InputConfig, QString>
										, public ListPtrContainerEventHandler<InputConfig>
										, public ListPtrContainerSearchCallback<InputConfig, QString>
	{
		Q_OBJECT

	public:
		// Construction & Destruction
							InputConfigContainer	();
							~InputConfigContainer	();

		// Configurations
		void				set_current				(SPtr<InputConfig> config);
		SPtr<InputConfig>	current					();
		SCPtr<InputConfig>	current					() const;

	signals:
		// Signals
		void				signal_config_added		(SPtr<InputConfig> config);
		void				signal_config_removed	(SPtr<InputConfig> config);
		void				signal_config_changed	(SPtr<InputConfig> config);

	private:
		// ListPtrContainerEventHandler Interface
		virtual void		on_entry_added			(SPtr<InputConfig> config) override;
		virtual void		on_entry_removed		(SPtr<InputConfig> config) override;

		// ListPtrContainerSearchCallback Interface
		virtual bool		equals					(const InputConfig& config, const QString& name) const override;

	private:
		struct Internal;
		const UPtr<Internal> m;
	};
}