#pragma once

// Project Includes
#include <GXQt/Container/ListPtrContainer.h>
#include <TransformVisualizer/Core/Transform/Transform.h>
#include <TransformVisualizer/API.h>

// Forward Declarations
class QJsonObject;

namespace TransformVisualizer
{
	class TransformContainer : public QObject
							 , public GX::QT::ListPtrContainer<Transform>
							 , private GX::QT::ListPtrContainerEventHandler<Transform>
	{
		Q_OBJECT

	public:
		// Construction & Destruction
							TransformContainer				(GX::QT::CommandController command_controller);
							~TransformContainer				();

		// Serialization
		void				serialize						(QJsonObject& json) const;
		void				deserialize						(const QJsonObject& json);

		// Accessors
		SPtr<Transform>		world_transform					();
		SCPtr<Transform>	world_transform					() const;

	signals:
		// Signals
		void				signal_to_be_reset				();
		void				signal_reset					();
		void				signal_transform_to_be_added	(SPtr<Transform> transform);
		void				signal_transform_added			(SPtr<Transform> transform);
		void				signal_transform_to_be_removed	(SPtr<Transform> transform);
		void				signal_transform_removed		(SPtr<Transform> transform);
		void				signal_transform_to_be_moved	(SPtr<Transform> transform, int index);
		void				signal_transform_moved			(SPtr<Transform> transform, int index);

	private:
		// ListPtrContainerEventHandler Interface
		virtual void		on_to_be_reset					(const QVector<SPtr<Transform>>& entries) override;
		virtual void		on_reset						(const QVector<SPtr<Transform>>& entries) override;
		virtual void		on_entry_to_be_added			(SPtr<Transform> entry) override;
		virtual void		on_entry_added					(SPtr<Transform> entry) override;
		virtual void		on_entry_to_be_removed			(SPtr<Transform> entry) override;
		virtual void		on_entry_removed				(SPtr<Transform> entry) override;
		virtual void		on_entry_to_be_moved			(SPtr<Transform> entry, int index) override;
		virtual void		on_entry_moved					(SPtr<Transform> entry, int index) override;

		// Helpers
		void				update_world_transform			();

		struct Internal;
		const UPtr<Internal> m;
	};
}