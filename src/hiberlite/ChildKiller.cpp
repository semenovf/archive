#include "../../include/cwt/debby/hiberlite/hiberlite.h"

namespace hiberlite {

ChildKiller::ChildKiller() : AVisitor<KillChildren>(&actor, DELETING)
{}

} //namespace hiberlite
