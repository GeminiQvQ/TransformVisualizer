#pragma once

// Project Includes
#include <TransformVisualizer/API.h>

// Third-Party Includes
#include <QObject>
#include <QVector>

// Forward Declarations
namespace GX
{
	class Camera;
}

namespace TransformVisualizer
{
	class Transform;
}

namespace TransformVisualizer
{
	class State : public QObject
	{
		Q_OBJECT

	public:
		// Construction & Destruction
											State						();
											~State						();

		// State
		void								select_transforms			(const QVector<SPtr<Transform>>& transforms);
		const QVector<SPtr<Transform>>&		selected_transforms			();
		const QVector<SCPtr<Transform>>&	selected_transforms			() const;
		
		GX::Camera&							camera						();
		const GX::Camera&					camera						() const;
		void								begin_camera_change			();
		void								end_camera_change			();

	signals:
		// Signals
		void								signal_transforms_selected	(const QVector<SPtr<Transform>>& transforms);
		void								signal_camera_change_begin	(const GX::Camera& camera);
		void								signal_camera_change_end	(const GX::Camera& camera_old, const GX::Camera& camera_new);

	private:
		struct Internal;
		const UPtr<Internal> m;
	};
}