// Local Includes
#include <TransformVisualizer/UI/SettingsWidget/SettingsWidget.h>

// Project Includes
#include <GXQt/MVC/ModelData/ModelDataUtility.h>
#include <GXQt/Utility/WidgetState/WidgetState.h>
#include <TransformVisualizer/UI/SettingsWidget/SettingsModel.h>

// Third-Party Includes
#include <QLayout>
#include <QTreeView>

namespace TransformVisualizer
{
	//-----------------------------------------------------------------------------------------------------
	// Internal
	//-----------------------------------------------------------------------------------------------------

	struct SettingsWidget::Internal
	{
		SettingsModel	model;
		QTreeView*		view;
		
		Internal(KernelInterface& kernel_interface)
			: model {kernel_interface}
			, view {}
		{
		}
	};

	//-----------------------------------------------------------------------------------------------------
	// Construction & Destruction
	//-----------------------------------------------------------------------------------------------------

	SettingsWidget::SettingsWidget(KernelInterface& kernel_interface, QWidget* parent)
		: QWidget {parent}
		, m {std::make_unique<Internal>(kernel_interface)}
	{
		// Widget.
		setWindowTitle("Settings");
		auto layout = new QHBoxLayout{};
		layout->setContentsMargins(0, 0, 0, 0);
		setLayout(layout);
		
		// View.
		m->view = new QTreeView{this};
		m->view->setModel(&m->model);
		layout->addWidget(m->view);
		GX::QT::install_model_data_view_delegate(*m->view, true);
		GX::QT::register_widget_state(*m->view, "SettingsWidgetView");
	}

	SettingsWidget::~SettingsWidget()
	{
	}
}