// Local Includes
#include <GXQt/Widget/InputBindWidget/InputBindWidget.h>
#include <ui_InputBindWidget.h>

namespace GX::QT
{
	//-----------------------------------------------------------------------------------------------------
	// Internal
	//-----------------------------------------------------------------------------------------------------

	struct InputBindWidget::Internal
	{
		Ui::InputBindWidget ui;
	};

	//-----------------------------------------------------------------------------------------------------
	// Construction & Destruction
	//-----------------------------------------------------------------------------------------------------

	InputBindWidget::InputBindWidget(QWidget* parent)
		: QWidget {parent}
		, m {std::make_unique<Internal>()}
	{
		// Widget.
		m->ui.setupUi(this);
	}

	InputBindWidget::~InputBindWidget()
	{
	}
}