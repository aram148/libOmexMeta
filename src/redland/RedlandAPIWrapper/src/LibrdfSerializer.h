//
// Created by Ciaran on 5/20/2020.
//

#ifndef LIBSEMSIM_LIBRDFSERIALIZER_H
#define LIBSEMSIM_LIBRDFSERIALIZER_H


#include <librdf.h>
#include <memory>

#include "World.h"
#include "LibrdfUri.h"
#include "LibrdfModel.h"
#include "RaptorIOStream.h"
#include "LibrdfNode.h"
#include "LibrdfModel.h"


namespace redland {

    class LibrdfSerializer {

        struct deleter {
            void operator()(librdf_serializer *serializer);
        };

        librdf_serializer *serializer_ = nullptr;

        explicit LibrdfSerializer(librdf_serializer *serializer);

    public:
        LibrdfSerializer() = default;

        ~LibrdfSerializer();

        LibrdfSerializer(const LibrdfSerializer &serializer) = delete;

        LibrdfSerializer(LibrdfSerializer &&serializer) noexcept;

        LibrdfSerializer &operator=(const LibrdfSerializer &serializer) = delete;

        LibrdfSerializer &operator=(LibrdfSerializer &&serializer) noexcept;


        static LibrdfSerializer fromRawPtr(librdf_serializer *serializer);

        explicit LibrdfSerializer(const char *format, const char *mime_type = nullptr,
                                  const char *type_uri = nullptr);

        [[nodiscard]] librdf_serializer *get() const;

        void setNamespace(const std::string &ns, const std::string &prefix) const;

        void setFeature(const std::string &ns, const std::string &prefix) const;

        std::string toString(std::string uri, const LibrdfModel &model);

        void freeSerializer();
    };
}


#endif //LIBSEMSIM_LIBRDFSERIALIZER_H
