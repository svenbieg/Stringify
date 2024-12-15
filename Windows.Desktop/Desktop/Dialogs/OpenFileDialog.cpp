//====================
// OpenFileDialog.cpp
//====================

#include "pch.h"


//=======
// Using
//=======

#include <commdlg.h>
#include "Resources/Strings/Files.h"
#include "UI/AppWindow.h"
#include "DialogHelper.h"
#include "OpenFileDialog.h"
#include "PathHelper.h"

using namespace Resources::Strings;
using namespace UI;


//===========
// Namespace
//===========

namespace Desktop {
	namespace Dialogs {


//========
// Common
//========

Handle<String> OpenFileDialog::Show(Handle<String> path, Handle<Sentence> title, Handle<Sentence> filter)
{
TCHAR file_path[MAX_PATH];
file_path[0]=0;
Handle<String> folder;
if(path)
	{
	auto path_ptr=path->Begin();
	UINT dir_len=PathHelper::GetDirectoryLength(path_ptr);
	if(dir_len)
		folder=new String(dir_len, path_ptr);
	StringHelper::Copy(file_path, MAX_PATH, &path_ptr[dir_len]);
	}
if(!title)
	title=STR_FILE_OPEN;
if(!filter)
	filter=STR_FILTER_ALL;
Handle<String> title_str=title->Begin();
Handle<String> filter_str=FilterFromSentence(filter);
OPENFILENAME ofn;
ZeroMemory(&ofn, sizeof(OPENFILENAME));
ofn.lStructSize=sizeof(OPENFILENAME);
ofn.hInstance=GetModuleHandle(nullptr);
ofn.hwndOwner=AppWindow::Current->GetHandle();
ofn.lpstrFile=file_path;
ofn.lpstrInitialDir=folder? folder->Begin(): nullptr;
ofn.lpstrTitle=title_str->Begin();
ofn.nMaxFile=MAX_PATH;
ofn.lpstrFilter=filter_str->Begin();
if(!GetOpenFileName(&ofn))
	return nullptr;
return new String(file_path);
}

}}