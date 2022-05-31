#pragma once
#include"head.h"
#include"VirtualDisk.h"
#include"Factory.h"
#include"Command.h"
/*
#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif 
#endif  // _DEBUG

#define CRTDBG_MAP_ALLOC  
#include <stdlib.h>  
#include <crtdbg.h>  */
//在入口函数中包含 _CrtDumpMemoryLeaks(); 
void test()
{
	//创建虚拟磁盘
	VirtualDisk* Newvirtual = VirtualDisk::GetVirtualDisk();
	//接收命令
	string input;
	//创建工厂
	Factory* fac = new Factory();
	//一条命令
	Command* cmdtype = nullptr;
	//一个调用者
	UseCommand* excucmd = new UseCommand();
	while (1)
	{
		
		cout << Newvirtual->GetLocalDir()->GetPath() +"/"+ Newvirtual->GetLocalDir()->GetName()<< ">";
		getline(cin, input);
		//识别到命令
		cmdtype=fac->GetCommand(Newvirtual->GetLocalDir(),input);
	
		if (cmdtype != nullptr)
		{
			//根据命令确定调用的命令类型
			excucmd->SetCommand(cmdtype);
			
			//执行命令
			if (excucmd->SendTask(Newvirtual)) 
			{
				delete cmdtype;
				cmdtype = nullptr;
				break; 
			}
			else
			{
				delete cmdtype;
				cmdtype = nullptr;
			}
		}
		
	}


	delete Newvirtual;
	Newvirtual = nullptr;

	delete fac;
	fac = nullptr;

	delete excucmd;
	excucmd = nullptr;

	//_CrtDumpMemoryLeaks();
}
int main()
{
	test();
	return 0;
}