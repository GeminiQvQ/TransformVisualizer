#pragma once

// Stdlib Includes
#include <algorithm>	
#include <string>
#include <string_view>
#include <vector>

namespace GX
{
	template <class T>
	struct EnumReflection;

	template <class T>
	struct EnumReflectionInfo
	{
		T			type;
		std::string type_name;
		std::string display_name;
	};
}

#define DECL_ENUM_REFLECTION(Type) \
	DECL_ENUM_REFLECTION_INFO(Type, GX::EnumReflectionInfo<Type>)

#define DECL_ENUM_REFLECTION_EXPORT(Type, Export) \
	DECL_ENUM_REFLECTION_INFO_EXPORT(Type, GX::EnumReflectionInfo<Type>, Export)

#define DECL_ENUM_REFLECTION_INFO(Type, Info) \
	DECL_ENUM_REFLECTION_PRIMARY(Type, Info, )

#define DECL_ENUM_REFLECTION_INFO_EXPORT(Type, Info, Export) \
	DECL_ENUM_REFLECTION_PRIMARY(Type, Info, Export)

#define DEF_ENUM_REFLECTION(Type) \
	DEF_ENUM_REFLECTION_PRIMARY(Type, GX::EnumReflectionInfo<Type>)

#define DEF_ENUM_REFLECTION_LOCAL(Type) \
	DEF_ENUM_REFLECTION_INFO_LOCAL(Type, GX::EnumReflectionInfo<Type>)

#define DEF_ENUM_REFLECTION_INFO_LOCAL(Type, Info) \
	DECL_ENUM_REFLECTION_INFO(Type, Info) \
	DEF_ENUM_REFLECTION_PRIMARY(Type, Info)

#define DECL_ENUM_REFLECTION_PRIMARY(Type, Info, Export)									\
template <>																					\
struct Export GX::EnumReflection<Type>														\
{																							\
public:																						\
	static size_t					size	();												\
	static const std::vector<Info>& info	();												\
	static const Info&				info	(size_t index);									\
	static const Info&				info	(Type type);									\
	static const Info*				info	(std::string_view type_name);					\
																							\
private:																					\
	static const std::vector<Info>	s_info;													\
};																							\

#define DEF_ENUM_REFLECTION_PRIMARY(Type, Info)												\
size_t GX::EnumReflection<Type>::size()														\
{																							\
	return s_info.size();																	\
}																							\
																							\
const std::vector<Info>& GX::EnumReflection<Type>::info()									\
{																							\
	return s_info;																			\
}																							\
																							\
const Info& GX::EnumReflection<Type>::info(size_t index)									\
{																							\
	return s_info[index];																	\
}																							\
																							\
const Info& GX::EnumReflection<Type>::info(Type type)										\
{																							\
	return *std::find_if(s_info.begin(), s_info.end(), [type] (auto&& info)					\
	{																						\
		return (info.type == type);															\
	});																						\
}																							\
																							\
const Info* GX::EnumReflection<Type>::info(std::string_view type_name)						\
{																							\
	const auto it = std::find_if(s_info.begin(), s_info.end(), [type_name] (auto&& info)	\
	{																						\
		return (info.type_name == type_name);												\
	});																						\
	return (it != s_info.end() ? &*it : nullptr);											\
}																							\
																							\
const std::vector<Info> GX::EnumReflection<Type>::s_info									\