//
// Created by Ciaran on 4/13/2020.
//

#ifndef LIBSEMSIM_SEMSIMUTILS_H
#define LIBSEMSIM_SEMSIMUTILS_H

#include "semsim/Query.h"
#include "semsim/CurlGet.h"
#include "semsim/MetaID.h"

#include <librdf.h>
#include <string>
#include <vector>
#include <sys/stat.h>
#include <stdexcept>
#include <sstream>

using namespace redland;


namespace semsim {

    class SemsimUtils {
    public:
        SemsimUtils() = default;

        static bool exists(const std::string &filename);

        static int removeFile(const std::string &filename);

        static void removeIfExists(const std::string &filename);

        static void download(const std::string &url, std::string filename);

        static std::vector<std::string> splitStringBy(const std::string &str, char delimiter);

        static std::string generateUniqueMetaid(
                librdf_model* model, std::string metaid_base,
                std::vector<std::string> exclusions = std::vector<std::string>()
        );

        static std::string addFilePrefixToString(std::string str);

        /*
         * @brief takes a uri as std::string and returns the string
         * with the last section removed;
         *
         * Example: www.uri.com/identifer/PD12345 will
         * turn into www.uri.com/identifier/
         */
        static std::string getNamespaceFromUri(const std::string& uri);

    };
}

#endif //LIBSEMSIM_SEMSIMUTILS_H
