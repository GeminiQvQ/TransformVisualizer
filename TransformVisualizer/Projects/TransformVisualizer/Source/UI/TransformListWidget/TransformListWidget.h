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
	class TransformListWidget : public QWidget
	{
	public:
		// Construction & Destruction
				TransformListWidget			(KernelInterface& kernel_interface, QWidget* parent = nullptr);
				~TransformListWidget		();

	private:
		// UI Slots
		void	slot_view_selection_changed	();
		void	slot_context_menu			(const QPoint& pos);
		void	slot_add					();
		void	slot_remove					();
		void	slot_clear					();

		// State Slots
		void	slot_transforms_selected	(const QVector<SPtr<Transform>>& transforms);

	private:
		struct Internal;
		const UPtr<Internal> m;
	};
}