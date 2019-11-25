namespace GX::QT
{
	//-----------------------------------------------------------------------------------------------------
	// Construction & Destruction
	//-----------------------------------------------------------------------------------------------------

	inline ModelDataEntry::ModelDataEntry()
		: ModelDataEntry(QVariant{})
	{
	}

	inline ModelDataEntry::ModelDataEntry(const QVariant& value)
		: m_value {value}
		, m_attributes {}
		, m_editor_pos {-1, -1}
	{
	}

	inline ModelDataEntry::~ModelDataEntry()
	{
	}

	//-----------------------------------------------------------------------------------------------------
	// Value
	//-----------------------------------------------------------------------------------------------------

	inline void ModelDataEntry::set_value(const QVariant& value)
	{
		m_value = value;
	}

	inline const QVariant& ModelDataEntry::value() const
	{
		return m_value;
	}

	//-----------------------------------------------------------------------------------------------------
	// Properties
	//-----------------------------------------------------------------------------------------------------

	inline void ModelDataEntry::set_editor_pos(int x, int y)
	{
		m_editor_pos = QPoint(x, y);
	}

	inline const QPoint& ModelDataEntry::editor_pos() const
	{
		return m_editor_pos;
	}

	//-----------------------------------------------------------------------------------------------------
	// Attributes
	//-----------------------------------------------------------------------------------------------------

	inline void ModelDataEntry::set_attribute(ModelDataAttribute attribute, const QVariant& value)
	{
		m_attributes[attribute] = value;
	}

	inline QVariant ModelDataEntry::attribute(ModelDataAttribute attribute, const QVariant& value_default) const
	{
		return m_attributes.value(attribute, value_default);
	}

	inline bool ModelDataEntry::has_attribute(ModelDataAttribute attribute) const
	{
		return m_attributes.contains(attribute);
	}

	//-----------------------------------------------------------------------------------------------------
	// Operators
	//-----------------------------------------------------------------------------------------------------

	inline bool ModelDataEntry::operator == (const ModelDataEntry& rhs) const
	{
		return (m_value == rhs.m_value);
	}

	inline bool ModelDataEntry::operator != (const ModelDataEntry& rhs) const
	{
		return (m_value != rhs.m_value);
	}
}

namespace GX::QT
{
	//-----------------------------------------------------------------------------------------------------
	// Properties
	//-----------------------------------------------------------------------------------------------------

	inline void ModelData::set_committed(bool committed)
	{
		m_committed = committed;
	}

	inline bool ModelData::is_committed() const
	{
		return m_committed;
	}

	//-----------------------------------------------------------------------------------------------------
	// Editor Creator
	//-----------------------------------------------------------------------------------------------------

	inline void ModelData::set_editor_creator(const ModelDataEditorCreator& creator)
	{
		m_editor_creator = creator;
	}

	inline const ModelDataEditorCreator& ModelData::editor_creator() const
	{
		return m_editor_creator;
	}

	//-----------------------------------------------------------------------------------------------------
	// Operators
	//-----------------------------------------------------------------------------------------------------

	inline bool ModelData::operator == (const ModelData& rhs) const
	{
		return (QVector<ModelDataEntry>::operator==(rhs) &&
				m_committed == rhs.m_committed);
	}

	inline bool ModelData::operator != (const ModelData& rhs) const
	{
		return !(*this == rhs);
	}
}

//-----------------------------------------------------------------------------------------------------
// Qt Overloads
//-----------------------------------------------------------------------------------------------------

inline uint qHash(GX::QT::ModelDataAttribute attribute, uint seed)
{
	return qHash((uint)attribute, seed);
}