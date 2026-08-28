//================
// OcrScanner.cpp
//================

#include "OcrScanner.h"


//=======
// Using
//=======

#pragma comment(lib, "windowsapp.lib")

#include <WinRT/Windows.Foundation.h>
#include <WinRT/Windows.Foundation.Collections.h>
#include <WinRT/Windows.Graphics.Imaging.h>
#include <WinRT/Windows.Media.Ocr.h>
#include <WinRT/Windows.Storage.h>
#include <WinRT/Windows.Storage.Streams.h>

using namespace winrt;

using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Graphics::Imaging;
using namespace Windows::Media::Ocr;
using namespace Windows::Storage;
using namespace Windows::Storage::Streams;


//===========
// Namespace
//===========

namespace Graphics {
	namespace Ocr {


//========
// Common
//========

Handle<OcrResult> OcrScanner::ScanImage(Handle<String> path)
{
auto file=StorageFile::GetFileFromPathAsync(path->Begin()).get();
auto stream=file.OpenAsync(FileAccessMode::Read).get();
auto decoder=BitmapDecoder::CreateAsync(stream).get();
auto bitmap=decoder.GetSoftwareBitmapAsync().get();
auto engine=OcrEngine::TryCreateFromUserProfileLanguages();
auto result=engine.RecognizeAsync(bitmap).get();
auto ocr_result=OcrResult::Create();
for(auto const& line: result.Lines())
	{
	auto ocr_line=OcrLine::Create();
	ocr_line->Text=line.Text().c_str();
	for(auto const& word: line.Words())
		{
		auto text=String::Create(word.Text().c_str());
		auto rc=word.BoundingRect();
		auto ocr_word=OcrWord::Create(text, rc.X, rc.Y, rc.X+rc.Width, rc.Y+rc.Height);
		ocr_line->Words.append(ocr_word);
		ocr_result->Index.add(text);
		}
	ocr_result->Lines.append(ocr_line);
	}
return ocr_result;
}

}}