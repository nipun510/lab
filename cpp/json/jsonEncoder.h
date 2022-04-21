
#ifndef JSON_ENCODER_H
#define JSON_ENCODER_H

class json;

class jsonEncoder 
{
  public:
    virtual json*            encode(const std::string &str) const = 0;
    virtual std::string      decode(const json *jsonObj) const = 0;

    virtual jsonEncoder*     clone() = 0;
    virtual                  ~jsonEncoder() = default;
};

#endif
