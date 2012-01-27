#include "NetworkMessageMaker.h"

namespace System
{
	namespace Net
	{
		///////////////////////////////////////////////////////////////////////
		NetworkMessageMaker::NetworkMessageMaker( int type )
		///////////////////////////////////////////////////////////////////////
		{
			Registry().insert (std::make_pair(type, this));
		}

		///////////////////////////////////////////////////////////////////////
		NetworkMessage* NetworkMessageMaker::ConstructMessage ( int type )
		///////////////////////////////////////////////////////////////////////
		{
			try 
			{
				NetworkMessageMaker* maker = 
					(*Registry().find (type)).second;

				return maker->MakeMessage();
			}
			catch (void*) {}
         
			return NULL;
		}

		///////////////////////////////////////////////////////////////////////
		NetworkMessageMaker::MessageMap& NetworkMessageMaker::Registry()
		///////////////////////////////////////////////////////////////////////
		{
			static NetworkMessageMaker::MessageMap registry;

			return registry;
		}
	}
}
