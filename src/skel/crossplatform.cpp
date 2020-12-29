#include "common.h"
#include "crossplatform.h"

#ifdef __WIIU__
#include <coreinit/time.h>
#endif

// Codes compatible with Windows and Linux
#ifndef _WIN32

// For internal use
// wMilliseconds is not needed
void tmToSystemTime(const tm *tm, SYSTEMTIME *out) {
    out->wYear = tm->tm_year + 1900;
    out->wMonth = tm->tm_mon + 1;
    out->wDayOfWeek = tm->tm_wday;
    out->wDay = tm->tm_mday;
    out->wHour = tm->tm_hour;
    out->wMinute = tm->tm_min;
    out->wSecond = tm->tm_sec;
}

void GetLocalTime_CP(SYSTEMTIME *out) {
#ifndef __WIIU__
    time_t timestamp = time(nil);
    tm *localTm = localtime(&timestamp);
    tmToSystemTime(localTm, out);
#else
    OSCalendarTime time;
    OSTicksToCalendarTime(OSGetTime(), &time);
    out->wYear = time.tm_year;
    out->wMonth = time.tm_mon + 1;
    out->wDayOfWeek = time.tm_wday;
    out->wDay = time.tm_mday;
    out->wHour = time.tm_hour;
    out->wMinute = time.tm_min;
    out->wSecond = time.tm_sec;
    out->wMilliseconds = time.tm_msec;
#endif
}
#endif

// Compatible with Linux/POSIX and MinGW on Windows
#ifndef _WIN32
HANDLE FindFirstFile(const char* pathname, WIN32_FIND_DATA* firstfile) {
	char pathCopy[MAX_PATH];
	strcpy(pathCopy, pathname);

	char *folder = strtok(pathCopy, "*");
	char *extension = strtok(NULL, "*");

	// because strtok doesn't return NULL for last delimiter
	if (extension - folder == strlen(pathname))
		extension = nil;
	
	// Case-sensitivity and backslashes...
	// Will be freed at the bottom
	char *realFolder = casepath(folder, false);
	if (realFolder) {
		folder = realFolder;
	}

	strncpy(firstfile->folder, folder, sizeof(firstfile->folder));

	if (extension)
		strncpy(firstfile->extension, extension, sizeof(firstfile->extension));
	else
		firstfile->extension[0] = '\0';

	if (realFolder)
		free(realFolder);

	HANDLE d;
	if ((d = (HANDLE)opendir(firstfile->folder)) == NULL || !FindNextFile(d, firstfile))
		return NULL;

	return d;
}

bool FindNextFile(HANDLE d, WIN32_FIND_DATA* finddata) {
	dirent *file;
	static struct stat fileStats;
	static char path[PATH_MAX], relativepath[NAME_MAX + sizeof(finddata->folder) + 1];
	int extensionLen = strlen(finddata->extension);
	while ((file = readdir((DIR*)d)) != NULL) {

		// We only want "DT_REG"ular Files, but reportedly some FS and OSes gives DT_UNKNOWN as type.
		if ((file->d_type == DT_UNKNOWN || file->d_type == DT_REG || file->d_type == DT_LNK) &&
			(extensionLen == 0 || strncasecmp(&file->d_name[strlen(file->d_name) - extensionLen], finddata->extension, extensionLen) == 0)) {

			sprintf(relativepath, "%s/%s", finddata->folder, file->d_name);
			realpath(relativepath, path);
			stat(path, &fileStats);
			strncpy(finddata->cFileName, file->d_name, sizeof(finddata->cFileName));
			finddata->ftLastWriteTime = fileStats.st_mtime;
			return true;
		}
	}
	return false;
}

void GetDateFormat(int unused1, int unused2, SYSTEMTIME* in, int unused3, char* out, int size) {
	tm linuxTime;
	linuxTime.tm_year = in->wYear - 1900;
	linuxTime.tm_mon = in->wMonth - 1;
	linuxTime.tm_wday = in->wDayOfWeek;
	linuxTime.tm_mday = in->wDay;
	linuxTime.tm_hour = in->wHour;
	linuxTime.tm_min = in->wMinute;
	linuxTime.tm_sec = in->wSecond;
	strftime(out, size, nl_langinfo(D_FMT), &linuxTime);
}

void FileTimeToSystemTime(time_t* writeTime, SYSTEMTIME* out) {
	tm *ptm = gmtime(writeTime);
	tmToSystemTime(ptm, out);
}
#endif

// Because wchar length differs between platforms.
wchar*
AllocUnicode(const char* src)
{
	wchar *dst = (wchar*)malloc(strlen(src)*2 + 2);
	wchar *i = dst;
	while((*i++ = (unsigned char)*src++) != '\0');
	return dst;
}

// Funcs/features from Windows that we need on other platforms
#ifndef _WIN32
char *strupr(char *s) {
    char* tmp = s;

    for (;*tmp;++tmp) {
        *tmp = toupper((unsigned char) *tmp);
    }

    return s;
}
char *strlwr(char *s) {
    char* tmp = s;

    for (;*tmp;++tmp) {
        *tmp = tolower((unsigned char) *tmp);
    }

    return s;
}

char *trim(char *s) {
    char *ptr;
    if (!s)
        return NULL;   // handle NULL string
    if (!*s)
        return s;      // handle empty string
    for (ptr = s + strlen(s) - 1; (ptr >= s) && isspace(*ptr); --ptr);
    ptr[1] = '\0';
    return s;
}

FILE* _fcaseopen(char const* filename, char const* mode)
{
    FILE* result;
    char* real = casepath(filename);
    if (!real)
        result = fopen(filename, mode);
    else {
        result = fopen(real, mode);
        free(real);
    }
    return result;
}

// Case-insensitivity on linux (from https://github.com/OneSadCookie/fcaseopen)
// Returned string should freed manually (if exists)
char* casepath(char const* path, bool checkPathFirst)
{
    if (checkPathFirst && access(path, F_OK) != -1) {
        // File path is correct
        return nil;
    }

    size_t l = strlen(path);
#ifdef __WIIU__
    // alloca seems to cause issues here, so just use the stack
    char _p[l+1];
    char* p = _p;
#else
    char* p = (char*)alloca(l + 1);
#endif
    char* out = (char*)malloc(l + 3); // for extra ./
    strcpy(p, path);

    // my addon: linux doesn't handle filenames with spaces at the end nicely
    p = trim(p);

    size_t rl = 0;

    DIR* d;
    if (p[0] == '/' || p[0] == '\\')
    {
#ifdef __WIIU__
        // skip the /vol/*/ as we can't open it
        if (strncmp(path, "/vol/content", strlen("/vol/content")) == 0) {
            d = opendir("/vol/content/");
            p += 13;
            strcpy(out, "/vol/content");
            rl += 12;
        }
        else if (strncmp(path, "/vol/external01", strlen("/vol/external01")) == 0) {
            d = opendir("/vol/external01/");
            p += 16;
            strcpy(out, "/vol/external01");
            rl += 15;
        }
#else
        d = opendir("/");
#endif
    }
    else
    {
        d = opendir(".");
        out[0] = '.';
        out[1] = 0;
        rl = 1;
    }

    bool cantProceed = false; // just convert slashes in what's left in string, not case sensitivity
    bool mayBeTrailingSlash = false;
    char* c;
    while (c = strsep(&p, "/\\"))
    {
        // May be trailing slash(allow), slash at the start(avoid), or multiple slashes(avoid)
        if (*c == '\0')
        {
            mayBeTrailingSlash = true;
            continue;
        } else {
            mayBeTrailingSlash = false;
        }

        out[rl] = '/';
        rl += 1;
        out[rl] = 0;

        if (cantProceed)
        {
            strcpy(out + rl, c);
            rl += strlen(c);
            continue;
        }

        struct dirent* e;
        while (e = readdir(d))
        {
            if (strcasecmp(c, e->d_name) == 0)
            {
                strcpy(out + rl, e->d_name);
                int reportedLen = (int)strlen(e->d_name);
                rl += reportedLen;
                assert(reportedLen == strlen(c) && "casepath: This is not good at all");

                closedir(d);
                d = opendir(out);

                // Either it wasn't a folder, or permission error, I/O error etc.
                if (!d) {
                    cantProceed = true;
                }

                break;
            }
        }

        if (!e)
        {
            debug("casepath couldn't find dir/file \"%s\", full path was %s\n", c, path);
            // No match, add original name and continue converting further slashes.
            strcpy(out + rl, c);
            rl += strlen(c);
            cantProceed = true;
        }
    }

    if (d) closedir(d);
    if (mayBeTrailingSlash) {
        out[rl] = '/';  rl += 1;
        out[rl] = '\0';
    }

    if (rl > l + 2) {
        debug("\n\ncasepath: Corrected path length is longer then original+2:\n\tOriginal: %s (%d chars)\n\tCorrected: %s (%d chars)\n\n", path, l, out, rl);
    }
    return out;
}
#endif
