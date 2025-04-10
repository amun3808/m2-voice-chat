#pragma once
#include <stdint.h>
#include <mutex>
#include <vector>
#include <unordered_map>
#include <iostream>

#ifdef _DEBUG
#define ENABLE_LOGGER
#endif

// I don't have time to make a class out of this shit as well
#ifdef ENABLE_LOGGER
#include <fstream>
#include <cstdarg>

const std::string filename = "voice_chat_log.txt";
static std::ofstream logFile;
static bool isCleared = false;
static bool isOpened = false;
void traceFn(const char* format, ...)
{
	if (!isCleared)
	{
		logFile = std::ofstream(filename, std::ios_base::trunc);
		logFile.close();
		isCleared = true;
	}
	if (!isOpened)
	{
		logFile = std::ofstream(filename, std::ios_base::app);
		isOpened = true;
	}

	char buffer[1024];
	va_list args;
	va_start(args, format);
	vsnprintf(buffer, sizeof(buffer), format, args);
	va_end(args);
	logFile << buffer << std::endl;
	std::cout << buffer << std::endl; // log to console as well
}
#define trace(str, ...) traceFn(str, __VA_ARGS__)
#else
#define trace(str, ...)
#endif


//#define USE_FLOAT
namespace amun
{
#ifdef USE_FLOAT
#define SAMPLE_TYPE float
#define SAMPLE_FORMAT ma_format_f32
#else
#define SAMPLE_TYPE int16_t
#define SAMPLE_FORMAT ma_format_s16
#endif
}

#include <FLAC/all.h>
#include <FLAC++/all.h>

#include "miniaudio.h"
#include "voice_chat_errors.h"
#include "signal_receiver.h"
#include "recorder.h"
#include "playback.h"
#include "flac_encoder.h"
#include "flac_decoder.h"

