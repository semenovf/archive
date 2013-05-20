/**
 * @file   file_unix.cpp
 * @author wladt
 * @date   Apr 11, 2013 3:21:57 PM
 *
 * @brief
 */

#include "../include/cwt/file.hpp"
#include <cerrno>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>

CWT_NS_BEGIN

static int __perms_to_mode(int perms);

class File::Impl {
public:
	Impl() : m_path(NULL), m_fd(-1) { ; }
	~Impl() { close(); }

	void flush();
	size_t bytesAvailable() const;
	int close();
	bool open(Errorable *ex, const char *path, int oflags);
	ssize_t readBytes(Errorable *ex, char bytes[], size_t n);
	ssize_t writeBytes(Errorable *ex, const char bytes[], size_t n);
	bool    setPermissions(Errorable *ex, int perms);

	static bool setPermissions(const char *path, int perms);
public:
	char *m_path;
	int   m_fd;
};


void File::Impl::flush()
{
	if (m_fd > 0) {
		::fsync(m_fd);
	}
}

size_t File::Impl::bytesAvailable() const
{
	off_t total;
	off_t cur;

	CWT_ASSERT(m_fd  >= 0);

	cur = ::lseek(m_fd, 0L, SEEK_CUR);
	total = ::lseek(m_fd, 0L, SEEK_END);
	CWT_ASSERT(total >= 0L);
	CWT_ASSERT(cur >= 0L);

	::lseek(m_fd, cur, SEEK_SET);
	CWT_ASSERT(total >= cur);

	return (size_t)(total - cur);
}

bool File::Impl::open(Errorable *ex, const char *path, int oflags)
{
	int fd;
	int native_oflags = 0;

	if (oflags & IODevice::ReadWrite) {
		native_oflags |= O_RDWR;
		native_oflags |= O_CREAT;
	} else if (oflags & IODevice::WriteOnly) {
		native_oflags |= O_WRONLY;
		native_oflags |= O_CREAT;
	} else {
		native_oflags |= O_RDONLY;
	}

	if (oflags & IODevice::NonBlocking)
		native_oflags |= O_NONBLOCK;

	m_path = CWT_MALLOCA(char, strlen(path)+1);
	memcpy(m_path, path, strlen(path)+1);

	fd = ::open(path, native_oflags);

	if (fd < 0) {
		ex->addSystemError(errno, _Tr("failed to open '%s'"), path);
		return false;
	}
	m_fd = fd;
	return true;
}

int File::Impl::close()
{
	int rc;

	if (m_fd > 0)
		rc = ::close(m_fd);
	if (m_path)
		::free(m_path);

	m_fd = -1;
	m_path = NULL;
	return rc;
}

ssize_t File::Impl::readBytes(Errorable *ex, char bytes[], size_t n)
{
	ssize_t sz;

	sz = ::read(m_fd, bytes, n);
	if (sz < 0) {
		ex->addSystemError(errno, _Tr("failed to read from file (%s)")
				, m_path != NULL ? m_path : "" );
	}
	return sz;
}

ssize_t File::Impl::writeBytes(Errorable *ex, const char bytes[], size_t n)
{
	ssize_t sz;

	sz = ::write(m_fd, bytes, n);
	if( sz < 0 ) {
		ex->addSystemError(errno, _Tr("failed to write to file (%s)")
				, m_path != NULL ? m_path : "" );
	}
	return sz;
}

static int __perms_to_mode(int perms)
{
	int mode = 0;
	if ((perms & File::ReadOwner) || (perms & File::ReadUser))
		mode |= S_IRUSR;
	if ((perms & File::WriteOwner) || (perms & File::WriteUser))
	    mode |= S_IWUSR;
	if ((perms & File::ExeOwner) || (perms & File::ExeUser))
		mode |= S_IXUSR;
	if (perms & File::ReadGroup)
		mode |= S_IRGRP;
	if (perms & File::WriteGroup)
		mode |= S_IWGRP;
	if (perms & File::ExeGroup)
		mode |= S_IXGRP;
	if (perms & File::ReadOther)
		mode |= S_IROTH;
	if (perms & File::WriteOther)
		mode |= S_IWOTH;
	if (perms & File::ExeOther)
		mode |= S_IXOTH;

	return mode;
}

bool File::Impl::setPermissions(Errorable *ex, int perms)
{
	CWT_ASSERT(m_path);

	if (::chmod(m_path, __perms_to_mode(perms)) != 0) {
		ex->addSystemError(errno, _Tr("failed to change permissions to file (%s)")
				, m_path != NULL ? m_path : "");
		return false;
	}

	return true;
}

bool File::Impl::setPermissions(const char *path, int perms)
{
	CWT_ASSERT(path);

	if (::chmod(path, __perms_to_mode(perms)) != 0) {
		Logger::error(_Tr("failed to change permissions to file (%s)")
				, path != NULL ? path : "");
		return false;
	}

	return true;
}

File::File() : pimpl(new File::Impl) { ; }
size_t  File::bytesAvailable() const { return pimpl->bytesAvailable(); }
bool    File::open(const char *path, int oflags) {	return pimpl->open(this, path, oflags); }
int     File::close()   { return pimpl->close(); }
void    File::flush()   { pimpl->flush(); }
bool    File::opened() const { return pimpl->m_fd; }
ssize_t File::readBytes(char bytes[], size_t n) { return pimpl->readBytes(this, bytes, n); }
ssize_t File::writeBytes(const char bytes[], size_t n) { return pimpl->writeBytes(this, bytes, n); }

bool File::setPermissions(int perms) { return pimpl->setPermissions(this, perms); }
bool File::setPermissions(const char *path, int perms) { return File::Impl::setPermissions(path, perms); }
CWT_NS_END
