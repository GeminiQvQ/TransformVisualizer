// Local Includes
#include <GXQt/Widget/ColorButtonWidget.h>

// Project Includes
#include <GXQt/Utility/WidgetState/WidgetState.h>

// Third-Party Includes
#include <QColorDialog>
#include <QPainter>
#include <QStyleOption>

namespace GX::QT
{
	//-----------------------------------------------------------------------------------------------------
	// Internal
	//-----------------------------------------------------------------------------------------------------

	struct ColorButtonWidget::Internal
	{
		QColor	color;
		bool	read_only;

		Internal(const QColor& color)
			: color {color}
			, read_only {false}
		{
		}
	};
	
	//-----------------------------------------------------------------------------------------------------
	// Construction & Destruction
	//-----------------------------------------------------------------------------------------------------

	ColorButtonWidget::ColorButtonWidget(QWidget* parent)
		: ColorButtonWidget {QColor{}, parent}
	{
	}

	ColorButtonWidget::ColorButtonWidget(const QColor& color, QWidget* parent)
		: QAbstractButton {parent}
		, m {std::make_unique<Internal>(color)}
	{
		setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
		connect(this, &ColorButtonWidget::clicked, this, &ColorButtonWidget::slot_clicked);
	}

	ColorButtonWidget::~ColorButtonWidget()
	{
	}

	//-----------------------------------------------------------------------------------------------------
	// Properties
	//-----------------------------------------------------------------------------------------------------

	void ColorButtonWidget::set_color(const QColor& color)
	{
		set_color(color, true);
	}

	void ColorButtonWidget::set_color(const QColor& color, bool committed)
	{
		m->color = color;
		update();
		emit (committed ? signal_color_committed(color) : signal_color_changed(color));
	}

	QColor ColorButtonWidget::color() const
	{
		return m->color;
	}

	//-----------------------------------------------------------------------------------------------------

	void ColorButtonWidget::set_read_only(bool read_only)
	{
		m->read_only = read_only;
	}

	bool ColorButtonWidget::read_only() const
	{
		return m->read_only;
	}


	//-----------------------------------------------------------------------------------------------------
	// QWdiget Interface
	//-----------------------------------------------------------------------------------------------------

	QSize ColorButtonWidget::sizeHint() const
	{
		QStyleOptionButton option {};
		return QSize{0, style()->sizeFromContents(QStyle::CT_PushButton, &option, QSize{}).height()};
	}

	//-----------------------------------------------------------------------------------------------------
	// Qt Events
	//-----------------------------------------------------------------------------------------------------
	
	void ColorButtonWidget::paintEvent(QPaintEvent* /*event*/)
	{
		QPainter painter(this);
		painter.setRenderHint(QPainter::Antialiasing);

		QPen pen;
		pen.setWidth(1);
		painter.setPen(pen);

		QBrush brush;
		brush.setColor(m->color);
		brush.setStyle(Qt::SolidPattern);
		painter.setBrush(brush);

		painter.drawRoundedRect(rect().adjusted(pen.width(), pen.width(), -pen.width(), -pen.width()), 5.0f, 5.0f);
	}

	//-----------------------------------------------------------------------------------------------------
	// UI Slots
	//-----------------------------------------------------------------------------------------------------

	void ColorButtonWidget::slot_clicked()
	{
		QColorDialog dialog {m->color, this};
		dialog.setOption(QColorDialog::ShowAlphaChannel);
		register_widget_state(dialog, "ColorButtonWidgetDialog");

		if (!m->read_only)
		{
			connect(&dialog, &QColorDialog::currentColorChanged, this, [this] (const QColor& color) { set_color(color, false); });
		}

		auto color = m->color;
		auto result = dialog.exec();

		if (!m->read_only)
		{
			if (result == QDialog::Accepted)
			{
				set_color(dialog.currentColor());
			}
			else
			{
				set_color(color, false);
			}
		}
	}
}