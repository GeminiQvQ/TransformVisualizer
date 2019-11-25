// Local Includes
#include <TransformVisualizer/UI/MainWindow/MainWindow.h>
#include <ui_MainWindow.h>

// Project Includes
#include <GXLib/Math/Matrix.h>
#include <GXQt/System/Input/InputSystem.h>
#include <GXQt/Utility/WidgetState/WidgetState.h>
#include <TransformVisualizer/Core/Transform/Transform.h>
#include <TransformVisualizer/Core/Transform/TransformContainer.h>
#include <TransformVisualizer/Core/State.h>
#include <TransformVisualizer/Data/Document.h>
#include <TransformVisualizer/UI/RenderWidget/RenderWidget.h>
#include <TransformVisualizer/UI/SettingsWidget/SettingsWidget.h>
#include <TransformVisualizer/UI/TransformListWidget/TransformListWidget.h>
#include <TransformVisualizer/UI/TransformPropertiesWidget/TransformPropertiesWidget.h>
#include <TransformVisualizer/KernelInterface.h>

// Third-Party Includes
#include <QDate>
#include <QDockWidget>
#include <QFile>
#include <QFileDialog>
#include <QLabel>
#include <QMessageBox>
#include <QSettings>
#include <QTextStream>
#include <QToolBar>

namespace TransformVisualizer
{
	//-----------------------------------------------------------------------------------------------------
	// Internal
	//-----------------------------------------------------------------------------------------------------

	struct MainWindow::Internal
	{
		Document&					document;

		RenderWidget*				render_widget;
		SettingsWidget*				settings_widget;
		TransformListWidget*		transform_list_widget;
		TransformPropertiesWidget*	transform_properties_widget;

		Ui::MainWindow				ui;
		QLabel*						status_label;

		Internal(Document& document)
			: document {document}
			, render_widget {}
			, settings_widget {}
			, transform_list_widget {}
			, transform_properties_widget {}
			, status_label {}
		{
		}
	};

	//-----------------------------------------------------------------------------------------------------
	// Construction & Destruction
	//-----------------------------------------------------------------------------------------------------

	MainWindow::MainWindow(KernelInterface& kernel_interface, Document& document, QWidget* parent, Qt::WindowFlags flags)
		: QMainWindow {parent, flags}
		, m {std::make_unique<Internal>(document)}
	{
		// Widget.
		m->ui.setupUi(this);
		setDockNestingEnabled(true);
		setContextMenuPolicy(Qt::NoContextMenu);
		GX::QT::register_widget_state(*this, "MainWindow");
		
		// Stylesheet.
		QFile style_file(":/qdarkstyle/style.qss");
		if (style_file.exists())
		{
			style_file.open(QFile::ReadOnly | QFile::Text);
			QTextStream style_stream(&style_file);
			setStyleSheet(style_stream.readAll());
		}
		
		// UI.
		m->render_widget = new RenderWidget{kernel_interface};
		setCentralWidget(m->render_widget);
		
		auto setup_dock_widget = [this] (QWidget* widget, Qt::DockWidgetArea area)
		{
			auto dock_widget = new QDockWidget(widget->windowTitle());
			dock_widget->setWidget(widget);
			dock_widget->setAllowedAreas(Qt::AllDockWidgetAreas);
			dock_widget->setObjectName(widget->windowTitle());
			addDockWidget(area, dock_widget);
		};
		
		m->transform_list_widget = new TransformListWidget{kernel_interface, this};
		setup_dock_widget(m->transform_list_widget, Qt::LeftDockWidgetArea);

		m->transform_properties_widget = new TransformPropertiesWidget{kernel_interface, this};
		setup_dock_widget(m->transform_properties_widget, Qt::LeftDockWidgetArea);

		m->settings_widget = new SettingsWidget{kernel_interface, this};
		setup_dock_widget(m->settings_widget, Qt::RightDockWidgetArea);		
		
		// Menu.
		auto menu_file = menuBar()->addMenu("File");
		menu_file->addAction(QIcon{":/Images/New"}, "New Configuration", this, &MainWindow::slot_menu_file_new);
		menu_file->addAction(QIcon{":/Images/Open"}, "Open Configuration...", this, &MainWindow::slot_menu_file_open);
		menu_file->addAction(QIcon{":/Images/Save"}, "Save Configuration", this, &MainWindow::slot_menu_file_save);
		menu_file->addAction(QIcon{":/Images/SaveAs"}, "Save Configuration As...", this, &MainWindow::slot_menu_file_save_as);
		menu_file->addSeparator();
		menu_file->addAction("Exit", this, &MainWindow::close);

		auto menu_transform = menuBar()->addMenu("Transform");
		menu_transform->addAction(QIcon{":/Images/Add"}, "Add Transform", this, &MainWindow::slot_menu_transform_add);
		menu_transform->addAction(QIcon{":/Images/Remove"}, "Remove Transforms", this, &MainWindow::slot_menu_transform_remove);
		menu_transform->addAction(QIcon{":/Images/Clear"}, "Clear Transforms", this, &MainWindow::slot_menu_transform_clear);
		
		auto menu_help = menuBar()->addMenu("Help");
		menu_help->addAction("About...", this, &MainWindow::slot_menu_help_about);

		// Toolbar.
		auto toolbar = new QToolBar{};
		toolbar->setIconSize(QSize{24, 24});
		toolbar->setMovable(false);
		addToolBar(Qt::TopToolBarArea, toolbar);

		toolbar->addAction(QIcon{":/Images/New"}, "New Configuration", this, &MainWindow::slot_menu_file_new);
		toolbar->addAction(QIcon{":/Images/Open"}, "Open Configuration...", this, &MainWindow::slot_menu_file_open);
		toolbar->addAction(QIcon{":/Images/Save"}, "Save Configuration", this, &MainWindow::slot_menu_file_save);
		toolbar->addAction(QIcon{":/Images/SaveAs"}, "Save Configuration As...", this, &MainWindow::slot_menu_file_save_as);
		toolbar->addSeparator();
		toolbar->addAction(QIcon{":/Images/Add"}, "Add Transform", this, &MainWindow::slot_menu_transform_add);
		toolbar->addAction(QIcon{":/Images/Remove"}, "Remove Transforms", this, &MainWindow::slot_menu_transform_remove);
		toolbar->addAction(QIcon{":/Images/Clear"}, "Clear Transforms", this, &MainWindow::slot_menu_transform_clear);

		// Status bar.
		m->status_label = new QLabel{};
		m->ui.status->addPermanentWidget(m->status_label, 1);
		m->ui.status->setSizeGripEnabled(false);
		connect(&m->document.command_stack(), &GX::QT::CommandStack::cleanChanged, this, &MainWindow::update_status);
		update_status();

		// Input bindings.
		kernel_interface.input_system().bind(GX::QT::InputTrigger{GX::QT::InputKeyboardKey{Qt::Key_Control}, GX::QT::InputKeyboardKey{Qt::Key_N}},
											 GX::QT::TriggerInputCallback{std::bind(&MainWindow::slot_menu_file_new, this)});

		kernel_interface.input_system().bind(GX::QT::InputTrigger{GX::QT::InputKeyboardKey{Qt::Key_Control}, GX::QT::InputKeyboardKey{Qt::Key_O}},
											 GX::QT::TriggerInputCallback{std::bind(&MainWindow::slot_menu_file_open, this)});

		kernel_interface.input_system().bind(GX::QT::InputTrigger{GX::QT::InputKeyboardKey{Qt::Key_Control}, GX::QT::InputKeyboardKey{Qt::Key_S}},
											 GX::QT::TriggerInputCallback{std::bind(&MainWindow::slot_menu_file_save, this)});

		kernel_interface.input_system().bind(GX::QT::InputTrigger{{GX::QT::InputKeyboardKey{Qt::Key_Control}, GX::QT::InputKeyboardKey{Qt::Key_Shift}}, GX::QT::InputKeyboardKey{Qt::Key_S}},
											 GX::QT::TriggerInputCallback{std::bind(&MainWindow::slot_menu_file_save_as, this)});

		kernel_interface.input_system().bind(GX::QT::InputTrigger{GX::QT::InputKeyboardKey{Qt::Key_Control}, GX::QT::InputKeyboardKey{Qt::Key_Z}},
											 GX::QT::TriggerInputCallback{std::bind(&GX::QT::CommandStack::undo, &m->document.command_stack())});

		kernel_interface.input_system().bind(GX::QT::InputTrigger{GX::QT::InputKeyboardKey{Qt::Key_Control}, GX::QT::InputKeyboardKey{Qt::Key_Y}},
											 GX::QT::TriggerInputCallback{std::bind(&GX::QT::CommandStack::redo, &m->document.command_stack())});
	}

	MainWindow::~MainWindow()
	{
	}
	
	//-----------------------------------------------------------------------------------------------------
	// Menu Slots
	//-----------------------------------------------------------------------------------------------------

	void MainWindow::slot_menu_file_new()
	{
		if (!check_unsaved_changes())
		{
			return;
		}

		m->document.clear();
	}

	void MainWindow::slot_menu_file_open()
	{
		auto filename = QFileDialog::getOpenFileName(this, "Open", "", "Transform Visualizer Configurations (*.TransformVisualizer.json)");
		if (filename.isEmpty())
		{
			return;
		}

		if (!check_unsaved_changes())
		{
			return;
		}

		if (!m->document.load(filename))
		{
			QMessageBox::critical(this, "Transform Visualizer", "Failed to load file.");
			return;
		}

		update_status();
	}

	void MainWindow::slot_menu_file_save()
	{
		auto filename = m->document.filename();
		if (filename.isEmpty())
		{
			slot_menu_file_save_as();
			return;
		}

		if (!m->document.save(filename))
		{
			QMessageBox::critical(this, "Transform Visualizer", "Failed to save file.");
			return;
		}

		update_status();
	}

	void MainWindow::slot_menu_file_save_as()
	{
		auto filename = QFileDialog::getSaveFileName(this, "Open", "", "Transform Visualizer Configurations (*.TransformVisualizer.json)");
		if (filename.isEmpty())
		{
			return;
		}

		if (!m->document.save(filename))
		{
			QMessageBox::critical(this, "Transform Visualizer", "Failed to save file.");
			return;
		}

		update_status();
	}

	//-----------------------------------------------------------------------------------------------------

	void MainWindow::slot_menu_transform_add()
	{
		auto transform = std::make_shared<Transform>();
		transform->set_matrix(GX::Matrix::create_identity());
		m->document.transform_container().add(transform);
	}

	void MainWindow::slot_menu_transform_remove()
	{
		auto transforms = m->document.state().selected_transforms();
		transforms.removeOne(m->document.transform_container().world_transform());

		if (!transforms.isEmpty())
		{
			m->document.state().select_transforms(QVector<SPtr<Transform>>{});
			m->document.transform_container().remove(transforms);
		}
	}

	void MainWindow::slot_menu_transform_clear()
	{
		m->document.transform_container().clear();
	}

	//-----------------------------------------------------------------------------------------------------

	void MainWindow::slot_menu_help_about()
	{
		const int year0 = 2019;
		const int year1 = QDate::currentDate().year();
		const auto year_string = (year1 > year0 ? QString("%1 - %2").arg(year0).arg(year1) : QString::number(year0));

		QMessageBox::about(this, "About", "Transform Visualizer\n\nVersion 1.0\n\nCopyright " + year_string + "\nChristian Skogen");
	}

	//-----------------------------------------------------------------------------------------------------
	// Helpers
	//-----------------------------------------------------------------------------------------------------

	bool MainWindow::check_unsaved_changes()
	{
		if (!m->document.command_stack().isClean())
		{
			const auto a = QMessageBox::question(this, "Transform Visualizer", "Unsaved changes will be discarded.\n\nContinue?", QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
			if (a != QMessageBox::Yes)
			{
				return false;
			}
		}

		return true;
	}

	void MainWindow::update_status()
	{
		const auto filename = (!m->document.filename().isEmpty() ? m->document.filename() : "Untitled");
		m->status_label->setText(QString{"%1%2"}.arg(filename).arg(!m->document.command_stack().isClean() ? " (*)" : ""));
	}
}