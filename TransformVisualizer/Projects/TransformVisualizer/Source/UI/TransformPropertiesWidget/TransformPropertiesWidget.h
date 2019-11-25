#pragma once

// Project Includes
#include <TransformVisualizer/API.h>

// Third-Party Includes
#include <QWidget>

namespace TransformVisualizer
{
	class KernelInterface;
	class Transform;
}

namespace TransformVisualizer
{
	class TransformPropertiesWidget : public QWidget
	{
	public:
		// Construction & Destruction
				TransformPropertiesWidget	(KernelInterface& kernel_interface, QWidget* parent = nullptr);
				~TransformPropertiesWidget	();

	private:
		// State Slots
		void	slot_transforms_selected	(const QVector<SPtr<Transform>>& transforms);

	private:
		struct Internal;
		const UPtr<Internal> m;
	};
}