#include "IWDVFileInterface.h"

wdvideo::IWDVFileInterface::IWDVFileInterface():  inlength(0),
  infile(nullptr),
  is_preloaded(false),
  inrawdata(nullptr)
{
}

wdvideo::IWDVFileInterface::~IWDVFileInterface()
{
}

int wdvideo::IWDVFileInterface::open(const char* filename, bool preload)
{
  if (filename == nullptr)
    return -1;

  if (infile)
    return -1;

#ifdef _MSC_VER
  const errno_t e = fopen_s(&infile, filename, "rb");

  if (e)
    return -1; // error
#else
  infile = fopen(fileName, "rb");

  if (infile == NULL)
    return -1;
#endif

  bool ret = !getFileSize();

  if (preload)
  {
    inrawdata = new unsigned char[inlength];
    Read(0, static_cast<long>(inlength), inrawdata);
  }

  is_preloaded = preload;

  return ret;
}

bool wdvideo::IWDVFileInterface::getFileSize()
{
  if (infile == NULL)
    return false;
#ifdef _MSC_VER
  int status = _fseeki64(infile, 0L, SEEK_END);

  if (status)
    return false; // error

  inlength = _ftelli64(infile);
#else
  fseek(infile, 0L, SEEK_END);
  inlength = ftell(infile);
#endif

  if (inlength < 0)
    return false;

#ifdef _MSC_VER
  status = _fseeki64(infile, 0L, SEEK_SET);

  if (status)
    return false; // error
#else
  fseek(infile, 0L, SEEK_SET);
#endif

  return true;
}


int wdvideo::IWDVFileInterface::Read(long long position, long length, unsigned char* buffer)
{
  if (infile == nullptr)
    return -1;

  if (position < 0)
    return -1;

  if (length < 0)
    return -1;

  if (length == 0)
    return 0;

  if (position >= inlength)
    return -1;

  if (!is_preloaded)
  {
#ifdef _MSC_VER
    const int status = _fseeki64(infile, position, SEEK_SET);

    if (status)
      return -1; // error
#else
    fseek(m_file, offset, SEEK_SET);
#endif

    const size_t size = fread(buffer, 1, length, infile);

    if (size < size_t(length))
      return -1; // error
  }
  // file preloaded
  else
  {
    size_t size = length;

    if ((position + length) > inlength)
      size = inlength - position;

    memcpy(buffer, inrawdata + position, size);

    if (size < size_t(length))
      return -1; // error
  }

  return 0; // success
}

int wdvideo::IWDVFileInterface::Length(long long* total, long long* available)
{
  if (infile == nullptr)
    return -1;

  if (total)
    *total = inlength;

  if (available)
    *available = inlength;

  return 0;
}
