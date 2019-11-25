#pragma once

// Project Includes
#include <GXQt/API.h>

// Forward Declarations
class QJsonObject;
class QString;
class QWidget;

namespace GX::QT
{
	GXQT_API void save_widget_state		(const QWidget& widget, QJsonObject& json);
	GXQT_API void load_widget_state		(QWidget& widget, const QJsonObject& json);
	GXQT_API void register_widget_state	(QWidget& widget, const QString& name);
}