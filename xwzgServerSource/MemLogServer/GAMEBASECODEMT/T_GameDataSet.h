// ���ݿ��¼�ļ���ͨ������CGameData����
// �ɽ��ޣ�2004.1.19
//////////////////////////////////////////////////////////////////////

#ifndef	T_GAMEDATASET_H
#define T_GAMEDATASET_H

#include "Define.h"
#include "basefunc.h"
#include "logfile.h"
#include "I_mydb.h"
#include "Myheap.h"
#include "T_SingleMap.h"

template	< class T >
class CGameDataSet : public CGameObjSet<T>
{
public:
	static CGameDataSet<T>*	CreateNew(bool bOwner=true) { return new CGameDataSet<T>(bOwner); }
	bool		Create(LPCTSTR szSQL, IDatabase* pDb);
	void		SaveInfo();	
private:
	CGameDataSet(bool bOwner) : CGameObjSet<T>(bOwner)		{}
};

//////////////////////////////////////////////////////////////////////
// �˺�����Ҫ���ݿ�֧��
template<class T>
bool CGameDataSet<T>::Create(LPCTSTR szSQL, IDatabase* pDb)
{
	IRecordset* pRes = pDb->CreateNewRecordset(szSQL, false);
	if(!pRes)
		return false;

	for(int i = 0; i < pRes->RecordCount(); i++)
	{
		T* pData = T::CreateNew();
		if(!pData)
		{
			pRes->Release();
			return false;
		}

		if(!pData->Create(pRes))
		{
			pData->ReleaseByOwner();
			pRes->Release();
			return false;
		}

		this->AddObj(pData);

		pRes->MoveNext();
	}

	pRes->Release();
	return true;
}
//////////////////////////////////////////////////////////////////////
template<class T>
void CGameDataSet<T>::SaveInfo()
{
	CGameObjSet<T>::Iterator pData = CGameObjSet<T>::NewEnum();
	while(pData.Next())
		pData->Update();
}
#endif // T_GAMEDATASET_H
