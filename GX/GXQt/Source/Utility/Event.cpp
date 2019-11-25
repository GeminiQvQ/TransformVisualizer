// Local Includes
#include <GXQt/Utility/Event.h>

// Third-Party Includes
#include <QApplication>

namespace GX::QT
{
	void post_function_event(QObject& receiver, const std::function<void()>& function)
	{
		// Event class.
		class FunctionEvent : public QEvent
		{
		public:
			FunctionEvent(const std::function<void()>& function)
				: QEvent {type()}
				, m_function {function}
			{
			}

			void run()
			{
				m_function();
			}

			static QEvent::Type type()
			{
				static QEvent::Type s_type = static_cast<QEvent::Type>(QEvent::registerEventType());
				return s_type;
			}

		private:
			std::function<void()> m_function;
		};

		// Event filter class.
		class FunctionEventFilter : public QObject
		{
		public:
			FunctionEventFilter(QObject& receiver)
				: QObject {&receiver}
			{
				receiver.installEventFilter(this);
			}

			virtual bool eventFilter(QObject* object, QEvent* event)
			{
				if (event->type() == FunctionEvent::type())
				{
					static_cast<FunctionEvent*>(event)->run();
					object->removeEventFilter(this);
					deleteLater();
					return true;
				}

				return false;
			}
		};

		// Post event.
		new FunctionEventFilter{receiver};
		qApp->postEvent(&receiver, new FunctionEvent{function});
	}
}