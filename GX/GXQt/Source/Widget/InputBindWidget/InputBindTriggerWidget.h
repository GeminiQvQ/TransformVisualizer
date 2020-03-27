#pragma once

// Project Includes
#include <GXQt/System/Input/InputDataTypes.h>
#include <GXQt/API.h>

// Third-Party Includes
#include <QLineEdit>
#include <QVector>

namespace GX::QT
{
	class GXQT_API InputBindTriggerWidget : public QLineEdit
	{
		Q_OBJECT
		
		Q_PROPERTY(bool editing_enabled READ editing_enabled WRITE set_editing_enabled)

	public:
		// Construction & Destruction
								InputBindTriggerWidget	(QWidget* parent = nullptr);
								~InputBindTriggerWidget	();

		// Properties
		void					set_data				(const InputDataList& data);
		const InputDataList&	data					() const;

		void					set_editing_enabled		(bool enabled);
		bool					editing_enabled			() const;

	signals:
		// Signals
		void					signal_data_changed		(const InputDataList& data);

	protected:
		// Qt Events
		virtual void			focusInEvent			(QFocusEvent* event) override;
		virtual void			focusOutEvent			(QFocusEvent* event) override;
		virtual bool			eventFilter				(QObject* object, QEvent* event) override;

	private:
		// Helpers
		void					add_data				(InputDataList& container, const InputData& data);
		void					remove_data				(InputDataList& container, const InputData& data);

		void					begin_editing			();
		void					end_editing				(bool commit);

		void					update_text				();

		struct Internal;
		const UPtr<Internal> m;
	};
}