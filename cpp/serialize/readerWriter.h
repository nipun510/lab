#ifndef READER_WRITER_H
#define READER_WRITER_H

#include "reader.h"
#include "writer.h"

template<typename Reader, typename Writer>
class readerWriter
{
public:
    using reader = Reader;
    using writer = Writer;
};

using simpleSerializer = readerWriter<simpleReader, simpleWriter>;
using reader = simpleReader;
using writer = simpleWriter;


#endif
