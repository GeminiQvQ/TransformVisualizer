#pragma once

// Stdlib Includes
#include <functional>

// Forward Declarations
class QObject;

namespace GX::QT
{
	void post_function_event(QObject& receiver, const std::function<void()>& function);
}