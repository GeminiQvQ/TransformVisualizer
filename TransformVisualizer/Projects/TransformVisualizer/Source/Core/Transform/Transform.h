#pragma once

// Project Includes
#include <TransformVisualizer/API.h>

// Third-Party Includes
#include <QObject>

// Forward Declarations
class QJsonObject;

namespace GX
{
	class Matrix;
}

namespace TransformVisualizer
{
	class Transform : public QObject
	{
		Q_OBJECT
		
	public:
		// Construction & Destruction
							Transform					();
							~Transform					();

		// Properties
		void				set_name					(const QString& name);
		const QString&		name						() const;

		void				set_matrix					(const GX::Matrix& matrix);
		const GX::Matrix&	matrix						() const;

		void				set_enabled					(bool enabled);
		bool				enabled						() const;

		void				set_read_only				(bool read_only);
		bool				read_only					() const;

		// Serialization
		void				serialize					(QJsonObject& json) const;
		void				deserialize					(const QJsonObject& json);

	signals:
		// Signals
		void				signal_name_changed			(const QString& name);
		void				signal_matrix_changed		(const GX::Matrix& matrix);
		void				signal_enabled_changed		(bool enabled);
		void				signal_read_only_changed	(bool read_only);

	private:
		struct Internal;
		const UPtr<Internal> m;
	};
}