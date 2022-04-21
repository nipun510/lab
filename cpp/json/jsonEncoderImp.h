
#ifndef JSONENCODERIMP_H
#define JSONENCODERIMP_H

#include "jsonEncoder.h"

class json;
class jsonValue;

class jsonEncoderImp  : public jsonEncoder
{
  public:
    json*         encode(const std::string &str) const override {return nullptr;}
    std::string   decode(const json *jsonObj) const override;

  private:
    void                  decodeInt(const jsonValue *jsonObj, std::ostringstream &os) const;
    virtual jsonEncoder*  clone() override { return new jsonEncoderImp(); }
};

#endif
