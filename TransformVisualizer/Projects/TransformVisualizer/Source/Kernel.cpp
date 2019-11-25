// Local Includes
#include <TransformVisualizer/Kernel.h>

// Project Includes
#include <GXQt/System/Input/InputSystem.h>
#include <GXQt/Utility/WidgetState/WidgetStateManager.h>
#include <GXQt/Utility/CommandController.h>
#include <TransformVisualizer/Core/Settings.h>
#include <TransformVisualizer/Data/Document.h>
#include <TransformVisualizer/UI/MainWindow/MainWindow.h>
#include <TransformVisualizer/KernelInterface.h>

// Third-Party Includes
#include <QJsonDocument>
#include <QJsonObject>
#include <QSettings>

namespace TransformVisualizer
{
	//-----------------------------------------------------------------------------------------------------
	// Internal
	//-----------------------------------------------------------------------------------------------------

	struct Kernel::Internal
	{
		class KernelInterfaceImpl : public KernelInterface
		{
		public:
			KernelInterfaceImpl(Internal& i) : i{i} {}

			virtual GX::QT::InputSystem&		input_system		() override			{ return i.input_system;					}
			virtual const GX::QT::InputSystem&	input_system		() const override	{ return i.input_system;					}
			virtual Settings&					settings			() override			{ return i.settings;						}
			virtual const Settings&				settings			() const override	{ return i.settings;						}
			virtual State&						state				() override			{ return i.document.state();				}
			virtual const State&				state				() const override	{ return i.document.state();				}
			virtual TransformContainer&			transform_container	() override			{ return i.document.transform_container();	}
			virtual const TransformContainer&	transform_container	() const override	{ return i.document.transform_container();	}
			virtual GX::QT::CommandController	command_controller	() override			{ return i.document.command_stack();		}

		private:
			Internal& i;
		};

		KernelInterfaceImpl			kernel_interface;
		Settings					settings;
		Document					document;
		GX::QT::InputSystem			input_system;
		GX::QT::WidgetStateManager	widget_state_manager;
		MainWindow*					main_window;

		Internal()
			: kernel_interface {*this}
			, settings {}
			, document {}
			, widget_state_manager {}
			, main_window {}
		{
		}
	};

	//-----------------------------------------------------------------------------------------------------
	// Construction & Destruction
	//-----------------------------------------------------------------------------------------------------

	Kernel::Kernel()
		: m {std::make_unique<Internal>()}
	{
		// Load settings.
		QSettings settings{"TransformVisualizer.ini", QSettings::IniFormat};
		
		auto settings_ = QJsonDocument::fromJson(qPrintable(settings.value("Settings").toString())).object();
		m->settings.load(settings_);

		auto widget_state = QJsonDocument::fromJson(qPrintable(settings.value("WidgetState").toString())).object();
		m->widget_state_manager.load(widget_state);
	}

	Kernel::~Kernel()
	{
		// Save settings.
		QSettings settings{"TransformVisualizer.ini", QSettings::IniFormat};

		QJsonObject settings_;
		m->settings.save(settings_);
		settings.setValue("Settings", QJsonDocument(settings_).toJson(QJsonDocument::Compact));

		QJsonObject widget_state;
		m->widget_state_manager.save(widget_state);
		settings.setValue("WidgetState", QJsonDocument(widget_state).toJson(QJsonDocument::Compact));
	}

	//-----------------------------------------------------------------------------------------------------
	// Initialization
	//-----------------------------------------------------------------------------------------------------

	void Kernel::create()
	{
		m->main_window = new MainWindow{m->kernel_interface, m->document};
		m->main_window->show();
	}

	void Kernel::destroy()
	{
		delete m->main_window;
		m->main_window = nullptr;
	}
}