#pragma once
#include "Core.h"
#include "UI/Widgets/LabelUI.h"
#include "UI/Widgets/InputUI.h"
#include "Utils.h"

class OptionsTabUI
{
public:
	OptionsTabUI();

	URef<LabelUI> numThreadsLabel;
	URef<InputUI> threadInput;

	URef<LabelUI> chunkSizeLabel;
	URef<InputUI> chunkInput;

	URef<LabelUI> saveDirLabel;
	URef<InputUI> saveInput;

	bool GetThreads(unsigned int& threadCount)const
	{
		return Utils::SafeCharToUint(threadInput->GetText().c_str(), threadCount);
	}
	bool GetChunkSize(unsigned int& chunkSize)const
	{
		return Utils::SafeCharToUint(chunkInput->GetText().c_str(), chunkSize);
	}
	bool GetSaveFolder(char* folder, size_t bufferSize)const
	{
		const char* value = saveInput->GetText().c_str();
		if (value == nullptr)
			return false;

		strncpy_s(folder, bufferSize, value, _TRUNCATE);
		return true;
	}
};