#include "Configuration.h"
#include "../Types/Integer.h"

using System::Types::Integer;

namespace System
{
	namespace Engine
	{
		//////////////////////////////////////////////////////////////////
		ConfigurationNode::ConfigurationNode ( TiXmlElement* pElement ) 
		//////////////////////////////////////////////////////////////////
		{
			_pElement = pElement;
		}

		//////////////////////////////////////////////////////////////////
		ConfigurationNode::~ConfigurationNode ( )
		//////////////////////////////////////////////////////////////////
		{

		}

		//////////////////////////////////////////////////////////////////
		std::string ConfigurationNode::GetString ( std::string xpath )
		//////////////////////////////////////////////////////////////////
		{
			return std::string (TinyXPath::S_xpath_string ( 
				_pElement, xpath.c_str() ).c_str());
		}

		//////////////////////////////////////////////////////////////////
		PConfigurationNode ConfigurationNode::GetNode ( std::string xpath )
		//////////////////////////////////////////////////////////////////
		{
			TiXmlNode* pNode = TinyXPath::XNp_xpath_node ( 
				_pElement, xpath.c_str() );

			if ( !pNode )
				return NULL;

			return new ConfigurationNode ( pNode->ToElement () );

		}

	}
}
