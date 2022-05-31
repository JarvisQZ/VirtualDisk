#pragma once

#include"head.h"
// cmd����С����
class Help
{
public:
	void static HelpDirCommand()
	{
		cout << endl;
		cout << "dir����:" << endl;
		cout << "  ����:�г�Ŀ¼�е��ļ�����Ŀ¼�б�" << endl;
		cout << "  ��ʽ:dir [/s] [/ad] [path1] [path2] ..." << endl;
		cout << "  /ad  ֻ�������Ŀ¼��������ļ�" << endl;
		cout << "  /s   ���Ŀ¼����������Ŀ¼�������ļ�" << endl;
	}
	void static HelpMkdirCommand()
	{
		cout << endl;
		cout << "mkdir����:" << endl;
		cout << "  ����:����Ŀ¼" << endl;
		cout << "  ��ʽ:mkdir path" << endl;
	}
	void static HelpCdCommand()
	{
		cout << endl;
		cout << "cd����:" << endl;
		cout << "  ����:��ʾ��ǰĿ¼�������л���ǰĿ¼" << endl;
		cout << "  ��ʽ:cd path" << endl;
	}
	void static HelpCopyCommand()
	{
		cout << endl;
		cout << "copy����:" << endl;
		cout << "  ����:������������ļ�����(�������ļ����ı��ļ�)���������ָ��Ŀ¼��" << endl;
		cout << "  ��ʽ:copy path1 path2" << endl;
	}
	void static HelpTouchCommand()
	{
		cout << endl;
		cout << "touch����:" << endl;
		cout << "  ����:�������ļ�" << endl;
		cout << "  ��ʽ:touch filename" << endl;
	}
	void static HelpDelCommand()
	{
		cout << endl;
		cout << "del����:" << endl;
		cout << "  ����:ɾ��һ�����߶��Ŀ¼�µ��ļ���Ŀ¼�ṹ�ᱣ��" << endl;
		cout << "  ��ʽ:del [/s] path [path1] [path2] ..." << endl;
		cout << "  /s   �ݹ�ɾ��Ŀ¼����������Ŀ¼�µ������ļ�" << endl;
	}
	void static HelpRdCommand()
	{
		cout << endl;
		cout << "rd����:" << endl;
		cout << "  ����:ɾ��һ���յ�Ŀ¼" << endl;
		cout << "  ��ʽ:rd [/s] path [path1] [path2] ..." << endl;
		cout << "  /s   ��Ŀ¼�����⣬����ɾ��ָ��Ŀ¼�µ�������Ŀ¼���ļ�������ɾ��Ŀ¼��" << endl;
	}
	void static HelpRenameCommand()
	{
		cout << endl;
		cout << "ren����:" << endl;
		cout << "  ����:��һ���ļ�����Ŀ¼������" << endl;
		cout << "  ��ʽ:ren oldname newname" << endl;
	}
	void static HelpMoveCommand()
	{
		cout << endl;
		cout << "move����:" << endl;
		cout << "  ����:�ƶ�һ�����߶��(�ļ�/�ļ���)" << endl;
		cout << "  ��ʽ:move srcpath dstpath" << endl;
		cout << "  /s   ����ͬ�����ļ������ļ��и���" << endl;
	}
	void static HelpMklinkCommand()
	{
		cout << endl;
		cout << "mklink����:" << endl;
		cout << "  ����:����һ���������ӣ�����������е�һ��·�� srcPath �� symbolLinkPath ��������" << endl;
		cout << "  ��ʽ:mklink srcPath symbolLinkPath" << endl;
	}
	void static HelpSaveCommand()
	{
		cout << endl;
		cout << "save����:" << endl;
		cout << "  ����:����������������л���һ����ʵ�����ļ��������ļ����ݣ���" << endl;
		cout << "  ��ʽ:save dstPath" << endl;
	}
	void static HelpLoadCommand()
	{
		cout << endl;
		cout << "load����:" << endl;
		cout << "  ����:�Ӵ�������save����洢���ļ����ݣ������л����ؽ�������̡�" << endl;
		cout << "  ��ʽ:load dstPath" << endl;
	}
	void static HelpClsCommand()
	{
		cout << endl;
		cout << "cls����:" << endl;
		cout << "  ����:ִ�к���Ļ��գ���궨λ����Ļ��һ��" << endl;
		cout << "  ��ʽ:cls" << endl;
	}
	void static HelpCatCommand()
	{
		cout << endl;
		cout << "cat����:" << endl;
		cout << "  ����:�鿴�ļ�����" << endl;
		cout << "  ��ʽ:cat filename" << endl;
	}
	void static HelpHelpCommand()
	{
		cout << endl;
		cout << "help����:" << endl;
		cout << "  ����:�鿴��������ʽ������" << endl;
		cout << "  ��ʽ:help [command]" << endl;
		cout << "  command: dir md cd copy touch del rd ren move mklink save load cls cat" << endl;
	}
	void static HelpSimpleUsage()
	{
		cout << "    �����������������ʹ������������г���!" << endl;
		cout << "    ���пո��·����ʹ��˫���Ű���" << endl;
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