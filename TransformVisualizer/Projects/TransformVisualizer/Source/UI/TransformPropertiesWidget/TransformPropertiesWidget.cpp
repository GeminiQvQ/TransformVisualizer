// Local Includes
#include <TransformVisualizer/UI/TransformPropertiesWidget/TransformPropertiesWidget.h>

// Project Includes
#include <GXQt/MVC/ModelData/ModelDataUtility.h>
#include <GXQt/Utility/WidgetState/WidgetState.h>
#include <TransformVisualizer/UI/TransformPropertiesWidget/TransformPropertiesModel.h>
#include <TransformVisualizer/Core/State.h>
#include <TransformVisualizer/KernelInterface.h>

// Third-Party Includes
#include <QLayout>
#include <QMenu>
#include <QTreeView>

namespace TransformVisualizer
{
	//-----------------------------------------------------------------------------------------------------
	// Internal
	//-----------------------------------------------------------------------------------------------------

	struct TransformPropertiesWidget::Internal
	{
		TransformPropertiesModel	model;
		QTreeView*					view;

		Internal(KernelInterface& kernel_interface)
			: model {kernel_interface}
			, view {}
		{
		}
	};

	//-----------------------------------------------------------------------------------------------------
	// Construction & Destruction
	//-----------------------------------------------------------------------------------------------------

	TransformPropertiesWidget::TransformPropertiesWidget(KernelInterface& kernel_interface, QWidget* parent)
		: QWidget {parent}
		, m {std::make_unique<Internal>(kernel_interface)}
	{
		// Widget.
		setWindowTitle("Transform Properties");
		auto layout = new QHBoxLayout{};
		layout->setContentsMargins(0, 0, 0, 0);
		setLayout(layout);
		
		// View.
		m->view = new QTreeView{this};
		m->view->setModel(&m->model);
		m->view->setAlternatingRowColors(true);
		GX::QT::install_model_data_view_delegate(*m->view, true);
		GX::QT::register_widget_state(*m->view, "TransformPropertiesView");
		layout->addWidget(m->view);

		// Signals.
		connect(&kernel_interface.state(), &State::signal_transforms_selected, this, &TransformPropertiesWidget::slot_transforms_selected);
	}

	TransformPropertiesWidget::~TransformPropertiesWidget()
	{
	}

	//-----------------------------------------------------------------------------------------------------
	// State Slots
	//-----------------------------------------------------------------------------------------------------

	void TransformPropertiesWidget::slot_transforms_selected(const QVector<SPtr<Transform>>& transforms)
	{
		m->model.set_transform(transforms.size() == 1 ? transforms[0] : nullptr);
	}
}