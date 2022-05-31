#pragma once
#include"head.h"
#include"Command.h"
#include"VirtualDisk.h"

//命令模式
//调用者

class UseCommand
{
public:
	//注入具体的命令类对象
	UseCommand() {}
	UseCommand(Command* command)
	{
		this->command = command;
	}
	//注入命令
	void SetCommand(Command* cmd)
	{
		this->command = cmd;
	}
	//分发任务
	bool SendTask(VirtualDisk* virtualdisk)
	{
		//如果命令合法
		if (this->command->IsCommandCorrect())
		{
			this->command->Execute(virtualdisk);
		}
		return this->command->IsQuit();
	}
private:
	Command* command = nullptr;
};

//简单工厂模式，负责判断并分发任务
class Factory
{
public:
	Factory() {}
	//获取命令类型，创建命令类型对象去执行
	Command* GetCommand(Dir* dir, string CommandOrder)
	{
		string CommandType = FileHelper::RemoveSpace(CommandOrder);
		//变小写
		transform(CommandType.begin(), CommandType.end(), CommandType.begin(), ::tolower);
		vector<string> CmdOrderSplited = FileHelper::GetCommandPara(CommandOrder);
		
		Command* cmd = nullptr;
		if (CommandType == "dir") { cmd = new DirCommand(dir, CmdOrderSplited); }
		else if (CommandType == "mkdir") { cmd = new MkdirCommand(dir, CmdOrderSplited); }
		else if (CommandType == "rd") { cmd = new RdCommand(dir, CmdOrderSplited); }
		else if (CommandType == "cd") { cmd = new CdCommand(dir, CmdOrderSplited); }
		else if (CommandType == "touch") { cmd = new TouchCommand(dir, CmdOrderSplited); }
		else if (CommandType == "del") { cmd = new DelCommand(dir, CmdOrderSplited); }
		else if (CommandType == "copy") { cmd = new CopyCommand(dir, CmdOrderSplited); }
		else if (CommandType == "ren") { cmd = new RenameCommand(dir, CmdOrderSplited); }
		else if (CommandType == "move") { cmd = new MoveCommand(dir, CmdOrderSplited); }
		else if (CommandType == "mklink") { cmd = new MklinkCommand(dir, CmdOrderSplited); }
		else if (CommandType == "save") { cmd = new SaveCommand(dir, CmdOrderSplited); }
		else if (CommandType == "load") { cmd = new LoadCommand(dir, CmdOrderSplited); }
		else if (CommandType == "cat") { cmd = new CatCommand(dir, CmdOrderSplited); }
		else if (CommandType == "cls") { cmd = new ClsCommand(dir, CmdOrderSplited); }
		else if (CommandType == "quit") { cmd = new QuitCommand(); }
		else { Help::HelpSimpleUsage(); }
		return cmd;
	}
};
