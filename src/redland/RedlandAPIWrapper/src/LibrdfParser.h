//
// Created by Ciaran on 5/20/2020.
//

#ifndef LIBSEMSIM_LIBRDFPARSER_H
#define LIBSEMSIM_LIBRDFPARSER_H


#include <librdf.h>
#include <memory>

#include "World.h"
#include "LibrdfNode.h"
#include "LibrdfUri.h"
#include "LibrdfModel.h"

namespace redland {

    class LibrdfParser {

        struct deleter {
            void operator()(librdf_parser *parser);
        };

        librdf_parser *parser_ = nullptr;

        void validateParserName() const;

        std::string format_ = std::string();
        std::string mime_type_ = std::string();
        librdf_uri *type_uri_ = nullptr;

    public:
        LibrdfParser() = default;

        ~LibrdfParser();

        LibrdfParser(const LibrdfParser &parser) = delete;

        LibrdfParser(LibrdfParser &&parser) noexcept;

        LibrdfParser &operator=(const LibrdfParser &parser) = delete;

        LibrdfParser &operator=(LibrdfParser &&parser) noexcept;

        explicit LibrdfParser(librdf_parser *parser);

        explicit LibrdfParser(std::string format, std::string mime_type = std::string(),
                              std::string type_uri = std::string());

        librdf_parser *get() const;

        void setFeature(std::string feature_uri, LibrdfNode node) const;

        [[nodiscard]] int numNamespacesSeen() const;

        [[nodiscard]] std::string getNamespacesSeenUri(int index) const;

        /*
         *
         * For developers. This method should be deprecated
         * and replaced with the other signature for this function. It is
         * better because the user does not need to remember to free the Uri used.
         */
        void parseString(const std::string &rdf_string, const LibrdfModel &model, const LibrdfUri &base_uri) const;

        void parseString(const std::string &rdf_string, const LibrdfModel &model, const std::string &base_uri) const;

        void parseFile(const std::string &filename_uri, const LibrdfModel &model) const;

        void parseUri(const std::string &uri_string, const LibrdfModel &model) const;

        [[nodiscard]] std::string getNamespacesSeenPrefix(int index) const;

        [[nodiscard]] std::string getName() const;

        void setName(const char *name);

        [[nodiscard]] std::string getMimeType() const;

        void setMimeType(const char *mimeType);

        [[nodiscard]] librdf_uri *getTypeUri() const;

        void setTypeUri(librdf_uri *typeUri);

        void setTypeUri(const std::string &type_uri);

        librdf_parser *makeParser();

        static void setFeature(librdf_parser *parser, const std::string &feature_uri, librdf_node *node);

        static void setOption(librdf_parser *parser, const std::string &option, const std::string &value);

        static void setOptions(librdf_parser *parser);

        [[nodiscard]] std::vector<std::string> getSeenNamespaces() const;


    };
}


#endif //LIBSEMSIM_LIBRDFPARSER_H
