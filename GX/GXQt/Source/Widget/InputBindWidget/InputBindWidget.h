#pragma once

// Project Includes
#include <GXQt/API.h>

// Third-Party Includes
#include <QWidget>

namespace GX::QT
{
	class InputBindWidget : public QWidget
	{
	public:
		// Construction & Destruction
		InputBindWidget(QWidget* parent = nullptr);
		~InputBindWidget();

	private:
		struct Internal;
		const UPtr<Internal> m;
	};
}