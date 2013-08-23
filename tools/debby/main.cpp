#include <cwt/app.hpp>
#include <cwt/logger.hpp>
#include <cwt/debby.hpp>

using namespace cwt;

void usage()
{
	puts("Usage:\n\tdebby debby-file");
}

class DebbyProc : public cwt::MainProc
{
public:
	DebbyProc(const String & path = String()) : cwt::MainProc(), m_debbyFilePath(path) {}
	virtual int operator () () { return proc(); }
	int proc();

private:
	String m_debbyFilePath;
};


int DebbyProc::proc()
{
	Debby debby;
	if (m_debbyFilePath.isEmpty()) {
		Logger::error("Debby file must be specified");
		usage();
		return -1;
	}
	return debby.parseFromFile(m_debbyFilePath) ? 0 : -1;
}

int main(int argc, char * argv[])
{
    App app(argc, argv);
    DebbyProc proc(argc > 1 ? String::fromUtf8(argv[1]) : String());
    return app.exec(proc);
}
