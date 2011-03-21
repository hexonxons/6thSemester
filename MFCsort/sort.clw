; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CSortDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "sort.h"

ClassCount=3
Class1=CSortApp
Class2=CSortDlg
Class3=CAboutDlg

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_SORT_DIALOG

[CLS:CSortApp]
Type=0
HeaderFile=sort.h
ImplementationFile=sort.cpp
Filter=N

[CLS:CSortDlg]
Type=0
HeaderFile=sortDlg.h
ImplementationFile=sortDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=CSortDlg

[CLS:CAboutDlg]
Type=0
HeaderFile=sortDlg.h
ImplementationFile=sortDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_SORT_DIALOG]
Type=1
Class=CSortDlg
ControlCount=11
Control1=IDOK,button,1342242816
Control2=IDC_LIST1,listbox,1352728835
Control3=IDC_LIST2,listbox,1352728835
Control4=IDC_SORT,button,1342242816
Control5=IDC_FIO,edit,1350631552
Control6=IDC_STATIC,static,1342308864
Control7=IDC_STATIC,static,1342308864
Control8=IDC_STATIC,button,1342177287
Control9=IDC_DATE,SysDateTimePick32,1342242864
Control10=IDC_ADD,button,1342242817
Control11=IDC_CLEAR_LIST,button,1342242816

