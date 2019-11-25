#pragma once

// Project Includes
#include <GXQt/MVC/ModelData/ModelData.h>
#include <GXQt/API.h>

// Third-Party Includes
#include <QLayout>
#include <QWidget>

namespace GX::QT
{
	//-----------------------------------------------------------------------------------------------------
	// Interface
	//-----------------------------------------------------------------------------------------------------

	class MDViewDelegateEditor : public QWidget
	{
		Q_OBJECT

	public:
		// Construction & Destruction
						MDViewDelegateEditor	(QLayout* layout = new QHBoxLayout{}, QWidget* parent = nullptr);

		// Data
		virtual void	set_data				(const ModelDataEntry& entry) = 0;
		virtual void	get_data				(ModelDataEntry& entry) const = 0;

	signals:
		// Signals
		void			signal_data_changed		(bool committed);

	protected:
		// Data Slots
		virtual void	data_changed			();
		virtual void	data_committed			();

		// Data
		void			cache_data				();

	private:
		QVariant		m_value_cached;
	};

	//-----------------------------------------------------------------------------------------------------
	// Model Data
	//-----------------------------------------------------------------------------------------------------

	class MDEModelData : public MDViewDelegateEditor
	{
		Q_OBJECT

	public:
		// Construction & Destruction
							MDEModelData		(QWidget* parent = nullptr);
							~MDEModelData		();

		// Data
		void				set_data			(const ModelData& data);
		const ModelData&	get_data			() const;

		virtual void		set_data			(const ModelDataEntry& entry) override;
		virtual void		get_data			(ModelDataEntry& entry) const override;

	protected:
		// Data Slots
		void				slot_data_changed	(bool committed);

	private:
		// Editors
		void				create_editors		(const ModelData& data);

		struct Internal;
		const UPtr<Internal> m;
	};

	//-----------------------------------------------------------------------------------------------------
	// Bool
	//-----------------------------------------------------------------------------------------------------

	class MDEBool : public MDViewDelegateEditor
	{
	public:
						MDEBool		();
						~MDEBool	();

		virtual void	set_data	(const ModelDataEntry& entry) override;
		virtual void	get_data	(ModelDataEntry& entry) const override;

	private:
		struct Internal;
		const UPtr<Internal> m;
	};

	//-----------------------------------------------------------------------------------------------------
	// Int
	//-----------------------------------------------------------------------------------------------------

	class MDEInt : public MDViewDelegateEditor
	{
	public:
						MDEInt		();
						~MDEInt		();

		virtual void	set_data	(const ModelDataEntry& entry) override;
		virtual void	get_data	(ModelDataEntry& entry) const override;

	private:
		struct Internal;
		const UPtr<Internal> m;
	};

	//-----------------------------------------------------------------------------------------------------
	
	class MDEIntColor : public MDViewDelegateEditor
	{
	public:
						MDEIntColor		();
						~MDEIntColor	();

		virtual void	set_data		(const ModelDataEntry& entry) override;
		virtual void	get_data		(ModelDataEntry& entry) const override;

	private:
		struct Internal;
		const UPtr<Internal> m;
	};

	//-----------------------------------------------------------------------------------------------------
	// Float
	//-----------------------------------------------------------------------------------------------------

	class MDEFloat : public MDViewDelegateEditor
	{
	public:
						MDEFloat	();
						~MDEFloat	();

		virtual void	set_data	(const ModelDataEntry& entry) override;
		virtual void	get_data	(ModelDataEntry& entry) const override;

	private:
		struct Internal;
		const UPtr<Internal> m;
	};

	//-----------------------------------------------------------------------------------------------------
	// String
	//-----------------------------------------------------------------------------------------------------

	class MDEString : public MDViewDelegateEditor
	{
	public:
						MDEString	();
						~MDEString	();

		virtual void	set_data	(const ModelDataEntry& entry) override;
		virtual void	get_data	(ModelDataEntry& entry) const override;

	private:
		struct Internal;
		const UPtr<Internal> m;
	};

	//-----------------------------------------------------------------------------------------------------
	// Vector4F
	//-----------------------------------------------------------------------------------------------------

	class MDEVector4FColor : public MDViewDelegateEditor
	{
	public:
						MDEVector4FColor	();
						~MDEVector4FColor	();

		virtual void	set_data			(const ModelDataEntry& entry) override;
		virtual void	get_data			(ModelDataEntry& entry) const override;

	private:
		struct Internal;
		const UPtr<Internal> m;
	};
}