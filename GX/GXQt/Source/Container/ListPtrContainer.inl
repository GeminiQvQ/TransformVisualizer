// Project Includes
#include <GXLib/StdExt/Invoke.h>

namespace GX::QT
{
	//-----------------------------------------------------------------------------------------------------
	// Construction & Destruction
	//-----------------------------------------------------------------------------------------------------

	template <class T>
	ListPtrContainer<T>::ListPtrContainer()
		: ListPtrContainer {CommandController{}}
	{
	}

	template <class T>
	ListPtrContainer<T>::ListPtrContainer(CommandController command_controller)
		: m_entries {}
		, m_handlers {}
		, m_command_controller {command_controller}
	{
	}

	//-----------------------------------------------------------------------------------------------------
	// Mutators
	//-----------------------------------------------------------------------------------------------------

	template <class T>
	class ListPtrContainer<T>::CmdSet : public CommandSingle
	{
	public:
		CmdSet(ListPtrContainer& container, const EntryList& entries)
			: CommandSingle {"Set"}
			, m_container {container}
			, m_entries {entries}
		{
		}

		virtual void exec() override
		{
			const auto entries_old = m_container.m_entries;

			invoke(m_container.m_handlers, &ListPtrContainer::EventHandler::on_to_be_reset, m_entries);
			m_container.m_entries = m_entries;
			invoke(m_container.m_handlers, &ListPtrContainer::EventHandler::on_reset, m_entries);

			m_entries = entries_old;
		}

	private:
		ListPtrContainer& m_container;
		EntryList		  m_entries;
	};

	template <class T>
	void ListPtrContainer<T>::set(const EntryList& entries)
	{
		m_command_controller.exec<CmdSet>(*this, entries);
	}

	template <class T>
	void ListPtrContainer<T>::clear()
	{
		set(EntryList{});
	}

	//-----------------------------------------------------------------------------------------------------

	template <class T>
	class ListPtrContainer<T>::CmdAddRemove : public Command
	{
	public:
		CmdAddRemove(const QString& text, ListPtrContainer& container, const EntryPtr& entry)
			: Command {text}
			, m_container {container}
			, m_entry {entry}
		{
		}

		void add()
		{
			invoke(m_container.m_handlers, &ListPtrContainer::EventHandler::on_entry_to_be_added, m_entry);
			m_container.m_entries.push_back(m_entry);
			invoke(m_container.m_handlers, &ListPtrContainer::EventHandler::on_entry_added, m_entry);
		}

		void remove()
		{
			invoke(m_container.m_handlers, &ListPtrContainer::EventHandler::on_entry_to_be_removed, m_entry);
			m_container.m_entries.removeOne(m_entry);
			invoke(m_container.m_handlers, &ListPtrContainer::EventHandler::on_entry_removed, m_entry);
		}

	private:
		ListPtrContainer& m_container;
		EntryPtr		  m_entry;
	};

	//-----------------------------------------------------------------------------------------------------

	template <class T>
	class ListPtrContainer<T>::CmdAdd : public ListPtrContainer::CmdAddRemove
	{
	public:
		CmdAdd(ListPtrContainer& container, const EntryPtr& entry)
			: CmdAddRemove {"Add", container, entry}
		{
		}

		virtual void redo() override
		{
			add();
		}

		virtual void undo() override
		{
			remove();
		}
	};

	template <class T>
	void ListPtrContainer<T>::add(EntryPtr entry)
	{
		add(EntryList{entry});
	}

	template <class T>
	void ListPtrContainer<T>::add(const EntryList& entries)
	{
		m_command_controller.begin();

		for (auto&& entry : entries)
		{
			m_command_controller.exec<CmdAdd>(*this, entry);
		}

		m_command_controller.end();
	}

	//-----------------------------------------------------------------------------------------------------

	template <class T>
	class ListPtrContainer<T>::CmdRemove : public ListPtrContainer::CmdAddRemove
	{
	public:
		CmdRemove(ListPtrContainer& container, const EntryPtr& entry)
			: CmdAddRemove {"Remove", container, entry}
		{
		}

		virtual void redo() override
		{
			remove();
		}

		virtual void undo() override
		{
			add();
		}
	};

	template <class T>
	void ListPtrContainer<T>::remove(EntryPtr entry)
	{
		remove(EntryList{entry});
	}

	template <class T>
	void ListPtrContainer<T>::remove(const EntryList& entries)
	{
		m_command_controller.begin();

		for (auto&& entry : entries)
		{
			if (index_of(*entry) < m_entries.size() - 1)
			{
				move(entry, -1);
			}

			m_command_controller.exec<CmdRemove>(*this, entry);
		}
		
		m_command_controller.end();
	}

	//-----------------------------------------------------------------------------------------------------

	template <class T>
	class ListPtrContainer<T>::CmdMove : public CommandSingle
	{
	public:
		CmdMove(ListPtrContainer& container, const EntryPtr& entry, int index)
			: CommandSingle {"Move"}
			, m_container {container}
			, m_entry {entry}
			, m_index {index}
		{
		}

		virtual void exec() override
		{
			int index_old = m_container.m_entries.indexOf(m_entry);

			int index_new_reported = m_index;
			if (m_index > index_old)
			{
				++index_new_reported;
			}

			invoke(m_container.m_handlers, &ListPtrContainer::EventHandler::on_entry_to_be_moved, m_entry, index_new_reported);
			m_container.m_entries.removeOne(m_entry);
			m_container.m_entries.insert(std::min(m_index, m_container.m_entries.size()), m_entry);
			invoke(m_container.m_handlers, &ListPtrContainer::EventHandler::on_entry_moved, m_entry, index_new_reported);

			m_index = index_old;
		}

	private:
		ListPtrContainer& m_container;
		EntryPtr		  m_entry;
		int				  m_index;
	};

	template <class T>
	void ListPtrContainer<T>::move(EntryPtr entry, int before_index)
	{
		move(EntryList{entry}, before_index);
	}

	template <class T>
	void ListPtrContainer<T>::move(EntryPtr entry, EntryPtr before_entry)
	{
		move(EntryList{entry}, before_entry);
	}

	template <class T>
	void ListPtrContainer<T>::move(const EntryList& entries, int before_index)
	{
		const auto before_entry = (before_index >= 0 && before_index < m_entries.size() ? m_entries[before_index] : nullptr);
		move(entries, before_entry);
	}

	template <class T>
	void ListPtrContainer<T>::move(const EntryList& entries, EntryPtr before_entry)
	{
		while (entries.contains(before_entry))
		{
			const int next_index = m_entries.indexOf(before_entry) + 1;
			before_entry = (next_index < m_entries.size() ? m_entries[next_index] : nullptr);
		}

		m_command_controller.begin();

		for (auto&& entry : entries)
		{
			int index_old = m_entries.indexOf(entry);
			int index_new = m_entries.indexOf(before_entry);

			if (index_new == -1)
			{
				index_new = m_entries.size();
			}

			if (index_new > index_old && index_new < m_entries.size())
			{
				--index_new;
			}

			if (index_new == index_old)
			{
				continue;
			}

			m_command_controller.exec<CmdMove>(*this, entry, index_new);
		}

		m_command_controller.end();
	}

	//-----------------------------------------------------------------------------------------------------
	// Accessors
	//-----------------------------------------------------------------------------------------------------

	template <class T>
	int ListPtrContainer<T>::size() const
	{
		return m_entries.size();
	}

	template <class T>
	typename ListPtrContainer<T>::EntryPtr ListPtrContainer<T>::get(int index)
	{
		return m_entries[index];
	}

	template <class T>
	typename ListPtrContainer<T>::EntryCPtr ListPtrContainer<T>::get(int index) const
	{
		return m_entries[index];
	}

	template <class T>
	typename ListPtrContainer<T>::EntryPtr ListPtrContainer<T>::get(Entry& entry)
	{
		const auto it = std::find_if(m_entries.begin(), m_entries.end(), [&entry] (auto&& entry_ptr)
		{
			return (entry_ptr.get() == &entry);
		});
		return (it != m_entries.end() ? *it : nullptr);
	}

	template <class T>
	typename ListPtrContainer<T>::EntryCPtr ListPtrContainer<T>::get(const Entry& entry) const
	{
		const auto it = std::find_if(m_entries.begin(), m_entries.end(), [&entry] (auto&& entry_ptr)
		{
			return (entry_ptr.get() == &entry);
		});
		return (it != m_entries.end() ? *it : nullptr);
	}

	template <class T>
	int ListPtrContainer<T>::index_of(const Entry& entry) const
	{
		const auto it = std::find_if(m_entries.begin(), m_entries.end(), [&entry] (auto&& entry_ptr)
		{
			return (entry_ptr.get() == &entry);
		});
		return (it != m_entries.end() ? std::distance(m_entries.begin(), it) : -1);
	}

	//-----------------------------------------------------------------------------------------------------
	// Iterators
	//-----------------------------------------------------------------------------------------------------

	template <class T>
	typename ListPtrContainer<T>::Iterator ListPtrContainer<T>::begin()
	{
		return m_entries.begin();
	}

	template <class T>
	typename ListPtrContainer<T>::ConstIterator ListPtrContainer<T>::begin() const
	{
		return m_entries.begin();
	}

	template <class T>
	typename ListPtrContainer<T>::Iterator ListPtrContainer<T>::end()
	{
		return m_entries.end();
	}

	template <class T>
	typename ListPtrContainer<T>::ConstIterator ListPtrContainer<T>::end() const
	{
		return m_entries.end();
	}

	//-----------------------------------------------------------------------------------------------------
	// Handlers
	//-----------------------------------------------------------------------------------------------------

	template <class T>
	void ListPtrContainer<T>::add_handler(EventHandler& handler)
	{
		assert(!m_handlers.contains(&handler));
		m_handlers.push_back(&handler);
	}

	template <class T>
	void ListPtrContainer<T>::remove_handler(EventHandler& handler)
	{
		assert(m_handlers.contains(&handler));
		m_handlers.removeOne(&handler);
	}
}

namespace GX::QT
{
	//-----------------------------------------------------------------------------------------------------
	// Construction & Destruction
	//-----------------------------------------------------------------------------------------------------

	template <class T, class U>
	ListPtrContainerSearchable<T, U>::ListPtrContainerSearchable(const SearchCallback& search_callback)
		//: ListPtrContainer {}
		: m_search_callback {search_callback}
	{
	}

	template <class T, class U>
	ListPtrContainerSearchable<T, U>::ListPtrContainerSearchable(const SearchCallback& search_callback, CommandController command_controller)
		: ListPtrContainer {command_controller}
		, m_search_callback {search_callback}
	{
	}

	//-----------------------------------------------------------------------------------------------------
	// Searching
	//-----------------------------------------------------------------------------------------------------

	template <class T, class U>
	typename ListPtrContainerSearchable<T, U>::EntryPtr ListPtrContainerSearchable<T, U>::find(const U& value)
	{
		const auto it = std::find_if(begin(), end(), [this, &value] (auto&& entry)
		{
			return m_search_callback.equals(*entry, value);
		});
		return (it != end() ? *it : nullptr);
	}

	template <class T, class U>
	typename ListPtrContainerSearchable<T, U>::EntryCPtr ListPtrContainerSearchable<T, U>::find(const U& value) const
	{
		const auto it = std::find_if(begin(), end(), [this, &value] (auto&& entry)
		{
			return m_search_callback.equals(*entry, value);
		});
		return (it != end() ? *it : nullptr);
	}
}

namespace GX::QT
{	
	//-----------------------------------------------------------------------------------------------------
	// ListPtrContainerEventHandler
	//-----------------------------------------------------------------------------------------------------

	template <class T>
	void ListPtrContainerEventHandler<T>::on_to_be_reset(const EntryList& /*entries*/)
	{
	}

	template <class T>
	void ListPtrContainerEventHandler<T>::on_reset(const EntryList& /*entries*/)
	{
	}

	template <class T>
	void ListPtrContainerEventHandler<T>::on_entry_to_be_added(EntryPtr /*entry*/)
	{
	}

	template <class T>
	void ListPtrContainerEventHandler<T>::on_entry_added(EntryPtr /*entry*/)
	{
	}

	template <class T>
	void ListPtrContainerEventHandler<T>::on_entry_to_be_removed(EntryPtr /*entry*/)
	{
	}

	template <class T>
	void ListPtrContainerEventHandler<T>::on_entry_removed(EntryPtr /*entry*/)
	{
	}

	template <class T>
	void ListPtrContainerEventHandler<T>::on_entry_to_be_moved(EntryPtr /*entry*/, int /*index*/)
	{
	}

	template <class T>
	void ListPtrContainerEventHandler<T>::on_entry_moved(EntryPtr /*entry*/, int /*index*/)
	{
	}
}