#pragma once

// Project Includes
#include <GXQt/System/Input/InputDataTypes.h>
#include <GXQt/API.h>

// Third-Party Includes
#include <QObject>

namespace GX::QT
{
	class GXQT_API InputSystem : public QObject
	{
	public:
		// Construction & Destruction
							InputSystem		();
							~InputSystem	();

		// Binding
		void				bind			(const InputTrigger& trigger, int poll_id);
		void				bind			(const InputTrigger& trigger, const InputCallback& callback);
		void				bind			(const InputTrigger& trigger, QObject* receiver, int poll_id);
		void				bind			(const InputTrigger& trigger, QObject* receiver, const InputCallback& callback);

	protected:
		// Qt Events
		virtual	void		timerEvent		(QTimerEvent* event) override;
		virtual bool		eventFilter		(QObject* object, QEvent* event) override;

	private:
		// Helpers
		bool				enable_binding	(const QVector<InputData>& modifiers, const InputData& trigger);
		void				disable_binding	(const InputData& data);
		QVector<InputData>	get_modifiers	() const;

		struct Internal;
		const UPtr<Internal> m;
	};
}