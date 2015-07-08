#ifndef GCMSTANZA_HPP
#define GCMSTANZA_HPP

#include <gloox/client.h>
#include <gloox/presencehandler.h>
#include <gloox/connectionlistener.h>
#include <gloox/loghandler.h>
#include <gloox/messagehandler.h>
#include <string>
#include <gloox/taghandler.h>
#include <gloox/stanzaextension.h>
#include <ostream>
#include "GcmMessageBase.hpp"

using namespace std;
using namespace gloox;

namespace obr_basementcontrol
{  
  class GcmStanza : public StanzaExtension
  {
  public:
    GcmStanza(const Tag* tag);
    
    ~GcmStanza();

    GcmStanza(const GcmStanza& source);

    GcmStanza(GcmStanza&& source);

    GcmStanza(MessageBase* msgBase);

    GcmStanza& operator=(const GcmStanza& source);
    
    GcmStanza& operator=(GcmStanza&& source);
    
    virtual const string& filterString() const;

    virtual StanzaExtension* newInstance(const Tag* tag) const;

    virtual Tag* tag() const;

    virtual StanzaExtension* clone() const;

    MessageBase* pilferMesage();
    
  private:
    string filter;
    MessageBase* message;
  };
}

#endif
