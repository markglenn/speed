#ifndef _CONFIGURATION_NODE_H_
#define _CONFIGURATION_NODE_H_

#include "../TinyXPath/xpath_static.h"
#include <string>

namespace System
{
	namespace Engine
	{
		class ConfigurationNode;
		
		typedef Memory::SmartPointer<ConfigurationNode> PConfigurationNode;

		class ConfigurationNode
		{
		public:
			ConfigurationNode ( TiXmlElement* pElement );
			virtual ~ConfigurationNode ( );

			std::string			GetString	( std::string xpath );
			PConfigurationNode	GetNode		( std::string xpath );

		private:
			TiXmlElement* _pElement;
		};
	}
}
#endif // _CONFIGURATION_NODE_H_
