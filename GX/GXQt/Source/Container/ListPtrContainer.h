#pragma once

// Project Includes
#include <GXQt/Utility/CommandController.h>

// Stdlib Includes
#include <memory>

// Third-Party Includes
#include <QVector>

// Forward Declarations
namespace GX::QT
{
	template <class T>
	class ListPtrContainerEventHandler;

	template <class T, class U>
	class ListPtrContainerSearchCallback;
}

namespace GX::QT
{
	//-----------------------------------------------------------------------------------------------------
	// ListPtrContainer
	//-----------------------------------------------------------------------------------------------------

	template <class T>
	class ListPtrContainer
	{
	public:
		using Entry			= T;
		using EntryPtr		= std::shared_ptr<Entry>;
		using EntryCPtr		= std::shared_ptr<const Entry>;
		using EntryList		= QVector<EntryPtr>;
		using Iterator		= typename EntryList::iterator;
		using ConstIterator	= typename EntryList::const_iterator;
		using EventHandler	= ListPtrContainerEventHandler<T>;

	public:
		// Construction & Destruction
						ListPtrContainer	();
						ListPtrContainer	(CommandController command_controller);

		// Mutators
		void			set					(const EntryList& entries);
		void			clear				();

		void			add					(EntryPtr entry);
		void			add					(const EntryList& entries);
		void			remove				(EntryPtr entry);
		void			remove				(const EntryList& entries);
		void			move				(EntryPtr entry, int before_index);
		void			move				(EntryPtr entry, EntryPtr before_entry);
		void			move				(const EntryList& entries, int before_index);
		void			move				(const EntryList& entries, EntryPtr before_entry);
	
		// Accessors
		int				size				() const;
		EntryPtr		get					(int index);
		EntryCPtr		get					(int index) const;
		EntryPtr		get					(Entry& entry);
		EntryCPtr		get					(const Entry& entry) const;
		int				index_of			(const Entry& entry) const;

		// Iterators
		Iterator		begin				();
		ConstIterator	begin				() const;
		Iterator		end					();
		ConstIterator	end					() const;

		// Handlers
		void			add_handler			(EventHandler& handler);
		void			remove_handler		(EventHandler& handler);

	private:
		class CmdSet;
		class CmdAddRemove;
		class CmdAdd;
		class CmdRemove;
		class CmdMove;

		friend class CmdSet;
		friend class CmdAddRemove;
		friend class CmdMove;

		QVector<EntryPtr>		m_entries;
		QVector<EventHandler*>	m_handlers;
		CommandController		m_command_controller;
	};

	//-----------------------------------------------------------------------------------------------------
	// ListPtrContainerSearchable
	//-----------------------------------------------------------------------------------------------------

	template <class T, class U>
	class ListPtrContainerSearchable : public ListPtrContainer<T>
	{
	public:
		using SearchCallback = ListPtrContainerSearchCallback<T, U>;

	public:
		// Construction & Destruction
					ListPtrContainerSearchable	(const SearchCallback& search_callback);
					ListPtrContainerSearchable	(const SearchCallback& search_callback, CommandController command_controller);

		// Searching
		EntryPtr	find						(const U& value);
		EntryCPtr	find						(const U& value) const;

	private:
		const SearchCallback& m_search_callback;
	};

	//-----------------------------------------------------------------------------------------------------
	// ListPtrContainerEventHandler
	//-----------------------------------------------------------------------------------------------------

	template <class T>
	class ListPtrContainerEventHandler
	{
	public:
		using EntryPtr	= typename ListPtrContainer<T>::EntryPtr;
		using EntryList = typename ListPtrContainer<T>::EntryList;

	public:
		virtual void on_to_be_reset			(const EntryList& entries);
		virtual void on_reset				(const EntryList& entries);
		virtual void on_entry_to_be_added	(EntryPtr entry);
		virtual void on_entry_added			(EntryPtr entry);
		virtual void on_entry_to_be_removed	(EntryPtr entry);
		virtual void on_entry_removed		(EntryPtr entry);
		virtual void on_entry_to_be_moved	(EntryPtr entry, int index);
		virtual void on_entry_moved			(EntryPtr entry, int index);
	};

	//-----------------------------------------------------------------------------------------------------
	// ListPtrContainerEventHandler
	//-----------------------------------------------------------------------------------------------------

	template <class T, class U>
	class ListPtrContainerSearchCallback
	{
	public:
		virtual bool equals(const T& entry, const U& value) const = 0;
	};
}

// Inline Includes
#include <GXQt/Container/ListPtrContainer.inl>