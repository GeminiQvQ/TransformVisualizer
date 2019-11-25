#pragma once

// Project Includes
#include <TransformVisualizer/API.h>

// Third-Party Includes
#include <QMainWindow>

// Forward Declarations
namespace TransformVisualizer
{
	class Document;
	class KernelInterface;
}

namespace TransformVisualizer
{
	class MainWindow : public QMainWindow
	{
	public:
		// Construction & Destruction
				MainWindow					(KernelInterface& kernel_interface, Document& document, QWidget* parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());
				~MainWindow					();

	private:
		// Menu Slots
		void	slot_menu_file_new			();
		void	slot_menu_file_open			();
		void	slot_menu_file_save			();
		void	slot_menu_file_save_as		();
		void	slot_menu_transform_add		();
		void	slot_menu_transform_remove	();
		void	slot_menu_transform_clear	();
		void	slot_menu_help_about		();

	private:
		// Helpers
		bool	check_unsaved_changes		();
		void	update_status				();

		struct Internal;
		const UPtr<Internal> m;
	};
}