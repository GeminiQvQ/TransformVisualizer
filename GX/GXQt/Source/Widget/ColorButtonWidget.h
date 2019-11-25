#pragma once

// Project Includes
#include <GXQt/API.h>

// Third-Party Includes
#include <QAbstractButton>
#include <QColor>

namespace GX::QT
{
	class GXQT_API ColorButtonWidget : public QAbstractButton
	{
		Q_OBJECT

	public:
		// Construction & Destruction
						ColorButtonWidget		(QWidget* parent = nullptr);
						ColorButtonWidget		(const QColor& color, QWidget* parent = nullptr);
						~ColorButtonWidget		();

		// Properties
		void			set_color				(const QColor& color);
		QColor			color					() const;

		void			set_read_only			(bool read_only);
		bool			read_only				() const;

		// QWidget Interface
		virtual QSize	sizeHint				() const override;

	signals:
		// Signals
		void			signal_color_changed	(const QColor& color);
		void			signal_color_committed	(const QColor& color);

	protected:
		// Qt Events
		virtual void	paintEvent				(QPaintEvent* event) override;

	private:
		// UI Slots
		void			slot_clicked			();

	private:
		// Properties
		void			set_color				(const QColor& color, bool committed);

		struct Internal;
		const UPtr<Internal> m;
	};
}