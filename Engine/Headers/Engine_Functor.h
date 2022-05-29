#ifndef Engine_Functor_h__
#define Engine_Functor_h__

// Functor
namespace Engine
{
	// for vector or list
	class CRelease_Single
	{
	public:
		explicit CRelease_Single(void) {}
		~CRelease_Single(void) {}
	public:
		template <typename T> void operator () (T& Pointer)
		{
			_ulong			dwRefCnt = 0;
			dwRefCnt = Pointer->Release();
			if(0 == dwRefCnt)
				Pointer = NULL;			
		}
	};

	// for map
	class CRelease_Pair
	{
	public:
		explicit CRelease_Pair(void) {}
		~CRelease_Pair(void) {}
	public:
		template <typename T> void operator () (T& Pair)
		{
			_ulong			dwRefCnt = 0;
			dwRefCnt = Pair.second->Release();
			if(0 == dwRefCnt)
				Pair.second = NULL;			
		}
	};

	// for vector or list
	class CDelete_Single
	{
	public:
		explicit CDelete_Single(void) {}
		~CDelete_Single(void) {}
	public:
		template <typename T> void operator () (T& Pointer)
		{
			if (Pointer != NULL)
			{
				delete Pointer;
				Pointer = NULL;
			}
		}
	};

	// for map
	class CDelete_Pair
	{
	public:
		explicit CDelete_Pair(void) {}
		~CDelete_Pair(void) {}
	public:
		template <typename T> void operator () (T& Pair)
		{
			if (Pair.second != NULL)
			{
				delete Pair.second;
				Pair.second = NULL;
			}
		}
	};



	class CTag_Finder
	{
	public:
		explicit CTag_Finder(const _tchar* pTag) : m_pTargetTag(pTag) {}
		~CTag_Finder(void) {}
	public:
		template <typename T> _bool operator () (const T& Pair)
		{
			if(0 == lstrcmp(m_pTargetTag, Pair.first))
				return true;
			return false;
		}
	private:
		const _tchar*			m_pTargetTag;
	};
}




#endif // Engine_Functor_h__
