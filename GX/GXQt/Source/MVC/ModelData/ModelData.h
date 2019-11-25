#pragma once

// Project Includes
#include <GXQt/Core/MetaType.h>
#include <GXQt/API.h>

// Stdlib Includes
#include <functional>

// Third-Party Includes
#include <QVariant>
#include <QVector>

// Forward Declarations
namespace GX::QT
{
	class ModelDataEntry;
	class MDViewDelegateEditor;
}

namespace GX::QT
{
	//-----------------------------------------------------------------------------------------------------
	// Types
	//-----------------------------------------------------------------------------------------------------

	constexpr int ModelDataRole = Qt::UserRole + 0xABCD;

	using ModelDataEditorCreator = std::function<MDViewDelegateEditor*(const ModelDataEntry&)>;

	//-----------------------------------------------------------------------------------------------------
	// Attributes
	//-----------------------------------------------------------------------------------------------------

	enum class ModelDataAttribute
	{
		//				Value Type		Target Type		Description

		Enabled,	//	bool			All				Determines enabled flag.
		Visible,	//	bool			All				Determines visible flag.
		ReadOnly,	//	bool			All				Determines read-only flag.
		ValueMin,	//	int, float		int, float		Minimum value.
		ValueMax,	//	int, float		int, float		Maximum value.
		Color,		//	-				int, Vector4F	Value is a color.
	};

	//-----------------------------------------------------------------------------------------------------
	// Model Data Entry
	//-----------------------------------------------------------------------------------------------------

	class GXQT_API ModelDataEntry
	{
	public:
		// Construction & Destruction
						ModelDataEntry	();
						ModelDataEntry	(const QVariant& value);
						~ModelDataEntry	();

		// Value
		void			set_value		(const QVariant& value);
		const QVariant& value			() const;
		
		// Properties
		void			set_editor_pos	(int x, int y);
		const QPoint&	editor_pos		() const;

		// Attributes
		void			set_attribute	(ModelDataAttribute attribute, const QVariant& value = QVariant{});
		QVariant		attribute		(ModelDataAttribute attribute, const QVariant& value_default = QVariant{}) const;
		bool			has_attribute	(ModelDataAttribute attribute) const;

		// Operators
		bool			operator ==		(const ModelDataEntry& rhs) const;
		bool			operator !=		(const ModelDataEntry& rhs) const;

	private:
		QVariant							m_value;
		QHash<ModelDataAttribute, QVariant>	m_attributes;
		QPoint								m_editor_pos;
	};

	//-----------------------------------------------------------------------------------------------------
	// Model Data
	//-----------------------------------------------------------------------------------------------------

	class GXQT_API ModelData : public QVector<ModelDataEntry>
	{
	public:
		// Construction & Destruction
										ModelData			();
										ModelData			(std::initializer_list<ModelDataEntry> args);

		// Properties
		void							set_committed		(bool committed);
		bool							is_committed		() const;

		// Editor Creator
		void							set_editor_creator	(const ModelDataEditorCreator& creator);
		const ModelDataEditorCreator&	editor_creator		() const;

		// Operators
		bool							operator ==			(const ModelData& rhs) const;
		bool							operator !=			(const ModelData& rhs) const;

	private:
		ModelDataEditorCreator	m_editor_creator;
		bool					m_committed;
	};
}

// Qt Meta-types
Q_DECLARE_METATYPE(GX::QT::ModelDataEntry)
Q_DECLARE_METATYPE(GX::QT::ModelData)

// Qt Overloads
uint qHash(GX::QT::ModelDataAttribute attribute, uint seed = 0);

// Inline Includes
#include <GXQt/MVC/ModelData/ModelData.inl>
