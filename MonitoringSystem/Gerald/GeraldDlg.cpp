// GeraldDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Gerald.h"
#include "GeraldDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGeraldDlg dialog

CGeraldDlg::CGeraldDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGeraldDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGeraldDlg)
	m_Status = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGeraldDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGeraldDlg)
	DDX_Control(pDX, map, m_map);
	DDX_Control(pDX, Name_Box, NameBOX);
	DDX_Control(pDX, UserBOX, m_Username);
	DDX_Control(pDX, IDC_LIST1, m_listname);
	DDX_Text(pDX, Status, m_Status);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CGeraldDlg, CDialog)
	//{{AFX_MSG_MAP(CGeraldDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, Oninput)
	ON_BN_CLICKED(IDC_BUTTON2, Oninputname)
	ON_BN_CLICKED(IDC_BUTTON3, Ondrawing)
	ON_BN_CLICKED(IDC_BUTTON4, OnCreatePlace)
	ON_BN_CLICKED(IDC_BUTTON5, Onpaintting)
	ON_BN_CLICKED(IDC_BUTTON6, OnCheck_button)
	ON_BN_CLICKED(IDC_BUTTON7, OnButton7)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGeraldDlg message handlers

BOOL CGeraldDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CGeraldDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CGeraldDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
          CRect   rect;  
          GetClientRect(&rect);  
          CDC   dcMem;  
          dcMem.CreateCompatibleDC(&dc);  
          CBitmap   bmpBackground;  
          bmpBackground.LoadBitmap(IDB_BITMAP2);  
                  //IDB_BITMAP是你自己的图对应的ID  
          BITMAP   bitmap;  
          bmpBackground.GetBitmap(&bitmap);  
          CBitmap   *pbmpOld=dcMem.SelectObject(&bmpBackground);  
          dc.StretchBlt(0,0,rect.Width(),rect.Height(),&dcMem,0,0,  
         bitmap.bmWidth,bitmap.bmHeight,SRCCOPY);  
 // Do not call CDialog::OnPaint() for painting messages
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CGeraldDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CGeraldDlg::Oninput() 
{
	// TODO: Add your control notification handler code here
	BOOL isOpen = TRUE;//是否打开（否则为保存）
	CString defaultDir=L"D:\\";//默认打开的文件路径
	CString fileName=L"";//默认打开的文件名
	CString filter =L"文件(*.txt; *.csv)|*.txt; *.csv;||";//文件过滤的类型
	CFileDialog openFileDlg(isOpen, defaultDir, fileName, OFN_HIDEREADONLY|OFN_READONLY, filter, NULL);
    INT_PTR result =openFileDlg.DoModal();



		if(result ==IDOK) {
		fileName = openFileDlg.GetPathName();
		CStdioFile f;
		CFileException e;
		if(!f.Open(fileName, CFile::modeRead, &e))
		{
			TRACE(_T("File could not be opened %d\n"), e.m_cause);
		}
		CString str="";
		CString temple;
		f.ReadString(str);
	    int i=0;
		while (str!="" && i++<589){
        //MessageBox(str);//显示一行
	   	m_listname.AddString(str);
		//	m_station=str;
		f.ReadString(str);
		}
		}
		/////////////////////////////////////////////////////////////////////////////////////
		//处理信息
		int pos=0;
	    int nextpos=0;
	    int i=0;                               //选择case量
	    int Line=0;                            //记录当前录入数据的行数,
	    CString s;
	    CString Chosed;
		while (Line<589)
	{
		i=0;                               //第Line行数据循环计数重新开始（初始化）
		pos=0;
		nextpos=0;
	    m_listname.GetText(Line++,s);  //获取第Line行的整行字符数据
	    while(i<6)
		{
	    	nextpos=s.Find(',',pos);       //从当前逗号出现的位置开始寻找下一个‘，’出现的位置存入nextpos
	        Chosed=s.Mid(pos,nextpos-pos); //截取从当前逗号位置pos开始到下个逗号出现位置间的字符串存入Chosed

            pos=nextpos+1;                 //当前逗号位置更新

			switch (i)
			{
			    case 0 :  stationID_.Add(Chosed);    //第一次截取的字符串是stationID
					          break;

				case 1 :  stationName_.Add(Chosed);
					      NameBOX.AddString(Chosed); //同时stationname存入组合框NameBOX总待用户选择
					          break;

				case 2 :  Address_.Add(Chosed);
					          break;

				case 3 :  Total_Docks_.Add(Chosed);
					          break;

				case 4 :  InService_.Add(Chosed);
					          break;

                case 5 :  Status_.Add(Chosed);
					          break;

				default:  ASSERT(i<6); 
					          break;

			}

	    	i++;
            Chosed=s.Mid(s.Find('"'));//截取从第一个引号 " 以后的所有字符串即经纬度
            Location_.Add(Chosed);


		}

	}

        MessageBox("导入成功！",_T("温馨提示"),MB_OK);    //提示成功
	
}

void CGeraldDlg::Oninputname() 
{
	// TODO: Add your control notification handler code here
	BOOL isOpen = TRUE;//是否打开（否则为保存）
	CString defaultDir=L"D:\\";//默认打开的文件路径
	CString fileName=L"";//默认打开的文件名
	CString filter =L"文件(*.txt; *.csv)|*.txt; *.csv;||";//文件过滤的类型
	CFileDialog openFileDlg(isOpen, defaultDir, fileName, OFN_HIDEREADONLY|OFN_READONLY, filter, NULL);
    INT_PTR result =openFileDlg.DoModal();



		if(result ==IDOK) {
		fileName = openFileDlg.GetPathName();
		CStdioFile f;
		CFileException e;
		if(!f.Open(fileName, CFile::modeRead, &e))
		{
			TRACE(_T("File could not be opened %d\n"), e.m_cause);
		}
		CString str="";
		CString temple;
		f.ReadString(str);
	    int i=0;
		while (str!="" && i++<70){
        //MessageBox(str);//显示一行
	   	m_Username.AddString(str);
		//	m_station=str;
		f.ReadString(str);
		}
		}
}

void CGeraldDlg::Ondrawing() 
{
	// TODO: Add your control notification handler code here
	GetDlgItem(map)->GetClientRect(rect); //获取pitcture控件的大小
	int item_y=rect.Height();              
	int item_x=rect.Width();                //item_x=359,item_y=779
                                  	
	CDC *pDC;                            // 获取picture控件DC
	pDC = m_map.GetDC();                 // m_map是picture控件对应的变量
	                                     //将兼容dc上的绘图，拷贝到目标控件上
	CPen pen;
	pen.CreatePen(PS_DOT,5,RGB(0,0,255));

	pDC->SelectObject(&pen);

	int Width;
	int Height;
	double x=0.0; double y=0.0;
	int i=0;
	while(i<589)
	{
		y=(atof(Gettitude(&Location_,i,"Latitude"))-42.1)*1947.50;//扩大779/（42.1-41.7)=1947.5倍，将字符串经度转为double型
		x=(atof(Gettitude(&Location_,i,"Longitude"))+87.8)*1432.56;//扩大359/(87.8-87.5494)≈1432.56185倍，将字符串纬度转为double型

    	Width=(int)x;
    	Height=-(int)y;

		pDC->MoveTo(Width,Height);                //划线开始(Width,Height)
		pDC->LineTo(Width,Height);

		i++;   
	}
	ReleaseDC(pDC);
	
}
//自定义函数//分解Location_
CString CGeraldDlg::Gettitude(CStringArray* S,int num,CString name)  
{                                                  //三个参数：Location_数组地址，字符串位置，字符串名字“Latitude”还是“Longitude”
	CString str;                                   // Gettitude(&Location_,0,"Latitude");
	CString str1;
	CString str2;
	str=S->GetAt(num);
		str=str.Mid(2,str.GetLength()-4);

    str1=str.Mid(0,str.Find(',',0));       //将location内的坐标分解成经纬度存入字符串数组中
	str2=str.Mid(str.Find(',',0)+1);       //Loation坐标分解完成

	if(name=="Latitude")                   //如果传入的"Latitude"字符串则返回纬度值
			return str1;
	else if(name=="Longitude")             //"Longitude"则返回经度值
			return str2;
	else
		return str="输入的第三个参数“name”有错!";//如果参数格式错误，则返回反馈
}

void CGeraldDlg::OnCreatePlace() 
{
	// TODO: Add your control notification handler code here
	int a=m_Username.GetCurSel();
	CString b;
	//更新加载位图

	CBitmap bitmap;  // CBitmap对象，用于加载位图   
	HBITMAP hBmp;    // 保存CBitmap加载的位图的句柄   

	bitmap.LoadBitmap(IDB_BITMAP1);  // 将位图IDB_BITMAP1加载到bitmap   
	hBmp = (HBITMAP)bitmap.GetSafeHandle();  // 获取bitmap加载位图的句柄   
	m_map.SetBitmap(hBmp);    // 设置图片控件m_jzmPicture的位图图片为IDB_BITMAP1    //复原位图

	//描点
    Width1=0;
	Width2=0;
	double x;        
	Height1=0;
	Height2=0;
	double y;

	CString str;
	int x1;
	int x2;
	do{
		srand((unsigned)time(NULL));
		x1=rand()%588;
		x2=rand()%588;
		if(x2==x1){x2=rand()%588;}

		y=(atof(Gettitude(&Location_,x1,"Latitude"))-42.1)*1947.50;//扩大779/（42.1-41.7)=1947.5倍，将字符串经度转为double型
		x=(atof(Gettitude(&Location_,x1,"Longitude"))+87.8)*1432.56;//扩大359/(87.8-87.5494)≈1432.56185倍，将字符串纬度转为double型

    	Width1=(int)x;
    	Height1=-(int)y;

		y=(atof(Gettitude(&Location_,x2,"Latitude"))-42.1)*1947.50;//扩大779/（42.1-41.7)=1947.5倍，将字符串经度转为double型
		x=(atof(Gettitude(&Location_,x2,"Longitude"))+87.8)*1432.56;//扩大359/(87.8-87.5494)≈1432.56185倍，将字符串纬度转为double型

    	Width2=(int)x;
    	Height2=-(int)y;

	}while(!((Width1-Width2>=50||Width2-Width1>=20)&&(Height1-Height2>=100||Height2-Height1>=20)));


	CDC *pDC;                            // 获取picture控件DC
	pDC = m_map.GetDC();                 // m_map是picture控件对应的变量                                   
	pDC->BitBlt(0,0,rect.Width(),rect.Height(),&m_MemDC,0,0,SRCCOPY); 

	GetDlgItem(map)->GetClientRect(rect);
    CPen pen;                            //将兼容dc上的绘图，拷贝到目标控件上
	pen.CreatePen(PS_DOT,8,RGB(0,0,255));
	pDC->SelectObject(&pen);

	GetDlgItem(startingplace)->SetWindowText(stationName_[x1]);
	GetDlgItem(destination)->SetWindowText(stationName_[x2]);

	pDC->MoveTo(Width1,Height1);                //划线开始(Width,Height)
	pDC->LineTo(Width1,Height1);
	pDC->SetBkMode(TRANSPARENT);


	int templeWidth1=Width1;
	int templeWidth2=Width2;

	while(stationName_[x1].GetLength()*7+templeWidth1+5>359)
	{
			templeWidth1=templeWidth1-10;                     //调整Text文本，尽量不让他画出控件
	}

	pDC->DrawText(stationName_[x1],CRect(templeWidth1+15,Height1+5,templeWidth1+20+stationName_[x1].GetLength()*7,Height1+20),DT_SINGLELINE|DT_LEFT|DT_VCENTER); //显示文本

	pDC->MoveTo(Width2,Height2);                 //划线开始(Width,Height)
	pDC->LineTo(Width2,Height2);
	pDC->SetBkMode(TRANSPARENT);

  	while(stationName_[x1].GetLength()*7+templeWidth2+5>359)
	{
			templeWidth2=templeWidth2-10;                     //调整Text文本，尽量不让他画出控件
	}

	pDC->DrawText(stationName_[x2],CRect(templeWidth2+15,Height2+5,templeWidth2+20+stationName_[x1].GetLength()*7,Height2+20),DT_SINGLELINE|DT_LEFT|DT_VCENTER); //显示文本


    CPen pen1;                            //将兼容dc上的绘图，拷贝到目标控件上
	pen1.CreatePen(1,4,RGB(0, 215, 64));
	pDC->SelectObject(&pen1);

	ReleaseDC(pDC);

	
}
UINT __cdecl track_user(LPVOID lpParam)
{

	CGeraldDlg* This=(CGeraldDlg*)lpParam;//制转换

		// TODO: Add your control notification handler code here
	CString str;
	long t1=GetTickCount();
	CDC *pDC;                            // 获取picture控件DC
	pDC =This->m_map.GetDC();                 // m_map是picture控件对应的变量                                   

    CPen pen1;                            //将兼容dc上的绘图，拷贝到目标控件上
	pen1.CreatePen(1,4,RGB(0, 215, 0));
	pDC->SelectObject(&pen1);


	int lenth=0;
	float k=(This->Height2-This->Height1)/float(This->Width2-This->Width1);             //储存斜率
	float valueK; if(k<0){valueK=-k;}else valueK=k;
	int randx,randy,templex,templey;
	int n=5;
	int y1y2Dvalue=This->Height2-This->Height1;
	    if(y1y2Dvalue<0){y1y2Dvalue=-y1y2Dvalue;}
	int i=0;
   	pDC->MoveTo(This->Width1,This->Height1); //划线开始(Width,Height)
	if(This->Width1>This->Width2)
	{
		while((This->Width1-This->Width2>=n)&&(y1y2Dvalue>=valueK*n))
		{
			templex=This->Width1-n;templey=This->Height1-int(n*k);
			srand((unsigned)time(NULL));
			randx=This->Width1-rand()%n;
			srand((unsigned)time(NULL));
			randy=This->Height1-int(k/valueK)*rand()%int(n*valueK);


			pDC->LineTo(randx,randy);
			Sleep(200);
			pDC->MoveTo(randx,randy);

			This->Width1=templex;
			This->Height1=templey;

			lenth+=(This->Width1-randx)+(int)(k/valueK)*(This->Height1-randy);
			str.Format(_T("%d"),-lenth);

			This->GetDlgItem(lennum)->SetWindowText(str); //声明一个字符串储存数字data_num并传出


		}
		randx=(This->Width2-This->Width1)/3+This->Width1;
		if(This->Height1>This->Height2) randy=(This->Height1-This->Height2)/3+This->Height2;
		randy=(This->Height2-This->Height1)/3+This->Height1;
		pDC->LineTo(randx,randy);

		lenth+=(randx-This->Width1)+(int)(k/valueK)*(This->Height1-randy);
		str.Format(_T("%d"),-lenth);

		This->GetDlgItem(lennum)->SetWindowText(str);
		Sleep(200);
		pDC->MoveTo(randx,randy);


		randx=(This->Width2-This->Width1)*2/3+This->Width1;
		if(This->Height1>This->Height2) randy=(This->Height1-This->Height2)*2/3+This->Height2;
		randy=(This->Height2-This->Height1)*2/3+This->Height1;
		pDC->LineTo(randx,randy);

		lenth+=(randx-This->Width1)+(int)(k/valueK)*(randy-This->Height1);
		str.Format(_T("%d"),-lenth);

		This->GetDlgItem(lennum)->SetWindowText(str);
		Sleep(200);
		pDC->MoveTo(randx,randy);
	}
	else if((This->Width1<This->Width2))
	{
		pDC->MoveTo(This->Width1,This->Height1);                //划线开始(Width,Height)
		while((This->Width2-This->Width1>=n)&&(y1y2Dvalue>=valueK*n))
		{
			templex=This->Width1+n;templey=This->Height1+int(n*k);
			srand((unsigned)time(NULL));
			randx=rand()%n+This->Width1;
			srand((unsigned)time(NULL));
			randy=int(k/valueK)*rand()%int(n*valueK)+This->Height1;


			pDC->LineTo(randx,randy);
			Sleep(200);
			pDC->MoveTo(randx,randy);

			This->Width1=templex;
			This->Height1=templey;

			lenth+=(randx-This->Width1)+(int)(k/valueK)*(randy-This->Height1);
			str.Format(_T("%d"),-lenth);

			This->GetDlgItem(lennum)->SetWindowText(str);


		}
		randx=(This->Width2-This->Width1)/3+This->Width1;
		if(This->Height1>This->Height2) randy=(This->Height1-This->Height2)/3+This->Height2;
		randy=(This->Height2-This->Height1)/3+This->Height1;
		pDC->LineTo(randx,randy);

		lenth+=(randx-This->Width1)+(int)(k/valueK)*(This->Height1-randy);
		str.Format(_T("%d"),-lenth);

		This->GetDlgItem(lennum)->SetWindowText(str);
		Sleep(200);
		pDC->MoveTo(randx,randy);

		randx=(This->Width2-This->Width1)*2/3+This->Width1;
		if(This->Height1>This->Height2) randy=(This->Height1-This->Height2)*2/3+This->Height2;
		randy=(This->Height2-This->Height1)*2/3+This->Height1;
		pDC->LineTo(randx,randy);

		lenth+=(randx-This->Width1)+(int)(k/valueK)*(This->Height1-randy);
		str.Format(_T("%d"),-lenth);

        This->GetDlgItem(lennum)->SetWindowText(str);

		Sleep(200);
		pDC->MoveTo(randx,randy);

}


	pDC->LineTo(This->Width2,This->Height2);

	long t2=GetTickCount();

	int h;
	int min;
	min=int(t2-t1)%60;
	h=int(t2-t1-min)/60;

	str.Format("到达的地,用时: %d分钟 %d秒",h,min);

    This->MessageBox(str);

	This->ReleaseDC(pDC);

	return 0;
}

void CGeraldDlg::Onpaintting() 
{
	// TODO: Add your control notification handler code here
	pthread=AfxBeginThread(track_user, this);
	Condition_num=1;
}

void CGeraldDlg::OnCheck_button() 
{
	// TODO: Add your control notification handler code here
	CString str;                                          //中间临时变量
	int strID=NameBOX.GetCurSel();                        //获取组合框选择的选择项ID
	NameBOX.GetLBText(strID,str);                        //将这个选择项ID对应的被选择的对应站点名称保存在str中
	int num=NameBOX.FindStringExact(0,str);               //匹配确定str储存的站点名称在数据中对应的位号num
	//调用数据数组，传入num得到对应站点的各个信息
     
	GetDlgItem(Address)->SetFont(&font); 
	GetDlgItem(Address)->SetWindowText(Address_[num]);        //传入站点地址

	GetDlgItem(stationID)->SetFont(&font);
	GetDlgItem(stationID)->SetWindowText(stationID_[num]);    //传入站点ID

	GetDlgItem(Total_Docks)->SetFont(&font);
	GetDlgItem(Total_Docks)->SetWindowText(Total_Docks_[num]);//传入站点单车总数

	GetDlgItem(InService)->SetFont(&font);
	GetDlgItem(InService)->SetWindowText(InService_[num]);    //传入站点可用单车数
      
	GetDlgItem(Status)->SetFont(&font);                                                                   //传入站点服务状态
	GetDlgItem(Status)->SetWindowText(Status_[num]);

    GetDlgItem(Location)->SetFont(&font);
	GetDlgItem(Location)->SetWindowText(Location_[num]);      //传入站点定位
	//更新加载地图
		CBitmap bitmap;  // CBitmap对象，用于加载位图   
	HBITMAP hBmp;    // 保存CBitmap加载的位图的句柄   

	bitmap.LoadBitmap(IDB_BITMAP1);  // 将位图IDB_BITMAP1加载到bitmap   
	hBmp = (HBITMAP)bitmap.GetSafeHandle();  // 获取bitmap加载位图的句柄   
	m_map.SetBitmap(hBmp);    // 设置图片控件m_jzmPicture的位图图片为IDB_BITMAP1    //复原位图


	//描点
    int Width;double x;        
	int Height;double y;

	CDC *pDC;                            // 获取picture控件DC
	pDC = m_map.GetDC();                 // m_map是picture控件对应的变量                                   
	pDC->BitBlt(0,0,rect.Width(),rect.Height(),&m_MemDC,0,0,SRCCOPY); 

	GetDlgItem(map)->GetClientRect(rect);
    CPen pen;                            //将兼容dc上的绘图，拷贝到目标控件上
	pen.CreatePen(PS_DOT,8,RGB(255,0,0));
	pDC->SelectObject(&pen);

		y=(atof(Gettitude(&Location_,num,"Latitude"))-42.1)*1947.50;//扩大779/（42.1-41.7)=1947.5倍，将字符串经度转为double型
		x=(atof(Gettitude(&Location_,num,"Longitude"))+87.8)*1432.56;//扩大359/(87.8-87.5494)≈1432.56185倍，将字符串纬度转为double型

    	Width=(int)x;
    	Height=-(int)y;

		pDC->MoveTo(Width,Height);                //划线开始(Width,Height)
		pDC->LineTo(Width,Height);
		pDC->SetBkMode(TRANSPARENT);

		while(str.GetLength()*7+Width+5>359)
		{
			Width=Width-10;
		}
		pDC->DrawText(str,CRect(Width+15,Height+5,Width+20+str.GetLength()*7,Height+20),DT_SINGLELINE|DT_LEFT|DT_VCENTER); //显示文本
		ReleaseDC(pDC);
	
}

void CGeraldDlg::OnButton7() 
{
	// TODO: Add your control notification handler code here
	MessageBox("先导入文件和导入用户，可查看站点分布，再进行随机模拟和查询站点信息","温馨提示");
}
