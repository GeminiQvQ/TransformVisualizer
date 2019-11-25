#pragma once

// Project Includes
#include <GXQt/Utility/Command.h>
#include <TransformVisualizer/API.h>

namespace TransformVisualizer
{
	class State;
	class TransformContainer;
	struct Settings;
}

namespace TransformVisualizer
{
	class Document
	{
	public:
		// Construction & Destruction
									Document			();
									~Document			();

		// Save & Load
		bool						save				(const QString& filename);
		bool						load				(const QString& filename);
		void						clear				();

		// Properties
		const QString&				filename			() const;

		// Accessors
		State&						state				();
		const State&				state				() const;
		GX::QT::CommandStack&		command_stack		();
		const GX::QT::CommandStack&	command_stack		() const;
		TransformContainer&			transform_container	();
		const TransformContainer&	transform_container	() const; 

	private:
		struct Internal;
		const UPtr<Internal> m;
	};
}