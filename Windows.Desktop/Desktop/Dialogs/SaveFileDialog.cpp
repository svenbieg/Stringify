//====================
// SaveFileDialog.cpp
//====================

#include "pch.h"


//=======
// Using
//=======

#include <commdlg.h>
#include "Resources/Strings/Files.h"
#include "UI/AppWindow.h"
#include "DialogHelper.h"
#include "SaveFileDialog.h"
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

Handle<String> SaveFileDialog::Show(Handle<String> path, Handle<Sentence> title, Handle<Sentence> filter)
{
TCHAR file_path[MAX_PATH];
file_path[0]=0;
Handle<String> folder;
if(path)
	{
	auto path_ptr=path->Begin();
	UINT dir_len=PathHelper::GetDirectoryLength(path_ptr);
	if(dir_len)
		folder=String::Create(dir_len, path_ptr);
	StringHelper::Copy(file_path, MAX_PATH, &path_ptr[dir_len]);
	}
if(!title)
	title=STR_FILE_SAVE;
if(!filter)
	filter=STR_FILE_FILTER_ALL;
Handle<String> title_str=title->Begin();
Handle<String> filter_str=FilterFromSentence(filter);
auto app_wnd=AppWindow::GetCurrent();
OPENFILENAME ofn={ 0 };
ofn.lStructSize=sizeof(OPENFILENAME);
ofn.hInstance=GetModuleHandle(nullptr);
ofn.hwndOwner=app_wnd? app_wnd->GetHandle(): HWND_DESKTOP;
ofn.lpstrFile=file_path;
ofn.lpstrInitialDir=folder? folder->Begin(): nullptr;
ofn.lpstrTitle=title_str->Begin();
ofn.nMaxFile=MAX_PATH;
ofn.lpstrFilter=filter_str->Begin();
if(!GetSaveFileName(&ofn))
	return nullptr;
return String::Create(file_path);
}

}}