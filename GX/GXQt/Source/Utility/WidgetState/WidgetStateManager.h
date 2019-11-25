#pragma once

// Project Includes
#include <GXQt/API.h>

// Third-Party Includes
#include <QObject>

namespace GX::QT
{
	class GXQT_API WidgetStateManager : public QObject
	{
	public:
		// Construction & Destruction
						WidgetStateManager	();
						~WidgetStateManager	();

		// Save & Load
		void			save				(QJsonObject& json);
		void			load				(const QJsonObject& json);

	protected:
		// Qt Events
		virtual bool	eventFilter			(QObject* object, QEvent* event) override;

	private:
		struct Internal;
		const UPtr<Internal> m;
	};
}