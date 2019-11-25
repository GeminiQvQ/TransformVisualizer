// Local Includes
#include <TransformVisualizer/Core/State.h>

// Project Includes
#include <GXLib/Graphics/Camera.h>

namespace TransformVisualizer
{
	//-----------------------------------------------------------------------------------------------------
	// Internal
	//-----------------------------------------------------------------------------------------------------

	struct State::Internal
	{
		QVector<SPtr<Transform>>	selected_transforms;
		QVector<SCPtr<Transform>>	selected_transforms_const;
		GX::Camera					camera;
		GX::Camera					camera_old;

		Internal()
			: selected_transforms {}
			, selected_transforms_const {}
			, camera {GX::Vector3F{0.0f, 0.0f, -5.0f}, GX::Vector3F{}, GX::Vector3F{0.0f, 1.0f, 0.0f}}
		{
		}
	};

	//-----------------------------------------------------------------------------------------------------
	// Construction & Destruction
	//-----------------------------------------------------------------------------------------------------

	State::State()
		: m {std::make_unique<Internal>()}
	{
	}

	State::~State()
	{
	}

	//-----------------------------------------------------------------------------------------------------
	// State
	//-----------------------------------------------------------------------------------------------------

	void State::select_transforms(const QVector<SPtr<Transform>>& transforms)
	{
		m->selected_transforms = transforms;

		m->selected_transforms_const.clear();
		std::copy(transforms.begin(), transforms.end(), std::back_inserter(m->selected_transforms_const));

		emit signal_transforms_selected(transforms);
	}

	const QVector<SPtr<Transform>>& State::selected_transforms()
	{
		return m->selected_transforms;
	}

	const QVector<SCPtr<Transform>>& State::selected_transforms() const
	{
		return m->selected_transforms_const;
	}

	//-----------------------------------------------------------------------------------------------------
	
	GX::Camera& State::camera()
	{
		return m->camera;
	}

	const GX::Camera& State::camera() const
	{
		return m->camera;
	}

	void State::begin_camera_change()
	{
		m->camera_old = m->camera;
		emit signal_camera_change_begin(m->camera);
	}

	void State::end_camera_change()
	{
		emit signal_camera_change_end(m->camera_old, m->camera);
		m->camera_old = m->camera;
	}
}