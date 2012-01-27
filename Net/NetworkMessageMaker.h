#ifndef _NETWORKMESSAGEMAKER_
#define _NETWORKMESSAGEMAKER_

#include <map>

namespace System
{
	namespace Net
	{
		class NetworkMessage;

		class NetworkMessageMaker
		{
		public:
			NetworkMessageMaker( int type );

			static NetworkMessage* ConstructMessage ( int type );
		protected:

			typedef std::map<int, NetworkMessageMaker*> MessageMap;
			static inline MessageMap& Registry();

			virtual NetworkMessage* MakeMessage ( ) = 0;
		};
	}
}



#define DEFINE_MESSAGE(messageClass,t) \
	class messageClass##_Maker : public System::Net::NetworkMessageMaker	\
	{																		\
		public:																\
			messageClass##_Maker(int type) : NetworkMessageMaker(type) {}   \
		protected:															\
			virtual System::Net::NetworkMessage* MakeMessage ( )			\
				{ return new messageClass(); }								\
	};																		\
	static messageClass##_Maker messageClass##_Maker_creator(t);

#endif // _NETWORKMESSAGEMAKER_
