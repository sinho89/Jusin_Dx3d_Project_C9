#ifndef Engine_Macro_h__
#define Engine_Macro_h__

#ifdef ENGINE_EXPORTS
#define ENGINE_DLL _declspec(dllexport)
#else
#define ENGINE_DLL _declspec(dllimport)
#endif

#define FAILED_CHECK(_hr)	if( ((HRESULT)(_hr)) < 0 )	\
{ MessageBox(NULL, L"Failed", L"System Error",MB_OK); __asm { int 3 }; return E_FAIL;}

#define MSG_BOX(text) MessageBox(NULL, TEXT(text), NULL, MB_OK)

#define BEGIN(NAMESPACE) namespace NAMESPACE {
#define END }
#define USING(NAMESPACE) using namespace NAMESPACE;

#define NO_COPY(CLASSNAME)							\
	private:										\
	CLASSNAME(const CLASSNAME&);					\
	CLASSNAME& operator = (const CLASSNAME&);		

#define DECLARE_SINGLETON(CLASSNAME)				\
	NO_COPY(CLASSNAME)								\
	private:										\
	static CLASSNAME*	m_pInstance;				\
	public:											\
	static CLASSNAME*	GetInstance( void );		\
	static void DestroyInstance( void );			

#define IMPLEMENT_SINGLETON(CLASSNAME)				\
	CLASSNAME*	CLASSNAME::m_pInstance = NULL;		\
	CLASSNAME*	CLASSNAME::GetInstance( void )	{	\
	if(NULL == m_pInstance) {						\
	m_pInstance = new CLASSNAME;					\
	}												\
	return m_pInstance;								\
	}												\
	void CLASSNAME::DestroyInstance( void ) {		\
	if(NULL != m_pInstance)	{						\
	m_pInstance->Release();							\
	m_pInstance = NULL;								\
	}												\
	}

#endif // Macro_h__
