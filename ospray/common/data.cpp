#include "data.h"
#include <sstream>

namespace ospray {
  inline size_t sizeOf(OSPDataType type)
  {
    switch (type) {
    case OSP_OBJECT: return sizeof(Data *);
    case OSP_vec2f:  return sizeof(vec2f);
    case OSP_vec3fa: return sizeof(vec3fa);
    case OSP_vec4i:  return sizeof(vec4i);
    default: break;
    };
    std::stringstream err;
    err << __FILE__ << ":" << __LINE__ << ": unknown OSPDataType " << (int)type;
    throw std::runtime_error(err.str());
  }

  Data::Data(size_t numItems, OSPDataType type, void *init, int flags)
    : numItems(numItems), numBytes(numItems*sizeOf(type)), type(type), flags(flags)
  {
    Assert(flags == 0); // nothing else implemented yet ...
    data = malloc(numBytes);
    if (init)
      memcpy(data,init,numBytes);
    else if (type == OSP_OBJECT)
      bzero(data,numBytes);
  }

  Data::~Data() 
  { 
    if (type == OSP_OBJECT) {
      Data **child = (Data **)data;
      for (int i=0;i<numItems;i++)
        if (child[i]) child[i]->refDec();
    }
    free(data); 
  }
}