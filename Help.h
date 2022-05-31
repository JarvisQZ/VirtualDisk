#pragma once

#include"head.h"
// cmd帮助小助手
class Help
{
public:
	void static HelpDirCommand()
	{
		cout << endl;
		cout << "dir命令:" << endl;
		cout << "  功能:列出目录中的文件和子目录列表" << endl;
		cout << "  格式:dir [/s] [/ad] [path1] [path2] ..." << endl;
		cout << "  /ad  只输出其子目录，不输出文件" << endl;
		cout << "  /s   输出目录及其所有子目录下所有文件" << endl;
	}
	void static HelpMkdirCommand()
	{
		cout << endl;
		cout << "mkdir命令:" << endl;
		cout << "  功能:创建目录" << endl;
		cout << "  格式:mkdir path" << endl;
	}
	void static HelpCdCommand()
	{
		cout << endl;
		cout << "cd命令:" << endl;
		cout << "  功能:显示当前目录名或者切换当前目录" << endl;
		cout << "  格式:cd path" << endl;
	}
	void static HelpCopyCommand()
	{
		cout << endl;
		cout << "copy命令:" << endl;
		cout << "  功能:拷贝任意磁盘文件内容(二进制文件和文本文件)到虚拟磁盘指定目录中" << endl;
		cout << "  格式:copy path1 path2" << endl;
	}
	void static HelpTouchCommand()
	{
		cout << endl;
		cout << "touch命令:" << endl;
		cout << "  功能:创建新文件" << endl;
		cout << "  格式:touch filename" << endl;
	}
	void static HelpDelCommand()
	{
		cout << endl;
		cout << "del命令:" << endl;
		cout << "  功能:删除一个或者多个目录下的文件，目录结构会保留" << endl;
		cout << "  格式:del [/s] path [path1] [path2] ..." << endl;
		cout << "  /s   递归删除目录及其所有子目录下的所有文件" << endl;
	}
	void static HelpRdCommand()
	{
		cout << endl;
		cout << "rd命令:" << endl;
		cout << "  功能:删除一个空的目录" << endl;
		cout << "  格式:rd [/s] path [path1] [path2] ..." << endl;
		cout << "  /s   除目录本身外，还将删除指定目录下的所有子目录和文件，用于删除目录树" << endl;
	}
	void static HelpRenameCommand()
	{
		cout << endl;
		cout << "ren命令:" << endl;
		cout << "  功能:给一个文件或者目录重命名" << endl;
		cout << "  格式:ren oldname newname" << endl;
	}
	void static HelpMoveCommand()
	{
		cout << endl;
		cout << "move命令:" << endl;
		cout << "  功能:移动一个或者多个(文件/文件夹)" << endl;
		cout << "  格式:move srcpath dstpath" << endl;
		cout << "  /s   遇到同名的文件或者文件夹覆盖" << endl;
	}
	void static HelpMklinkCommand()
	{
		cout << endl;
		cout << "mklink命令:" << endl;
		cout << "  功能:创建一个符号链接，将虚拟磁盘中的一个路径 srcPath 和 symbolLinkPath 建立链接" << endl;
		cout << "  格式:mklink srcPath symbolLinkPath" << endl;
	}
	void static HelpSaveCommand()
	{
		cout << endl;
		cout << "save命令:" << endl;
		cout << "  功能:将整个虚拟磁盘序列化到一个真实磁盘文件（包括文件内容）。" << endl;
		cout << "  格式:save dstPath" << endl;
	}
	void static HelpLoadCommand()
	{
		cout << endl;
		cout << "load命令:" << endl;
		cout << "  功能:从磁盘载入save命令存储的文件内容，反序列化，重建虚拟磁盘。" << endl;
		cout << "  格式:load dstPath" << endl;
	}
	void static HelpClsCommand()
	{
		cout << endl;
		cout << "cls命令:" << endl;
		cout << "  功能:执行后屏幕清空，光标定位在屏幕第一行" << endl;
		cout << "  格式:cls" << endl;
	}
	void static HelpCatCommand()
	{
		cout << endl;
		cout << "cat命令:" << endl;
		cout << "  功能:查看文件内容" << endl;
		cout << "  格式:cat filename" << endl;
	}
	void static HelpHelpCommand()
	{
		cout << endl;
		cout << "help命令:" << endl;
		cout << "  功能:查看命令具体格式及功能" << endl;
		cout << "  格式:help [command]" << endl;
		cout << "  command: dir md cd copy touch del rd ren move mklink save load cls cat" << endl;
	}
	void static HelpSimpleUsage()
	{
		cout << "    您输入的命令有误！请使用以下命令进行尝试!" << endl;
		cout << "    带有空格的路径请使用双引号包裹" << endl;
		cout << "    dir      [/s]      [/ad]    [path1]    [path2] ..." << endl;
		cout << "    del      [/s]      path     [path1]    [path2] ..." << endl;
		cout << "    rd       [/s]      path     [path1]    [path2] ..." << endl;
		cout << "    move     /y        oldplace newplace" << endl;
		cout << "    mklink   srcpath   symbollinkpath" << endl;
		cout << "    copy     [path1]   [path2]" << endl;
		cout << "    ren      oldname   newname" << endl;
		cout << "    help     [command]" << endl;
		cout << "    touch    filename" << endl;
		cout << "    cat      filename" << endl;
		cout << "    cd       [path]" << endl;
		cout << "    mkdir    path" << endl;
		cout << "    save     path" << endl;
		cout << "    load     path" << endl;
		cout << "    cls" << endl;
	}
};