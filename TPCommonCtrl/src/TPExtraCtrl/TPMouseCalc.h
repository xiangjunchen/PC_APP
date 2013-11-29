// SeMouseCalc.h: CTPMouseCalc クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SEMOUSECALC_H__0D451A6E_1194_4CBE_9B2C_AAF17DD74C76__INCLUDED_)
#define AFX_SEMOUSECALC_H__0D451A6E_1194_4CBE_9B2C_AAF17DD74C76__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(push, 3)
#include <vector>
#pragma warning(pop)

using namespace std;

//#define	_INTELLIGENT_SLIDER_DEBUG_

//////////////////////////////////////////////////////////////////////
// MouseDeviceHookerがマウスの移動量を計算する計算クラス
// ※派生して独自のマウス計算をMouseDeviceHookerに行われることが出来ます。
enum
{
	SE_CALC_Y		= 0x01
	,	SE_CALC_X		= 0x02
	,	SE_CALC_BOTH	= SE_CALC_Y | SE_CALC_X
};


class CTPMouseCalc  
{
public:
	CTPMouseCalc();
	virtual ~CTPMouseCalc();

public:
	// Attribute
	enum
	{
		SE_SLT_ORIGINAL		= 0
	,	SE_SLT_INTELLIGENT
	,	SE_SLT_NORMAL	
	};

public:
	// Attribte

	// 計算のベースとなる軸(X,Y,XY)の設定
	virtual void		SetCalcAxis(DWORD dwAxis);
	// 計算のベースとなる軸(X,Y,XY)の取得
	virtual DWORD		GetCalcAxis();

	//////////////////////////////////////////////////////////////////
	// DEBUG用
	//////////////////////////////////////////////////////////////////
	// デバイスから読み出したマウスの移動量累積値のリスト取得
	virtual const vector<SIZE>&		GetMouseAccumulateList();

public:
	// Operation

	// マウスデバイスHook開始
	virtual void		Initialize();
	// マウスデバイスHook終了
	virtual void		Uninitialize();

	// マウスデバイス入力計算(値の累積をする)
#ifdef TP_NS_NET
	virtual BOOL		CalcCursorPos(int xMickey, int yMickey, DWORD dwTime,BOOL bCtrl = FALSE);
#else
	virtual BOOL		CalcCursorPos(int xMickey, int yMickey, DWORD dwTime,BOOL bCtrl);
#endif
	// 累積した値を整数の座標に変換して取得する
	virtual CPoint		GetAccumulatePosition() const;
	// 累積した値を実数の座標に変換して取得する
	virtual SEPOINTREAL	GetAccumulatePostionReal() const;

protected:
	DWORD			m_dwAxis;				// 計算軸
	vector<SIZE>	m_vCumulativeVal;		// 各gear毎にMickey値を累積するリスト
};

inline void CTPMouseCalc::SetCalcAxis(DWORD dwAxis)
{	m_dwAxis = dwAxis; }

inline DWORD CTPMouseCalc::GetCalcAxis()
{	return m_dwAxis; }

inline const vector<SIZE>& CTPMouseCalc::GetMouseAccumulateList()
{	return m_vCumulativeVal;	}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Intelligentスライダ用に派生したマウス計算クラス
//////////////////////////////////////////////////////////////////////

class CTPMouseCalcIntelligent : public CTPMouseCalc
{
public:
	CTPMouseCalcIntelligent();
	virtual ~CTPMouseCalcIntelligent();

public:
	// Attribute
	enum 
	{
		SE_INVALIDE	= -1
	,	SE_SPEED_HIGHTEST
	};
	enum 
	{
		SE_MANUAL
	,	SE_CALCULATE
	};

	/////////////////////////////////////////////////////////////////////////
	// mickey->device point変換tableや各gearのmickey/msecの変換比を計算する為
	// の情報構造体
	/////////////////////////////////////////////////////////////////////////
	struct SETABLECALCINFO
	{
		DWORD	dwTableType;		// table構築タイプ(manual/auto)

		// manual/auto共通
		double	dTotalAdjust;		// mickey->device point変換の最終調整値
		double	dInitialMMRate;		// gearを判定するmickey/msecの基準値
		double	dIntervalMMRate;	// 各gearを判定するmickey/msec基準値を一定間隔で分割する値(比率)
		int		nCountPrevious;		// 直前のgearとして参照する数
		int		nCountAverage;		// 平均化する時に参照する過去のgear数

		// autoの時のみ使用(manual時は無視される)
		double	dMinimumDivision;	// 最小分割値
		double	dInitialM2DPRate;	// mickey->device point変換tableの基準値
		double	dIntervalM2DPRate;	// mickey->device point変換tableを作成する時に基準値を一定間隔で分割する値(比率)

		// 初回gear判定計算用データ
		int		nFGCount;			// 初回のGearを判定するgear数
		double	dFGInitialMMRate;	// 初回のGearを判定するmickey/msec基準値
	};

	/////////////////////////////////////////////////////////////////////////
	// 連続するgear構造体
	/////////////////////////////////////////////////////////////////////////
	struct SChainGear
	{
		int			nGear;			// Gear
		SChainGear*	pNext;			// 次のGearへのポインタ
	};

	/////////////////////////////////////////////////////////////////////////
	// gearの順番情報構造体(レジストリ読み込みに使用)
	/////////////////////////////////////////////////////////////////////////
	struct SEGEARORDERINFO
	{
		int		nOrder;			// 順位
		double	dM2DPRate;		// 変換レート
	};

#ifdef _INTELLIGENT_SLIDER_DEBUG_
	/////////////////////////////////////////////////////////////////////////
	// DEBUG:mouse deviceから取得したデータログ構造体
	/////////////////////////////////////////////////////////////////////////
	struct SEMOUSEDEICELOGINFO
	{
		int		nMickey;			// 生データ
		DWORD	dwTime;				// データ割り込み間隔
		int		nGear;				// 計算結果
	};
#endif

public:
	// Attribte

	// mickey->device point変換用tableの取得
	virtual const vector<double>&	GetMickey2DPTable();
	// mickey->device point変換用tableの設定
	virtual void		SetMickey2DPTable(const vector<double>& vMickey2DPTable);

	// gear全体のmickey->device point変換値を調整する値の設定
	virtual void		SetTotalAdjust(double dRegVal);
	// gear全体のmickey->device point変換値を調整する値の取得
	virtual double		GetTotalAdjust();

	// table info設定
	virtual void		SetTableCalcInfo(const SETABLECALCINFO& seTableInfo);
	// table infoの取得
	virtual SETABLECALCINFO	GetTableCalcInfo();

#ifdef _INTELLIGENT_SLIDER_DEBUG_
	//////////////////////////////////////////////////////////////////
	// DEBUG：デバッグ情報を取得する為のメソッド郡
	//////////////////////////////////////////////////////////////////
	// Dump
	void	DebugDump();
	// デバイスから読み出したデータをどのgearに累積したかgear毎のcounter
	virtual const vector<int>&		GetSelectCounterList();
	// 平均値を計算する為に蓄積したgearのリスト
	virtual const vector<int>&		GetAvgAccumuGearList();
	// 上記のリストに最後に蓄積したインデックス
	virtual int						GetLatestAccumuIndex();
	// マウスデバイスから取得した生データ(mickey)と割り込み間隔(msec)、選択されたgearを記録したリストを取得
	virtual const vector<SEMOUSEDEICELOGINFO>&		GetRawDeviceData();
	// 平均化した後の全てのgearリストを取得
	virtual const vector<int>&		GetAccumulatedAllGear();
#endif

public:
	// Operation

	// マウスデバイスHook開始
	virtual void		Initialize();

	// マウスデバイス入力計算(値の累積をする)
	virtual BOOL		CalcCursorPos(int xMickey, int yMickey, DWORD dwTime,BOOL bCtrl = FALSE);
	// 累積値を実数の座標位置に変換して取得する
	virtual SEPOINTREAL	GetAccumulatePostionReal() const;
	// mickey -> device point変換テーブルを作成する
	static	void		MakeMickey2DPTable(double dMinVal, double dInitialRate, double dIntervalRate, vector<double>& vM2DPTable);

	// Data Operation

	// 決められたレジストリから計算用データを取得する
	virtual BOOL		ReadDataRegistryAll();
	// 決められたレジストリへ計算用データを設定する
	virtual BOOL		WriteDataRegistryAll();
	// 計算用データを定義済みの値で初期化する
	virtual void		ResetDataAll();

protected:
	// Calculate

	// 速度計算(速度変化が滑らかになるよう調整する)
	virtual int			CalcGear(int nMickey, DWORD dwTime, BOOL bUpdateData,BOOL bCtrl = FALSE);
	// 速度計算(mickey/msecでの単純計算)
	virtual int			CalcAdjustGear(int nMickey, DWORD dwTime);
	// Tableの初期化
	virtual void		InitTableInfo();
	// 平滑化するための情報の初期化
	virtual void		InitNormalizeInfo();

	// Data

	// 指定したレジストリ(Key)からM2DP変換テーブルデータを読み込む
	virtual BOOL		ReadM2DPTableFormRegistry(HKEY hRootKey);
	// 指定したレジストリ(Key)から平滑化情報を読み込む
	virtual BOOL		ReadNormalizeInfoFormRegistry(HKEY hRootKey);
	// 指定レジストリ(Key)へM2DP変換テーブルデータを書き込む
	virtual BOOL		WriteM2DPTableFormRegistry(HKEY hRootKey);
	// 指定レジストリ(Key)へ平滑化情報を書き込む
	virtual BOOL		WriteNormalizeInfoFormRegistry(HKEY hRootKey);

protected:
	DWORD			m_dwLastTime;			// 最後に計算した時間(msec)

	vector<double>	m_vMickey2DPTable;		// mickey値(生データ)->device point変換用table

	vector<int>		m_vAccumuGearForAvg;	// 平均値を求める為のgearリスト
	int				m_nLastAccumuIndex;		// 最後に格納したgearリストインデックス

	SETABLECALCINFO	m_sTableInfo;			// table計算する為の情報構造体

	vector<SChainGear>	m_vAccumeGearForPrev;	// 直前のgearとして参照するギヤリスト
	SChainGear*			m_pNextGearForPrev;		// 次のgearへのポインタ

#ifdef _INTELLIGENT_SLIDER_DEBUG_
	vector<int>					m_vAccumuAllGear;		// 選択されたgearのリスト
	vector<int>					m_vSelectGearCounter;	// 選択された各gearのcounter
	vector<SEMOUSEDEICELOGINFO>	m_vRawDeviceData;		// mickey値(生データ)とハードウェア割り込み時間のログ
	BOOL						m_bDbgDump;
	CString						m_strDumpPath;
#endif
};

inline const vector<double>& CTPMouseCalcIntelligent::GetMickey2DPTable()
{	return m_vMickey2DPTable;	}

inline double CTPMouseCalcIntelligent::GetTotalAdjust()
{	return m_sTableInfo.dTotalAdjust; }

inline void CTPMouseCalcIntelligent::SetTotalAdjust(double dVal)
{	m_sTableInfo.dTotalAdjust = dVal; }

inline CTPMouseCalcIntelligent::SETABLECALCINFO CTPMouseCalcIntelligent::GetTableCalcInfo()
{	return m_sTableInfo; }

#ifdef _INTELLIGENT_SLIDER_DEBUG_
//////////////////////////////////////////////////////////////////
// DEBUG用：データ取得関数
//////////////////////////////////////////////////////////////////
inline const vector<int>& CTPMouseCalcIntelligent::GetSelectCounterList()
{	return m_vSelectGearCounter;	}

inline const vector<int>& CTPMouseCalcIntelligent::GetAvgAccumuGearList()
{	return m_vAccumuGearForAvg;	}

inline int CTPMouseCalcIntelligent::GetLatestAccumuIndex()
{	return m_nLastAccumuIndex; }

inline const vector<CTPMouseCalcIntelligent::SEMOUSEDEICELOGINFO>& CTPMouseCalcIntelligent::GetRawDeviceData()
{	return m_vRawDeviceData; }

inline const vector<int>& CTPMouseCalcIntelligent::GetAccumulatedAllGear()
{	return m_vAccumuAllGear;	}
#endif

#endif // !defined(AFX_SEMOUSECALC_H__0D451A6E_1194_4CBE_9B2C_AAF17DD74C76__INCLUDED_)
