#ifndef __TRANSPORT_MESSAGE_RELATION_H__
#define __TRANSPORT_MESSAGE_RELATION_H__
#include <inttypes.h>

namespace xag_nav {
namespace os {
namespace transport{


enum Relation : uint8_t {
  NO_RELATION = 0,
  DIFF_HOST,  // different host
  DIFF_PROC,  // same host, but different process
  SAME_PROC,  // same process
};

}
}
}

#endif