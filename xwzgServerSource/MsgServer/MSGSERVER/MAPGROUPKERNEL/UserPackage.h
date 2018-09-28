// UserPackage.h: interface for the CUserPackage class.
// �����࣬���ڹ���������ϱ�����Ʒ
// zlong 2004-02-05
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_USERPACKAGE_H__726A0F38_4530_4036_98E0_F00DD64DE3D7__INCLUDED_)
#define AFX_USERPACKAGE_H__726A0F38_4530_4036_98E0_F00DD64DE3D7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../share/define.h"
#include "GameObjSet.h"
#include "Myheap.h"
//#include "User.h"
#include "Item.h"


////////////////////////////////////////////////////////////////
// ������������
const int   _MAX_GRID_EACHROW				= 6;				// ����ÿ����󾧸���08.9.26�������
const int	_MAX_EUDEMONSIZE				= 5;				// �����������
const int	_MAX_EUDEMONEGGSIZE				= 6;				// ���޵��������
const int	_MAX_GHOSTGEMSIZE				= 40;				// ħ�걦ʯ�������
/*-------06.2.16-------�����޸�--------------------------------------------------------------*/
const int	_MAX_USERITEMSIZE				= 40;				// ��ͨ��Ʒ�����������,�û�����4�������ı���

const int	_MAX_ALLUSERITEMSIZE			= _MAX_EUDEMONSIZE + _MAX_EUDEMONEGGSIZE + _MAX_GHOSTGEMSIZE +
 _MAX_USERITEMSIZE*4 + (ITEMPOSITION_EQUIPEND - ITEMPOSITION_EQUIPBEGIN);		// + װ������

const int	_MAX_PACK_TYPE	= 5;//�ĸ�����

const int	_MAX_STORAGE_TYPE	= 36;//�ֿ����36��

///*const */int	_MAX_PACK_SIZES[_MAX_PACK_TYPE]; /*= {
//	_MAX_USERITEMSIZE,				// ����1����
//	_MAX_USERITEMSIZE,				//����2
//	_MAX_USERITEMSIZE,				//����3
//	_MAX_USERITEMSIZE,				//����4
//	_MAX_USERITEMSIZE,				//����5
//};*/
/*-------------------------------------------------------------------------------------------------------------------*/

enum DROP_MAP_TYPE {
	_MAP_NONE	= 0,
	_MAP_PKFIELD,
	_MAP_SYN,
	_MAP_PRISON,
};

typedef	IGameObjSet<CItem>	IItemSet;
typedef	CGameObjSet<CItem>	CItemSet;

class CUser;
class CMapGroup;
//////////////////////////////////////////////////////////////////////
class IUserPackage
{
public:
	static IUserPackage* CreateNew();
	virtual void	Release()													PURE_VIRTUAL_FUNCTION

public:
	virtual bool	Create		(CUser* pUser, IDatabase* pDb,bool bLogin)		PURE_VIRTUAL_FUNCTION_0
	virtual bool	AddItem		(CItem* pItem, bool bUpdate = false)			PURE_VIRTUAL_FUNCTION_0
	virtual bool    AddItemByChangeMap(CItem* pItem)							PURE_VIRTUAL_FUNCTION_0
	virtual bool	AddItemToNormalBag( CItem* pItem,bool bUpdate = false)		PURE_VIRTUAL_FUNCTION_0
	virtual bool	AddItemToPosition(CUser* pUser, CItem* pItem, int nPosition, int nIndex, bool bUpdate = false)		PURE_VIRTUAL_FUNCTION_0
	virtual bool	OrderPackage(int package = 47)								PURE_VIRTUAL_FUNCTION_0
	virtual bool	HasLockedItem(int package)									PURE_VIRTUAL_FUNCTION_0
	//06.3.13�������
	virtual bool	DelItem		(OBJID idItem)									PURE_VIRTUAL_FUNCTION_0
	virtual CItem*	PopItem		(OBJID idItem)									PURE_VIRTUAL_FUNCTION_0
	virtual int		MultiGetItem(OBJID idTypeFirst, OBJID idTypeLast, int nNum, OBJID* pId, int nSize)		PURE_VIRTUAL_FUNCTION_0
	virtual bool	MultiCheckItem(OBJID idTypeFirst, OBJID idTypeLast, int nNum)							PURE_VIRTUAL_FUNCTION_0
	virtual bool	PureMagicStoneBox() PURE_VIRTUAL_FUNCTION_0//���ħ����
	virtual int		RandDropItem(int nMapType, int nChance)						PURE_VIRTUAL_FUNCTION_0
	virtual int		RandDropItem(int nDropNum, bool isPlayerKill=false)									PURE_VIRTUAL_FUNCTION_0
	virtual OBJID	RandGetItem()												PURE_VIRTUAL_FUNCTION_0

	virtual CItem*	GetItemFromSpecificPack( OBJID idItem,int nPackArrayIndex ) PURE_VIRTUAL_FUNCTION_0
	virtual CItem*	GetItem			(OBJID idItem)								PURE_VIRTUAL_FUNCTION_0
	virtual CItem*	GetItemByType	(OBJID idType)								PURE_VIRTUAL_FUNCTION_0
    virtual  int  	GetItemNumByType(OBJID idItemType)                          PURE_VIRTUAL_FUNCTION_0
	virtual	CItem*	GetItemByPackageIndex(int nPosition, int nIndex)			PURE_VIRTUAL_FUNCTION_0
	virtual	CItem*	GetPetEatItem()												PURE_VIRTUAL_FUNCTION_0
	virtual	CItem*	GetPetLifeSpanItem()										PURE_VIRTUAL_FUNCTION_0
	virtual	CItem*	GetPetTrainItem()											PURE_VIRTUAL_FUNCTION_0
	//20061010����.������ѯby TypeID.��ָ���������idItem[]
	//�����ò�������ʵ�ʵ�size.�ɺ�������amount
	//����-1ʱ��ʾidItem[]��ʵ��size����maxSize.��������������maxSize֮�ڵ�����
	virtual int		GetItemByTypeMulti	(OBJID idType,int &maxSize,OBJID *idItem,bool isKidnap = true)				PURE_VIRTUAL_FUNCTION_0
	virtual CItem*  GetFirstCarryItem()											PURE_VIRTUAL_FUNCTION_0
	virtual CItem*	GetTaskItem		(LPCTSTR szItemName)						PURE_VIRTUAL_FUNCTION_0
	virtual CItem*	GetTaskItemByType	(OBJID idType, int nAmount)				PURE_VIRTUAL_FUNCTION_0
	

	virtual void	ClearAll		()											PURE_VIRTUAL_FUNCTION
	virtual void	SaveAllInfo		()											PURE_VIRTUAL_FUNCTION
	virtual int		SendAllItemInfo	()											PURE_VIRTUAL_FUNCTION_0
	virtual void	SendAllObjInfo	(PROCESS_ID idProcess)		PURE_VIRTUAL_FUNCTION

	virtual int		IsEmpty			(int nPosition, int nIndex)					PURE_VIRTUAL_FUNCTION_0
public:
	virtual CItem*	FindSpaceTransSpell	()										PURE_VIRTUAL_FUNCTION_0
	virtual CItem*	FindCombineItem		(OBJID idType)							PURE_VIRTUAL_FUNCTION_0
	virtual bool	IsPackFull			(int nNeed = 0,OBJID idItemType = 0,int nPosition = 47,bool bIncludeOrder=true)							PURE_VIRTUAL_FUNCTION_0
	virtual int		GetAmount			()										PURE_VIRTUAL_FUNCTION_0
	

	virtual bool	IsPackSpare			(int nNum, OBJID idItemType, DWORD dwData)			PURE_VIRTUAL_FUNCTION_0
	virtual int		GetAvailablePosition(OBJID idType, bool bOrdered=true)										PURE_VIRTUAL_FUNCTION_0
	virtual int		GetPositionByItemType(OBJID idType)										PURE_VIRTUAL_FUNCTION_0
	//20070226����:������װ���;���ʧһ���ٷֱ�.����ʵ����ʧ�;�װ���ĸ���
	virtual int		PackageDurationLose(int durationLosePercent)				PURE_VIRTUAL_FUNCTION_0

	//  [2007-9-15] ���	�����������
	virtual void	CheckItemLifeTime(BOOL bUpdateToClient, BOOL bSaveToDatabase) PURE_VIRTUAL_FUNCTION
	// �����ص���Ʒ [�½���10/13/2007]
	virtual void    DropItemWhenIsDead  () PURE_VIRTUAL_FUNCTION
	virtual int		GetFreeSpaceCount(int nPosition, bool bIncludeOrder=false)	PURE_VIRTUAL_FUNCTION_0
	virtual void	SetIndexOrderAmount(int nPosition, int nAmount)	PURE_VIRTUAL_FUNCTION
	virtual int		GetIndexOrderAmount(int nPosition)				PURE_VIRTUAL_FUNCTION_0
};


//////////////////////////////////////////////////////////////////////
class CUserPackage  :	public IUserPackage
{
public:
	CUserPackage();
	virtual ~CUserPackage();
public:
	int		_MAX_PACK_SIZES[_MAX_PACK_TYPE];
	static IUserPackage* CreateNew()		{ return new CUserPackage; }
	void	Release()						{ delete this; }
	void	ReSetPackSize();//�������08.9.26���ñ����������					

public: // IUserPackage
	bool	Create		(CUser* pUser, IDatabase* pDb,bool bLogin);
	bool	AddItem		(CItem* pItem, bool bUpdate = false);
	bool    AddItemByChangeMap(CItem* pItem);
	bool	AddItemToPosition(CUser* pUser, CItem* pItem, int nPosition, int nIndex, bool bUpdate = false);
	bool	DelItem		(OBJID idItem);
	CItem*	PopItem		(OBJID idItem);
	int		MultiGetItem(OBJID idTypeFirst, OBJID idTypeLast, int nNum, OBJID* pId, int nSize);
	bool	MultiCheckItem(OBJID idTypeFirst, OBJID idTypeLast, int nNum);

	int		RandDropItem(int nDropNum,bool isPlayerKill = false);
	int		RanDropEquip(int nDropNum,bool isPlayerKill = false);
	int		RandDropItem(int nMapType, int nChance);
	bool	OrderPackage(int package = 47);
	bool	HasLockedItem(int package);

	OBJID	RandGetItem();
	bool	PureMagicStoneBox();//���ħ����

	CItem*	GetItem				(OBJID idItem);
	CItem*	GetItemByType		(OBJID idType);
	int     GetItemNumByType    (OBJID idType);
	CItem*	GetItemByPackageIndex(int nPosition, int nIndex);
	CItem*	GetPetEatItem();
	CItem*	GetPetLifeSpanItem();
	CItem*  GetPetTrainItem();
	//20061010����.������ѯby TypeID.��ָ���������idItem[]
	//�����ò�������ʵ�ʵ�size.�ɺ�������amount
	//����-1ʱ��ʾidItem[]��ʵ��size����maxSize.��������������maxSize֮�ڵ�����
	int	GetItemByTypeMulti		(OBJID idType,int &maxSize,OBJID *idItem, bool isKidnap = true);
	CItem*	GetTaskItem			(LPCTSTR szItemName);
	CItem*	GetTaskItemByType	(OBJID idType, int nAmount);
	// �������������ص���Ʒ [�½���10/13/2007]
	void    DropItemWhenIsDead  ();

	void	ClearAll		();
	void	SaveAllInfo		();
	int		SendAllItemInfo	();
	void	SendAllObjInfo	(PROCESS_ID idProcess);	// ��ʱ��ô����CMapGroup
	bool	AddItemToNormalBag( CItem* pItem,bool bUpdate = false );//06.3.13�������
	int		IsEmpty			(int nPosition, int nIndex);
	int		GetFreeSpaceCount(int nPosition, bool bIncludeOrder=false);

public:
	//06.10.28�������------------------------begin--------------------------------------------------
	//�޸ı�������ʱ���
	CItem* GetItemFromSpecificPack( OBJID idItem,int nPackArrayIndex );
	//------------------------------------------end----------------------------------------------------
	void	SetOwnerUser( CUser* pUser ) { m_pOwner = pUser; }	//�������

	IItemSet*	QueryItemSet( int index )	{ if( index>=0 && index<= _MAX_PACK_TYPE) return m_setItem[ index ];else return NULL;}
	CItem*	FindCombineItem		(OBJID idType);

	bool	IsPackFull			(int nNeed = 1,OBJID idItemType = 0,int nPosition = 47,bool bIncludeOrder=true);
	int		GetAmount			();


	bool	IsPackSpare			(int nNum, OBJID idItemType, DWORD dwData);
	int		GetAvailablePosition(OBJID idType, bool bOrdered=true);
	int		GetPositionByItemType(OBJID idItemType);
protected:
	bool	IsPackSpare	(int nNum, int nPosition);
	int		GetPackType	(int nPosition)		{ if (nPosition < ITEMPOSITION_PACK_BEGIN || nPosition >= ITEMPOSITION_PACK_END) 
													return -1;
												return nPosition - ITEMPOSITION_PACK_BEGIN; }
//06.9.20�������---------------------begin-------------------------------------------------------------------------------
public:
//	__inline bool		SetIndexState( UCHAR ucPosition,UCHAR ucIndex,bool bStatus );	
//	__inline bool		GetIndexState( UCHAR usPosition,UCHAR ucIndex );
	
	int GetIndexState( UCHAR usPosition,UCHAR ucIndex );
	bool SetIndexState( UCHAR ucPosition,UCHAR ucIndex,int nStatus );
	int	 GetFreeIndex( UCHAR ucPosition );
																			
	void SetIndexOrderAmount(int nPosition, int nAmount);
	int	 GetIndexOrderAmount(int nPosition);
//---------------------------------------end----------------------------------------------------------------------------------
protected:
	int		m_bIndexHasItem[ _MAX_PACK_TYPE ][ _MAX_USERITEMSIZE ];//06.9.20�������,����λ���õ������ݽṹ,Ϊ������һ���µ���Ʒʱȷ������pakcage_index;
	IItemSet*	m_setItem[_MAX_PACK_TYPE];
	CUser*		m_pOwner;
	short		m_nIndexOrdered[4];//goto Ԫ��������Ʒʱ�˺ŷ���������֮ǰҪԤ��λ�ã�������ֱ����ռ䲻��ȴ����Ԫ�������
public:
	void CheckItemLifeTime(BOOL bUpdateToClient, BOOL bSaveToDatabase);

	MYHEAP_DECLARATION(s_heap)
};

#endif // !defined(AFX_USERPACKAGE_H__726A0F38_4530_4036_98E0_F00DD64DE3D7__INCLUDED_)
