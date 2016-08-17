#include <iostream>
#include <pfs/thread.hpp>
#include <pfs/condition_variable.hpp>
#include <pfs/queue.hpp>

using std::cout;
using std::endl;

static pfs::queue<int> produced_nums;
static pfs::mutex m;
static pfs::condition_variable cond_var;
static bool done = false;
static bool notified = false;

class producer : public pfs::thread
{
public:
	producer () : pfs::thread() {}

	virtual void run()
	{
        for (int i = 0; i < 5; ++i) {
            msleep(100);
            pfs::unique_lock<pfs::mutex> lock(m);
            cout << "producing " << i << endl;
            produced_nums.push(i);
            notified = true;
            cond_var.notify_one();
        }
 
        done = true;
        cond_var.notify_one();
	}
};

class consumer : public pfs::thread
{
public:
    consumer () : pfs::thread() {}
    
    virtual void run ()
    {
        pfs::unique_lock<pfs::mutex> lock(m);
        
        while (!done) {
            cout << "1" << endl;
            
            while (!notified) {  // loop to avoid spurious wakeups
                cout << "2" << endl;
                cond_var.wait(lock);
                cout << "3" << endl;
            }
            
            cout << "4" << endl;
            
            while (!produced_nums.empty()) {
                cout << "5" << endl;
                std::cout << "consuming " << produced_nums.front() << '\n';
                produced_nums.pop();
                cout << "6" << endl;
            }
            
            cout << "7" << endl;
            
            notified = false;
        }
    }
};
 
int main (int argc, char * argv[])
{
    PFS_UNUSED2(argc, argv);

    producer prod;
    consumer cons;

    prod.start();
    cons.start();
    
    prod.wait();
    cons.wait();
    
    return 0;
}
