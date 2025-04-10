// add
#ifdef ENABLE_VOICE_CHAT
#include "char.h"
#endif


// find
		void		InviteDeny( DWORD dwPID );

// add(not needed on Marty, I think it's part of "new stuff" or something)
#ifdef ENABLE_VOICE_CHAT
		template <class Func> void ForEachOnMapMember(Func& f, int32_t lMapIndex)
		{
			for (auto ch : m_memberOnline)
			{
				if (ch && ch->GetMapIndex() == lMapIndex)
					f(ch);
			}
		}
#endif

