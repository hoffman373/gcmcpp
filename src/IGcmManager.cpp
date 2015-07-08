#include "IGcmManager.hpp"

using namespace obr_basementcontrol;

IGcmManager::IGcmManager(const string& hostnm, int prt, const string& usrname, const string& passwd)
  : hostname(hostnm), username(usrname), password(passwd), port(prt)
{
}
