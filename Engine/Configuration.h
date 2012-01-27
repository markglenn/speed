#ifndef _CONFIGURATION_H_
#define _CONFIGURATION_H_

#include "../TinyXPath/xpath_static.h"
#include "../Memory/SmartPointer.h"
#include "ConfigurationNode.h"

#include <string>

namespace System
{
	namespace Engine
	{
		class Configuration
		{
		public:
			Configuration ( );
			virtual ~Configuration ( );

			bool LoadConfiguration ( std::string filename );
			
			std::string			GetString	( std::string xpath );
			PConfigurationNode	GetNode		( std::string xpath );

		private:
			TiXmlDocument _xmlDocument;
			PConfigurationNode _rootNode;
		};
	}
}
#endif // _CONFIGURATION_H_
