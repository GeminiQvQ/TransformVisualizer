#pragma once

// Project Includes
#include <GXQt/API.h>

// Third-Party Includes
#include <QModelIndexList>
#include <QObject>

// Forward Declarations
class QTreeView;

namespace GX::QT
{
	class MDTreeViewController : public QObject
	{
	public:
		// Construction & Destruction
				MDTreeViewController	(QTreeView& view, QObject* parent = nullptr);
				~MDTreeViewController	();

	private:
		// Model Controller SLots
		void	slot_request_editors	(const QModelIndexList& indices);

	private:
		// Helpers
		void	toggle_editors			(const QModelIndexList& indices);

		struct Internal;
		const UPtr<Internal> m;
	};
}