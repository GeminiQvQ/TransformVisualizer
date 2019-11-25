// Project Includes
#include <GXLib/API.h>

// Stdlib Includes
#include <vector>

namespace GX
{
	class GXLIBS_API EnableTypeID
	{
	public:
		virtual					 ~EnableTypeID	() = default;

		virtual int				 get_type_id	() const;
		virtual std::vector<int> get_type_ids	() const;
		static int				 create_type_id	();
	};										  
}

#define GX_DECLARE_TYPE_ID_TEMPLATE(base_type, crtp_type)														\
	template <typename T, class Base = base_type>																\
	class GXLIBS_API crtp_type : public Base																	\
	{																											\
	public:																										\
		template <typename... Args>																				\
		explicit				 crtp_type			 (Args&&... args) : Base(std::forward<Args>(args)...) {}	\
								 crtp_type			 (const crtp_type<T, Base>& rhs) : Base(rhs) {}				\
																												\
		static int				 get_static_type_id	 ();														\
		virtual int				 get_type_id		 () const override { return s_type_id; }					\
																												\
		virtual std::vector<int> get_type_ids		 () const override											\
		{																										\
			auto type_ids = Base::get_type_ids();																\
			type_ids.push_back(s_type_id);																		\
			return type_ids;																					\
		}																										\
	private:																									\
		static const int		 s_type_id;																		\
	};

#define GX_REGISTER_TYPE_ID_TEMPLATE(base_type, crtp_type, derived_type)										\
	const int crtp_type<derived_type, base_type>::s_type_id = GX::EnableTypeID::create_type_id();				\
	int crtp_type<derived_type, base_type>::get_static_type_id() { return s_type_id; } 