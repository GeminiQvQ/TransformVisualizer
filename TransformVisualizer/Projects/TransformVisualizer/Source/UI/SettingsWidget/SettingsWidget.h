#pragma once

// Project Includes
#include <TransformVisualizer/API.h>

// Third-Party Includes
#include <QWidget>

// Forward Declarations
namespace TransformVisualizer
{
	class KernelInterface;
}

namespace TransformVisualizer
{
	class SettingsWidget : public QWidget
	{
	public:
		// Construction & Destruction
		SettingsWidget(KernelInterface& kernel_interface, QWidget* parent = nullptr);
		~SettingsWidget();

	private:
		struct Internal;
		const UPtr<Internal> m;
	};
}