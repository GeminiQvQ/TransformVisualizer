// Local Includes
#include <TransformVisualizer/UI/TransformListWidget/TransformListWidget.h>

// Project Includes
#include <GXLib/Math/Matrix.h>
#include <GXQt/MVC/ModelData/ModelDataUtility.h>
#include <GXQt/MVC/ViewUtility.h>
#include <GXQt/System/Input/InputSystem.h>
#include <GXQt/Utility/WidgetState/WidgetState.h>
#include <TransformVisualizer/Core/Transform/Transform.h>
#include <TransformVisualizer/Core/Transform/TransformContainer.h>
#include <TransformVisualizer/Core/State.h>
#include <TransformVisualizer/UI/TransformListWidget/TransformListModel.h>
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

	struct TransformListWidget::Internal
	{
		TransformContainer&	transform_container;
		State&				state;
		TransformListModel	model;
		QTreeView*			view;
		
		Internal(KernelInterface& kernel_interface)
			: transform_container {kernel_interface.transform_container()}
			, state {kernel_interface.state()}
			, model {kernel_interface}
			, view {}
		{
		}
	};

	//-----------------------------------------------------------------------------------------------------
	// Construction & Destruction
	//-----------------------------------------------------------------------------------------------------

	TransformListWidget::TransformListWidget(KernelInterface& kernel_interface, QWidget* parent)
		: QWidget {parent}
		, m {std::make_unique<Internal>(kernel_interface)}
	{
		// Widget.
		setWindowTitle("Transform List");
		auto layout = new QHBoxLayout{};
		layout->setContentsMargins(0, 0, 0, 0);
		setLayout(layout);
		
		// View.
		m->view = new QTreeView{this};
		m->view->setModel(&m->model);
		m->view->setDragDropMode(QAbstractItemView::DragDrop);
		m->view->setDragEnabled(true);
		m->view->setSelectionMode(QAbstractItemView::ExtendedSelection);
		m->view->setContextMenuPolicy(Qt::CustomContextMenu);
		connect(m->view, &QTreeView::customContextMenuRequested, this, &TransformListWidget::slot_context_menu);
		connect(m->view->selectionModel(), &QItemSelectionModel::selectionChanged, this, &TransformListWidget::slot_view_selection_changed);
		GX::QT::install_model_data_view_delegate(*m->view, true);
		GX::QT::register_widget_state(*m->view, "TransformListView");
		layout->addWidget(m->view);

		// Input bindings.
		kernel_interface.input_system().bind(GX::QT::InputTrigger{GX::QT::InputKeyboardKey{Qt::Key_Delete}}, this,
											 GX::QT::TriggerInputCallback{std::bind(&TransformListWidget::slot_remove, this)});

		// Signals.
		connect(&kernel_interface.state(), &State::signal_transforms_selected, this, &TransformListWidget::slot_transforms_selected);
	}

	TransformListWidget::~TransformListWidget()
	{
	}

	//-----------------------------------------------------------------------------------------------------
	// UI Slots
	//-----------------------------------------------------------------------------------------------------

	void TransformListWidget::slot_view_selection_changed()
	{
		QVector<SPtr<Transform>> transforms;

		for (const auto& index : GX::QT::selected_rows(*m->view))
		{
			const auto transform = m->model.map_to_transform(index);
			if (transform != nullptr)
			{
				transforms.push_back(transform);
			}
		}

		m->state.select_transforms(transforms);
	}

	void TransformListWidget::slot_context_menu(const QPoint& /*pos*/)
	{
		QMenu menu(this);

		menu.addAction(QIcon{":/Images/Add"}, "Add", this, &TransformListWidget::slot_add);

		const auto selected_indices = GX::QT::selected_rows(*m->view);
		if (!selected_indices.isEmpty())
		{
			menu.addAction(QIcon{":/Images/Remove"}, "Remove", this, &TransformListWidget::slot_remove);
		}

		menu.addAction(QIcon{":/Images/Clear"}, "Clear", this, &TransformListWidget::slot_clear);

		menu.exec(QCursor::pos());
	}

	void TransformListWidget::slot_add()
	{
		auto transform = std::make_shared<Transform>();
		transform->set_matrix(GX::Matrix::create_identity());
		m->transform_container.add(transform);
	}
	
	void TransformListWidget::slot_remove()
	{
		auto transforms = m->state.selected_transforms();
		transforms.removeOne(m->transform_container.world_transform());

		if (!transforms.isEmpty())
		{
			m->state.select_transforms(QVector<SPtr<Transform>>{});
			m->transform_container.remove(transforms);
		}
	}

	void TransformListWidget::slot_clear()
	{
		m->transform_container.clear();
	}

	//-----------------------------------------------------------------------------------------------------
	// State Slots
	//-----------------------------------------------------------------------------------------------------

	void TransformListWidget::slot_transforms_selected(const QVector<SPtr<Transform>>& transforms)
	{
		QItemSelection selection;

		for (const auto& transform : transforms)
		{
			selection.push_back(QItemSelectionRange{m->model.map_to_index(*transform)});
		}

		m->view->selectionModel()->select(selection, QItemSelectionModel::ClearAndSelect);
	}
}